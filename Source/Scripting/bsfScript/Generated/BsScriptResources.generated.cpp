//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptResources.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Resources/BsResources.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "Wrappers/BsScriptResource.h"
#include "BsScriptResourceManifest.generated.h"

namespace bs
{
#if !BS_IS_BANSHEE3D
	ScriptResources::onResourceLoadedThunkDef ScriptResources::onResourceLoadedThunk; 
	ScriptResources::onResourceDestroyedThunkDef ScriptResources::onResourceDestroyedThunk; 
	ScriptResources::onResourceModifiedThunkDef ScriptResources::onResourceModifiedThunk; 

	HEvent ScriptResources::onResourceLoadedConn;
	HEvent ScriptResources::onResourceDestroyedConn;
	HEvent ScriptResources::onResourceModifiedConn;

	ScriptResources::ScriptResources(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{
		mGCHandle = MonoUtil::NewWeakGcHandle(managedInstance);
	}

	void ScriptResources::InitRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_Load", (void*)&ScriptResources::InternalLoad);
		metaData.scriptClass->AddInternalCall("Internal_LoadAsync", (void*)&ScriptResources::InternalLoadAsync);
		metaData.scriptClass->AddInternalCall("Internal_LoadFromUuid", (void*)&ScriptResources::InternalLoadFromUuid);
		metaData.scriptClass->AddInternalCall("Internal_Release", (void*)&ScriptResources::InternalRelease);
		metaData.scriptClass->AddInternalCall("Internal_UnloadAllUnused", (void*)&ScriptResources::InternalUnloadAllUnused);
		metaData.scriptClass->AddInternalCall("Internal_UnloadAll", (void*)&ScriptResources::InternalUnloadAll);
		metaData.scriptClass->AddInternalCall("Internal_Save", (void*)&ScriptResources::InternalSave);
		metaData.scriptClass->AddInternalCall("Internal_Save0", (void*)&ScriptResources::InternalSave0);
		metaData.scriptClass->AddInternalCall("Internal_GetDependencies", (void*)&ScriptResources::InternalGetDependencies);
		metaData.scriptClass->AddInternalCall("Internal_IsLoaded", (void*)&ScriptResources::InternalIsLoaded);
		metaData.scriptClass->AddInternalCall("Internal_GetLoadProgress", (void*)&ScriptResources::InternalGetLoadProgress);
		metaData.scriptClass->AddInternalCall("Internal_RegisterResourceManifest", (void*)&ScriptResources::InternalRegisterResourceManifest);
		metaData.scriptClass->AddInternalCall("Internal_UnregisterResourceManifest", (void*)&ScriptResources::InternalUnregisterResourceManifest);
		metaData.scriptClass->AddInternalCall("Internal_GetResourceManifest", (void*)&ScriptResources::InternalGetResourceManifest);
		metaData.scriptClass->AddInternalCall("Internal_GetFilePathFromUuid", (void*)&ScriptResources::InternalGetFilePathFromUuid);
		metaData.scriptClass->AddInternalCall("Internal_GetUuidFromFilePath", (void*)&ScriptResources::InternalGetUuidFromFilePath);

		onResourceLoadedThunk = (onResourceLoadedThunkDef)metaData.scriptClass->GetMethodExact("InternalonResourceLoaded", "RRefBase")->GetThunk();
		onResourceDestroyedThunk = (onResourceDestroyedThunkDef)metaData.scriptClass->GetMethodExact("InternalonResourceDestroyed", "UUID&")->GetThunk();
		onResourceModifiedThunk = (onResourceModifiedThunkDef)metaData.scriptClass->GetMethodExact("InternalonResourceModified", "RRefBase")->GetThunk();
	}

	void ScriptResources::StartUp()
	{
		onResourceLoadedConn = Resources::Instance().onResourceLoaded.Connect(&ScriptResources::onResourceLoaded);
		onResourceDestroyedConn = Resources::Instance().onResourceDestroyed.Connect(&ScriptResources::onResourceDestroyed);
		onResourceModifiedConn = Resources::Instance().onResourceModified.Connect(&ScriptResources::onResourceModified);
	}
	void ScriptResources::ShutDown()
	{
		onResourceLoadedConn.Disconnect();
		onResourceDestroyedConn.Disconnect();
		onResourceModifiedConn.Disconnect();
	}

	void ScriptResources::onResourceLoaded(const ResourceHandle<Resource>& p0)
	{
		MonoObject* tmpp0;
		ScriptRRefBase* scriptp0;
		scriptp0 = ScriptResourceManager::Instance().GetScriptRRef(p0);
		if(scriptp0 != nullptr)
			tmpp0 = scriptp0->GetManagedInstance();
		else
			tmpp0 = nullptr;
		MonoUtil::InvokeThunk(onResourceLoadedThunk, tmpp0);
	}

	void ScriptResources::onResourceDestroyed(const UUID& p0)
	{
		MonoObject* tmpp0;
		tmpp0 = ScriptUUID::Box(p0);
		MonoUtil::InvokeThunk(onResourceDestroyedThunk, tmpp0);
	}

	void ScriptResources::onResourceModified(const ResourceHandle<Resource>& p0)
	{
		MonoObject* tmpp0;
		ScriptRRefBase* scriptp0;
		scriptp0 = ScriptResourceManager::Instance().GetScriptRRef(p0);
		if(scriptp0 != nullptr)
			tmpp0 = scriptp0->GetManagedInstance();
		else
			tmpp0 = nullptr;
		MonoUtil::InvokeThunk(onResourceModifiedThunk, tmpp0);
	}
	MonoObject* ScriptResources::InternalLoad(MonoString* filePath, ResourceLoadFlag loadFlags)
	{
		ResourceHandle<Resource> tmp__output;
		Path tmpfilePath;
		tmpfilePath = MonoUtil::MonoToString(filePath);
		tmp__output = Resources::Instance().Load(tmpfilePath, loadFlags);

		MonoObject* __output;
		ScriptResourceBase* script__output;
		script__output = ScriptResourceManager::Instance().GetScriptResource(tmp__output, true);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	MonoObject* ScriptResources::InternalLoadAsync(MonoString* filePath, ResourceLoadFlag loadFlags)
	{
		ResourceHandle<Resource> tmp__output;
		Path tmpfilePath;
		tmpfilePath = MonoUtil::MonoToString(filePath);
		tmp__output = Resources::Instance().LoadAsync(tmpfilePath, loadFlags);

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().GetScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	MonoObject* ScriptResources::InternalLoadFromUuid(UUID* uuid, bool async, ResourceLoadFlag loadFlags)
	{
		ResourceHandle<Resource> tmp__output;
		tmp__output = Resources::Instance().LoadFromUuid(*uuid, async, loadFlags);

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().GetScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptResources::InternalRelease(MonoObject* resource)
	{
		ResourceHandle<Resource> tmpresource;
		ScriptRRefBase* scriptresource;
		scriptresource = ScriptRRefBase::ToNative(resource);
		if(scriptresource != nullptr)
			tmpresource = static_resource_cast<Resource>(scriptresource->GetHandle());
		Resources::Instance().Release(tmpresource);
	}

	void ScriptResources::InternalUnloadAllUnused()
	{
		Resources::Instance().UnloadAllUnused();
	}

	void ScriptResources::InternalUnloadAll()
	{
		Resources::Instance().UnloadAll();
	}

	void ScriptResources::InternalSave(MonoObject* resource, MonoString* filePath, bool overwrite, bool compress)
	{
		ResourceHandle<Resource> tmpresource;
		ScriptResource* scriptresource;
		scriptresource = ScriptResource::ToNative(resource);
		if(scriptresource != nullptr)
			tmpresource = static_resource_cast<Resource>(scriptresource->GetGenericHandle());
		Path tmpfilePath;
		tmpfilePath = MonoUtil::MonoToString(filePath);
		Resources::Instance().Save(tmpresource, tmpfilePath, overwrite, compress);
	}

	void ScriptResources::InternalSave0(MonoObject* resource, bool compress)
	{
		ResourceHandle<Resource> tmpresource;
		ScriptResource* scriptresource;
		scriptresource = ScriptResource::ToNative(resource);
		if(scriptresource != nullptr)
			tmpresource = static_resource_cast<Resource>(scriptresource->GetGenericHandle());
		Resources::Instance().Save(tmpresource, compress);
	}

	MonoArray* ScriptResources::InternalGetDependencies(MonoString* filePath)
	{
		Vector<UUID> vec__output;
		Path tmpfilePath;
		tmpfilePath = MonoUtil::MonoToString(filePath);
		vec__output = Resources::Instance().GetDependencies(tmpfilePath);

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::Create<ScriptUUID>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, vec__output[i]);
		}
		__output = array__output.GetInternal();

		return __output;
	}

	bool ScriptResources::InternalIsLoaded(UUID* uuid, bool checkInProgress)
	{
		bool tmp__output;
		tmp__output = Resources::Instance().IsLoaded(*uuid, checkInProgress);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	float ScriptResources::InternalGetLoadProgress(MonoObject* resource, bool includeDependencies)
	{
		float tmp__output;
		ResourceHandle<Resource> tmpresource;
		ScriptRRefBase* scriptresource;
		scriptresource = ScriptRRefBase::ToNative(resource);
		if(scriptresource != nullptr)
			tmpresource = static_resource_cast<Resource>(scriptresource->GetHandle());
		tmp__output = Resources::Instance().GetLoadProgress(tmpresource, includeDependencies);

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptResources::InternalRegisterResourceManifest(MonoObject* manifest)
	{
		SPtr<ResourceManifest> tmpmanifest;
		ScriptResourceManifest* scriptmanifest;
		scriptmanifest = ScriptResourceManifest::ToNative(manifest);
		if(scriptmanifest != nullptr)
			tmpmanifest = scriptmanifest->GetInternal();
		Resources::Instance().RegisterResourceManifest(tmpmanifest);
	}

	void ScriptResources::InternalUnregisterResourceManifest(MonoObject* manifest)
	{
		SPtr<ResourceManifest> tmpmanifest;
		ScriptResourceManifest* scriptmanifest;
		scriptmanifest = ScriptResourceManifest::ToNative(manifest);
		if(scriptmanifest != nullptr)
			tmpmanifest = scriptmanifest->GetInternal();
		Resources::Instance().UnregisterResourceManifest(tmpmanifest);
	}

	MonoObject* ScriptResources::InternalGetResourceManifest(MonoString* name)
	{
		SPtr<ResourceManifest> tmp__output;
		String tmpname;
		tmpname = MonoUtil::MonoToString(name);
		tmp__output = Resources::Instance().GetResourceManifest(tmpname);

		MonoObject* __output;
		__output = ScriptResourceManifest::Create(tmp__output);

		return __output;
	}

	bool ScriptResources::InternalGetFilePathFromUuid(UUID* uuid, MonoString** filePath)
	{
		bool tmp__output;
		Path tmpfilePath;
		tmp__output = Resources::Instance().GetFilePathFromUuid(*uuid, tmpfilePath);

		bool __output;
		__output = tmp__output;
		MonoUtil::ReferenceCopy(filePath,  (MonoObject*)MonoUtil::StringToMono(tmpfilePath.ToString()));

		return __output;
	}

	bool ScriptResources::InternalGetUuidFromFilePath(MonoString* path, UUID* uuid)
	{
		bool tmp__output;
		Path tmppath;
		tmppath = MonoUtil::MonoToString(path);
		tmp__output = Resources::Instance().GetUuidFromFilePath(tmppath, *uuid);

		bool __output;
		__output = tmp__output;

		return __output;
	}
#endif
}
