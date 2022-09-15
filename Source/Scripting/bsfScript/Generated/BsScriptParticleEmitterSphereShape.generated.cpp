//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptParticleEmitterSphereShape.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptPARTICLE_SPHERE_SHAPE_DESC.generated.h"
#include "BsScriptParticleEmitterSphereShape.generated.h"

namespace bs
{
	ScriptParticleEmitterSphereShape::ScriptParticleEmitterSphereShape(MonoObject* managedInstance, const SPtr<ParticleEmitterSphereShape>& value)
		:TScriptReflectable(managedInstance, value)
	{
		mInternal = value;
	}

	void ScriptParticleEmitterSphereShape::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_setOptions", (void*)&ScriptParticleEmitterSphereShape::InternalSetOptions);
		metaData.scriptClass->AddInternalCall("Internal_getOptions", (void*)&ScriptParticleEmitterSphereShape::InternalGetOptions);
		metaData.scriptClass->AddInternalCall("Internal_create", (void*)&ScriptParticleEmitterSphereShape::InternalCreate);
		metaData.scriptClass->AddInternalCall("Internal_create0", (void*)&ScriptParticleEmitterSphereShape::InternalCreate0);

	}

	MonoObject* ScriptParticleEmitterSphereShape::Create(const SPtr<ParticleEmitterSphereShape>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptParticleEmitterSphereShape>()) ScriptParticleEmitterSphereShape(managedInstance, value);
		return managedInstance;
	}
	void ScriptParticleEmitterSphereShape::InternalSetOptions(ScriptParticleEmitterSphereShape* thisPtr, PARTICLE_SPHERE_SHAPE_DESC* options)
	{
		thisPtr->GetInternal()->SetOptions(*options);
	}

	void ScriptParticleEmitterSphereShape::InternalGetOptions(ScriptParticleEmitterSphereShape* thisPtr, PARTICLE_SPHERE_SHAPE_DESC* __output)
	{
		PARTICLE_SPHERE_SHAPE_DESC tmp__output;
		tmp__output = thisPtr->GetInternal()->GetOptions();

		*__output = tmp__output;
	}

	void ScriptParticleEmitterSphereShape::InternalCreate(MonoObject* managedInstance, PARTICLE_SPHERE_SHAPE_DESC* desc)
	{
		SPtr<ParticleEmitterSphereShape> instance = ParticleEmitterSphereShape::Create(*desc);
		new (bs_alloc<ScriptParticleEmitterSphereShape>())ScriptParticleEmitterSphereShape(managedInstance, instance);
	}

	void ScriptParticleEmitterSphereShape::InternalCreate0(MonoObject* managedInstance)
	{
		SPtr<ParticleEmitterSphereShape> instance = ParticleEmitterSphereShape::Create();
		new (bs_alloc<ScriptParticleEmitterSphereShape>())ScriptParticleEmitterSphereShape(managedInstance, instance);
	}
}
