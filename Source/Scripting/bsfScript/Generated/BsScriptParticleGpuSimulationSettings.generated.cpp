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

	void ScriptParticleGpuSimulationSettings::InitRuntimeData()
	{
		metaData.ScriptClass->AddInternalCall("Internal_GetvectorField", (void*)&ScriptParticleGpuSimulationSettings::InternalGetvectorField);
		metaData.ScriptClass->AddInternalCall("Internal_SetvectorField", (void*)&ScriptParticleGpuSimulationSettings::InternalSetvectorField);
		metaData.ScriptClass->AddInternalCall("Internal_GetcolorOverLifetime", (void*)&ScriptParticleGpuSimulationSettings::InternalGetcolorOverLifetime);
		metaData.ScriptClass->AddInternalCall("Internal_SetcolorOverLifetime", (void*)&ScriptParticleGpuSimulationSettings::InternalSetcolorOverLifetime);
		metaData.ScriptClass->AddInternalCall("Internal_GetsizeScaleOverLifetime", (void*)&ScriptParticleGpuSimulationSettings::InternalGetsizeScaleOverLifetime);
		metaData.ScriptClass->AddInternalCall("Internal_SetsizeScaleOverLifetime", (void*)&ScriptParticleGpuSimulationSettings::InternalSetsizeScaleOverLifetime);
		metaData.ScriptClass->AddInternalCall("Internal_Getacceleration", (void*)&ScriptParticleGpuSimulationSettings::InternalGetacceleration);
		metaData.ScriptClass->AddInternalCall("Internal_Setacceleration", (void*)&ScriptParticleGpuSimulationSettings::InternalSetacceleration);
		metaData.ScriptClass->AddInternalCall("Internal_Getdrag", (void*)&ScriptParticleGpuSimulationSettings::InternalGetdrag);
		metaData.ScriptClass->AddInternalCall("Internal_Setdrag", (void*)&ScriptParticleGpuSimulationSettings::InternalSetdrag);
		metaData.ScriptClass->AddInternalCall("Internal_GetdepthCollision", (void*)&ScriptParticleGpuSimulationSettings::InternalGetdepthCollision);
		metaData.ScriptClass->AddInternalCall("Internal_SetdepthCollision", (void*)&ScriptParticleGpuSimulationSettings::InternalSetdepthCollision);

	}

	MonoObject* ScriptParticleGpuSimulationSettings::Create(const SPtr<ParticleGpuSimulationSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.ScriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptParticleGpuSimulationSettings>()) ScriptParticleGpuSimulationSettings(managedInstance, value);
		return managedInstance;
	}
	MonoObject* ScriptParticleGpuSimulationSettings::InternalGetvectorField(ScriptParticleGpuSimulationSettings* thisPtr)
	{
		SPtr<ParticleVectorFieldSettings> tmp__output = bs_shared_ptr_new<ParticleVectorFieldSettings>();
		*tmp__output = thisPtr->GetInternal()->VectorField;

		MonoObject* __output;
		__output = ScriptParticleVectorFieldSettings::Create(tmp__output);

		return __output;
	}

	void ScriptParticleGpuSimulationSettings::InternalSetvectorField(ScriptParticleGpuSimulationSettings* thisPtr, MonoObject* value)
	{
		SPtr<ParticleVectorFieldSettings> tmpvalue;
		ScriptParticleVectorFieldSettings* scriptvalue;
		scriptvalue = ScriptParticleVectorFieldSettings::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->VectorField = *tmpvalue;
	}

	MonoObject* ScriptParticleGpuSimulationSettings::InternalGetcolorOverLifetime(ScriptParticleGpuSimulationSettings* thisPtr)
	{
		SPtr<TColorDistribution<ColorGradient>> tmp__output = bs_shared_ptr_new<TColorDistribution<ColorGradient>>();
		*tmp__output = thisPtr->GetInternal()->ColorOverLifetime;

		MonoObject* __output;
		__output = ScriptTColorDistributionColorGradient::Create(tmp__output);

		return __output;
	}

	void ScriptParticleGpuSimulationSettings::InternalSetcolorOverLifetime(ScriptParticleGpuSimulationSettings* thisPtr, MonoObject* value)
	{
		SPtr<TColorDistribution<ColorGradient>> tmpvalue;
		ScriptTColorDistributionColorGradient* scriptvalue;
		scriptvalue = ScriptTColorDistributionColorGradient::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->ColorOverLifetime = *tmpvalue;
	}

	MonoObject* ScriptParticleGpuSimulationSettings::InternalGetsizeScaleOverLifetime(ScriptParticleGpuSimulationSettings* thisPtr)
	{
		SPtr<TDistribution<Vector2>> tmp__output = bs_shared_ptr_new<TDistribution<Vector2>>();
		*tmp__output = thisPtr->GetInternal()->SizeScaleOverLifetime;

		MonoObject* __output;
		__output = ScriptTDistributionVector2::Create(tmp__output);

		return __output;
	}

	void ScriptParticleGpuSimulationSettings::InternalSetsizeScaleOverLifetime(ScriptParticleGpuSimulationSettings* thisPtr, MonoObject* value)
	{
		SPtr<TDistribution<Vector2>> tmpvalue;
		ScriptTDistributionVector2* scriptvalue;
		scriptvalue = ScriptTDistributionVector2::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->SizeScaleOverLifetime = *tmpvalue;
	}

	void ScriptParticleGpuSimulationSettings::InternalGetacceleration(ScriptParticleGpuSimulationSettings* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->Acceleration;

		*__output = tmp__output;


	}

	void ScriptParticleGpuSimulationSettings::InternalSetacceleration(ScriptParticleGpuSimulationSettings* thisPtr, Vector3* value)
	{
		thisPtr->GetInternal()->Acceleration = *value;
	}

	float ScriptParticleGpuSimulationSettings::InternalGetdrag(ScriptParticleGpuSimulationSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->Drag;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleGpuSimulationSettings::InternalSetdrag(ScriptParticleGpuSimulationSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->Drag = value;
	}

	MonoObject* ScriptParticleGpuSimulationSettings::InternalGetdepthCollision(ScriptParticleGpuSimulationSettings* thisPtr)
	{
		SPtr<ParticleDepthCollisionSettings> tmp__output = bs_shared_ptr_new<ParticleDepthCollisionSettings>();
		*tmp__output = thisPtr->GetInternal()->DepthCollision;

		MonoObject* __output;
		__output = ScriptParticleDepthCollisionSettings::Create(tmp__output);

		return __output;
	}

	void ScriptParticleGpuSimulationSettings::InternalSetdepthCollision(ScriptParticleGpuSimulationSettings* thisPtr, MonoObject* value)
	{
		SPtr<ParticleDepthCollisionSettings> tmpvalue;
		ScriptParticleDepthCollisionSettings* scriptvalue;
		scriptvalue = ScriptParticleDepthCollisionSettings::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->DepthCollision = *tmpvalue;
	}
}
