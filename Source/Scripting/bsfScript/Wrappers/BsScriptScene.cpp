//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/BsScriptScene.h"
#include "BsMonoManager.h"
#include "BsMonoClass.h"
#include "BsMonoMethod.h"
#include "BsMonoUtil.h"
#include "Scene/BsSceneManager.h"
#include "Resources/BsResources.h"
#include "Scene/BsPrefab.h"
#include "BsApplication.h"
#include "Scene/BsSceneObject.h"
#include "Renderer/BsCamera.h"
#include "BsScriptGameObjectManager.h"
#include "Resources/BsGameResourceManager.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptPrefab.h"
#include "Wrappers/BsScriptSceneObject.h"
#include "BsScriptObjectManager.h"

namespace bs
{
	HEvent ScriptScene::OnRefreshDomainLoadedConn;
	HEvent ScriptScene::OnRefreshStartedConn;

	UUID ScriptScene::sActiveSceneUUID;
	String ScriptScene::sActiveSceneName;
	bool ScriptScene::sIsGenericPrefab;

#if BS_IS_BANSHEE3D
	ScriptScene::OnUpdateThunkDef ScriptScene::onUpdateThunk;
#endif

	ScriptScene::ScriptScene(MonoObject* instance)
		:ScriptObject(instance)
	{ }

	void ScriptScene::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_GetRoot", (void*)&ScriptScene::InternalGetRoot);
		metaData.scriptClass->AddInternalCall("Internal_GetMainCameraSO", (void*)&ScriptScene::InternalGetMainCameraSo);

#if BS_IS_BANSHEE3D
		metaData.scriptClass->AddInternalCall("Internal_SetActiveScene", (void*)&ScriptScene::InternalSetActiveScene);
		metaData.scriptClass->AddInternalCall("Internal_ClearScene", (void*)&ScriptScene::InternalClearScene);

		MonoMethod* updateMethod = metaData.scriptClass->getMethod("OnUpdate");
		onUpdateThunk = (OnUpdateThunkDef)updateMethod->getThunk();
#endif
	}

	void ScriptScene::StartUp()
	{
		OnRefreshStartedConn = ScriptObjectManager::Instance().onRefreshStarted.connect(&onRefreshStarted);
		OnRefreshDomainLoadedConn = ScriptObjectManager::Instance().onRefreshDomainLoaded.connect(&onRefreshDomainLoaded);
	}

	void ScriptScene::ShutDown()
	{
		OnRefreshStartedConn.disconnect();
		OnRefreshDomainLoadedConn.disconnect();
	}

	void ScriptScene::Update()
	{
#if BS_IS_BANSHEE3D
		MonoUtil::invokeThunk(onUpdateThunk);
#endif
	}

	void ScriptScene::SetActiveScene(const HPrefab& prefab)
	{
		if (prefab.isLoaded(false))
		{
			// If scene replace current root node, otherwise just append to the current root node
			if (prefab->isScene())
				gSceneManager().loadScene(prefab);
			else
			{
				gSceneManager().clearScene();
				prefab->instantiate();
			}
		}
		else
		{
			BS_LOG(Error, Scene, "Attempting to activate a scene that hasn't finished loading yet.");
		}
	}

	void ScriptScene::OnRefreshStarted()
	{
		MonoMethod* uuidMethod = metaData.scriptClass->getMethod("GetSceneUUID");
		if (uuidMethod != nullptr)
			sActiveSceneUUID = ScriptUUID::unbox(uuidMethod->invoke(nullptr, nullptr));

		MonoMethod* nameMethod = metaData.scriptClass->getMethod("GetSceneName");
		if (nameMethod != nullptr)
			sActiveSceneName = MonoUtil::monoToString((MonoString*)nameMethod->invoke(nullptr, nullptr));

		MonoMethod* genericPrefabMethod = metaData.scriptClass->getMethod("GetIsGenericPrefab");
		if (genericPrefabMethod != nullptr)
			sIsGenericPrefab = *(bool*)MonoUtil::unbox(genericPrefabMethod->invoke(nullptr, nullptr));
	}

	void ScriptScene::OnRefreshDomainLoaded()
	{
		MonoMethod* uuidMethod = metaData.scriptClass->getMethod("SetSceneUUID", 1);
		if (uuidMethod != nullptr)
		{
			void* params[1];
			params[0] = ScriptUUID::box(sActiveSceneUUID);

			uuidMethod->invoke(nullptr, params);
		}
			
		MonoMethod* nameMethod = metaData.scriptClass->getMethod("SetSceneName", 1);
		if (nameMethod != nullptr)
		{
			void* params[1];
			params[0] = MonoUtil::stringToMono(sActiveSceneName);

			nameMethod->invoke(nullptr, params);
		}

		MonoMethod* genericPrefabMethod = metaData.scriptClass->getMethod("SetIsGenericPrefab", 1);
		if (genericPrefabMethod != nullptr)
		{
			void* params[1] = { &sIsGenericPrefab };
			genericPrefabMethod->invoke(nullptr, params);
		}
	}

	MonoObject* ScriptScene::InternalGetRoot()
	{
		HSceneObject root = SceneManager::Instance().getMainScene()->getRoot();

		ScriptSceneObject* scriptRoot = ScriptGameObjectManager::Instance().getOrCreateScriptSceneObject(root);
		return scriptRoot->getManagedInstance();
	}

	MonoObject* ScriptScene::InternalGetMainCameraSo()
	{
		SPtr<Camera> camera = gSceneManager().getMainCamera();
		HSceneObject so = gSceneManager().GetActorSOInternal(camera);
		if (so == nullptr)
			return nullptr;

		ScriptSceneObject* cameraSo = ScriptGameObjectManager::Instance().getOrCreateScriptSceneObject(so);
		return cameraSo->getManagedInstance();
	}

#if BS_IS_BANSHEE3D
	void ScriptScene::InternalSetActiveScene(ScriptPrefab* scriptPrefab)
	{
		HPrefab prefab = scriptPrefab->getHandle();
		setActiveScene(prefab);
	}

	void ScriptScene::InternalClearScene()
	{
		gSceneManager().clearScene();
	}
#endif
}
