//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Particles/BsParticleManager.h"
#include "Particles/BsParticleSystem.h"
#include "Utility/BsTime.h"
#include "Threading/BsTaskScheduler.h"
#include "Allocators/BsPoolAlloc.h"
#include "Private/Particles/BsParticleSet.h"
#include "Animation/BsAnimationManager.h"
#include "Image/BsPixelUtil.h"

namespace bs
{
	/** Helper method used for writing particle data into the @p pixels buffer. */
	template<class T, class PR>
	void iterateOverPixels(PixelData& pixels, UINT32 count, UINT32 stride, PR predicate)
	{
		auto dest = (UINT8*)pixels.GetData();

		UINT32 x = 0;
		for (UINT32 i = 0; i < count; i++)
		{
			predicate((T*)dest, i);

			dest += stride;
			x++;

			if (x >= pixels.GetWidth())
			{
				x = 0;
				dest += pixels.GetRowSkip();
			}
		}
	}

	/** Helper method used for writing particle data into the @p pixels buffer. */
	template<class T, class PR>
	void iterateOverPixels(PixelData& pixels, UINT32 count, PR predicate)
	{
		iterateOverPixels<T>(pixels, count, sizeof(T), predicate);
	}

	/**
	 * Maintains a pool of buffers that are used for passing results of particle simulation from the simulation to the
	 * core thread.
	 */
	class ParticleSimulationDataPool
	{
		/** Contains a list of buffers for the specified size. */
		struct BuffersPerSize
		{
			Vector<ParticleRenderData*> Buffers;
			UINT32 NextFreeIdx = 0;
		};

	public:
		~ParticleSimulationDataPool()
		{
			Lock lock(mMutex);

			for (auto& sizeEntry : mBillboardBufferList)
			{
				for (auto& entry : sizeEntry.second.Buffers)
					mBillboardAlloc.Destruct(static_cast<ParticleBillboardRenderData*>(entry));
			}

			for (auto& sizeEntry : mMeshBufferList)
			{
				for (auto& entry : sizeEntry.second.Buffers)
					mMeshAlloc.Destruct(static_cast<ParticleMeshRenderData*>(entry));
			}

			for (auto& entry : mGPUBufferList)
				mGPUAlloc.Destruct(entry);
		}

		/**
		 * Returns a set of buffers containing particle data from the provided particle set. Usable for rendering the
		 * results of the CPU particle simulation as billboards.
		 */
		ParticleBillboardRenderData* AllocCpuBillboard(const ParticleSet& particleSet)
		{
			const UINT32 size = particleSet.DetermineTextureSize();

			ParticleBillboardRenderData* output = nullptr;

			{
				Lock lock(mMutex);

				BuffersPerSize& buffers = mBillboardBufferList[size];
				if (buffers.NextFreeIdx < (UINT32)buffers.Buffers.size())
				{
					output = static_cast<ParticleBillboardRenderData*>(buffers.Buffers[buffers.NextFreeIdx]);
					buffers.NextFreeIdx++;
				}
			}

			if (!output)
			{
				output = CreateNewBillboardBuffersCpu(size);

				Lock lock(mMutex);

				BuffersPerSize& buffers = mBillboardBufferList[size];
				buffers.Buffers.push_back(output);
				buffers.NextFreeIdx++;
			}

			// Populate buffer contents
			const UINT32 count = particleSet.GetParticleCount();
			const ParticleSetData& particles = particleSet.GetParticles();

			// TODO: Use non-temporal writes?
			iterateOverPixels<Vector4>(output->PositionAndRotation, count,
				[&particles](Vector4* dst, UINT32 idx)
			{
				dst->X = particles.Position[idx].X;
				dst->Y = particles.Position[idx].Y;
				dst->Z = particles.Position[idx].Z;
				dst->W = particles.Rotation[idx].X * Math::DEG2RAD;

			});

			iterateOverPixels<RGBA>(output->Color, count,
				[&particles](RGBA* dst, UINT32 idx)
			{
				*dst = particles.Color[idx];
			});

			iterateOverPixels<UINT16>(output->SizeAndFrameIdx, count, sizeof(UINT16) * 4,
				[&particles](UINT16* dst, UINT32 idx)
			{
				dst[0] = Bitwise::FloatToHalf(particles.Size[idx].X);
				dst[1] = Bitwise::FloatToHalf(particles.Size[idx].Y);
				dst[2] = Bitwise::FloatToHalf(particles.Frame[idx]);
			});

			output->Indices.clear();
			output->Indices.resize(count);

			return output;
		}

		/**
		 * Returns a set of buffers containing particle data from the provided particle set. Usable for rendering the
		 * results of the CPU particle simulation as 3D meshes.
		 */
		ParticleMeshRenderData* AllocCpuMesh(const ParticleSet& particleSet)
		{
			const UINT32 size = particleSet.DetermineTextureSize();

			ParticleMeshRenderData* output = nullptr;

			{
				Lock lock(mMutex);

				BuffersPerSize& buffers = mMeshBufferList[size];
				if (buffers.NextFreeIdx < (UINT32)buffers.Buffers.size())
				{
					output = static_cast<ParticleMeshRenderData*>(buffers.Buffers[buffers.NextFreeIdx]);
					buffers.NextFreeIdx++;
				}
			}

			if (!output)
			{
				output = CreateNewMeshBuffersCpu(size);

				Lock lock(mMutex);

				BuffersPerSize& buffers = mMeshBufferList[size];
				buffers.Buffers.push_back(output);
				buffers.NextFreeIdx++;
			}

			// Populate buffer contents
			const UINT32 count = particleSet.GetParticleCount();
			const ParticleSetData& particles = particleSet.GetParticles();

			// TODO: Use non-temporal writes?
			iterateOverPixels<Vector4>(output->Position, count,
				[&particles](Vector4* dst, UINT32 idx)
			{
				dst->X = particles.Position[idx].X;
				dst->Y = particles.Position[idx].Y;
				dst->Z = particles.Position[idx].Z;

			});

			iterateOverPixels<RGBA>(output->Color, count,
				[&particles](RGBA* dst, UINT32 idx)
			{
				*dst = particles.Color[idx];
			});

			iterateOverPixels<UINT16>(output->Rotation, count, sizeof(UINT16) * 4,
				[&particles](UINT16* dst, UINT32 idx)
			{
				dst[0] = Bitwise::FloatToHalf(particles.Rotation[idx].X * Math::DEG2RAD);
				dst[1] = Bitwise::FloatToHalf(particles.Rotation[idx].Y * Math::DEG2RAD);
				dst[2] = Bitwise::FloatToHalf(particles.Rotation[idx].Z * Math::DEG2RAD);
			});

			iterateOverPixels<UINT16>(output->Size, count, sizeof(UINT16) * 4,
				[&particles](UINT16* dst, UINT32 idx)
			{
				dst[0] = Bitwise::FloatToHalf(particles.Size[idx].X);
				dst[1] = Bitwise::FloatToHalf(particles.Size[idx].Y);
				dst[2] = Bitwise::FloatToHalf(particles.Size[idx].Z);
			});

			output->Indices.clear();
			output->Indices.resize(count);

			return output;
		}

		/**
		 * Returns a list of particles from the provided particle set that may be used for inserting the particles into the
		 * GPU simulation.
		 */
		ParticleGPUSimulationData* AllocGpu(const ParticleSet& particleSet)
		{
			ParticleGPUSimulationData* output = nullptr;

			{
				Lock lock(mMutex);

				if (mNextFreeGPUBuffer < (UINT32)mGPUBufferList.size())
				{
					output = mGPUBufferList[mNextFreeGPUBuffer];
					mNextFreeGPUBuffer++;
				}
			}

			if (!output)
			{
				output = CreateNewBuffersGpu();

				Lock lock(mMutex);

				mGPUBufferList.push_back(output);
				mNextFreeGPUBuffer++;
			}

			// Populate buffer contents
			const UINT32 count = particleSet.GetParticleCount();
			const ParticleSetData& particles = particleSet.GetParticles();

			output->Particles.clear();
			output->Particles.resize(count);

			// TODO: Use non-temporal writes?
			for (UINT32 i = 0; i < count; i++)
			{
				GpuParticle particle;
				particle.Position = particles.Position[i];
				particle.Lifetime = particles.Lifetime[i];
				particle.InitialLifetime =  particles.InitialLifetime[i];
				particle.Velocity = particles.Velocity[i];
				particle.Size = Vector2(particles.Size[i].X, particles.Size[i].Z);
				particle.Rotation = particles.Rotation[i].Z;

				output->Particles[i] = particle;
			}

			return output;
		}

		/** Makes all the buffers available for allocations. Does not free internal buffer memory. */
		void Clear()
		{
			Lock lock(mMutex);

			for(auto& buffers : mBillboardBufferList)
				buffers.second.NextFreeIdx = 0;

			for(auto& buffers : mMeshBufferList)
				buffers.second.NextFreeIdx = 0;

			mNextFreeGPUBuffer = 0;
		}

	private:
		/** Allocates a new set of CPU buffers used for billboard rendering of the provided @p size width and height. */
		ParticleBillboardRenderData* CreateNewBillboardBuffersCpu(UINT32 size)
		{
			auto output = mBillboardAlloc.Construct<ParticleBillboardRenderData>();

			output->PositionAndRotation = PixelData(size, size, 1, PF_RGBA32F);
			output->Color = PixelData(size, size, 1, PF_RGBA8);
			output->SizeAndFrameIdx = PixelData(size, size, 1, PF_RGBA16F);

			// Note: Potentially allocate them all in one large block
			output->PositionAndRotation.AllocateInternalBuffer();
			output->Color.AllocateInternalBuffer();
			output->SizeAndFrameIdx.AllocateInternalBuffer();

			return output;
		}

		/** Allocates a new set of CPU buffers used for mesh rendering of the provided @p size width and height. */
		ParticleMeshRenderData* CreateNewMeshBuffersCpu(UINT32 size)
		{
			auto output = mMeshAlloc.Construct<ParticleMeshRenderData>();

			output->Position = PixelData(size, size, 1, PF_RGBA32F);
			output->Color = PixelData(size, size, 1, PF_RGBA8);
			output->Size = PixelData(size, size, 1, PF_RGBA16F);
			output->Rotation = PixelData(size, size, 1, PF_RGBA16F);

			// Note: Potentially allocate them all in one large block
			output->Position.AllocateInternalBuffer();
			output->Color.AllocateInternalBuffer();
			output->Size.AllocateInternalBuffer();
			output->Rotation.AllocateInternalBuffer();

			return output;
		}

		/** Allocates a new set of GPU buffers of the provided @p size width and height. */
		ParticleGPUSimulationData* CreateNewBuffersGpu()
		{
			return mGPUAlloc.Construct<ParticleGPUSimulationData>();
		}

		UnorderedMap<UINT32, BuffersPerSize> mBillboardBufferList;
		UnorderedMap<UINT32, BuffersPerSize> mMeshBufferList;
		Vector<ParticleGPUSimulationData*> mGPUBufferList;
		UINT32 mNextFreeGPUBuffer = 0;

		PoolAlloc<sizeof(ParticleBillboardRenderData), 32, 4, true> mBillboardAlloc;
		PoolAlloc<sizeof(ParticleMeshRenderData), 32, 4, true> mMeshAlloc;
		PoolAlloc<sizeof(ParticleGPUSimulationData), 32, 4, true> mGPUAlloc;
		Mutex mMutex;
	};

	struct ParticleManager::Members
	{
		// TODO - Perhaps sharing one pool is better
		ParticleSimulationDataPool SimDataPool[CoreThread::NUM_SYNC_BUFFERS];
	};

	ParticleManager::ParticleManager()
		:m(bs_new<Members>())
	{ }

	ParticleManager::~ParticleManager()
	{
		bs_delete(m);
	}

	ParticlePerFrameData* ParticleManager::Update(const EvaluatedAnimationData& animData)
	{
		// Note: Allow the worker threads to work alongside the main thread? Would require extra synchronization but
		// potentially no benefit?

		// Advance the buffers (last write buffer becomes read buffer)
		if (mSwapBuffers)
		{
			mReadBufferIdx = (mReadBufferIdx + 1) % CoreThread::NUM_SYNC_BUFFERS;
			mWriteBufferIdx = (mWriteBufferIdx + 1) % CoreThread::NUM_SYNC_BUFFERS;

			mSwapBuffers = false;
		}

		if(mPaused)
			return &mSimulationData[mReadBufferIdx];

		// TODO - Perform culling (but only on deterministic particle systems). In which case cache the bounds so we don't
		// need to recalculate them below

		// Prepare the write buffer
		ParticlePerFrameData& simulationData = mSimulationData[mWriteBufferIdx];
		simulationData.CpuData.clear();
		simulationData.GpuData.clear();

		// Queue evaluation tasks
		{
			Lock lock(mMutex);
			mNumActiveWorkers = (UINT32)mSystems.size();
		}

		float timeDelta = gTime().GetFrameDelta();

		ParticleSimulationDataPool& simDataPool = m->SimDataPool[mWriteBufferIdx];
		simDataPool.Clear();

		for (auto& system : mSystems)
		{
			const auto evaluateWorker = [this, timeDelta, system, &animData, &simDataPool, &simulationData]()
			{
				// Advance the simulation
				system->SimulateInternal(timeDelta, &animData);

				ParticleRenderData* simulationDataCPU = nullptr;
				ParticleGPUSimulationData* simulationDataGPU = nullptr;
				if(system->mParticleSet)
				{
					// Generate simulation data to transfer to the core thread
					const UINT32 numParticles = system->mParticleSet->GetParticleCount();
					const ParticleSystemSettings& settings = system->GetSettings();

					if(settings.GpuSimulation)
						simulationDataGPU = simDataPool.AllocGpu(*system->mParticleSet);
					else
					{
						if(settings.RenderMode == ParticleRenderMode::Billboard)
							simulationDataCPU = simDataPool.AllocCpuBillboard(*system->mParticleSet);
						else
							simulationDataCPU = simDataPool.AllocCpuMesh(*system->mParticleSet);

						simulationDataCPU->NumParticles = numParticles;

						if(settings.UseAutomaticBounds)
							simulationDataCPU->Bounds = system->CalculateBoundsInternal();
						else
							simulationDataCPU->Bounds = settings.CustomBounds;

						// If using a camera-independant sorting mode, sort the particles right away
						switch (settings.SortMode)
						{
						default:
						case ParticleSortMode::None: // No sort, just point the indices back to themselves
							for (UINT32 i = 0; i < numParticles; i++)
								simulationDataCPU->Indices[i] = i;
							break;
						case ParticleSortMode::OldToYoung:
						case ParticleSortMode::YoungToOld:
							SortParticles(*system->mParticleSet, settings.SortMode, Vector3::ZERO, simulationDataCPU->Indices.data());
							break;
						case ParticleSortMode::Distance: break;
						}
					}
				}

				{
					Lock lock(mMutex);

					assert(mNumActiveWorkers > 0);
					mNumActiveWorkers--;

					if(simulationDataCPU)
						simulationData.CpuData[system->mId] = simulationDataCPU;
					else if(simulationDataGPU)
						simulationData.GpuData[system->mId] = simulationDataGPU;
				}

				mWorkerDoneSignal.notify_one();
			};

			SPtr<Task> task = Task::Create("ParticleWorker", evaluateWorker);
			TaskScheduler::Instance().AddTask(task);
		}

		// Wait for tasks to complete
		TaskScheduler::Instance().AddWorker(); // Make the current core available for work (since this thread waits)
		{
			Lock lock(mMutex);

			while (mNumActiveWorkers > 0)
				mWorkerDoneSignal.wait(lock);
		}
		TaskScheduler::Instance().RemoveWorker();

		mSwapBuffers = true;

		return &mSimulationData[mWriteBufferIdx];
	}

	void ParticleManager::SortParticles(const ParticleSet& set, ParticleSortMode sortMode, const Vector3& viewPoint,
		UINT32* indices)
	{
		assert(sortMode != ParticleSortMode::None);

		struct ParticleSortData
		{
			ParticleSortData(float key, UINT32 idx)
				:Key(key), Idx(idx)
			{ }

			float Key;
			UINT32 Idx;
		};

		const UINT32 count = set.GetParticleCount();
		const ParticleSetData& particles = set.GetParticles();

		bs_frame_mark();
		{
			FrameVector<ParticleSortData> sortData;
			sortData.reserve(count);

			switch(sortMode)
			{
			default:
			case ParticleSortMode::Distance:
				for(UINT32 i = 0; i < count; i++)
				{
					float distance = viewPoint.SquaredDistance(particles.Position[i]);
					sortData.emplace_back(distance, i);
				}
				break;
			case ParticleSortMode::OldToYoung:
				for(UINT32 i = 0; i < count; i++)
				{
					float lifetime = particles.Lifetime[i];
					sortData.emplace_back(lifetime, i);
				}
				break;
			case ParticleSortMode::YoungToOld:
				for(UINT32 i = 0; i < count; i++)
				{
					float lifetime = particles.InitialLifetime[i] - particles.Lifetime[i];
					sortData.emplace_back(lifetime, i);
				}
				break;
			}

			std::sort(sortData.begin(), sortData.end(),
				[](const ParticleSortData& lhs, const ParticleSortData& rhs)
			{
				return rhs.Key < lhs.Key;
			});

			for (UINT32 i = 0; i < count; i++)
				indices[i] = sortData[i].Idx;
		}
		bs_frame_clear();
	}

	UINT32 ParticleManager::RegisterParticleSystem(ParticleSystem* system)
	{
		mSystems.insert(system);

		return mNextId++;
	}

	void ParticleManager::UnregisterParticleSystem(ParticleSystem* system)
	{
		mSystems.erase(system);
	}
}
