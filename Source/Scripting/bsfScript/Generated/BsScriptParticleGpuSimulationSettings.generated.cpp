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
		metaData.scriptClass->addInternalCall("Internal_getvectorField", (void*)&ScriptParticleGpuSimulationSettings::Internal_getvectorField);
		metaData.scriptClass->addInternalCall("Internal_setvectorField", (void*)&ScriptParticleGpuSimulationSettings::Internal_setvectorField);
		metaData.scriptClass->addInternalCall("Internal_getcolorOverLifetime", (void*)&ScriptParticleGpuSimulationSettings::Internal_getcolorOverLifetime);
		metaData.scriptClass->addInternalCall("Internal_setcolorOverLifetime", (void*)&ScriptParticleGpuSimulationSettings::Internal_setcolorOverLifetime);
		metaData.scriptClass->addInternalCall("Internal_getsizeScaleOverLifetime", (void*)&ScriptParticleGpuSimulationSettings::Internal_getsizeScaleOverLifetime);
		metaData.scriptClass->addInternalCall("Internal_setsizeScaleOverLifetime", (void*)&ScriptParticleGpuSimulationSettings::Internal_setsizeScaleOverLifetime);
		metaData.scriptClass->addInternalCall("Internal_getacceleration", (void*)&ScriptParticleGpuSimulationSettings::Internal_getacceleration);
		metaData.scriptClass->addInternalCall("Internal_setacceleration", (void*)&ScriptParticleGpuSimulationSettings::Internal_setacceleration);
		metaData.scriptClass->addInternalCall("Internal_getdrag", (void*)&ScriptParticleGpuSimulationSettings::Internal_getdrag);
		metaData.scriptClass->addInternalCall("Internal_setdrag", (void*)&ScriptParticleGpuSimulationSettings::Internal_setdrag);
		metaData.scriptClass->addInternalCall("Internal_getdepthCollision", (void*)&ScriptParticleGpuSimulationSettings::Internal_getdepthCollision);
		metaData.scriptClass->addInternalCall("Internal_setdepthCollision", (void*)&ScriptParticleGpuSimulationSettings::Internal_setdepthCollision);

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
		*tmp__output = thisPtr->getInternal()->vectorField;

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
			tmpvalue = scriptvalue->getInternal();
		thisPtr->getInternal()->vectorField = *tmpvalue;
	}

	MonoObject* ScriptParticleGpuSimulationSettings::InternalGetcolorOverLifetime(ScriptParticleGpuSimulationSettings* thisPtr)
	{
		SPtr<TColorDistribution<ColorGradient>> tmp__output = bs_shared_ptr_new<TColorDistribution<ColorGradient>>();
		*tmp__output = thisPtr->getInternal()->colorOverLifetime;

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
			tmpvalue = scriptvalue->getInternal();
		thisPtr->getInternal()->colorOverLifetime = *tmpvalue;
	}

	MonoObject* ScriptParticleGpuSimulationSettings::InternalGetsizeScaleOverLifetime(ScriptParticleGpuSimulationSettings* thisPtr)
	{
		SPtr<TDistribution<Vector2>> tmp__output = bs_shared_ptr_new<TDistribution<Vector2>>();
		*tmp__output = thisPtr->getInternal()->sizeScaleOverLifetime;

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
			tmpvalue = scriptvalue->getInternal();
		thisPtr->getInternal()->sizeScaleOverLifetime = *tmpvalue;
	}

	void ScriptParticleGpuSimulationSettings::InternalGetacceleration(ScriptParticleGpuSimulationSettings* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->getInternal()->acceleration;

		*__output = tmp__output;


	}

	void ScriptParticleGpuSimulationSettings::InternalSetacceleration(ScriptParticleGpuSimulationSettings* thisPtr, Vector3* value)
	{
		thisPtr->getInternal()->acceleration = *value;
	}

	float ScriptParticleGpuSimulationSettings::InternalGetdrag(ScriptParticleGpuSimulationSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getInternal()->drag;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleGpuSimulationSettings::InternalSetdrag(ScriptParticleGpuSimulationSettings* thisPtr, float value)
	{
		thisPtr->getInternal()->drag = value;
	}

	MonoObject* ScriptParticleGpuSimulationSettings::InternalGetdepthCollision(ScriptParticleGpuSimulationSettings* thisPtr)
	{
		SPtr<ParticleDepthCollisionSettings> tmp__output = bs_shared_ptr_new<ParticleDepthCollisionSettings>();
		*tmp__output = thisPtr->getInternal()->depthCollision;

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
			tmpvalue = scriptvalue->getInternal();
		thisPtr->getInternal()->depthCollision = *tmpvalue;
	}
}
