//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptParticleGpuSimulationSettings.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptParticleDepthCollisionSettings.generated.h"
#include "BsScriptParticleVectorFieldSettings.generated.h"
#include "Wrappers/BsScriptVector.h"
#include "BsScriptTColorDistribution.generated.h"
#include "BsScriptTDistribution.generated.h"

namespace bs
{
	ScriptParticleGpuSimulationSettings::ScriptParticleGpuSimulationSettings(MonoObject* managedInstance, const SPtr<ParticleGpuSimulationSettings>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptParticleGpuSimulationSettings::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_getvectorField", (void*)&ScriptParticleGpuSimulationSettings::InternalGetvectorField);
		metaData.scriptClass->AddInternalCall("Internal_setvectorField", (void*)&ScriptParticleGpuSimulationSettings::InternalSetvectorField);
		metaData.scriptClass->AddInternalCall("Internal_getcolorOverLifetime", (void*)&ScriptParticleGpuSimulationSettings::InternalGetcolorOverLifetime);
		metaData.scriptClass->AddInternalCall("Internal_setcolorOverLifetime", (void*)&ScriptParticleGpuSimulationSettings::InternalSetcolorOverLifetime);
		metaData.scriptClass->AddInternalCall("Internal_getsizeScaleOverLifetime", (void*)&ScriptParticleGpuSimulationSettings::InternalGetsizeScaleOverLifetime);
		metaData.scriptClass->AddInternalCall("Internal_setsizeScaleOverLifetime", (void*)&ScriptParticleGpuSimulationSettings::InternalSetsizeScaleOverLifetime);
		metaData.scriptClass->AddInternalCall("Internal_getacceleration", (void*)&ScriptParticleGpuSimulationSettings::InternalGetacceleration);
		metaData.scriptClass->AddInternalCall("Internal_setacceleration", (void*)&ScriptParticleGpuSimulationSettings::InternalSetacceleration);
		metaData.scriptClass->AddInternalCall("Internal_getdrag", (void*)&ScriptParticleGpuSimulationSettings::InternalGetdrag);
		metaData.scriptClass->AddInternalCall("Internal_setdrag", (void*)&ScriptParticleGpuSimulationSettings::InternalSetdrag);
		metaData.scriptClass->AddInternalCall("Internal_getdepthCollision", (void*)&ScriptParticleGpuSimulationSettings::InternalGetdepthCollision);
		metaData.scriptClass->AddInternalCall("Internal_setdepthCollision", (void*)&ScriptParticleGpuSimulationSettings::InternalSetdepthCollision);

	}

	MonoObject* ScriptParticleGpuSimulationSettings::Create(const SPtr<ParticleGpuSimulationSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptParticleGpuSimulationSettings>()) ScriptParticleGpuSimulationSettings(managedInstance, value);
		return managedInstance;
	}
	MonoObject* ScriptParticleGpuSimulationSettings::InternalGetvectorField(ScriptParticleGpuSimulationSettings* thisPtr)
	{
		SPtr<ParticleVectorFieldSettings> tmp__output = bs_shared_ptr_new<ParticleVectorFieldSettings>();
		*tmp__output = thisPtr->GetInternal()->vectorField;

		MonoObject* __output;
		__output = ScriptParticleVectorFieldSettings::Create(tmp__output);

		return __output;
	}

	void ScriptParticleGpuSimulationSettings::InternalSetvectorField(ScriptParticleGpuSimulationSettings* thisPtr, MonoObject* value)
	{
		SPtr<ParticleVectorFieldSettings> tmpvalue;
		ScriptParticleVectorFieldSettings* scriptvalue;
		scriptvalue = ScriptParticleVectorFieldSettings::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->vectorField = *tmpvalue;
	}

	MonoObject* ScriptParticleGpuSimulationSettings::InternalGetcolorOverLifetime(ScriptParticleGpuSimulationSettings* thisPtr)
	{
		SPtr<TColorDistribution<ColorGradient>> tmp__output = bs_shared_ptr_new<TColorDistribution<ColorGradient>>();
		*tmp__output = thisPtr->GetInternal()->colorOverLifetime;

		MonoObject* __output;
		__output = ScriptTColorDistributionColorGradient::Create(tmp__output);

		return __output;
	}

	void ScriptParticleGpuSimulationSettings::InternalSetcolorOverLifetime(ScriptParticleGpuSimulationSettings* thisPtr, MonoObject* value)
	{
		SPtr<TColorDistribution<ColorGradient>> tmpvalue;
		ScriptTColorDistributionColorGradient* scriptvalue;
		scriptvalue = ScriptTColorDistributionColorGradient::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->colorOverLifetime = *tmpvalue;
	}

	MonoObject* ScriptParticleGpuSimulationSettings::InternalGetsizeScaleOverLifetime(ScriptParticleGpuSimulationSettings* thisPtr)
	{
		SPtr<TDistribution<Vector2>> tmp__output = bs_shared_ptr_new<TDistribution<Vector2>>();
		*tmp__output = thisPtr->GetInternal()->sizeScaleOverLifetime;

		MonoObject* __output;
		__output = ScriptTDistributionVector2::Create(tmp__output);

		return __output;
	}

	void ScriptParticleGpuSimulationSettings::InternalSetsizeScaleOverLifetime(ScriptParticleGpuSimulationSettings* thisPtr, MonoObject* value)
	{
		SPtr<TDistribution<Vector2>> tmpvalue;
		ScriptTDistributionVector2* scriptvalue;
		scriptvalue = ScriptTDistributionVector2::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->sizeScaleOverLifetime = *tmpvalue;
	}

	void ScriptParticleGpuSimulationSettings::InternalGetacceleration(ScriptParticleGpuSimulationSettings* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->acceleration;

		*__output = tmp__output;


	}

	void ScriptParticleGpuSimulationSettings::InternalSetacceleration(ScriptParticleGpuSimulationSettings* thisPtr, Vector3* value)
	{
		thisPtr->GetInternal()->acceleration = *value;
	}

	float ScriptParticleGpuSimulationSettings::InternalGetdrag(ScriptParticleGpuSimulationSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->drag;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleGpuSimulationSettings::InternalSetdrag(ScriptParticleGpuSimulationSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->drag = value;
	}

	MonoObject* ScriptParticleGpuSimulationSettings::InternalGetdepthCollision(ScriptParticleGpuSimulationSettings* thisPtr)
	{
		SPtr<ParticleDepthCollisionSettings> tmp__output = bs_shared_ptr_new<ParticleDepthCollisionSettings>();
		*tmp__output = thisPtr->GetInternal()->depthCollision;

		MonoObject* __output;
		__output = ScriptParticleDepthCollisionSettings::Create(tmp__output);

		return __output;
	}

	void ScriptParticleGpuSimulationSettings::InternalSetdepthCollision(ScriptParticleGpuSimulationSettings* thisPtr, MonoObject* value)
	{
		SPtr<ParticleDepthCollisionSettings> tmpvalue;
		ScriptParticleDepthCollisionSettings* scriptvalue;
		scriptvalue = ScriptParticleDepthCollisionSettings::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->depthCollision = *tmpvalue;
	}
}
