//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptParticleDepthCollisionSettings.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptParticleDepthCollisionSettings::ScriptParticleDepthCollisionSettings(MonoObject* managedInstance, const SPtr<ParticleDepthCollisionSettings>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptParticleDepthCollisionSettings::InitRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_ParticleDepthCollisionSettings", (void*)&ScriptParticleDepthCollisionSettings::InternalParticleDepthCollisionSettings);
		metaData.scriptClass->AddInternalCall("Internal_Getenabled", (void*)&ScriptParticleDepthCollisionSettings::InternalGetenabled);
		metaData.scriptClass->AddInternalCall("Internal_Setenabled", (void*)&ScriptParticleDepthCollisionSettings::InternalSetenabled);
		metaData.scriptClass->AddInternalCall("Internal_Getrestitution", (void*)&ScriptParticleDepthCollisionSettings::InternalGetrestitution);
		metaData.scriptClass->AddInternalCall("Internal_Setrestitution", (void*)&ScriptParticleDepthCollisionSettings::InternalSetrestitution);
		metaData.scriptClass->AddInternalCall("Internal_Getdampening", (void*)&ScriptParticleDepthCollisionSettings::InternalGetdampening);
		metaData.scriptClass->AddInternalCall("Internal_Setdampening", (void*)&ScriptParticleDepthCollisionSettings::InternalSetdampening);
		metaData.scriptClass->AddInternalCall("Internal_GetradiusScale", (void*)&ScriptParticleDepthCollisionSettings::InternalGetradiusScale);
		metaData.scriptClass->AddInternalCall("Internal_SetradiusScale", (void*)&ScriptParticleDepthCollisionSettings::InternalSetradiusScale);

	}

	MonoObject* ScriptParticleDepthCollisionSettings::Create(const SPtr<ParticleDepthCollisionSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptParticleDepthCollisionSettings>()) ScriptParticleDepthCollisionSettings(managedInstance, value);
		return managedInstance;
	}
	void ScriptParticleDepthCollisionSettings::InternalParticleDepthCollisionSettings(MonoObject* managedInstance)
	{
		SPtr<ParticleDepthCollisionSettings> instance = bs_shared_ptr_new<ParticleDepthCollisionSettings>();
		new (bs_alloc<ScriptParticleDepthCollisionSettings>())ScriptParticleDepthCollisionSettings(managedInstance, instance);
	}

	bool ScriptParticleDepthCollisionSettings::InternalGetenabled(ScriptParticleDepthCollisionSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->enabled;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleDepthCollisionSettings::InternalSetenabled(ScriptParticleDepthCollisionSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->enabled = value;
	}

	float ScriptParticleDepthCollisionSettings::InternalGetrestitution(ScriptParticleDepthCollisionSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->restitution;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleDepthCollisionSettings::InternalSetrestitution(ScriptParticleDepthCollisionSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->restitution = value;
	}

	float ScriptParticleDepthCollisionSettings::InternalGetdampening(ScriptParticleDepthCollisionSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->dampening;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleDepthCollisionSettings::InternalSetdampening(ScriptParticleDepthCollisionSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->dampening = value;
	}

	float ScriptParticleDepthCollisionSettings::InternalGetradiusScale(ScriptParticleDepthCollisionSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->radiusScale;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleDepthCollisionSettings::InternalSetradiusScale(ScriptParticleDepthCollisionSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->radiusScale = value;
	}
}
