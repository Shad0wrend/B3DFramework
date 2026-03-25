//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Components/B3DDecal.h"

#include "CoreObject/B3DCoreObjectSync.h"
#include "ECS/B3DRegistry.h"
#include "RTTI/B3DDecalRTTI.h"
#include "Renderer/B3DRendererScene.h"
#include "Material/B3DMaterial.h"
#include "Scene/B3DSceneInstance.h"
#include "Scene/B3DSceneObjectFragments.h"

using namespace b3d;

namespace b3d::ecs
{
	/** Tag indicating a Decal needs to sync all of its properties to its render proxy. */
	struct DecalDirty {};

	/** Tag indicating a Decal needs to sync transform to its render proxy. */
	struct DecalTransformDirty {};
} // namespace b3d::ecs

Bounds b3d::ComputeDecalBounds(const Vector2& size, float maxDistance, const Matrix4& worldTransform)
{
	const Vector2& extents = size * 0.5f;

	AABox localAABB(
		Vector3(-extents.X, -extents.Y, -maxDistance),
		Vector3(extents.X, extents.Y, 0.0f));

	localAABB.TransformAffine(worldTransform);

	return Bounds(localAABB);
}

template<bool IsRenderProxy>
TDecal<IsRenderProxy>::TDecal()
{
	UpdateBounds();
}

template<bool IsRenderProxy>
void TDecal<IsRenderProxy>::SetLayer(u64 layer)
{
	const bool isPowerOfTwo = layer && !((layer - 1) & layer);

	if(!isPowerOfTwo)
	{
		B3D_LOG(Warning, LogRenderer, "Invalid layer provided. Only one layer bit may be set. Ignoring.");
		return;
	}

	this->Layer = layer;
	MarkRenderProxyDataDirty();
}

template<bool IsRenderProxy>
void TDecal<IsRenderProxy>::UpdateBounds()
{
	mBounds = ComputeDecalBounds(this->Size, this->MaxDistance, mWorldTransformMatrix);
}

template <bool IsRenderProxy>
void TDecal<IsRenderProxy>::MarkRenderProxyDataDirty(ComponentDirtyFlag flag)
{
	if constexpr(!IsRenderProxy)
		CoreObject::MarkRenderProxyDataDirty((u32)flag);
}

template <bool IsRenderProxy>
void TDecal<IsRenderProxy>::MarkCoreObjectDependenciesDirty()
{
	if constexpr(!IsRenderProxy)
		CoreObject::MarkDependenciesDirty();
}

template class TDecal<true>;

namespace b3d
{
	B3D_SYNC_BLOCK_BEGIN_CUSTOM(ecs::Decal, FullSyncPacket, render::Decal)
		B3D_SYNC_BLOCK_ENTRY(Size)
		B3D_SYNC_BLOCK_ENTRY(MaxDistance)
		B3D_SYNC_BLOCK_ENTRY(Material)
		B3D_SYNC_BLOCK_ENTRY(Layer)
		B3D_SYNC_BLOCK_ENTRY(LayerMask)
		B3D_SYNC_BLOCK_ENTRY_CUSTOM_SETTER(bool, mActive)
		B3D_SYNC_BLOCK_ENTRY_CUSTOM_SETTER(SPtr<SceneInstance>, mSceneInstance)
		B3D_SYNC_BLOCK_ENTRY_CUSTOM_SETTER(Transform, mTransform)
	B3D_SYNC_BLOCK_END

	B3D_SYNC_BLOCK_BEGIN_CUSTOM(ecs::Decal, TransformSyncPacket, render::Decal)
		B3D_SYNC_BLOCK_ENTRY_CUSTOM_SETTER(Transform, mTransform)
	B3D_SYNC_BLOCK_END

	// ECS sync packets targeting TDecalData<true> (for new DecalProxy path)
	B3D_SYNC_BLOCK_BEGIN_CUSTOM(ecs::Decal, FullSyncPacketECS, TDecalData<true>)
		B3D_SYNC_BLOCK_ENTRY(Size)
		B3D_SYNC_BLOCK_ENTRY(MaxDistance)
		B3D_SYNC_BLOCK_ENTRY(Material)
		B3D_SYNC_BLOCK_ENTRY(Layer)
		B3D_SYNC_BLOCK_ENTRY(LayerMask)
		B3D_SYNC_BLOCK_ENTRY_CUSTOM(Matrix4, WorldTransformMatrix)
		B3D_SYNC_BLOCK_ENTRY_CUSTOM(Matrix4, WorldTransformMatrixWithoutScale)
	B3D_SYNC_BLOCK_END

	B3D_SYNC_BLOCK_BEGIN_CUSTOM(ecs::Decal, TransformSyncPacketECS, TDecalData<true>)
		B3D_SYNC_BLOCK_ENTRY_CUSTOM(Matrix4, WorldTransformMatrix)
		B3D_SYNC_BLOCK_ENTRY_CUSTOM(Matrix4, WorldTransformMatrixWithoutScale)
	B3D_SYNC_BLOCK_END

	struct DecalFullUpdateChannel : TRendererObjectECSSyncChannel
	<
		DecalFullUpdateChannel,
		ecs::Decal::FullSyncPacketECS,
		ecs::DecalDirty,
		ecs::Decal, ecs::WorldTransform, ecs::DecalId
	>
	{
		void Write(DecalObjectStorageBase& storage, FrameAllocator& allocator)
		{
			Vector<PackedRendererId, StdFrameAlloc<PackedRendererId>> renderStatesToCreate(&allocator);
			Vector<PackedRendererId, StdFrameAlloc<PackedRendererId>> renderStatesToDestroy(&allocator);

			WritePackets(storage, allocator, [&renderStatesToCreate, &renderStatesToDestroy, &storage](ecs::Decal::FullSyncPacketECS& packet, PackedRendererId rendererId)
			{
				render::DecalProxy& proxy = storage.GetDecalProxy(rendererId);

				bool wasRegistered = proxy.mRendererId != kInvalidPackedRendererId;
				proxy.mRendererId = rendererId;
				packet.ApplySyncData(&proxy.mData);

				proxy.mWorldTransformMatrix = packet.WorldTransformMatrix;
				proxy.mWorldTransformMatrixWithoutScale = packet.WorldTransformMatrixWithoutScale;
				proxy.mBounds = ComputeDecalBounds(proxy.mData.Size, proxy.mData.MaxDistance, proxy.mWorldTransformMatrix);

				if(wasRegistered)
					renderStatesToDestroy.push_back(rendererId);

				renderStatesToCreate.push_back(rendererId);
			});

			if(!renderStatesToDestroy.empty())
				storage.DestroyRenderState(renderStatesToDestroy);

			if(!renderStatesToCreate.empty())
				storage.CreateRenderState(renderStatesToCreate);
		}

		void CreateAndPopulatePacket(ecs::Decal& fragment, ecs::WorldTransform& transform, ecs::DecalId& id, FrameAllocator& allocator)
		{
			auto& packet = CreatePacket(id.Id, fragment, allocator, 0);
			packet.WorldTransformMatrix = transform.GetMatrix();
			packet.WorldTransformMatrixWithoutScale = Matrix4::TRS(
				transform.GetPosition(), transform.GetRotation(), Vector3::kOne);
		}
	};

	struct DecalTransformUpdateChannel : TRendererObjectECSSyncChannel
	<
		DecalTransformUpdateChannel,
		ecs::Decal::TransformSyncPacketECS,
		ecs::DecalTransformDirty,
		ecs::Decal, ecs::WorldTransform, ecs::DecalId
	>
	{
		void Write(DecalObjectStorageBase& storage, FrameAllocator& allocator)
		{
			Vector<PackedRendererId, StdFrameAlloc<PackedRendererId>> renderStatesToUpdate(&allocator);

			WritePackets(storage, allocator, [&renderStatesToUpdate, &storage](ecs::Decal::TransformSyncPacketECS& packet, PackedRendererId rendererId)
			{
				render::DecalProxy& proxy = storage.GetDecalProxy(rendererId);
				proxy.mWorldTransformMatrix = packet.WorldTransformMatrix;
				proxy.mWorldTransformMatrixWithoutScale = packet.WorldTransformMatrixWithoutScale;
				proxy.mBounds = ComputeDecalBounds(proxy.mData.Size, proxy.mData.MaxDistance, proxy.mWorldTransformMatrix);

				renderStatesToUpdate.push_back(rendererId);
			});

			if(!renderStatesToUpdate.empty())
				storage.UpdateRenderState(renderStatesToUpdate);
		}

		void CreateAndPopulatePacket(ecs::Decal& fragment, ecs::WorldTransform& transform, ecs::DecalId& id, FrameAllocator& allocator)
		{
			auto& packet = CreatePacket(id.Id, fragment, allocator, 0);
			packet.WorldTransformMatrix = transform.GetMatrix();
			packet.WorldTransformMatrixWithoutScale = Matrix4::TRS(
				transform.GetPosition(), transform.GetRotation(), Vector3::kOne);
		}
	};

	using DecalSyncBatch = TRendererObjectECSSyncBatch<DecalFullUpdateChannel, DecalTransformUpdateChannel>;
}

// ecs::Decal fragment access

ecs::Decal& Decal::GetFragment()
{
	return GetECSRegistry()->GetComponents<ecs::Decal>(GetECSEntity());
}

const ecs::Decal& Decal::GetFragment() const
{
	return GetECSRegistry()->GetComponents<ecs::Decal>(GetECSEntity());
}

const TDecalData<false>& Decal::GetDecalData() const
{
	return GetFragment();
}

// b3d::Decal setters

void Decal::SetSize(const Vector2& size)
{
	GetFragment().Size = Vector2::Max(Vector2::kZero, size);
	MarkRenderProxyDataDirty();
	UpdateBounds();
}

void Decal::SetMaterial(const HMaterial& material)
{
	GetFragment().Material = material;
	MarkRenderProxyDataDirty();
}

void Decal::SetMaxDistance(float distance)
{
	GetFragment().MaxDistance = Math::Max(0.0f, distance);
	MarkRenderProxyDataDirty();
	UpdateBounds();
}

void Decal::SetLayerMask(u32 mask)
{
	GetFragment().LayerMask = mask;
	MarkRenderProxyDataDirty();
}

void Decal::SetLayer(u64 layer)
{
	const bool isPowerOfTwo = layer && !((layer - 1) & layer);

	if(!isPowerOfTwo)
	{
		B3D_LOG(Warning, LogRenderer, "Invalid layer provided. Only one layer bit may be set. Ignoring.");
		return;
	}

	GetFragment().Layer = layer;
	MarkRenderProxyDataDirty();
}

void Decal::UpdateBounds()
{
	const ecs::Decal& fragment = GetFragment();
	mBounds = ComputeDecalBounds(fragment.Size, fragment.MaxDistance, mWorldTransformMatrix);
}

void Decal::MarkRenderProxyDataDirty(ComponentDirtyFlag flag)
{
	CoreObject::MarkRenderProxyDataDirty((u32)flag);
}

// b3d::Decal lifecycle

Decal::Decal(const HSceneObject& parent)
	: Component(parent)
{
	SetFlag(ComponentFlag::AlwaysRun, true);
	SetName("Decal");
	mNotifyFlags = TCF_Transform;
}

Decal::Decal()
	: Decal(nullptr)
{ }

void Decal::Initialize()
{
	SetShared(B3DStaticGameObjectCast<Decal>(mThisHandle).GetShared());

	ecs::Registry* registry = GetECSRegistry();
	ecs::Entity entity = GetECSEntity();

	if(!registry->HasAllOf<ecs::Decal>(entity))
	{
		ecs::Decal fragmentData;
		registry->AddComponent<ecs::Decal>(entity, std::move(fragmentData));
	}

	Component::Initialize();
	CoreObject::Initialize();
}

void Decal::OnEnabled()
{
	MarkRenderProxyDataDirty();
}

void Decal::OnDisabled()
{
	MarkRenderProxyDataDirty();
}

void Decal::OnDestroyed()
{
	ecs::Registry* registry = GetECSRegistry();
	ecs::Entity entity = GetECSEntity();

	registry->RemoveComponents<ecs::Decal>(entity);

	CoreObject::Destroy();
}

void Decal::OnSceneChanged(SceneInstance* oldScene, ecs::Entity oldEntity)
{
	ecs::Registry* oldRegistry = oldScene != nullptr ? &oldScene->GetECSRegistry() : nullptr;
	ecs::Registry* registry = GetECSRegistry();
	ecs::Entity entity = GetECSEntity();

	if(oldRegistry != nullptr && oldRegistry->HasAllOf<ecs::Decal>(oldEntity))
	{
		ecs::Decal fragmentCopy = oldRegistry->GetComponents<ecs::Decal>(oldEntity);
		registry->AddComponent<ecs::Decal>(entity, std::move(fragmentCopy));
	}
}

void Decal::OnTransformChanged(TransformChangedFlags flags)
{
	const Transform& transform = SceneObject()->GetTransform();
	mWorldTransformMatrix = transform.GetMatrix();
	mWorldTransformMatrixWithoutScale = Matrix4::TRS(transform.GetPosition(), transform.GetRotation(), Vector3::kOne);

	UpdateBounds();
	MarkRenderProxyDataDirty(ComponentDirtyFlag::Transform);
}

SPtr<render::RenderProxy> Decal::CreateRenderProxy() const
{
	const SPtr<SceneInstance>& scene = SceneObject()->GetScene();

	render::Decal* renderProxy = new(B3DAllocate<render::Decal>()) render::Decal(B3DGetRenderProxy(scene));
	SPtr<render::Decal> renderProxyShared = B3DMakeSharedFromExisting<render::Decal>(renderProxy);
	renderProxyShared->SetShared(renderProxyShared);

	return renderProxyShared;
}

void Decal::GetCoreDependencies(Vector<CoreObject*>& dependencies)
{
	const auto& material = GetFragment().Material;
	if(material.IsLoaded())
		dependencies.push_back(material.Get());
}

RenderProxySyncPacket* Decal::CreateRenderProxySyncPacket(FrameAllocator& allocator, u32 flags)
{
	ecs::Decal& fragment = GetFragment();

	if(flags != (u32)ComponentDirtyFlag::Transform)
	{
		ecs::Decal::FullSyncPacket* const syncPacket = allocator.Construct<ecs::Decal::FullSyncPacket>(fragment, allocator, flags);
		syncPacket->mActive = GetEnabled();
		syncPacket->mSceneInstance = B3DGetRenderProxy(SceneObject()->GetScene());
		syncPacket->mTransform = SceneObject()->GetTransform();

		return syncPacket;
	}
	else
	{
		ecs::Decal::TransformSyncPacket* const syncPacket = allocator.Construct<ecs::Decal::TransformSyncPacket>(fragment, allocator, flags);
		syncPacket->mTransform = SceneObject()->GetTransform();

		return syncPacket;
	}
}

RTTIType* Decal::GetRttiStatic()
{
	return DecalRTTI::Instance();
}

RTTIType* Decal::GetRtti() const
{
	return Decal::GetRttiStatic();
}

namespace b3d::ecs
{
	RTTIType* Decal::GetRttiStatic() { return ECSDecalRTTI::Instance(); }
	RTTIType* Decal::GetRtti() const { return Decal::GetRttiStatic(); }
}

namespace b3d { namespace render
{
Decal::Decal(const SPtr<SceneInstance>& scene)
	: mSceneInstance(scene)
{}

Decal::~Decal()
{
	const SPtr<RendererScene>& rendererScene = mSceneInstance->GetRendererScene();
	rendererScene->UnregisterDecal(this);
}

void Decal::Initialize()
{
	UpdateBounds();

	const SPtr<RendererScene>& rendererScene = mSceneInstance->GetRendererScene();
	rendererScene->RegisterDecal(this);

	RenderProxy::Initialize();
}

void Decal::SyncFromCoreObject(const CoreSyncData& data, FrameAllocator& allocator)
{
	RenderProxySyncPacket* const syncPacket = data.GetSyncPacket();
	if(syncPacket == nullptr)
		return;

	bool previousActiveState = mActive;
	syncPacket->ApplySyncData(this);

	mWorldTransformMatrix = mTransform.GetMatrix();
	mWorldTransformMatrixWithoutScale = Matrix4::TRS(mTransform.GetPosition(), mTransform.GetRotation(), Vector3::kOne);

	UpdateBounds();

	const SPtr<RendererScene>& rendererScene = mSceneInstance->GetRendererScene();

	const u32 flags = syncPacket->Flags;
	const u32 updateEverythingFlag = ~(u32)ComponentDirtyFlag::Transform;
	if((flags & updateEverythingFlag) != 0)
	{
		if(previousActiveState != mActive)
		{
			if(mActive)
				rendererScene->RegisterDecal(this);
			else
				rendererScene->UnregisterDecal(this);
		}
		else
		{
			rendererScene->UnregisterDecal(this);
			rendererScene->RegisterDecal(this);
		}
	}
	else
	{
		if(mActive)
			rendererScene->UpdateDecal(this);
	}
}
}}

// DecalObjectStorageBase

void* DecalObjectStorageBase::SyncRead(ecs::Registry& registry, FrameAllocator& allocator)
{
	return DecalSyncBatch::Read(*this, registry, allocator);
}

void DecalObjectStorageBase::SyncWrite(void* rawData, FrameAllocator& allocator)
{
	DecalSyncBatch::Write(*this, rawData, allocator);
}
