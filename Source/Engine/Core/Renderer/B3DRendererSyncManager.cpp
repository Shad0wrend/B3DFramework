//************************************ B3D Framework - Copyright 2026 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Renderer/B3DRendererSyncManager.h"
#include "CoreObject/B3DRenderThread.h"
#include "Renderer/B3DRendererScene.h"
#include "Scene/B3DSceneManager.h"
#include "Scene/B3DSceneInstance.h"
#include "Scene/B3DGameObjectCollection.h"

using namespace b3d;

RendererSyncManager::RendererSyncManager()
{
	for(u32 allocatorIndex = 0; allocatorIndex < B3DSize(mSyncAllocators); allocatorIndex++)
		mSyncAllocators[allocatorIndex] = B3DNew<FrameAllocator>();
}

RendererSyncManager::~RendererSyncManager()
{
	for(u32 allocatorIndex = 0; allocatorIndex < B3DSize(mSyncAllocators); allocatorIndex++)
		B3DDelete(mSyncAllocators[allocatorIndex]);
}

void RendererSyncManager::SyncToRenderThread(bool swapBuffers)
{
	Lock lock(mSyncDataMutex);

	SyncRead(mSyncAllocators[mActiveFrameAllocatorIndex]);

	GetRenderThread().PostCommand([this] { SyncWrite(); }, "RendererSyncManager::SyncWrite");

	if(swapBuffers)
	{
		mActiveFrameAllocatorIndex = (mActiveFrameAllocatorIndex + 1) % B3DSize(mSyncAllocators);
		mSyncAllocators[mActiveFrameAllocatorIndex]->Clear();
	}
}

void RendererSyncManager::SyncRead(FrameAllocator* allocator)
{
	PerFrameSyncData syncData;
	syncData.Allocator = allocator;

	for(auto& [scenePtr, weakScene] : SceneManager::Instance().GetAllScenes())
	{
		SPtr<SceneInstance> scene = weakScene.lock();
		if(!scene)
			continue;

		ecs::Registry& registry = scene->GetGameObjectCollection()->GetECSRegistry();
		RendererScene* rendererScene = scene->GetRendererScene().get();
		RendererSceneSyncData* sceneSyncData = rendererScene->SyncRead(registry, *allocator);
		if(sceneSyncData != nullptr)
		{
			auto renderScene = B3DGetRenderProxy(rendererScene);
			syncData.SceneData.Add({renderScene.get(), sceneSyncData});
		}
	}

	mPerFrameSyncData.emplace_back(std::move(syncData));
}

void RendererSyncManager::SyncWrite()
{
	PerFrameSyncData syncData;
	{
		Lock lock(mSyncDataMutex);

		if(mPerFrameSyncData.empty())
			return;

		syncData = std::move(mPerFrameSyncData.front());
		mPerFrameSyncData.pop_front();
	}

	for(u32 sceneIndex = 0; sceneIndex < (u32)syncData.SceneData.Size(); ++sceneIndex)
	{
		auto& sceneData = syncData.SceneData[sceneIndex];
		sceneData.RenderScene->SyncWrite(*sceneData.BatchData, *syncData.Allocator);
	}
}
