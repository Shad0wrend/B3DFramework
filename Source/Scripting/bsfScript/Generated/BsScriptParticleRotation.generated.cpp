//********************************* B3D Framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptParticleRotation.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptPARTICLE_ROTATION_DESC.generated.h"
#include "BsScriptParticleRotation.generated.h"

namespace b3d
{
	ScriptParticleRotation::ScriptParticleRotation(const SPtr<ParticleRotation>& nativeObject)
		:TScriptReflectableWrapper(nativeObject)
	{
		RegisterEvents();
	}

	ScriptParticleRotation::~ScriptParticleRotation()
	{
		UnregisterEvents();
	}

	void ScriptParticleRotation::SetupScriptBindings()
	{
		sInteropMetaData.ScriptClass->AddInternalCall("Internal_SetOptions", (void*)&ScriptParticleRotation::InternalSetOptions);
		sInteropMetaData.ScriptClass->AddInternalCall("Internal_GetOptions", (void*)&ScriptParticleRotation::InternalGetOptions);
		sInteropMetaData.ScriptClass->AddInternalCall("Internal_Create", (void*)&ScriptParticleRotation::InternalCreate);
		sInteropMetaData.ScriptClass->AddInternalCall("Internal_Create0", (void*)&ScriptParticleRotation::InternalCreate0);

	}

	MonoObject* ScriptParticleRotation::CreateScriptObject(bool construct)
	{
		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		if(construct)
			return sInteropMetaData.ScriptClass->CreateInstance("bool", ctorParams);

		return sInteropMetaData.ScriptClass->CreateInstance(false);
	}
	void ScriptParticleRotation::InternalSetOptions(ScriptParticleRotation* self, __PARTICLE_ROTATION_DESCInterop* options)
	{
		if(!self->IsNativeObjectValid())
			return;

		ParticleRotationSettings tmpoptions;
		tmpoptions = ScriptParticleRotationOptions::FromInterop(*options);
		static_cast<ParticleRotation*>(self->GetNativeObject())->SetSettings(tmpoptions);
	}

	void ScriptParticleRotation::InternalGetOptions(ScriptParticleRotation* self, __PARTICLE_ROTATION_DESCInterop* __output)
	{
		if(!self->IsNativeObjectValid())
		{
			*__output = {};
			return;
		}

		ParticleRotationSettings tmp__output;
		tmp__output = static_cast<ParticleRotation*>(self->GetNativeObject())->GetSettings();

		__PARTICLE_ROTATION_DESCInterop interop__output;
		interop__output = ScriptParticleRotationOptions::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptParticleRotationOptions::GetMetaData()->ScriptClass->GetInternalClass());
	}

	void ScriptParticleRotation::InternalCreate(MonoObject* scriptObject, __PARTICLE_ROTATION_DESCInterop* desc)
	{
		ParticleRotationSettings tmpdesc;
		tmpdesc = ScriptParticleRotationOptions::FromInterop(*desc);
		SPtr<ParticleRotation> nativeObject = ParticleRotation::Create(tmpdesc);
		ScriptObjectWrapper::Create<ScriptParticleRotation>(nativeObject, scriptObject);
	}

	void ScriptParticleRotation::InternalCreate0(MonoObject* scriptObject)
	{
		SPtr<ParticleRotation> nativeObject = ParticleRotation::Create();
		ScriptObjectWrapper::Create<ScriptParticleRotation>(nativeObject, scriptObject);
	}
}
