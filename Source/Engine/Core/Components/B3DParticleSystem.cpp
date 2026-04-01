//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Components/B3DParticleSystem.h"

#include "CoreObject/B3DCoreObjectSync.h"
#include "ECS/B3DRegistry.h"
#include "Private/Particles/B3DParticleSet.h"
#include "Scene/B3DSceneObject.h"
#include "Scene/B3DSceneObjectFragments.h"
#include "Utility/B3DTime.h"
#include "RTTI/B3DParticleSystemRTTI.h"
#include "Renderer/B3DRendererScene.h"
#include "Scene/B3DSceneInstance.h"
#include "Mesh/B3DMesh.h"
#include "Particles/B3DVectorField.h"
#include "Material/B3DMaterial.h"
#include "Particles/B3DParticleScene.h"

using namespace b3d;

static constexpr u32 kInitialParticleCapacity = 1000;

RTTIType* ecs::ParticleSystem::GetRttiStatic()
{
	return ecs::ECSParticleSystemRTTI::Instance();
}

RTTIType* ecs::ParticleSystem::GetRtti() const
{
	return GetRttiStatic();
}

namespace b3d::ecs
{
	/** Tag indicating a ParticleSystem needs to sync all of its properties to its render proxy. */
	struct ParticleSystemDirty {};

	/** Tag indicating a ParticleSystem needs to sync transform to its render proxy. */
	struct ParticleSystemTransformDirty {};
} // namespace b3d::ecs

namespace b3d
{
	B3D_SYNC_BLOCK_BEGIN(ParticleSystemSettings, SyncPacket)
		B3D_SYNC_BLOCK_ENTRY(GpuSimulation)
		B3D_SYNC_BLOCK_ENTRY(SimulationSpace)
		B3D_SYNC_BLOCK_ENTRY(Orientation)
		B3D_SYNC_BLOCK_ENTRY(OrientationPlaneNormal)
		B3D_SYNC_BLOCK_ENTRY(OrientationLockY)
		B3D_SYNC_BLOCK_ENTRY(Duration)
		B3D_SYNC_BLOCK_ENTRY(IsLooping)
		B3D_SYNC_BLOCK_ENTRY(SortMode)
		B3D_SYNC_BLOCK_ENTRY(Material)
		B3D_SYNC_BLOCK_ENTRY(UseAutomaticBounds)
		B3D_SYNC_BLOCK_ENTRY(CustomBounds)
		B3D_SYNC_BLOCK_ENTRY(RenderMode)
		B3D_SYNC_BLOCK_ENTRY(Mesh)
	B3D_SYNC_BLOCK_END
}

RTTIType* ParticleSystemSettings::GetRttiStatic()
{
	return ParticleSystemSettingsRTTI::Instance();
}

RTTIType* ParticleSystemSettings::GetRtti() const
{
	return GetRttiStatic();
}

namespace b3d
{
	B3D_SYNC_BLOCK_BEGIN(ParticleVectorFieldSettings, SyncPacket)
		B3D_SYNC_BLOCK_ENTRY(Intensity)
		B3D_SYNC_BLOCK_ENTRY(Tightness)
		B3D_SYNC_BLOCK_ENTRY(Scale)
		B3D_SYNC_BLOCK_ENTRY(Offset)
		B3D_SYNC_BLOCK_ENTRY(Rotation)
		B3D_SYNC_BLOCK_ENTRY(RotationRate)
		B3D_SYNC_BLOCK_ENTRY(TilingX)
		B3D_SYNC_BLOCK_ENTRY(TilingY)
		B3D_SYNC_BLOCK_ENTRY(TilingZ)
		B3D_SYNC_BLOCK_ENTRY(VectorField)
	B3D_SYNC_BLOCK_END
}

RTTIType* ParticleVectorFieldSettings::GetRttiStatic()
{
	return ParticleVectorFieldSettingsRTTI::Instance();
}

RTTIType* ParticleVectorFieldSettings::GetRtti() const
{
	return GetRttiStatic();
}

namespace b3d
{
	B3D_SYNC_BLOCK_BEGIN(ParticleDepthCollisionSettings, SyncPacket)
		B3D_SYNC_BLOCK_ENTRY(Enabled)
		B3D_SYNC_BLOCK_ENTRY(Restitution)
		B3D_SYNC_BLOCK_ENTRY(Dampening)
		B3D_SYNC_BLOCK_ENTRY(RadiusScale)
	B3D_SYNC_BLOCK_END
}

RTTIType* ParticleDepthCollisionSettings::GetRttiStatic()
{
	return ParticleDepthCollisionSettingsRTTI::Instance();
}

RTTIType* ParticleDepthCollisionSettings::GetRtti() const
{
	return GetRttiStatic();
}

namespace b3d
{
	B3D_SYNC_BLOCK_BEGIN(ParticleGpuSimulationSettings, SyncPacket)
		B3D_SYNC_BLOCK_ENTRY(ColorOverLifetime)
		B3D_SYNC_BLOCK_ENTRY(SizeScaleOverLifetime)
		B3D_SYNC_BLOCK_ENTRY(Acceleration)
		B3D_SYNC_BLOCK_ENTRY(Drag)
		B3D_SYNC_BLOCK_ENTRY(DepthCollision)
		B3D_SYNC_BLOCK_ENTRY_PACKET_FIELD(VectorField, SyncPacket)
	B3D_SYNC_BLOCK_END
}

RTTIType* ParticleGpuSimulationSettings::GetRttiStatic()
{
	return ParticleGpuSimulationSettingsRTTI::Instance();
}

RTTIType* ParticleGpuSimulationSettings::GetRtti() const
{
	return GetRttiStatic();
}

// New ECS-based sync blocks for particle system data

namespace b3d
{
	B3D_SYNC_BLOCK_BEGIN_CUSTOM(ecs::ParticleSystem, FullSyncPacket, TParticleSystemData<true>)
		B3D_SYNC_BLOCK_ENTRY_PACKET_FIELD(Settings, SyncPacket)
		B3D_SYNC_BLOCK_ENTRY_PACKET_FIELD(GpuSimulationSettings, SyncPacket)
		B3D_SYNC_BLOCK_ENTRY(Layer)
		B3D_SYNC_BLOCK_ENTRY_CUSTOM(Transform, TransformData)
		B3D_SYNC_BLOCK_ENTRY_CUSTOM(u32, Id)
	B3D_SYNC_BLOCK_END

	B3D_SYNC_BLOCK_BEGIN_CUSTOM(ecs::ParticleSystem, TransformSyncPacket, TParticleSystemData<true>)
		B3D_SYNC_BLOCK_ENTRY_CUSTOM(Transform, TransformData)
	B3D_SYNC_BLOCK_END

	struct ParticleSystemFullUpdateChannel : TRendererObjectECSSyncChannel
	<
		ParticleSystemFullUpdateChannel,
		ecs::ParticleSystem::FullSyncPacket,
		ecs::ParticleSystemDirty,
		ecs::ParticleSystem, ecs::WorldTransform, ecs::ParticleSystemId
	>
	{
		void Write(ParticleSystemObjectStorageBase& storage, FrameAllocator& allocator)
		{
			Vector<PackedRendererId, StdFrameAlloc<PackedRendererId>> renderStatesToCreate(&allocator);
			Vector<PackedRendererId, StdFrameAlloc<PackedRendererId>> renderStatesToDestroy(&allocator);

			WritePackets(storage, allocator, [&renderStatesToCreate, &renderStatesToDestroy, &storage](ecs::ParticleSystem::FullSyncPacket& packet, PackedRendererId rendererId)
			{
				render::ParticleSystemProxy& proxy = storage.GetParticleSystemProxy(rendererId);

				bool wasRegistered = proxy.mRendererId != kInvalidPackedRendererId;
				proxy.mRendererId = rendererId;
				packet.ApplySyncData(&proxy.mData);

				proxy.mTransform = packet.TransformData;
				proxy.mId = packet.Id;

				if(wasRegistered)
					renderStatesToDestroy.push_back(rendererId);

				renderStatesToCreate.push_back(rendererId);
			});

			if(!renderStatesToDestroy.empty())
				storage.DestroyRenderState(renderStatesToDestroy);

			if(!renderStatesToCreate.empty())
				storage.CreateRenderState(renderStatesToCreate);
		}

		void CreateAndPopulatePacket(ecs::ParticleSystem& fragment, ecs::WorldTransform& transform, ecs::ParticleSystemId& id, FrameAllocator& allocator)
		{
			auto& packet = CreatePacket(id.Id, fragment, allocator, 0);
			packet.TransformData = transform;
			packet.Id = fragment.Id;
		}
	};

	struct ParticleSystemTransformUpdateChannel : TRendererObjectECSSyncChannel
	<
		ParticleSystemTransformUpdateChannel,
		ecs::ParticleSystem::TransformSyncPacket,
		ecs::ParticleSystemTransformDirty,
		ecs::ParticleSystem, ecs::WorldTransform, ecs::ParticleSystemId
	>
	{
		void Write(ParticleSystemObjectStorageBase& storage, FrameAllocator& allocator)
		{
			Vector<PackedRendererId, StdFrameAlloc<PackedRendererId>> renderStatesToUpdate(&allocator);

			WritePackets(storage, allocator, [&renderStatesToUpdate, &storage](ecs::ParticleSystem::TransformSyncPacket& packet, PackedRendererId rendererId)
			{
				render::ParticleSystemProxy& proxy = storage.GetParticleSystemProxy(rendererId);
				proxy.mTransform = packet.TransformData;

				renderStatesToUpdate.push_back(rendererId);
			});

			if(!renderStatesToUpdate.empty())
				storage.UpdateRenderState(renderStatesToUpdate);
		}

		void CreateAndPopulatePacket(ecs::ParticleSystem& fragment, ecs::WorldTransform& transform, ecs::ParticleSystemId& id, FrameAllocator& allocator)
		{
			auto& packet = CreatePacket(id.Id, fragment, allocator, 0);
			packet.TransformData = transform;
		}
	};

	using ParticleSystemSyncBatch = TRendererObjectECSSyncBatch<ParticleSystemFullUpdateChannel, ParticleSystemTransformUpdateChannel>;
}

// ParticleSystemObjectStorageBase

void* ParticleSystemObjectStorageBase::SyncRead(ecs::Registry& registry, FrameAllocator& allocator)
{
	return ParticleSystemSyncBatch::Read(*this, registry, allocator);
}

void ParticleSystemObjectStorageBase::SyncWrite(void* batchData, FrameAllocator& allocator)
{
	ParticleSystemSyncBatch::Write(*this, batchData, allocator);
}

ParticleSystem::ParticleSystem(const HSceneObject& parent)
	: Component(parent)
{
	SetName("ParticleSystem");
	SetFlag(ComponentFlag::AlwaysRun, true);
	mNotifyFlags = TCF_Transform;
}

ParticleSystem::ParticleSystem()
	: ParticleSystem(nullptr)
{ }

void ParticleSystem::SetSettings(const ParticleSystemSettings& settings)
{
	const ParticleSystemSettings& oldSettings = GetSettings();

	if(settings.UseAutomaticSeed != oldSettings.UseAutomaticSeed)
	{
		if(settings.UseAutomaticSeed)
			mSeed = rand();
		else
			mSeed = settings.ManualSeed;

		mRandom.SetSeed(mSeed);
	}
	else
	{
		if(!settings.UseAutomaticSeed)
		{
			mSeed = settings.ManualSeed;
			mRandom.SetSeed(mSeed);
		}
	}

	if(mParticleSet && settings.MaxParticles < oldSettings.MaxParticles)
		mParticleSet->Clear(settings.MaxParticles);

	GetFragment().Settings = settings;
	MarkRenderProxyDataDirty();
	MarkDependenciesDirty();
}

void ParticleSystem::SetGpuSimulationSettings(const ParticleGpuSimulationSettings& settings)
{
	GetFragment().GpuSimulationSettings = settings;
	MarkRenderProxyDataDirty();
}

void ParticleSystem::SetEvolvers(const Vector<SPtr<ParticleEvolver>>& evolvers)
{
	mEvolvers = evolvers;

	std::sort(mEvolvers.begin(), mEvolvers.end(), [](const SPtr<ParticleEvolver>& a, const SPtr<ParticleEvolver>& b)
			  {
			const i32 priorityA = a ? a->GetProperties().Priority : 0;
			const i32 priorityB = b ? b->GetProperties().Priority : 0;

			if (priorityA == priorityB)
				return a > b; // Use address, at this point it doesn't matter, but sorting requires us to differentiate
			else
				return priorityA > priorityB; });

	MarkRenderProxyDataDirty();
}

void ParticleSystem::SetEmitters(const Vector<SPtr<ParticleEmitter>>& emitters)
{
	mEmitters = emitters;
	MarkRenderProxyDataDirty();
}

void ParticleSystem::SetLayer(u64 layer)
{
	const bool isPow2 = layer && !((layer - 1) & layer);

	if(!isPow2)
	{
		B3D_LOG(Warning, LogParticles, "Invalid layer provided. Only one layer bit may be set. Ignoring.");
		return;
	}

	GetFragment().Layer = layer;
	MarkRenderProxyDataDirty();
}

void ParticleSystem::Play()
{
	if(mState == State::Playing)
		return;

	if(mState == State::Uninitialized)
	{
		u32 particleCapacity = std::min(GetSettings().MaxParticles, kInitialParticleCapacity);
		mParticleSet = B3DNew<ParticleSet>(particleCapacity);
	}

	mState = State::Playing;
	mTime = 0.0f;
	mRandom.SetSeed(mSeed);
}

void ParticleSystem::Pause()
{
	if(mState == State::Playing)
		mState = State::Paused;
}

void ParticleSystem::Stop()
{
	if(mState != State::Playing && mState != State::Paused)
		return;

	mState = State::Stopped;
	mParticleSet->Clear();
}


void ParticleSystem::Simulate(float timeDelta, const EvaluatedAnimationData* animData)
{
	if(mState != State::Playing)
		return;

	const ParticleSystemSettings& settings = GetSettings();

	float timeStep;
	const float newTime = AdvanceTime(mTime, timeDelta, settings.Duration, settings.IsLooping, timeStep);

	if(timeStep < 0.00001f)
		return;

	const HSceneObject& sceneObject = SceneObject();

	// Generate per-frame state
	ParticleSystemState state;
	state.TimeStart = mTime;
	state.TimeEnd = newTime;
	state.NrmTimeStart = state.TimeStart / settings.Duration;
	state.NrmTimeEnd = state.TimeEnd / settings.Duration;
	state.Length = settings.Duration;
	state.TimeStep = timeStep;
	state.MaxParticles = settings.MaxParticles;
	state.WorldSpace = settings.SimulationSpace == ParticleSimulationSpace::World;
	state.GpuSimulated = settings.GpuSimulation;
	state.LocalToWorld = sceneObject->GetTransform().GetMatrix();
	state.WorldToLocal = state.LocalToWorld.InverseAffine();
	state.System = this;
	state.Scene = sceneObject->GetScene().get();
	state.AnimData = animData;

	// For GPU simulation we only care about newly spawned particles, so clear old ones
	if(settings.GpuSimulation)
		mParticleSet->Clear();

	// Spawn new particles
	for(auto& emitter : mEmitters)
	{
		if(emitter)
			emitter->Spawn(mRandom, state, *mParticleSet);
	}

	// Simulate if running on CPU, otherwise just pass the spawned particles off to the render thread
	if(!settings.GpuSimulation)
	{
		const u32 particleCount = mParticleSet->GetParticleCount();

		PreSimulate(state, 0, particleCount, false, 0.0f);
		Simulate(state, 0, particleCount, false, 0.0f);
		PostSimulate(state, 0, particleCount, false, 0.0f);
	}

	mTime = newTime;
}

AABox ParticleSystem::CalculateBounds() const
{
	// TODO - If evolvers are deterministic (as well as their properties), calculate the maximal bounds in an
	// analytical way

	const u32 particleCount = mParticleSet->GetParticleCount();
	if(particleCount == 0)
		return AABox::kEmpty;

	const ParticleSetData& particles = mParticleSet->GetParticles();
	AABox bounds(Vector3(-(float)kMaximumSceneExtent), Vector3((float)kMaximumSceneExtent));
	for(u32 particleIndex = 0; particleIndex < particleCount; particleIndex++)
		bounds.Merge(particles.Position[particleIndex]);

	return bounds;
}

float ParticleSystem::AdvanceTime(float time, float timeDelta, float duration, bool loop, float& outTimeStep)
{
	outTimeStep = timeDelta;
	float newTime = time + outTimeStep;
	if(newTime >= duration)
	{
		if(loop)
			newTime = fmod(newTime, duration);
		else
		{
			outTimeStep = time - duration;
			newTime = duration;
		}
	}

	return newTime;
}

void ParticleSystem::PreSimulate(const ParticleSystemState& state, u32 startIndex, u32 count, bool spacing, float spacingOffset)
{
	ParticleSetData& particles = mParticleSet->GetParticles();
	const float subFrameSpacing = (spacing && count > 0) ? 1.0f / count : 1.0f;
	const u32 endIndex = startIndex + count;

	// Decrement lifetime
	for(u32 particleIndex = startIndex; particleIndex < endIndex; particleIndex++)
	{
		float timeStep = state.TimeStep;
		if(spacing)
		{
			// Note: We're calculating this in a few places during a single frame. Store it and re-use?
			const u32 localIndex = particleIndex - startIndex;
			const float subFrameOffset = ((float)localIndex + spacingOffset) * subFrameSpacing;
			timeStep *= subFrameOffset;
		}

		particles.Lifetime[particleIndex] -= timeStep;
	}

	// Kill expired particles
	u32 particleCount = count;
	for(u32 particleSubIndex = 0; particleSubIndex < particleCount;)
	{
		const u32 particleIndex = startIndex + particleSubIndex;
		if(particles.Lifetime[particleIndex] <= 0.0f)
		{
			mParticleSet->FreeParticle(particleIndex);
			particleCount--;
		}
		else
			particleSubIndex++;
	}

	// Remember old positions
	for(u32 particleIndex = startIndex; particleIndex < endIndex; particleIndex++)
		particles.PrevPosition[particleIndex] = particles.Position[particleIndex];

	// Evolve pre-simulation
	for(auto& evolver : mEvolvers)
	{
		if(!evolver)
			continue;

		const ParticleEvolverProperties& props = evolver->GetProperties();
		if(props.Priority < 0)
			break;

		evolver->Evolve(mRandom, state, *mParticleSet, startIndex, count, spacing, spacingOffset);
	}
}

void ParticleSystem::Simulate(const ParticleSystemState& state, u32 startIndex, u32 count, bool spacing, float spacingOffset)
{
	ParticleSetData& particles = mParticleSet->GetParticles();
	const float subFrameSpacing = (spacing && count > 0) ? 1.0f / count : 1.0f;
	const u32 endIndex = startIndex + count;

	for(u32 particleIndex = startIndex; particleIndex < endIndex; particleIndex++)
	{
		float timeStep = state.TimeStep;
		if(spacing)
		{
			const u32 localIndex = particleIndex - startIndex;
			const float subFrameOffset = ((float)localIndex + spacingOffset) * subFrameSpacing;
			timeStep *= subFrameOffset;
		}

		particles.Position[particleIndex] += particles.Velocity[particleIndex] * timeStep;
	}
}

void ParticleSystem::PostSimulate(const ParticleSystemState& state, u32 startIndex, u32 count, bool spacing, float spacingOffset)
{
	// Evolve post-simulation
	for(auto& evolver : mEvolvers)
	{
		if(!evolver)
			continue;

		const ParticleEvolverProperties& props = evolver->GetProperties();
		if(props.Priority >= 0)
			continue;

		evolver->Evolve(mRandom, state, *mParticleSet, startIndex, count, spacing, spacingOffset);
	}
}

ecs::ParticleSystem& ParticleSystem::GetFragment()
{
	return GetECSRegistry()->GetComponents<ecs::ParticleSystem>(GetECSEntity());
}

const ecs::ParticleSystem& ParticleSystem::GetFragment() const
{
	return GetECSRegistry()->GetComponents<ecs::ParticleSystem>(GetECSEntity());
}

const TParticleSystemData<false>& ParticleSystem::GetParticleSystemData() const
{
	return GetFragment();
}

void ParticleSystem::MarkRenderProxyDataDirty(ComponentDirtyFlag flag)
{
	if(!SceneObject().IsValid())
		return;

	ecs::Registry* registry = GetECSRegistry();
	ecs::Entity entity = GetECSEntity();

	if(flag == ComponentDirtyFlag::Transform)
	{
		if(!registry->HasAllOf<ecs::ParticleSystemDirty>(entity))
			registry->AddTag<ecs::ParticleSystemTransformDirty>(entity);
	}
	else
		registry->AddTag<ecs::ParticleSystemDirty>(entity);
}

void ParticleSystem::Initialize()
{
	SetShared(B3DStaticGameObjectCast<ParticleSystem>(mThisHandle).GetShared());

	ecs::Registry* registry = GetECSRegistry();
	ecs::Entity entity = GetECSEntity();

	if(!registry->HasAllOf<ecs::ParticleSystem>(entity))
	{
		ecs::ParticleSystem fragmentData;
		registry->AddComponent<ecs::ParticleSystem>(entity, std::move(fragmentData));
	}

	Component::Initialize();
	CoreObject::Initialize();
}

void ParticleSystem::OnCreated()
{
	const SPtr<ParticleScene>& particleScene = SceneObject()->GetScene()->GetParticleScene();
	GetFragment().Id = particleScene->RegisterParticleSystem(this);
	mSeed = rand();
}

void ParticleSystem::OnDestroyed()
{
	ecs::Registry* registry = GetECSRegistry();
	ecs::Entity entity = GetECSEntity();

	// Deallocate only if currently active (has a ParticleSystemId fragment)
	if(registry->HasAllOf<ecs::ParticleSystemId>(entity))
	{
		const SPtr<RendererScene>& rendererScene = SceneObject()->GetScene()->GetRendererScene();
		rendererScene->DeallocateParticleSystemId(*registry, entity);
	}

	registry->RemoveComponents<ecs::ParticleSystemDirty>(entity);
	registry->RemoveComponents<ecs::ParticleSystemTransformDirty>(entity);
	registry->RemoveComponents<ecs::ParticleSystem>(entity);

	const SPtr<ParticleScene>& particleScene = SceneObject()->GetScene()->GetParticleScene();
	particleScene->UnregisterParticleSystem(this);

	CoreObject::Destroy();
}

void ParticleSystem::OnDisabled()
{
	ecs::Registry* registry = GetECSRegistry();
	ecs::Entity entity = GetECSEntity();

	const SPtr<RendererScene>& rendererScene = SceneObject()->GetScene()->GetRendererScene();
	rendererScene->DeallocateParticleSystemId(*registry, entity);

	registry->RemoveComponents<ecs::ParticleSystemDirty>(entity);
	registry->RemoveComponents<ecs::ParticleSystemTransformDirty>(entity);

	Stop();
}

void ParticleSystem::OnEnabled()
{
	if(mPreviewMode)
	{
		Stop();
		mPreviewMode = false;
	}

	ecs::Registry* registry = GetECSRegistry();
	ecs::Entity entity = GetECSEntity();

	const SPtr<RendererScene>& rendererScene = SceneObject()->GetScene()->GetRendererScene();
	rendererScene->AllocateParticleSystemId(*registry, entity);
	registry->AddTag<ecs::ParticleSystemDirty>(entity);

	const SPtr<SceneInstance>& scene = SceneObject()->GetScene();
	if(scene->IsRunning())
		Play();
}

void ParticleSystem::OnSceneChanged(SceneInstance* oldScene, ecs::Entity oldEntity)
{
	ecs::Registry* oldRegistry = oldScene != nullptr ? &oldScene->GetECSRegistry() : nullptr;
	ecs::Registry* registry = GetECSRegistry();
	ecs::Entity entity = GetECSEntity();

	// Deallocate from old scene only if was active
	if(oldRegistry != nullptr && oldRegistry->HasAllOf<ecs::ParticleSystemId>(oldEntity))
		oldScene->GetRendererScene()->DeallocateParticleSystemId(*oldRegistry, oldEntity);

	// Migrate ecs::ParticleSystem fragment to new entity
	if(oldRegistry != nullptr && oldRegistry->HasAllOf<ecs::ParticleSystem>(oldEntity))
	{
		ecs::ParticleSystem fragmentCopy = oldRegistry->GetComponents<ecs::ParticleSystem>(oldEntity);
		registry->AddComponent<ecs::ParticleSystem>(entity, std::move(fragmentCopy));
	}

	// Allocate in new scene only if currently active
	if(GetEnabled())
	{
		const SPtr<RendererScene>& rendererScene = SceneObject()->GetScene()->GetRendererScene();
		rendererScene->AllocateParticleSystemId(*registry, entity);

		registry->AddTag<ecs::ParticleSystemDirty>(entity);
	}
}

void ParticleSystem::OnTransformChanged(TransformChangedFlags flags)
{
	MarkRenderProxyDataDirty(ComponentDirtyFlag::Transform);
}

void ParticleSystem::GetCoreDependencies(Vector<CoreObject*>& dependencies)
{
	const ecs::ParticleSystem& fragment = GetFragment();

	if(fragment.Settings.Mesh.IsLoaded())
		dependencies.push_back(fragment.Settings.Mesh.Get());

	if(fragment.Settings.Material.IsLoaded())
		dependencies.push_back(fragment.Settings.Material.Get());
}

bool ParticleSystem::TogglePreviewMode(bool enabled)
{
	const SPtr<SceneInstance>& scene = SceneObject()->GetScene();
	const bool isRunning = scene->IsRunning();

	if(enabled)
	{
		// Cannot enable preview while running
		if(isRunning)
			return false;

		if(!mPreviewMode)
		{
			Play();
			mPreviewMode = true;
		}

		return true;
	}
	else
	{
		if(!isRunning)
			Stop();

		mPreviewMode = false;
		return false;
	}
}

RTTIType* ParticleSystem::GetRttiStatic()
{
	return ParticleSystemRTTI::Instance();
}

RTTIType* ParticleSystem::GetRtti() const
{
	return ParticleSystem::GetRttiStatic();
}

