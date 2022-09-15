//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptParticleForce.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptPARTICLE_FORCE_DESC.generated.h"
#include "BsScriptParticleForce.generated.h"

namespace bs
{
	ScriptParticleForce::ScriptParticleForce(MonoObject* managedInstance, const SPtr<ParticleForce>& value)
		:TScriptReflectable(managedInstance, value)
	{
		mInternal = value;
	}

	void ScriptParticleForce::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_setOptions", (void*)&ScriptParticleForce::Internal_setOptions);
		metaData.scriptClass->addInternalCall("Internal_getOptions", (void*)&ScriptParticleForce::Internal_getOptions);
		metaData.scriptClass->AddInternalCall("Internal_create", (void*)&ScriptParticleForce::InternalCreate);
		metaData.scriptClass->addInternalCall("Internal_create0", (void*)&ScriptParticleForce::Internal_create0);

	}

	MonoObject* ScriptParticleForce::Create(const SPtr<ParticleForce>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptParticleForce>()) ScriptParticleForce(managedInstance, value);
		return managedInstance;
	}
	void ScriptParticleForce::InternalSetOptions(ScriptParticleForce* thisPtr, __PARTICLE_FORCE_DESCInterop* options)
	{
		PARTICLE_FORCE_DESC tmpoptions;
		tmpoptions = ScriptPARTICLE_FORCE_DESC::fromInterop(*options);
		thisPtr->GetInternal()->SetOptions(tmpoptions);
	}

	void ScriptParticleForce::InternalGetOptions(ScriptParticleForce* thisPtr, __PARTICLE_FORCE_DESCInterop* __output)
	{
		PARTICLE_FORCE_DESC tmp__output;
		tmp__output = thisPtr->GetInternal()->GetOptions();

		__PARTICLE_FORCE_DESCInterop interop__output;
		interop__output = ScriptPARTICLE_FORCE_DESC::toInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptPARTICLE_FORCE_DESC::getMetaData()->scriptClass->GetInternalClassInternal());
	}

	void ScriptParticleForce::InternalCreate(MonoObject* managedInstance, __PARTICLE_FORCE_DESCInterop* desc)
	{
		PARTICLE_FORCE_DESC tmpdesc;
		tmpdesc = ScriptPARTICLE_FORCE_DESC::fromInterop(*desc);
		SPtr<ParticleForce> instance = ParticleForce::Create(tmpdesc);
		new (bs_alloc<ScriptParticleForce>())ScriptParticleForce(managedInstance, instance);
	}

	void ScriptParticleForce::InternalCreate0(MonoObject* managedInstance)
	{
		SPtr<ParticleForce> instance = ParticleForce::Create();
		new (bs_alloc<ScriptParticleForce>())ScriptParticleForce(managedInstance, instance);
	}
}
