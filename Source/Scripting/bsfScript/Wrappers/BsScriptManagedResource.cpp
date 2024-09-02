//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/BsScriptManagedResource.h"
#include "BsScriptResourceManager.h"
#include "Serialization/BsScriptAssemblyManager.h"
#include "BsScriptMeta.h"
#include "BsMonoClass.h"
#include "BsManagedResource.h"
#include "Resources/BsResources.h"
#include "BsMonoUtil.h"

using namespace bs;
ScriptManagedResource::ScriptManagedResource(const HManagedResource& nativeObject, MonoObject* scriptObject)
	: TScriptResourceWrapper(nativeObject, scriptObject)
{
	RegisterEvents();
}

void ScriptManagedResource::SetupScriptBindings()
{
	sInteropMetaData.ScriptClass->AddInternalCall("Internal_CreateInstance", (void*)&ScriptManagedResource::InternalCreateInstance);
}

void ScriptManagedResource::InternalCreateInstance(MonoObject* scriptObject)
{
	HManagedResource resource = ManagedResource::Create(scriptObject);
}

void ScriptManagedResource::CreateAndBindScriptObject()
{
	B3D_ENSURE(GetScriptObject() == nullptr);

	if(!IsNativeObjectValid())
		return;

	HManagedResource resource = GetNativeObjectAsHandle();
	SPtr<ManagedSerializableObjectInfo> objectInformation;
	MonoObject* const scriptObject = resource->CreateScriptObject(objectInformation);

	if(scriptObject != nullptr)
	{
		CreateStrongScriptObjectHandle(scriptObject);
		BindSelfToScriptObject(scriptObject);
	}

	resource->BindToScriptObject(objectInformation);
}

void ScriptManagedResource::RecreateScriptObjectAfterScriptReload()
{
	CreateAndBindScriptObject();
}

Optional<ScriptObjectReloadPersistentData> ScriptManagedResource::BackupDataBeforeScriptReload()
{
	if(!IsNativeObjectValid())
		return { };

	HManagedResource managedResource = GetNativeObjectAsHandle();

	ScriptObjectReloadPersistentData backupData;
	backupData.Data = managedResource->Backup();

	return backupData;
}

void ScriptManagedResource::RestoreDataAfterScriptReload(const ScriptObjectReloadPersistentData& data)
{
	HManagedResource managedResource = GetNativeObjectAsHandle();

	ResourceBackupData resourceBackup = AnyCast<ResourceBackupData>(data.Data);
	managedResource->Restore(resourceBackup);

	MonoObject* instance = MonoUtil::GetObjectFromGcHandle(mGCHandle);

	// If we could not find resource type after refresh, treat it as if it was destroyed
	if(instance == nullptr)
		OnManagedInstanceDeletedInternal(false);
}

MonoObject* ScriptManagedResource::CreateManagedInstanceInternal(bool construct)
{
	SPtr<ManagedSerializableObjectInfo> currentObjInfo = nullptr;

	// See if this type even still exists
	if(!ScriptAssemblyManager::Instance().GetSerializableObjectInfo(mNamespace, mType, currentObjInfo))
		return nullptr;

	MonoObject* instance = currentObjInfo->ScriptClass->CreateInstance(construct);
	mGCHandle = MonoUtil::NewGcHandle(instance, false);

	return instance;
}

void ScriptManagedResource::OnManagedInstanceDeletedInternal(bool assemblyRefresh)
{
	mGCHandle = 0;

	if(!assemblyRefresh || mResource->IsDestroyed())
	{
		// The only way this method should be reachable is when Resource::unload is called, which means the resource
		// has had to been already freed. Even if all managed instances are released ManagedResource itself holds the last
		// instance which is only freed on unload().
		// Note: During domain unload this could get called even if not all instances are released, but ManagedResourceManager
		// should make sure all instances are unloaded before that happens.
		B3D_ASSERT(mResource == nullptr || !mResource.IsLoaded());

		ScriptResourceManager::Instance().DestroyScriptResource(this);
	}
}
