//********************************* B3D Framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptParticleEmitterHemisphereShape.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptPARTICLE_HEMISPHERE_SHAPE_DESC.generated.h"
#include "BsScriptParticleEmitterHemisphereShape.generated.h"

namespace b3d
{
	ScriptParticleEmitterHemisphereShape::ScriptParticleEmitterHemisphereShape(const SPtr<ParticleEmitterHemisphereShape>& nativeObject)
		:TScriptReflectableWrapper(nativeObject)
	{
		RegisterEvents();
	}

	ScriptParticleEmitterHemisphereShape::~ScriptParticleEmitterHemisphereShape()
	{
		UnregisterEvents();
	}

	void ScriptParticleEmitterHemisphereShape::SetupScriptBindings()
	{
		sInteropMetaData.ScriptClass->AddInternalCall("Internal_SetOptions", (void*)&ScriptParticleEmitterHemisphereShape::InternalSetOptions);
		sInteropMetaData.ScriptClass->AddInternalCall("Internal_GetOptions", (void*)&ScriptParticleEmitterHemisphereShape::InternalGetOptions);
		sInteropMetaData.ScriptClass->AddInternalCall("Internal_Create", (void*)&ScriptParticleEmitterHemisphereShape::InternalCreate);
		sInteropMetaData.ScriptClass->AddInternalCall("Internal_Create0", (void*)&ScriptParticleEmitterHemisphereShape::InternalCreate0);

	}

	MonoObject* ScriptParticleEmitterHemisphereShape::CreateScriptObject(bool construct)
	{
		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		if(construct)
			return sInteropMetaData.ScriptClass->CreateInstance("bool", ctorParams);

		return sInteropMetaData.ScriptClass->CreateInstance(false);
	}
	void ScriptParticleEmitterHemisphereShape::InternalSetOptions(ScriptParticleEmitterHemisphereShape* self, ParticleHemisphereShapeSettings* options)
	{
		if(!self->IsNativeObjectValid())
			return;

		static_cast<ParticleEmitterHemisphereShape*>(self->GetNativeObject())->SetSettings(*options);
	}

	void ScriptParticleEmitterHemisphereShape::InternalGetOptions(ScriptParticleEmitterHemisphereShape* self, ParticleHemisphereShapeSettings* __output)
	{
		if(!self->IsNativeObjectValid())
		{
			*__output = {};
			return;
		}

		ParticleHemisphereShapeSettings tmp__output;
		tmp__output = static_cast<ParticleEmitterHemisphereShape*>(self->GetNativeObject())->GetSettings();

		*__output = tmp__output;
	}

	void ScriptParticleEmitterHemisphereShape::InternalCreate(MonoObject* scriptObject, ParticleHemisphereShapeSettings* desc)
	{
		SPtr<ParticleEmitterHemisphereShape> nativeObject = ParticleEmitterHemisphereShape::Create(*desc);
		ScriptObjectWrapper::Create<ScriptParticleEmitterHemisphereShape>(nativeObject, scriptObject);
	}

	void ScriptParticleEmitterHemisphereShape::InternalCreate0(MonoObject* scriptObject)
	{
		SPtr<ParticleEmitterHemisphereShape> nativeObject = ParticleEmitterHemisphereShape::Create();
		ScriptObjectWrapper::Create<ScriptParticleEmitterHemisphereShape>(nativeObject, scriptObject);
	}
}
