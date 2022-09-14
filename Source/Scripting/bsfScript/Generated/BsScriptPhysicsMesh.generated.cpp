//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptPhysicsMesh.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Physics/BsPhysicsMesh.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "../../../Foundation/bsfCore/Physics/BsPhysicsMesh.h"
#include "BsScriptRendererMeshData.generated.h"
#include "../Extensions/BsPhysicsMeshEx.h"

namespace bs
{
	ScriptPhysicsMesh::ScriptPhysicsMesh(MonoObject* managedInstance, const ResourceHandle<PhysicsMesh>& value)
		:TScriptResource(managedInstance, value)
	{
	}

	void ScriptPhysicsMesh::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_GetRef", (void*)&ScriptPhysicsMesh::InternalGetRef);
		metaData.scriptClass->AddInternalCall("Internal_getType", (void*)&ScriptPhysicsMesh::InternalGetType);
		metaData.scriptClass->AddInternalCall("Internal_create", (void*)&ScriptPhysicsMesh::InternalCreate);
		metaData.scriptClass->AddInternalCall("Internal_getMeshData", (void*)&ScriptPhysicsMesh::InternalGetMeshData);

	}

	 MonoObject*ScriptPhysicsMesh::CreateInstance()
	{
		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		return metaData.scriptClass->CreateInstance("bool", ctorParams);
	}
	MonoObject* ScriptPhysicsMesh::InternalGetRef(ScriptPhysicsMesh* thisPtr)
	{
		return thisPtr->GetRRef();
	}

	PhysicsMeshType ScriptPhysicsMesh::InternalGetType(ScriptPhysicsMesh* thisPtr)
	{
		PhysicsMeshType tmp__output;
		tmp__output = thisPtr->GetHandle()->GetType();

		PhysicsMeshType __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptPhysicsMesh::InternalCreate(MonoObject* managedInstance, MonoObject* meshData, PhysicsMeshType type)
	{
		SPtr<RendererMeshData> tmpmeshData;
		ScriptRendererMeshData* scriptmeshData;
		scriptmeshData = ScriptRendererMeshData::toNative(meshData);
		if(scriptmeshData != nullptr)
			tmpmeshData = scriptmeshData->getInternal();
		ResourceHandle<PhysicsMesh> instance = PhysicsMeshEx::Create(tmpmeshData, type);
		ScriptResourceManager::Instance().createBuiltinScriptResource(instance, managedInstance);
	}

	MonoObject* ScriptPhysicsMesh::InternalGetMeshData(ScriptPhysicsMesh* thisPtr)
	{
		SPtr<RendererMeshData> tmp__output;
		tmp__output = PhysicsMeshEx::getMeshData(thisPtr->getHandle());

		MonoObject* __output;
		__output = ScriptRendererMeshData::Create(tmp__output);

		return __output;
	}
}
