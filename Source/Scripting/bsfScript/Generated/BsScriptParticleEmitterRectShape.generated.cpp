//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptParticleEmitterRectShape.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptPARTICLE_RECT_SHAPE_DESC.generated.h"
#include "BsScriptParticleEmitterRectShape.generated.h"

namespace bs
{
	ScriptParticleEmitterRectShape::ScriptParticleEmitterRectShape(MonoObject* managedInstance, const SPtr<ParticleEmitterRectShape>& value)
		:TScriptReflectable(managedInstance, value)
	{
		mInternal = value;
	}

	void ScriptParticleEmitterRectShape::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_setOptions", (void*)&ScriptParticleEmitterRectShape::InternalSetOptions);
		metaData.scriptClass->AddInternalCall("Internal_getOptions", (void*)&ScriptParticleEmitterRectShape::InternalGetOptions);
		metaData.scriptClass->AddInternalCall("Internal_create", (void*)&ScriptParticleEmitterRectShape::InternalCreate);
		metaData.scriptClass->AddInternalCall("Internal_create0", (void*)&ScriptParticleEmitterRectShape::InternalCreate0);

	}

	MonoObject* ScriptParticleEmitterRectShape::Create(const SPtr<ParticleEmitterRectShape>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptParticleEmitterRectShape>()) ScriptParticleEmitterRectShape(managedInstance, value);
		return managedInstance;
	}
	void ScriptParticleEmitterRectShape::InternalSetOptions(ScriptParticleEmitterRectShape* thisPtr, __PARTICLE_RECT_SHAPE_DESCInterop* options)
	{
		PARTICLE_RECT_SHAPE_DESC tmpoptions;
		tmpoptions = ScriptPARTICLE_RECT_SHAPE_DESC::FromInterop(*options);
		thisPtr->GetInternal()->SetOptions(tmpoptions);
	}

	void ScriptParticleEmitterRectShape::InternalGetOptions(ScriptParticleEmitterRectShape* thisPtr, __PARTICLE_RECT_SHAPE_DESCInterop* __output)
	{
		PARTICLE_RECT_SHAPE_DESC tmp__output;
		tmp__output = thisPtr->GetInternal()->GetOptions();

		__PARTICLE_RECT_SHAPE_DESCInterop interop__output;
		interop__output = ScriptPARTICLE_RECT_SHAPE_DESC::toInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptPARTICLE_RECT_SHAPE_DESC::getMetaData()->scriptClass->GetInternalClassInternal());
	}

	void ScriptParticleEmitterRectShape::InternalCreate(MonoObject* managedInstance, __PARTICLE_RECT_SHAPE_DESCInterop* desc)
	{
		PARTICLE_RECT_SHAPE_DESC tmpdesc;
		tmpdesc = ScriptPARTICLE_RECT_SHAPE_DESC::fromInterop(*desc);
		SPtr<ParticleEmitterRectShape> instance = ParticleEmitterRectShape::Create(tmpdesc);
		new (bs_alloc<ScriptParticleEmitterRectShape>())ScriptParticleEmitterRectShape(managedInstance, instance);
	}

	void ScriptParticleEmitterRectShape::InternalCreate0(MonoObject* managedInstance)
	{
		SPtr<ParticleEmitterRectShape> instance = ParticleEmitterRectShape::Create();
		new (bs_alloc<ScriptParticleEmitterRectShape>())ScriptParticleEmitterRectShape(managedInstance, instance);
	}
}
