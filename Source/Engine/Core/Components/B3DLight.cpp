//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Components/B3DLight.h"

#include "CoreObject/B3DCoreObjectSync.h"
#include "ECS/B3DRegistry.h"
#include "Image/B3DColor.h"
#include "RTTI/B3DLightRTTI.h"
#include "Renderer/B3DRendererScene.h"
#include "Scene/B3DSceneInstance.h"
#include "Scene/B3DSceneObjectFragments.h"

using namespace b3d;

namespace b3d::ecs
{
	/** Tag indicating a Light needs to sync all of its properties to its render proxy. */
	struct LightDirty {};

	/** Tag indicating a Light needs to sync transform to its render proxy. */
	struct LightTransformDirty {};
} // namespace b3d::ecs

namespace b3d
{
	B3D_SYNC_BLOCK_BEGIN_CUSTOM(ecs::Light, FullSyncPacket, TLightData<true>)
		B3D_SYNC_BLOCK_ENTRY(Type)
		B3D_SYNC_BLOCK_ENTRY(CastsShadows)
		B3D_SYNC_BLOCK_ENTRY(LightColor)
		B3D_SYNC_BLOCK_ENTRY(AttRadius)
		B3D_SYNC_BLOCK_ENTRY(SourceRadius)
		B3D_SYNC_BLOCK_ENTRY(Intensity)
		B3D_SYNC_BLOCK_ENTRY(SpotAngle)
		B3D_SYNC_BLOCK_ENTRY(SpotFalloffAngle)
		B3D_SYNC_BLOCK_ENTRY(AutoAttenuation)
		B3D_SYNC_BLOCK_ENTRY(Bounds)
		B3D_SYNC_BLOCK_ENTRY(ShadowBias)
		B3D_SYNC_BLOCK_ENTRY_CUSTOM(Transform, TransformData)
		B3D_SYNC_BLOCK_ENTRY_CUSTOM(bool, ActiveState)
		B3D_SYNC_BLOCK_ENTRY_CUSTOM(SPtr<SceneInstance>, SceneInstanceData)
	B3D_SYNC_BLOCK_END

	B3D_SYNC_BLOCK_BEGIN_CUSTOM(ecs::Light, TransformSyncPacket, TLightData<true>)
		B3D_SYNC_BLOCK_ENTRY_CUSTOM(Transform, TransformData)
	B3D_SYNC_BLOCK_END

	struct LightSyncBatch
	{
		TBatchSyncBuffer<ecs::Light::FullSyncPacket> Full;
		TBatchSyncBuffer<ecs::Light::TransformSyncPacket> Transform;

		RendererObjectStorage::FlushedCommands Commands;
	};
}

template<bool IsRenderProxy>
TLight<IsRenderProxy>::TLight()
{ }

template<bool IsRenderProxy>
TLight<IsRenderProxy>::TLight(LightType type, Color color, float intensity, float attRadius, float srcRadius, bool castsShadows, Degree spotAngle, Degree spotFalloffAngle)
	: TLightData<IsRenderProxy>{ type, castsShadows, color, attRadius, srcRadius, intensity, spotAngle, spotFalloffAngle, false, Sphere(), 0.5f }
{ }

template<bool IsRenderProxy>
void TLight<IsRenderProxy>::SetUseAutoAttenuation(bool enabled)
{
	this->AutoAttenuation = enabled;

	if(enabled)
		UpdateAttenuationRange();

	MarkRenderProxyDataDirty();
}

template<bool IsRenderProxy>
void TLight<IsRenderProxy>::SetAttenuationRadius(float radius)
{
	if(this->AutoAttenuation)
		return;

	this->AttRadius = radius;
	MarkRenderProxyDataDirty();
	UpdateBounds();
}

template<bool IsRenderProxy>
void TLight<IsRenderProxy>::SetSourceRadius(float radius)
{
	this->SourceRadius = radius;

	if(this->AutoAttenuation)
		UpdateAttenuationRange();

	MarkRenderProxyDataDirty();
}

template<bool IsRenderProxy>
void TLight<IsRenderProxy>::SetIntensity(float intensity)
{
	this->Intensity = intensity;

	if(this->AutoAttenuation)
		UpdateAttenuationRange();

	MarkRenderProxyDataDirty();
}

template <bool IsRenderProxy>
void TLightData<IsRenderProxy>::ComputeBounds(const Transform& transform)
{
	switch(Type)
	{
	case LightType::Directional:
		Bounds = Sphere(transform.GetPosition(), std::numeric_limits<float>::infinity());
		break;
	case LightType::Radial:
		Bounds = Sphere(transform.GetPosition(), AttRadius);
		break;
	case LightType::Spot:
		{
			// Note: We could use the formula for calculating the circumcircle of a triangle (after projecting the cone),
			// but the radius of the sphere is the same as in the formula we use here, yet it is much simpler with no need
			// to calculate multiple determinants. Neither are good approximations when cone angle is wide.
			Vector3 offset(0, 0, AttRadius * 0.5f);

			// Direction along the edge of the cone, on the YZ plane (doesn't matter if we used XZ instead)
			Degree angle = Math::Clamp(SpotAngle * 0.5f, Degree(-89), Degree(89));
			Vector3 coneDir(0, Math::Tan(angle) * AttRadius, AttRadius);

			// Distance between the "corner" of the cone and our center, must be the radius (provided the center is at
			// the middle of the range)
			float radius = (offset - coneDir).Length();

			Vector3 center = transform.GetPosition() - transform.GetRotation().Rotate(offset);
			Bounds = Sphere(center, radius);
		}
		break;
	default:
		break;
	}
}

template void TLightData<true>::ComputeBounds(const Transform& transform);
template void TLightData<false>::ComputeBounds(const Transform& transform);

template <bool IsRenderProxy>
float TLightData<IsRenderProxy>::ComputeLuminance() const
{
	float radiusSquared = SourceRadius * SourceRadius;

	switch(Type)
	{
	case LightType::Radial:
		if(SourceRadius > 0.0f)
			return Intensity / (4 * radiusSquared * Math::kPi); // Luminous flux -> luminance
		else
			return Intensity / (4 * Math::kPi); // Luminous flux -> luminous intensity
	case LightType::Spot:
		{
			if(SourceRadius > 0.0f)
				return Intensity / (radiusSquared * Math::kPi); // Luminous flux -> luminance
			else
			{
				// Note: Consider using the simpler conversion I / PI to match with the area-light conversion
				float cosTotalAngle = Math::Cos(SpotAngle);
				float cosFalloffAngle = Math::Cos(SpotFalloffAngle);

				// Luminous flux -> luminous intensity
				return Intensity / (Math::kTwoPi * (1.0f - (cosFalloffAngle + cosTotalAngle) * 0.5f));
			}
		}
	case LightType::Directional:
		if(SourceRadius > 0.0f)
		{
			// Use cone solid angle formulae to calculate disc solid angle
			float solidAngle = Math::kTwoPi * (1 - cos(SourceRadius * Math::kDeG2Rad));
			return Intensity / solidAngle; // Illuminance -> luminance
		}
		else
			return Intensity; // In luminance units by default
	default:
		return 0.0f;
	}
}

template float TLightData<true>::ComputeLuminance() const;
template float TLightData<false>::ComputeLuminance() const;

template <bool IsRenderProxy>
void TLightData<IsRenderProxy>::ComputeAttenuationRange(const Transform& transform)
{
	// Value to which intensity needs to drop in order for the light contribution to fade out to zero
	const float minAttenuation = 0.2f;

	if(SourceRadius > 0.0f)
	{
		// Inverse of the attenuation formula for area lights:
		//   a = I / (1 + (2/r) * d + (1/r^2) * d^2
		// Where r is the source radius, and d is the distance from the light. As derived here:
		//   https://imdoingitwrong.wordpress.com/2011/01/31/light-attenuation/

		float luminousFlux = Intensity;

		float a = sqrt(minAttenuation);
		AttRadius = (SourceRadius * (sqrt(luminousFlux - a))) / a;
	}
	else // Based on the basic inverse square distance formula
	{
		float luminousIntensity = Intensity;

		float a = minAttenuation;
		AttRadius = sqrt(std::max(0.0f, luminousIntensity / a));
	}

	ComputeBounds(transform);
}

template void TLightData<true>::ComputeAttenuationRange(const Transform& transform);
template void TLightData<false>::ComputeAttenuationRange(const Transform& transform);

template<bool IsRenderProxy>
void TLight<IsRenderProxy>::UpdateBounds()
{
	this->ComputeBounds(this->GetTransform());
}

template<bool IsRenderProxy>
void TLight<IsRenderProxy>::UpdateAttenuationRange()
{
	this->ComputeAttenuationRange(this->GetTransform());
}

template <bool IsRenderProxy>
void TLight<IsRenderProxy>::MarkRenderProxyDataDirty(ComponentDirtyFlag flag)
{
	if constexpr(!IsRenderProxy)
		CoreObject::MarkRenderProxyDataDirty((u32)flag);
}

template <bool IsRenderProxy>
const Transform& TLight<IsRenderProxy>::GetTransform() const
{
	return static_cast<const render::Light*>(this)->GetWorldTransform();
}

template class TLight<true>;

// ecs::Light fragment access

ecs::Light& Light::GetFragment()
{
	return GetECSRegistry()->GetComponents<ecs::Light>(GetECSEntity());
}

const ecs::Light& Light::GetFragment() const
{
	return GetECSRegistry()->GetComponents<ecs::Light>(GetECSEntity());
}

const TLightData<false>& Light::GetLightData() const
{
	return GetFragment();
}

// b3d::Light setters

void Light::SetType(LightType type)
{
	GetFragment().Type = type;
	CoreObject::MarkRenderProxyDataDirty((u32)ComponentDirtyFlag::Everything);
	UpdateBounds();
}

void Light::SetCastsShadow(bool castsShadow)
{
	GetFragment().CastsShadows = castsShadow;
	CoreObject::MarkRenderProxyDataDirty((u32)ComponentDirtyFlag::Everything);
}

void Light::SetShadowBias(float bias)
{
	GetFragment().ShadowBias = bias;
	CoreObject::MarkRenderProxyDataDirty((u32)ComponentDirtyFlag::Everything);
}

void Light::SetColor(const Color& color)
{
	GetFragment().LightColor = color;
	CoreObject::MarkRenderProxyDataDirty((u32)ComponentDirtyFlag::Everything);
}

void Light::SetAttenuationRadius(float radius)
{
	ecs::Light& fragment = GetFragment();
	if(fragment.AutoAttenuation)
		return;

	fragment.AttRadius = radius;
	CoreObject::MarkRenderProxyDataDirty((u32)ComponentDirtyFlag::Everything);
	UpdateBounds();
}

void Light::SetSourceRadius(float radius)
{
	ecs::Light& fragment = GetFragment();
	fragment.SourceRadius = radius;

	if(fragment.AutoAttenuation)
		UpdateAttenuationRange();

	CoreObject::MarkRenderProxyDataDirty((u32)ComponentDirtyFlag::Everything);
}

void Light::SetUseAutoAttenuation(bool enabled)
{
	ecs::Light& fragment = GetFragment();
	fragment.AutoAttenuation = enabled;

	if(enabled)
		UpdateAttenuationRange();

	CoreObject::MarkRenderProxyDataDirty((u32)ComponentDirtyFlag::Everything);
}

void Light::SetIntensity(float intensity)
{
	ecs::Light& fragment = GetFragment();
	fragment.Intensity = intensity;

	if(fragment.AutoAttenuation)
		UpdateAttenuationRange();

	CoreObject::MarkRenderProxyDataDirty((u32)ComponentDirtyFlag::Everything);
}

void Light::SetSpotAngle(const Degree& spotAngle)
{
	GetFragment().SpotAngle = spotAngle;
	CoreObject::MarkRenderProxyDataDirty((u32)ComponentDirtyFlag::Everything);
	UpdateBounds();
}

void Light::SetSpotFalloffAngle(const Degree& spotFallofAngle)
{
	GetFragment().SpotFalloffAngle = spotFallofAngle;
	CoreObject::MarkRenderProxyDataDirty((u32)ComponentDirtyFlag::Everything);
	UpdateBounds();
}

float Light::GetLuminance() const
{
	return GetFragment().ComputeLuminance();
}

void Light::UpdateBounds()
{
	GetFragment().ComputeBounds(SceneObject()->GetTransform());
}

void Light::UpdateAttenuationRange()
{
	GetFragment().ComputeAttenuationRange(SceneObject()->GetTransform());
}

// b3d::Light lifecycle

Light::Light(const HSceneObject& parent)
	: Component(parent)
{
	SetFlag(ComponentFlag::AlwaysRun, true);
	SetName("Light");
}

Light::Light()
	: Light(nullptr)
{ }

SPtr<render::RenderProxy> Light::CreateRenderProxy() const
{
	const ecs::Light& fragment = GetFragment();
	const SPtr<SceneInstance>& scene = SceneObject()->GetScene();

	render::Light* renderProxy = new(B3DAllocate<render::Light>()) render::Light(
		B3DGetRenderProxy(scene), fragment.Type, fragment.LightColor, fragment.Intensity,
		fragment.AttRadius, fragment.SourceRadius, fragment.CastsShadows,
		fragment.SpotAngle, fragment.SpotFalloffAngle);
	SPtr<render::Light> renderProxyShared = B3DMakeSharedFromExisting<render::Light>(renderProxy);
	renderProxyShared->SetShared(renderProxyShared);

	return renderProxyShared;
}

RenderProxySyncPacket* Light::CreateRenderProxySyncPacket(FrameAllocator& allocator, u32 flags)
{
	ecs::Light& fragment = GetFragment();

	if(flags != (u32)ComponentDirtyFlag::Transform)
	{
		ecs::Light::FullSyncPacket* const syncPacket = allocator.Construct<ecs::Light::FullSyncPacket>(fragment, allocator, flags);
		syncPacket->TransformData = SceneObject()->GetTransform();
		syncPacket->ActiveState = GetEnabled();
		syncPacket->SceneInstanceData = B3DGetRenderProxy(SceneObject()->GetScene());

		return syncPacket;
	}
	else
	{
		ecs::Light::TransformSyncPacket* const syncPacket = allocator.Construct<ecs::Light::TransformSyncPacket>(fragment, allocator, flags);
		syncPacket->TransformData = SceneObject()->GetTransform();

		return syncPacket;
	}
}

void Light::Initialize()
{
	SetShared(B3DStaticGameObjectCast<Light>(mThisHandle).GetShared());

	ecs::Registry* registry = GetECSRegistry();
	ecs::Entity entity = GetECSEntity();

	if(!registry->HasAllOf<ecs::Light>(entity))
	{
		ecs::Light fragmentData;
		registry->AddComponent<ecs::Light>(entity, std::move(fragmentData));
	}

	Component::Initialize();
	CoreObject::Initialize();

	UpdateAttenuationRange();
}

void Light::OnCreated()
{
	UpdateBounds();
}

void Light::OnEnabled()
{
	CoreObject::MarkRenderProxyDataDirty((u32)ComponentDirtyFlag::Everything);
}

void Light::OnDisabled()
{
	CoreObject::MarkRenderProxyDataDirty((u32)ComponentDirtyFlag::Everything);
}

void Light::OnDestroyed()
{
	GetECSRegistry()->RemoveComponents<ecs::Light>(GetECSEntity());

	CoreObject::Destroy();
}

void Light::OnSceneChanged(SceneInstance* oldScene, ecs::Entity oldEntity)
{
	ecs::Registry* oldRegistry = oldScene != nullptr ? &oldScene->GetECSRegistry() : nullptr;
	ecs::Registry* registry = GetECSRegistry();
	ecs::Entity entity = GetECSEntity();

	// Migrate ecs::Light fragment to new entity
	if(oldRegistry != nullptr && oldRegistry->HasAllOf<ecs::Light>(oldEntity))
	{
		ecs::Light fragmentCopy = oldRegistry->GetComponents<ecs::Light>(oldEntity);
		registry->AddComponent<ecs::Light>(entity, std::move(fragmentCopy));
	}

	CoreObject::MarkRenderProxyDataDirty((u32)ComponentDirtyFlag::Everything);
}

void Light::OnTransformChanged(TransformChangedFlags flags)
{
	UpdateBounds();

	CoreObject::MarkRenderProxyDataDirty((u32)ComponentDirtyFlag::Transform);
}

RTTIType* Light::GetRttiStatic()
{
	return LightRTTI::Instance();
}

RTTIType* Light::GetRtti() const
{
	return Light::GetRttiStatic();
}

namespace b3d::ecs
{
	RTTIType* Light::GetRttiStatic() { return ECSLightRTTI::Instance(); }
	RTTIType* Light::GetRtti() const { return Light::GetRttiStatic(); }
}

namespace b3d { namespace render
{
const u32 Light::kLightConeSideCount = 20;
const u32 Light::kLightConeSliceCount = 10;

Light::Light(const SPtr<SceneInstance>& scene, LightType type, Color color, float intensity, float attRadius, float srcRadius, bool castsShadows, Degree spotAngle, Degree spotFalloffAngle)
	: TLight(type, color, intensity, attRadius, srcRadius, castsShadows, spotAngle, spotFalloffAngle), mRendererId(0), mSceneInstance(scene)
{
	UpdateAttenuationRange();
}

Light::~Light()
{
	const SPtr<RendererScene>& rendererScene = mSceneInstance->GetRendererScene();
	rendererScene->UnregisterLight(this);
}

void Light::Initialize()
{
	UpdateBounds();

	const SPtr<RendererScene>& rendererScene = mSceneInstance->GetRendererScene();
	rendererScene->RegisterLight(this);

	RenderProxy::Initialize();
}

void Light::SyncFromCoreObject(const CoreSyncData& data, FrameAllocator& allocator)
{
	RenderProxySyncPacket* const syncPacket = data.GetSyncPacket();
	if(syncPacket == nullptr)
		return;

	const u32 flags = syncPacket->Flags;
	const u32 updateEverythingFlag = ~(u32)ComponentDirtyFlag::Transform;

	if((flags & updateEverythingFlag) != 0)
	{
		auto* fullPacket = static_cast<ecs::Light::FullSyncPacket*>(syncPacket);

		bool previousActiveState = mActive;
		LightType previousType = Type;

		fullPacket->ApplySyncData(static_cast<TLightData<true>*>(this));
		mTransform = fullPacket->TransformData;
		mActive = fullPacket->ActiveState;
		mSceneInstance = fullPacket->SceneInstanceData;

		UpdateBounds();

		const SPtr<RendererScene>& rendererScene = mSceneInstance->GetRendererScene();

		if(previousActiveState != mActive)
		{
			if(mActive)
				rendererScene->RegisterLight(this);
			else
			{
				LightType currentType = Type;
				Type = previousType;
				rendererScene->UnregisterLight(this);
				Type = currentType;
			}
		}
		else
		{
			LightType currentType = Type;
			Type = previousType;
			rendererScene->UnregisterLight(this);
			Type = currentType;

			rendererScene->RegisterLight(this);
		}
	}
	else
	{
		auto* transformPacket = static_cast<ecs::Light::TransformSyncPacket*>(syncPacket);
		mTransform = transformPacket->TransformData;

		UpdateBounds();

		if(mActive)
		{
			const SPtr<RendererScene>& rendererScene = mSceneInstance->GetRendererScene();
			rendererScene->UpdateLight(this);
		}
	}
}
}}

// LightObjectStorageBase

RendererObjectApplyAction LightObjectStorageBase::ApplyPacket(ecs::Light::FullSyncPacket& packet, render::LightProxy& proxy, PackedRendererId rendererId)
{
	bool wasRegistered = proxy.mRendererId != kInvalidPackedRendererId;
	proxy.mRendererId = rendererId;
	packet.ApplySyncData(&proxy.mData);
	proxy.mTransform = packet.TransformData;
	proxy.mData.ComputeBounds(proxy.mTransform);
	proxy.mBounds = proxy.mData.Bounds;

	return wasRegistered ? RendererObjectApplyAction::Reregister : RendererObjectApplyAction::Register;
}

void* LightObjectStorageBase::SyncRead(ecs::Registry& registry, FrameAllocator& allocator)
{
	FlushedCommands flushedCommands = FlushCommands(allocator);
	const u32 commandCount = (u32)flushedCommands.Deallocations.Size() + (u32)flushedCommands.Allocations.Size();

	auto* fullStorage = registry.TryGetStorage<ecs::LightDirty>();
	auto* transformStorage = registry.TryGetStorage<ecs::LightTransformDirty>();

	u32 fullCount = fullStorage ? (u32)fullStorage->Size() : 0;
	u32 transformCount = transformStorage ? (u32)transformStorage->Size() : 0;

	if(fullCount == 0 && transformCount == 0 && commandCount == 0)
		return nullptr;

	auto* batchData = allocator.Construct<LightSyncBatch>();
	batchData->Commands = flushedCommands;

	if(fullCount > 0)
	{
		batchData->Full.Allocate(allocator, fullCount);

		auto view = registry.CreateView<ecs::LightDirty, ecs::Light, ecs::WorldTransform, ecs::LightId>();
		view.SetLeadingType<ecs::LightDirty>();

		for(auto entity : view)
		{
#if B3D_BUILD_TYPE_DEVELOPMENT
			B3D_ASSERT(!registry.HasAllOf<ecs::TransformDirty>(entity) && "WorldTransform is stale during SyncRead — TransformSystem must flush before renderer sync");
#endif
			auto& lightData = view.Get<ecs::Light>(entity);
			auto& worldTransform = view.Get<ecs::WorldTransform>(entity);
			RendererId objectId = view.Get<ecs::LightId>(entity).Id;

			auto& packet = batchData->Full.Add(objectId, lightData, allocator, 0);
			packet.TransformData = worldTransform;
		}
	}

	if(transformCount > 0)
	{
		batchData->Transform.Allocate(allocator, transformCount);

		auto view = registry.CreateView<ecs::LightTransformDirty, ecs::Light, ecs::WorldTransform, ecs::LightId>(ecs::TExcludedTypes<ecs::LightDirty>{});
		view.SetLeadingType<ecs::LightTransformDirty>();

		for(auto entity : view)
		{
#if B3D_BUILD_TYPE_DEVELOPMENT
			B3D_ASSERT(!registry.HasAllOf<ecs::TransformDirty>(entity) && "WorldTransform is stale during SyncRead — TransformSystem must flush before renderer sync");
#endif
			auto& lightData = view.Get<ecs::Light>(entity);
			auto& worldTransform = view.Get<ecs::WorldTransform>(entity);
			RendererId objectId = view.Get<ecs::LightId>(entity).Id;

			auto& packet = batchData->Transform.Add(objectId, lightData, allocator, 0);
			packet.TransformData = worldTransform;
		}
	}

	registry.ClearStorage<ecs::LightDirty>();
	registry.ClearStorage<ecs::LightTransformDirty>();

	return batchData;
}

void LightObjectStorageBase::SyncWrite(void* rawData, FrameAllocator& allocator)
{
	LightSyncBatch* batch = static_cast<LightSyncBatch*>(rawData);

	const FlushedCommands& commands = batch->Commands;

	if(commands.Deallocations.Size() > 0 || commands.Allocations.Size() > 0)
		ApplyCommands(commands, allocator);

	// Upper-bound counts for batch arrays
	const u32 fullUpdateCount = batch->Full.Count;
	const u32 transformUpdateCount = batch->Transform.Count;

	// Allocate batch arrays from the FrameAllocator
	PackedRendererId* createRenderStateList = nullptr;
	u32 createRenderStateCount = 0;

	PackedRendererId* destroyRenderStateList = nullptr;
	u32 destroyRenderStateCount = 0;

	PackedRendererId* updateRenderStateList = nullptr;
	u32 updateRenderStateCount = 0;

	if(fullUpdateCount > 0)
	{
		createRenderStateList = reinterpret_cast<PackedRendererId*>(allocator.AllocateAligned(sizeof(PackedRendererId) * fullUpdateCount, alignof(PackedRendererId)));
		destroyRenderStateList = reinterpret_cast<PackedRendererId*>(allocator.AllocateAligned(sizeof(PackedRendererId) * fullUpdateCount, alignof(PackedRendererId)));
	}

	if(transformUpdateCount > 0)
		updateRenderStateList = reinterpret_cast<PackedRendererId*>(allocator.AllocateAligned(sizeof(PackedRendererId) * transformUpdateCount, alignof(PackedRendererId)));

	// Apply full sync packets, collect render state create/destroy actions
	batch->Full.Each([this, createRenderStateList, &createRenderStateCount, destroyRenderStateList, &destroyRenderStateCount](ecs::Light::FullSyncPacket& packet, RendererId objectId)
	{
		PackedRendererId rendererId = GetPackedRendererId(objectId);
		if(rendererId == kInvalidPackedRendererId)
			return;

		RendererObjectApplyAction action = ApplyPacket(packet, mLightProxies[rendererId], rendererId);
		switch(action)
		{
		case RendererObjectApplyAction::Register:
			createRenderStateList[createRenderStateCount++] = rendererId;
			break;
		case RendererObjectApplyAction::Reregister:
			destroyRenderStateList[destroyRenderStateCount++] = rendererId;
			createRenderStateList[createRenderStateCount++] = rendererId;
			break;
		}
	});

	// Apply transform packets, collect render update actions
	batch->Transform.Each([&](ecs::Light::TransformSyncPacket& packet, RendererId objectId)
	{
		PackedRendererId rendererId = GetPackedRendererId(objectId);
		if(rendererId == kInvalidPackedRendererId)
			return;

		render::LightProxy& proxy = mLightProxies[rendererId];
		proxy.mTransform = packet.TransformData;
		proxy.mData.ComputeBounds(proxy.mTransform);
		proxy.mBounds = proxy.mData.Bounds;

		updateRenderStateList[updateRenderStateCount++] = rendererId;
	});

	// Update render state in batches
	if(destroyRenderStateCount > 0)
		DestroyRenderState(TArrayView<const PackedRendererId>(destroyRenderStateList, destroyRenderStateCount));

	if(createRenderStateCount > 0)
		CreateRenderState(TArrayView<const PackedRendererId>(createRenderStateList, createRenderStateCount));

	if(updateRenderStateCount > 0)
		UpdateRenderState(TArrayView<const PackedRendererId>(updateRenderStateList, updateRenderStateCount));

	if(updateRenderStateList)
		allocator.Free(reinterpret_cast<u8*>(updateRenderStateList));

	if(destroyRenderStateList)
		allocator.Free(reinterpret_cast<u8*>(destroyRenderStateList));

	if(createRenderStateList)
		allocator.Free(reinterpret_cast<u8*>(createRenderStateList));

	batch->Full.Free(allocator);
	batch->Transform.Free(allocator);

	allocator.Destruct(batch);
}
