//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/BsScriptResources.h"
#include "BsMonoManager.h"
#include "BsMonoClass.h"
#include "BsMonoMethod.h"
#include "BsMonoUtil.h"
#include "Resources/BsGameResourceManager.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptResource.h"
#include "BsApplication.h"

namespace bs
{
#if BS_IS_BANSHEE3D
	ScriptResources::ScriptResources(MonoObject* instance)
		:ScriptObject(instance)
	{ }

	void ScriptResources::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_Load", (void*)&ScriptResources::InternalLoad);
		metaData.scriptClass->AddInternalCall("Internal_LoadFromUUID", (void*)&ScriptResources::InternalLoadFromUuid);
		metaData.scriptClass->AddInternalCall("Internal_LoadAsync", (void*)&ScriptResources::InternalLoadAsync);
		metaData.scriptClass->AddInternalCall("Internal_LoadAsyncFromUUID", (void*)&ScriptResources::InternalLoadAsyncFromUuid);
		metaData.scriptClass->AddInternalCall("Internal_UnloadUnused", (void*)&ScriptResources::InternalUnloadUnused);
		metaData.scriptClass->AddInternalCall("Internal_Release", (void*)&ScriptResources::InternalRelease);
		metaData.scriptClass->AddInternalCall("Internal_ReleaseRef", (void*)&ScriptResources::InternalReleaseRef);
		metaData.scriptClass->AddInternalCall("Internal_GetLoadProgress", (void*)&ScriptResources::InternalGetLoadProgress);
	}

	MonoObject* ScriptResources::InternalLoad(MonoString* path, ResourceLoadFlag flags)
	{
		Path nativePath = MonoUtil::monoToString(path);

		ResourceLoadFlags loadFlags = flags;

		if (gApplication().isEditor())
			loadFlags |= ResourceLoadFlag::KeepSourceData;

		HResource resource = GameResourceManager::Instance().load(nativePath, loadFlags, false);
		if (!resource.isLoaded(false))
			return nullptr;

		ScriptResourceBase* scriptResource = ScriptResourceManager::Instance().getScriptResource(resource, true);
		return scriptResource->getManagedInstance();
	}

	MonoObject* ScriptResources::InternalLoadFromUuid(UUID* uuid, ResourceLoadFlag flags)
	{
		ResourceLoadFlags loadFlags = flags;

		if (gApplication().isEditor())
			loadFlags |= ResourceLoadFlag::KeepSourceData;

		HResource resource = gResources().loadFromUUID(*uuid, false, loadFlags);
		if (!resource.isLoaded(false))
			return nullptr;

		ScriptResourceBase* scriptResource = ScriptResourceManager::Instance().getScriptResource(resource, true);
		return scriptResource->getManagedInstance();
	}

	MonoObject* ScriptResources::InternalLoadAsync(MonoString* path, ResourceLoadFlag flags)
	{
		Path nativePath = MonoUtil::monoToString(path);

		ResourceLoadFlags loadFlags = flags;

		if (gApplication().isEditor())
			loadFlags |= ResourceLoadFlag::KeepSourceData;

		HResource resource = GameResourceManager::Instance().load(nativePath, loadFlags, true);
		if (resource == nullptr)
			return nullptr;

		ScriptRRefBase* scriptResource = ScriptResourceManager::Instance().getScriptRRef(resource);
		if(scriptResource != nullptr)
			return scriptResource->getManagedInstance();

		return nullptr;
	}

	MonoObject* ScriptResources::InternalLoadAsyncFromUuid(UUID* uuid, ResourceLoadFlag flags)
	{
		ResourceLoadFlags loadFlags = flags;

		if (gApplication().isEditor())
			loadFlags |= ResourceLoadFlag::KeepSourceData;

		HResource resource = gResources().loadFromUUID(*uuid, true, loadFlags);
		if (resource == nullptr)
			return nullptr;

		ScriptRRefBase* scriptResource = ScriptResourceManager::Instance().getScriptRRef(resource);
		if(scriptResource != nullptr)
			return scriptResource->getManagedInstance();

		return nullptr;
	}

	float ScriptResources::InternalGetLoadProgress(ScriptRRefBase* resource, bool loadDependencies)
	{
		return gResources().getLoadProgress(resource->getHandle(), loadDependencies);
	}

	void ScriptResources::InternalRelease(ScriptResourceBase* resource)
	{
		resource->getGenericHandle().release();
	}

	void ScriptResources::InternalReleaseRef(ScriptRRefBase* resourceRef)
	{
		resourceRef->getHandle().release();
	}

	void ScriptResources::InternalUnloadUnused()
	{
		gResources().unloadAllUnused();
	}
#endif
}
