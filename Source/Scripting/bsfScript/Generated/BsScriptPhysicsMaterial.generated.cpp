//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptPhysicsMaterial.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Physics/BsPhysicsMaterial.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "../../../Foundation/bsfCore/Physics/BsPhysicsMaterial.h"

namespace bs
{
	ScriptPhysicsMaterial::ScriptPhysicsMaterial(MonoObject* managedInstance, const ResourceHandle<PhysicsMaterial>& value)
		:TScriptResource(managedInstance, value)
	{
	}

	void ScriptPhysicsMaterial::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_GetRef", (void*)&ScriptPhysicsMaterial::InternalGetRef);
		metaData.scriptClass->AddInternalCall("Internal_setStaticFriction", (void*)&ScriptPhysicsMaterial::InternalSetStaticFriction);
		metaData.scriptClass->AddInternalCall("Internal_getStaticFriction", (void*)&ScriptPhysicsMaterial::InternalGetStaticFriction);
		metaData.scriptClass->AddInternalCall("Internal_setDynamicFriction", (void*)&ScriptPhysicsMaterial::InternalSetDynamicFriction);
		metaData.scriptClass->AddInternalCall("Internal_getDynamicFriction", (void*)&ScriptPhysicsMaterial::InternalGetDynamicFriction);
		metaData.scriptClass->AddInternalCall("Internal_setRestitutionCoefficient", (void*)&ScriptPhysicsMaterial::InternalSetRestitutionCoefficient);
		metaData.scriptClass->AddInternalCall("Internal_getRestitutionCoefficient", (void*)&ScriptPhysicsMaterial::InternalGetRestitutionCoefficient);
		metaData.scriptClass->AddInternalCall("Internal_create", (void*)&ScriptPhysicsMaterial::InternalCreate);

	}

	 MonoObject*ScriptPhysicsMaterial::CreateInstance()
	{
		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		return metaData.scriptClass->CreateInstance("bool", ctorParams);
	}
	MonoObject* ScriptPhysicsMaterial::InternalGetRef(ScriptPhysicsMaterial* thisPtr)
	{
		return thisPtr->getRRef();
	}

	void ScriptPhysicsMaterial::InternalSetStaticFriction(ScriptPhysicsMaterial* thisPtr, float value)
	{
		thisPtr->getHandle()->setStaticFriction(value);
	}

	float ScriptPhysicsMaterial::InternalGetStaticFriction(ScriptPhysicsMaterial* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getHandle()->getStaticFriction();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptPhysicsMaterial::InternalSetDynamicFriction(ScriptPhysicsMaterial* thisPtr, float value)
	{
		thisPtr->getHandle()->setDynamicFriction(value);
	}

	float ScriptPhysicsMaterial::InternalGetDynamicFriction(ScriptPhysicsMaterial* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getHandle()->getDynamicFriction();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptPhysicsMaterial::InternalSetRestitutionCoefficient(ScriptPhysicsMaterial* thisPtr, float value)
	{
		thisPtr->getHandle()->setRestitutionCoefficient(value);
	}

	float ScriptPhysicsMaterial::InternalGetRestitutionCoefficient(ScriptPhysicsMaterial* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getHandle()->getRestitutionCoefficient();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptPhysicsMaterial::InternalCreate(MonoObject* managedInstance, float staticFriction, float dynamicFriction, float restitution)
	{
		ResourceHandle<PhysicsMaterial> instance = PhysicsMaterial::Create(staticFriction, dynamicFriction, restitution);
		ScriptResourceManager::Instance().createBuiltinScriptResource(instance, managedInstance);
	}
}
