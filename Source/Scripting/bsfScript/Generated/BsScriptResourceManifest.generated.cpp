//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptResourceManifest.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptResourceManifest.generated.h"

namespace bs
{
#if !BS_IS_BANSHEE3D
	ScriptResourceManifest::ScriptResourceManifest(MonoObject* managedInstance, const SPtr<ResourceManifest>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptResourceManifest::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_getName", (void*)&ScriptResourceManifest::InternalGetName);
		metaData.scriptClass->AddInternalCall("Internal_registerResource", (void*)&ScriptResourceManifest::InternalRegisterResource);
		metaData.scriptClass->AddInternalCall("Internal_unregisterResource", (void*)&ScriptResourceManifest::InternalUnregisterResource);
		metaData.scriptClass->AddInternalCall("Internal_uuidToFilePath", (void*)&ScriptResourceManifest::InternalUuidToFilePath);
		metaData.scriptClass->AddInternalCall("Internal_filePathToUUID", (void*)&ScriptResourceManifest::InternalFilePathToUUID);
		metaData.scriptClass->AddInternalCall("Internal_uuidExists", (void*)&ScriptResourceManifest::InternalUuidExists);
		metaData.scriptClass->AddInternalCall("Internal_filePathExists", (void*)&ScriptResourceManifest::InternalFilePathExists);
		metaData.scriptClass->AddInternalCall("Internal_save", (void*)&ScriptResourceManifest::InternalSave);
		metaData.scriptClass->AddInternalCall("Internal_load", (void*)&ScriptResourceManifest::InternalLoad);
		metaData.scriptClass->AddInternalCall("Internal_create", (void*)&ScriptResourceManifest::InternalCreate);

	}

	MonoObject* ScriptResourceManifest::Create(const SPtr<ResourceManifest>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptResourceManifest>()) ScriptResourceManifest(managedInstance, value);
		return managedInstance;
	}
	MonoString* ScriptResourceManifest::InternalGetName(ScriptResourceManifest* thisPtr)
	{
		String tmp__output;
		tmp__output = thisPtr->GetInternal()->GetName();

		MonoString* __output;
		__output = MonoUtil::stringToMono(tmp__output);

		return __output;
	}

	void ScriptResourceManifest::InternalRegisterResource(ScriptResourceManifest* thisPtr, UUID* uuid, MonoString* filePath)
	{
		Path tmpfilePath;
		tmpfilePath = MonoUtil::MonoToString(filePath);
		thisPtr->GetInternal()->registerResource(*uuid, tmpfilePath);
	}

	void ScriptResourceManifest::InternalUnregisterResource(ScriptResourceManifest* thisPtr, UUID* uuid)
	{
		thisPtr->GetInternal()->unregisterResource(*uuid);
	}

	bool ScriptResourceManifest::InternalUuidToFilePath(ScriptResourceManifest* thisPtr, UUID* uuid, MonoString** filePath)
	{
		bool tmp__output;
		Path tmpfilePath;
		tmp__output = thisPtr->GetInternal()->uuidToFilePath(*uuid, tmpfilePath);

		bool __output;
		__output = tmp__output;
		MonoUtil::referenceCopy(filePath,  (MonoObject*)MonoUtil::stringToMono(tmpfilePath.toString()));

		return __output;
	}

	bool ScriptResourceManifest::InternalFilePathToUUID(ScriptResourceManifest* thisPtr, MonoString* filePath, UUID* outUUID)
	{
		bool tmp__output;
		Path tmpfilePath;
		tmpfilePath = MonoUtil::MonoToString(filePath);
		tmp__output = thisPtr->GetInternal()->filePathToUUID(tmpfilePath, *outUUID);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptResourceManifest::InternalUuidExists(ScriptResourceManifest* thisPtr, UUID* uuid)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->uuidExists(*uuid);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptResourceManifest::InternalFilePathExists(ScriptResourceManifest* thisPtr, MonoString* filePath)
	{
		bool tmp__output;
		Path tmpfilePath;
		tmpfilePath = MonoUtil::MonoToString(filePath);
		tmp__output = thisPtr->GetInternal()->filePathExists(tmpfilePath);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptResourceManifest::InternalSave(MonoObject* manifest, MonoString* path, MonoString* relativePath)
	{
		SPtr<ResourceManifest> tmpmanifest;
		ScriptResourceManifest* scriptmanifest;
		scriptmanifest = ScriptResourceManifest::toNative(manifest);
		if(scriptmanifest != nullptr)
			tmpmanifest = scriptmanifest->GetInternal();
		Path tmppath;
		tmppath = MonoUtil::MonoToString(path);
		Path tmprelativePath;
		tmprelativePath = MonoUtil::MonoToString(relativePath);
		ResourceManifest::save(tmpmanifest, tmppath, tmprelativePath);
	}

	MonoObject* ScriptResourceManifest::InternalLoad(MonoString* path, MonoString* relativePath)
	{
		SPtr<ResourceManifest> tmp__output;
		Path tmppath;
		tmppath = MonoUtil::MonoToString(path);
		Path tmprelativePath;
		tmprelativePath = MonoUtil::MonoToString(relativePath);
		tmp__output = ResourceManifest::load(tmppath, tmprelativePath);

		MonoObject* __output;
		__output = ScriptResourceManifest::Create(tmp__output);

		return __output;
	}

	void ScriptResourceManifest::InternalCreate(MonoObject* managedInstance, MonoString* name)
	{
		String tmpname;
		tmpname = MonoUtil::MonoToString(name);
		SPtr<ResourceManifest> instance = ResourceManifest::Create(tmpname);
		new (bs_alloc<ScriptResourceManifest>())ScriptResourceManifest(managedInstance, instance);
	}
#endif
}
