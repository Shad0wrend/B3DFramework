//********************************* B3D Framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptParticleEmitterCircleShape.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptPARTICLE_CIRCLE_SHAPE_DESC.generated.h"
#include "BsScriptParticleEmitterCircleShape.generated.h"

namespace b3d
{
	ScriptParticleEmitterCircleShape::ScriptParticleEmitterCircleShape(const SPtr<ParticleEmitterCircleShape>& nativeObject)
		:TScriptReflectableWrapper(nativeObject)
	{
		RegisterEvents();
	}

	ScriptParticleEmitterCircleShape::~ScriptParticleEmitterCircleShape()
	{
		UnregisterEvents();
	}

	void ScriptParticleEmitterCircleShape::SetupScriptBindings()
	{
		sInteropMetaData.ScriptClass->AddInternalCall("Internal_SetOptions", (void*)&ScriptParticleEmitterCircleShape::InternalSetOptions);
		sInteropMetaData.ScriptClass->AddInternalCall("Internal_GetOptions", (void*)&ScriptParticleEmitterCircleShape::InternalGetOptions);
		sInteropMetaData.ScriptClass->AddInternalCall("Internal_Create", (void*)&ScriptParticleEmitterCircleShape::InternalCreate);
		sInteropMetaData.ScriptClass->AddInternalCall("Internal_Create0", (void*)&ScriptParticleEmitterCircleShape::InternalCreate0);

	}

	MonoObject* ScriptParticleEmitterCircleShape::CreateScriptObject(bool construct)
	{
		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		if(construct)
			return sInteropMetaData.ScriptClass->CreateInstance("bool", ctorParams);

		return sInteropMetaData.ScriptClass->CreateInstance(false);
	}
	void ScriptParticleEmitterCircleShape::InternalSetOptions(ScriptParticleEmitterCircleShape* self, __PARTICLE_CIRCLE_SHAPE_DESCInterop* options)
	{
		if(!self->IsNativeObjectValid())
			return;

		ParticleCircleShapeSettings tmpoptions;
		tmpoptions = ScriptParticleCircleShapeOptions::FromInterop(*options);
		static_cast<ParticleEmitterCircleShape*>(self->GetNativeObject())->SetSettings(tmpoptions);
	}

	void ScriptParticleEmitterCircleShape::InternalGetOptions(ScriptParticleEmitterCircleShape* self, __PARTICLE_CIRCLE_SHAPE_DESCInterop* __output)
	{
		if(!self->IsNativeObjectValid())
		{
			*__output = {};
			return;
		}

		ParticleCircleShapeSettings tmp__output;
		tmp__output = static_cast<ParticleEmitterCircleShape*>(self->GetNativeObject())->GetSettings();

		__PARTICLE_CIRCLE_SHAPE_DESCInterop interop__output;
		interop__output = ScriptParticleCircleShapeOptions::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptParticleCircleShapeOptions::GetMetaData()->ScriptClass->GetInternalClass());
	}

	void ScriptParticleEmitterCircleShape::InternalCreate(MonoObject* scriptObject, __PARTICLE_CIRCLE_SHAPE_DESCInterop* desc)
	{
		ParticleCircleShapeSettings tmpdesc;
		tmpdesc = ScriptParticleCircleShapeOptions::FromInterop(*desc);
		SPtr<ParticleEmitterCircleShape> nativeObject = ParticleEmitterCircleShape::Create(tmpdesc);
		ScriptObjectWrapper::Create<ScriptParticleEmitterCircleShape>(nativeObject, scriptObject);
	}

	void ScriptParticleEmitterCircleShape::InternalCreate0(MonoObject* scriptObject)
	{
		SPtr<ParticleEmitterCircleShape> nativeObject = ParticleEmitterCircleShape::Create();
		ScriptObjectWrapper::Create<ScriptParticleEmitterCircleShape>(nativeObject, scriptObject);
	}
}
