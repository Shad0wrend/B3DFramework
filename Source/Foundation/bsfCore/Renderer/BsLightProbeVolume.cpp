//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Renderer/BsLightProbeVolume.h"
#include "Private/RTTI/BsLightProbeVolumeRTTI.h"
#include "Renderer/BsRenderer.h"
#include "Renderer/BsLight.h"
#include "Image/BsTexture.h"
#include "Renderer/BsIBLUtility.h"
#include "Scene/BsSceneObject.h"
#include "CoreThread/BsCoreObjectSync.h"

namespace bs
{
	LightProbeVolume::LightProbeVolume(const AABox& volume, const Vector3I& cellCount)
		:mVolume(volume), mCellCount(cellCount)
	{
		Reset();
	}

	LightProbeVolume::~LightProbeVolume()
	{
		if (mRendererTask)
			mRendererTask->Cancel();
	}

	UINT32 LightProbeVolume::AddProbe(const Vector3& position)
	{
		UINT32 handle = mNextProbeId++;
		mProbes[handle] = ProbeInfo(LightProbeFlags::Clean, position);

		MarkCoreDirtyInternal();
		return handle;
	}

	void LightProbeVolume::RemoveProbe(UINT32 handle)
	{
		auto iterFind = mProbes.find(handle);
		if (iterFind != mProbes.end() && mProbes.size() > 4)
		{
			iterFind->second.Flags = LightProbeFlags::Removed;
			MarkCoreDirtyInternal();
		}
	}

	void LightProbeVolume::SetProbePosition(UINT32 handle, const Vector3& position)
	{
		auto iterFind = mProbes.find(handle);
		if (iterFind != mProbes.end())
		{
			iterFind->second.Position = position;
			MarkCoreDirtyInternal();
		}
	}

	Vector3 LightProbeVolume::GetProbePosition(UINT32 handle) const
	{
		auto iterFind = mProbes.find(handle);
		if (iterFind != mProbes.end())
			return iterFind->second.Position;
		
		return Vector3::ZERO;
	}

	Vector<LightProbeInfo> LightProbeVolume::GetProbes() const
	{
		Vector<LightProbeInfo> output;

		for(auto& entry : mProbes)
		{
			if (entry.second.Flags == LightProbeFlags::Removed || entry.second.Flags == LightProbeFlags::Empty)
				continue;

			LightProbeInfo info;
			info.Position = entry.second.Position;
			info.Handle = entry.first;
			info.ShCoefficients = entry.second.Coefficients;

			output.push_back(info);
		}

		return output;		
	}

	void LightProbeVolume::Resize(const AABox& volume, const Vector3I& cellCount)
	{
		UINT32 numProbesX = std::max(1, mCellCount.X) + 1;
		UINT32 numProbesY = std::max(1, mCellCount.Y) + 1;
		UINT32 numProbesZ = std::max(1, mCellCount.Z) + 1;

		Vector3 size = mVolume.GetSize();
		for(UINT32 z = 0; z < numProbesZ; ++z)
		{
			for(UINT32 y = 0; y < numProbesY; ++y)
			{
				for(UINT32 x = 0; x < numProbesX; ++x)
				{
					Vector3 position = mVolume.GetMin();
					position.X += size.X * (x / (float)numProbesX);
					position.Y += size.Y * (y / (float)numProbesY);
					position.Z += size.Z * (z / (float)numProbesZ);

					if (mVolume.Contains(position))
						continue;

					AddProbe(position);
				}
			}
		}

		mVolume = volume;
		mCellCount = cellCount;

		MarkCoreDirtyInternal();
	}

	void LightProbeVolume::Reset()
	{
		UINT32 numProbesX = std::max(1, mCellCount.X) + 1;
		UINT32 numProbesY = std::max(1, mCellCount.Y) + 1;
		UINT32 numProbesZ = std::max(1, mCellCount.Z) + 1;

		UINT32 numProbes = numProbesX * numProbesY * numProbesZ;

		// Make sure there are adequate number of probes to fill the volume
		while((UINT32)mProbes.size() < numProbes)
			AddProbe(Vector3::ZERO);

		UINT32 idx = 0;
		UINT32 rowPitch = numProbesX;
		UINT32 slicePitch = numProbesX * numProbesY;

		Vector3 size = mVolume.GetSize();

		auto iter = mProbes.begin();
		while (iter != mProbes.end())
		{
			UINT32 x = idx % numProbesX;
			UINT32 y = (idx / rowPitch) % numProbesY;
			UINT32 z = (idx / slicePitch);

			Vector3 position = mVolume.GetMin();
			position.X += size.X * (x / (float)(numProbesX - 1));
			position.Y += size.Y * (y / (float)(numProbesY - 1));
			position.Z += size.Z * (z / (float)(numProbesZ - 1));

			iter->second.Position = position;
			iter->second.Flags = LightProbeFlags::Clean;

			++idx;
			++iter;

			if (idx >= numProbes)
				break;
		}

		// Set remaining probes to removed state
		while(iter != mProbes.end())
		{
			iter->second.Flags = LightProbeFlags::Removed;
			++iter;
		}

		MarkCoreDirtyInternal();
	}

	void LightProbeVolume::Clip()
	{
		for (auto& entry : mProbes)
		{
			if (!mVolume.Contains(entry.second.Position))
				entry.second.Flags = LightProbeFlags::Removed;
		}

		MarkCoreDirtyInternal();
	}

	void LightProbeVolume::RenderProbe(UINT32 handle)
	{
		auto iterFind = mProbes.find(handle);
		if (iterFind != mProbes.end())
		{
			if (iterFind->second.Flags == LightProbeFlags::Clean)
			{
				iterFind->second.Flags = LightProbeFlags::Dirty;

				MarkCoreDirtyInternal();
				RunRenderProbeTask();
			}
		}
	}

	void LightProbeVolume::RenderProbes()
	{
		bool anyModified = false;
		for(auto& entry : mProbes)
		{
			if (entry.second.Flags == LightProbeFlags::Clean)
			{
				entry.second.Flags = LightProbeFlags::Dirty;
				anyModified = true;
			}
		}

		if (anyModified)
		{
			MarkCoreDirtyInternal();
			RunRenderProbeTask();
		}
	}

	void LightProbeVolume::RunRenderProbeTask()
	{
		// If a task is already running cancel it
		// Note: If the task is just about to start processing, cancelling it will skip the update this frame
		// (which might be fine if we just changed positions of dirty probes it was about to update, but it might also
		// waste a frame if those positions needed to be updated anyway). For now I'm ignoring it as it seems like a rare
		// enough situation, plus it's one that will only happen during development time.
		if (mRendererTask)
			mRendererTask->Cancel();

		auto renderComplete = [this]()
		{
			mRendererTask = nullptr;
		};

		SPtr<ct::LightProbeVolume> coreProbeVolume = GetCore();
		auto renderProbes = [coreProbeVolume]()
		{
			return coreProbeVolume->RenderProbes(3);
		};

		mRendererTask = ct::RendererTask::Create("RenderLightProbes", renderProbes);

		mRendererTask->OnComplete.Connect(renderComplete);
		ct::gRenderer()->AddTask(mRendererTask);
	}

	void LightProbeVolume::UpdateCoefficients()
	{
		// Ensure all light probe coefficients are generated
		if (mRendererTask)
			mRendererTask->Wait();

		ct::LightProbeVolume* coreVolume = GetCore().get();

		Vector<LightProbeCoefficientInfo> coeffInfo;
		auto getSaveData = [coreVolume, &coeffInfo]()
		{
			coreVolume->GetProbeCoefficients(coeffInfo);
		};

		gCoreThread().QueueCommand(getSaveData);
		gCoreThread().Submit(true);

		for(auto& entry : coeffInfo)
		{
			auto iterFind = mProbes.find(entry.Handle);
			if (iterFind == mProbes.end())
				continue;

			iterFind->second.Coefficients = entry.Coefficients;
		}
	}

	SPtr<ct::LightProbeVolume> LightProbeVolume::GetCore() const
	{
		return std::static_pointer_cast<ct::LightProbeVolume>(mCoreSpecific);
	}

	SPtr<LightProbeVolume> LightProbeVolume::Create(const AABox& volume, const Vector3I& cellCount)
	{
		LightProbeVolume* probeVolume = new (bs_alloc<LightProbeVolume>()) LightProbeVolume(volume, cellCount);
		SPtr<LightProbeVolume> probeVolumePtr = bs_core_ptr<LightProbeVolume>(probeVolume);
		probeVolumePtr->SetThisPtrInternal(probeVolumePtr);
		probeVolumePtr->Initialize();

		return probeVolumePtr;
	}

	SPtr<LightProbeVolume> LightProbeVolume::CreateEmpty()
	{
		LightProbeVolume* probeVolume = new (bs_alloc<LightProbeVolume>()) LightProbeVolume();
		SPtr<LightProbeVolume> probleVolumePtr = bs_core_ptr<LightProbeVolume>(probeVolume);
		probleVolumePtr->SetThisPtrInternal(probleVolumePtr);

		return probleVolumePtr;
	}

	SPtr<ct::CoreObject> LightProbeVolume::CreateCore() const
	{
		ct::LightProbeVolume* handler = new (bs_alloc<ct::LightProbeVolume>()) ct::LightProbeVolume(mProbes);
		SPtr<ct::LightProbeVolume> handlerPtr = bs_shared_ptr<ct::LightProbeVolume>(handler);
		handlerPtr->SetThisPtrInternal(handlerPtr);

		return handlerPtr;
	}

	CoreSyncData LightProbeVolume::SyncToCore(FrameAlloc* allocator)
	{
		UINT32 size = 0;
		UINT8* buffer = nullptr;

		bs_frame_mark();
		{
			FrameVector<std::pair<UINT32, ProbeInfo>> dirtyProbes;
			FrameVector<UINT32> removedProbes;
			for (auto& probe : mProbes)
			{
				if (probe.second.Flags == LightProbeFlags::Dirty)
				{
					dirtyProbes.push_back(std::make_pair(probe.first, probe.second));
					probe.second.Flags = LightProbeFlags::Clean;
				}
				else if (probe.second.Flags == LightProbeFlags::Removed)
				{
					removedProbes.push_back(probe.first);
					probe.second.Flags = LightProbeFlags::Empty;
				}
			}

			for (auto& probe : removedProbes)
				mProbes.erase(probe);

			UINT32 numDirtyProbes = (UINT32)dirtyProbes.size();
			UINT32 numRemovedProbes = (UINT32)removedProbes.size();

			size += csync_size((SceneActor&)*this);
			size += rtti_size(numDirtyProbes).Bytes;
			size += rtti_size(numRemovedProbes).Bytes;
			size += (sizeof(UINT32) + sizeof(Vector3) + sizeof(LightProbeFlags)) * numDirtyProbes;
			size += sizeof(UINT32) * numRemovedProbes;

			buffer = allocator->Alloc(size);
			Bitstream stream(buffer, size);

			csync_write((SceneActor&)*this, stream);
			rtti_write(numDirtyProbes, stream);
			rtti_write(numRemovedProbes, stream);

			for (auto& entry : dirtyProbes)
			{
				rtti_write(entry.first, stream);
				rtti_write(entry.second.Position, stream);
				rtti_write(entry.second.Flags, stream);
			}

			for(auto& entry : removedProbes)
				rtti_write(entry, stream);
		}
		bs_frame_clear();

		return CoreSyncData(buffer, size);
	}

	void LightProbeVolume::MarkCoreDirtyInternal(ActorDirtyFlag dirtyFlag)
	{
		MarkCoreDirty((UINT32)dirtyFlag);
	}

	RTTITypeBase* LightProbeVolume::GetRttiStatic()
	{
		return LightProbeVolumeRTTI::Instance();
	}

	RTTITypeBase* LightProbeVolume::GetRtti() const
	{
		return LightProbeVolume::GetRttiStatic();
	}

	namespace ct
	{
	LightProbeVolume::LightProbeVolume(const UnorderedMap<UINT32, bs::LightProbeVolume::ProbeInfo>& probes)
	{
		mInitCoefficients.resize(probes.size());
		mProbePositions.resize(probes.size());
		mProbeInfos.resize(probes.size());

		UINT32 probeIdx = 0;
		for(auto& entry : probes)
		{
			mProbeMap[entry.first] = probeIdx;
			mProbePositions[probeIdx] = entry.second.Position;
			
			LightProbeInfo probeInfo;
			probeInfo.Flags = LightProbeFlags::Dirty;
			probeInfo.BufferIdx = probeIdx;
			probeInfo.Handle = entry.first;

			mProbeInfos[probeIdx] = probeInfo;
			mInitCoefficients[probeIdx] = entry.second.Coefficients;

			probeIdx++;
		}
	}

	LightProbeVolume::~LightProbeVolume()
	{
		gRenderer()->NotifyLightProbeVolumeRemoved(this);
	}

	void LightProbeVolume::Initialize()
	{
		// Set SH coefficients loaded from the file
		UINT32 numCoefficients = (UINT32)mInitCoefficients.size();
		assert(mInitCoefficients.size() == mProbeMap.size());

		ResizeCoefficientTexture(std::max(32U, numCoefficients));

		SPtr<PixelData> coeffData = mCoefficients->GetProperties().AllocBuffer(0, 0);
		coeffData->SetColors(Color::ZERO);

		UINT32 probesPerRow = coeffData->GetWidth() / 9;
		UINT32 probeIdx = 0;
		for(UINT32 y = 0; y < coeffData->GetHeight(); ++y)
		{
			for(UINT32 x = 0; x < probesPerRow; ++x)
			{
				if(probeIdx >= numCoefficients)
					break;

				for(UINT32 i = 0; i < 9; i++)
				{
					Color value;
					value.R = mInitCoefficients[probeIdx].CoeffsR[i];
					value.G = mInitCoefficients[probeIdx].CoeffsG[i];
					value.B = mInitCoefficients[probeIdx].CoeffsB[i];

					coeffData->SetColorAt(value, x * 9, y);
				}

				probeIdx++;
			}
		}

		mCoefficients->WriteData(*coeffData, 0, 0, true);
		mInitCoefficients.clear();

		gRenderer()->NotifyLightProbeVolumeAdded(this);
		CoreObject::Initialize();
	}

	bool LightProbeVolume::RenderProbes(UINT32 maxProbes)
	{
		// Probe map only contains active probes
		UINT32 numUsedProbes = (UINT32)mProbeMap.size();
		if(numUsedProbes > mCoeffBufferSize)
			ResizeCoefficientTexture(std::max(32U, numUsedProbes * 2));

		UINT32 numProbeUpdates = 0;
		for (; mFirstDirtyProbe < (UINT32)mProbeInfos.size(); ++mFirstDirtyProbe)
		{
			LightProbeInfo& probeInfo = mProbeInfos[mFirstDirtyProbe];

			if(probeInfo.Flags == LightProbeFlags::Dirty)
			{
				TEXTURE_DESC cubemapDesc;
				cubemapDesc.Type = TEX_TYPE_CUBE_MAP;
				cubemapDesc.Format = PF_RGBA16F;
				cubemapDesc.Width = 256; // Note: Test different sizes and their effect on quality
				cubemapDesc.Height = 256;
				cubemapDesc.Usage = TU_STATIC | TU_RENDERTARGET;

				SPtr<Texture> cubemap = Texture::Create(cubemapDesc);

				Vector3 localPos = mProbePositions[mFirstDirtyProbe];

				const Transform& tfrm = GetTransform();
				const Vector3& position = tfrm.GetPosition();
				const Quaternion& rotation = tfrm.GetRotation();
				Vector3 transformedPos = rotation.Rotate(localPos) + position;

				gRenderer()->CaptureSceneCubeMap(cubemap, transformedPos, CaptureSettings());
				gIBLUtility().FilterCubemapForIrradiance(cubemap, mCoefficients, probeInfo.BufferIdx);

				probeInfo.Flags = LightProbeFlags::Clean;
				numProbeUpdates++;
			}

			if (maxProbes != 0 && numProbeUpdates >= maxProbes)
				break;
		}

		gRenderer()->NotifyLightProbeVolumeUpdated(this);

		return mFirstDirtyProbe == (UINT32)mProbeInfos.size();
	}

	void LightProbeVolume::SyncToCore(const CoreSyncData& data)
	{
		Bitstream stream(data.GetBuffer(), data.GetBufferSize());

		bool oldIsActive = mActive;

		csync_read((SceneActor&)*this, stream);

		UINT32 numDirtyProbes, numRemovedProbes;
		rtti_read(numDirtyProbes, stream);
		rtti_read(numRemovedProbes, stream);

		for (UINT32 i = 0; i < numDirtyProbes; ++i)
		{
			UINT32 handle;
			rtti_read(handle, stream);

			Vector3 position;
			rtti_read(position, stream);

			LightProbeFlags flags;
			rtti_read(flags, stream);

			auto iterFind = mProbeMap.find(handle);
			if(iterFind != mProbeMap.end())
			{
				// Update existing probe information
				UINT32 compactIdx = iterFind->second;
				
				mProbeInfos[compactIdx].Flags = LightProbeFlags::Dirty;
				mProbePositions[compactIdx] = position;

				mFirstDirtyProbe = std::min(compactIdx, mFirstDirtyProbe);
			}
			else // Add a new probe
			{
				// Empty slots always start at a specific index because we always move them to the back of the array
				UINT32 emptyProbeStartIdx = (UINT32)mProbeMap.size();
				UINT32 numProbes = (UINT32)mProbeInfos.size();

				// Find an empty slot to place the probe information at
				UINT32 compactIdx = -1;
				for(UINT32 j = emptyProbeStartIdx; j < numProbes; ++j)
				{
					if(mProbeInfos[j].Flags == LightProbeFlags::Empty)
					{
						compactIdx = j;
						break;
					}
				}

				// Found an empty slot
				if (compactIdx == (UINT32)-1)
				{
					compactIdx = (UINT32)mProbeInfos.size();

					LightProbeInfo info;
					info.Flags = LightProbeFlags::Dirty;
					info.BufferIdx = compactIdx;
					info.Handle = handle;

					mProbeInfos.push_back(info);
					mProbePositions.push_back(position);
				}
				else // No empty slot, add a new one
				{
					LightProbeInfo& info = mProbeInfos[compactIdx];
					info.Flags = LightProbeFlags::Dirty;
					info.Handle = handle;

					mProbePositions[compactIdx] = position;
				}

				mProbeMap[handle] = compactIdx;
				mFirstDirtyProbe = std::min(compactIdx, mFirstDirtyProbe);
			}
		}

		// Mark slots for removed probes as empty, and move them back to the end of the array
		for (UINT32 i = 0; i < numRemovedProbes; ++i)
		{
			UINT32 idx;
			rtti_read(idx, stream);

			auto iterFind = mProbeMap.find(idx);
			if(iterFind != mProbeMap.end())
			{
				UINT32 compactIdx = iterFind->second;
				
				LightProbeInfo& info = mProbeInfos[compactIdx];
				info.Flags = LightProbeFlags::Empty;

				// Move the empty info to the back of the array so all non-empty probes are contiguous
				// Search from back to current index, and find first non-empty probe to switch switch
				UINT32 lastSearchIdx = (UINT32)mProbeInfos.size() - 1;
				while (lastSearchIdx >= (UINT32)compactIdx)
				{
					LightProbeFlags flags = mProbeInfos[lastSearchIdx].Flags;
					if (flags != LightProbeFlags::Empty)
					{
						std::swap(mProbeInfos[i], mProbeInfos[lastSearchIdx]);
						std::swap(mProbePositions[i], mProbePositions[lastSearchIdx]);

						mProbeMap[mProbeInfos[lastSearchIdx].Handle] = i;
						break;
					}

					lastSearchIdx--;
				}
				
				mProbeMap.erase(iterFind);
			}
		}

		if (oldIsActive != mActive)
		{
			if (mActive)
				gRenderer()->NotifyLightProbeVolumeAdded(this);
			else
				gRenderer()->NotifyLightProbeVolumeRemoved(this);
		}
	}

	void LightProbeVolume::GetProbeCoefficients(Vector<LightProbeCoefficientInfo>& output) const
	{
		UINT32 numActiveProbes = (UINT32)mProbeMap.size();
		if (numActiveProbes == 0)
			return;

		output.resize(numActiveProbes);

		LightProbeSHCoefficients* coefficients = bs_stack_alloc<LightProbeSHCoefficients>(numActiveProbes);

		SPtr<PixelData> coeffData = mCoefficients->GetProperties().AllocBuffer(0, 0);
		mCoefficients->ReadData(*coeffData);

		UINT32 probesPerRow = coeffData->GetWidth() / 9;
		UINT32 probeIdx = 0;
		for(UINT32 y = 0; y < coeffData->GetHeight(); ++y)
		{
			for(UINT32 x = 0; x < probesPerRow; ++x)
			{
				if(probeIdx >= numActiveProbes)
					break;

				for(UINT32 i = 0; i < 9; i++)
				{
					Color value = coeffData->GetColorAt(x * 9, y);

					coefficients[probeIdx].CoeffsR[i] = value.R;
					coefficients[probeIdx].CoeffsG[i] = value.G;
					coefficients[probeIdx].CoeffsB[i] = value.B;
				}

				probeIdx++;
			}
		}

		for(UINT32 i = 0; i < numActiveProbes; ++i)
		{
			output[i].Coefficients = coefficients[mProbeInfos[i].BufferIdx];
			output[i].Handle = mProbeInfos[i].Handle;
		}

		bs_stack_free(coefficients);
	}

	void LightProbeVolume::ResizeCoefficientTexture(UINT32 count)
	{
		Vector2I texSize = IBLUtility::GetShCoeffTextureSize(count, 3);

		TEXTURE_DESC desc;
		desc.Width = (UINT32)texSize.X;
		desc.Height = (UINT32)texSize.Y;
		desc.Usage = TU_LOADSTORE | TU_RENDERTARGET;
		desc.Format = PF_RGBA32F;

		SPtr<Texture> newTexture = Texture::Create(desc);

		if (mCoefficients)
			mCoefficients->Copy(newTexture);

		mCoefficients = newTexture;
		mCoeffBufferSize = count;
	}
}}
