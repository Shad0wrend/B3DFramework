//************************************ B3D Framework - Copyright 2025 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Renderer/B3DRendererScene.h"
#include "Allocators/B3DFrameAllocator.h"
#include "Components/B3DRenderable.h"
#include "B3DRenderer.h"

namespace b3d
{
	RendererScene::~RendererScene() = default;

	SPtr<RendererScene> RendererScene::Create()
	{
		RendererScene* rendererScene = new (B3DAllocate<RendererScene>()) RendererScene();
		SPtr<RendererScene> rendererSceneShared = B3DMakeSharedFromExisting(rendererScene);
		rendererSceneShared->SetShared(rendererSceneShared);
		rendererSceneShared->Initialize();

		return rendererSceneShared;
	}

	void RendererScene::Initialize()
	{
		CoreObject::Initialize();

		SPtr<render::RendererScene> renderProxy = B3DGetRenderProxy(this);
		mRenderableStorage = renderProxy->GetRenderableStorage();
	}

	SlotId RendererScene::AllocateRenderableSlot(ecs::Entity entity)
	{
		return mRenderableStorage->AllocateSlot(entity);
	}

	void RendererScene::DeallocateRenderableSlot(ecs::Entity entity, ecs::Registry& registry)
	{
		mRenderableStorage->DeallocateSlot(entity, registry);
	}

	SPtr<render::RenderProxy> RendererScene::CreateRenderProxy() const
	{
		return render::GetRenderer()->CreateScene();
	}

	RendererSceneSyncData* RendererScene::SyncRead(ecs::Registry& registry, FrameAllocator& allocator)
	{
		RendererSceneSyncData* batch = nullptr;

		if(mRenderableStorage != nullptr)
		{
			void* renderableBatchData = mRenderableStorage->SyncRead(registry, allocator);
			if(renderableBatchData != nullptr)
			{
				if(batch == nullptr)
					batch = allocator.Construct<RendererSceneSyncData>();

				batch->RenderableBatchData = renderableBatchData;
			}
		}

		return batch;
	}

	namespace render
	{
		void RendererScene::SyncWrite(RendererSceneSyncData& batchData, FrameAllocator& allocator)
		{
			if(batchData.RenderableBatchData != nullptr)
				mRenderableStorage->SyncWrite(batchData.RenderableBatchData, allocator);
		}

		void RendererScene::UpdateCombinedRendererExtensionsIfNeeded(const Set<RendererExtension*, RendererExtension::SortFunction>& globalRendererExtensions, bool forceUpdate)
		{
			if(!forceUpdate && !mCombinedRendererExtensionsDirty)
				return;

			mCombinedRendererExtensions.clear();

			for(const auto& entry : globalRendererExtensions)
				mCombinedRendererExtensions.insert(entry);

			for(const auto& entry : mRendererExtensions)
				mCombinedRendererExtensions.insert(entry);

			mCombinedRendererExtensionsDirty = false;
		}
	}
}
