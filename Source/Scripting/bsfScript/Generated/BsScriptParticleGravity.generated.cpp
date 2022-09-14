//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptParticleGravity.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptPARTICLE_GRAVITY_DESC.generated.h"
#include "BsScriptParticleGravity.generated.h"

namespace bs
{
	ScriptParticleGravity::ScriptParticleGravity(MonoObject* managedInstance, const SPtr<ParticleGravity>& value)
		:TScriptReflectable(managedInstance, value)
	{
		mInternal = value;
	}

	void ScriptParticleGravity::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_setOptions", (void*)&ScriptParticleGravity::Internal_setOptions);
		metaData.scriptClass->addInternalCall("Internal_getOptions", (void*)&ScriptParticleGravity::Internal_getOptions);
		metaData.scriptClass->AddInternalCall("Internal_create", (void*)&ScriptParticleGravity::InternalCreate);
		metaData.scriptClass->addInternalCall("Internal_create0", (void*)&ScriptParticleGravity::Internal_create0);

	}

	MonoObject* ScriptParticleGravity::Create(const SPtr<ParticleGravity>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptParticleGravity>()) ScriptParticleGravity(managedInstance, value);
		return managedInstance;
	}
	void ScriptParticleGravity::InternalSetOptions(ScriptParticleGravity* thisPtr, PARTICLE_GRAVITY_DESC* options)
	{
		thisPtr->getInternal()->setOptions(*options);
	}

	void ScriptParticleGravity::InternalGetOptions(ScriptParticleGravity* thisPtr, PARTICLE_GRAVITY_DESC* __output)
	{
		PARTICLE_GRAVITY_DESC tmp__output;
		tmp__output = thisPtr->getInternal()->getOptions();

		*__output = tmp__output;
	}

	void ScriptParticleGravity::InternalCreate(MonoObject* managedInstance, PARTICLE_GRAVITY_DESC* desc)
	{
		SPtr<ParticleGravity> instance = ParticleGravity::Create(*desc);
		new (bs_alloc<ScriptParticleGravity>())ScriptParticleGravity(managedInstance, instance);
	}

	void ScriptParticleGravity::InternalCreate0(MonoObject* managedInstance)
	{
		SPtr<ParticleGravity> instance = ParticleGravity::Create();
		new (bs_alloc<ScriptParticleGravity>())ScriptParticleGravity(managedInstance, instance);
	}
}
