//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptParticleVectorFieldSettings.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "../../../Foundation/bsfCore/Particles/BsVectorField.h"
#include "Wrappers/BsScriptVector.h"
#include "BsScriptTDistribution.generated.h"
#include "Wrappers/BsScriptQuaternion.h"

namespace bs
{
	ScriptParticleVectorFieldSettings::ScriptParticleVectorFieldSettings(MonoObject* managedInstance, const SPtr<ParticleVectorFieldSettings>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptParticleVectorFieldSettings::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_getvectorField", (void*)&ScriptParticleVectorFieldSettings::InternalGetvectorField);
		metaData.scriptClass->AddInternalCall("Internal_setvectorField", (void*)&ScriptParticleVectorFieldSettings::InternalSetvectorField);
		metaData.scriptClass->AddInternalCall("Internal_getintensity", (void*)&ScriptParticleVectorFieldSettings::InternalGetintensity);
		metaData.scriptClass->AddInternalCall("Internal_setintensity", (void*)&ScriptParticleVectorFieldSettings::InternalSetintensity);
		metaData.scriptClass->AddInternalCall("Internal_gettightness", (void*)&ScriptParticleVectorFieldSettings::InternalGettightness);
		metaData.scriptClass->AddInternalCall("Internal_settightness", (void*)&ScriptParticleVectorFieldSettings::InternalSettightness);
		metaData.scriptClass->AddInternalCall("Internal_getscale", (void*)&ScriptParticleVectorFieldSettings::InternalGetscale);
		metaData.scriptClass->AddInternalCall("Internal_setscale", (void*)&ScriptParticleVectorFieldSettings::InternalSetscale);
		metaData.scriptClass->AddInternalCall("Internal_getoffset", (void*)&ScriptParticleVectorFieldSettings::InternalGetoffset);
		metaData.scriptClass->AddInternalCall("Internal_setoffset", (void*)&ScriptParticleVectorFieldSettings::InternalSetoffset);
		metaData.scriptClass->AddInternalCall("Internal_getrotation", (void*)&ScriptParticleVectorFieldSettings::InternalGetrotation);
		metaData.scriptClass->AddInternalCall("Internal_setrotation", (void*)&ScriptParticleVectorFieldSettings::InternalSetrotation);
		metaData.scriptClass->AddInternalCall("Internal_getrotationRate", (void*)&ScriptParticleVectorFieldSettings::InternalGetrotationRate);
		metaData.scriptClass->AddInternalCall("Internal_setrotationRate", (void*)&ScriptParticleVectorFieldSettings::InternalSetrotationRate);
		metaData.scriptClass->AddInternalCall("Internal_gettilingX", (void*)&ScriptParticleVectorFieldSettings::InternalGettilingX);
		metaData.scriptClass->AddInternalCall("Internal_settilingX", (void*)&ScriptParticleVectorFieldSettings::InternalSettilingX);
		metaData.scriptClass->AddInternalCall("Internal_gettilingY", (void*)&ScriptParticleVectorFieldSettings::InternalGettilingY);
		metaData.scriptClass->AddInternalCall("Internal_settilingY", (void*)&ScriptParticleVectorFieldSettings::InternalSettilingY);
		metaData.scriptClass->AddInternalCall("Internal_gettilingZ", (void*)&ScriptParticleVectorFieldSettings::InternalGettilingZ);
		metaData.scriptClass->AddInternalCall("Internal_settilingZ", (void*)&ScriptParticleVectorFieldSettings::InternalSettilingZ);

	}

	MonoObject* ScriptParticleVectorFieldSettings::Create(const SPtr<ParticleVectorFieldSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptParticleVectorFieldSettings>()) ScriptParticleVectorFieldSettings(managedInstance, value);
		return managedInstance;
	}
	MonoObject* ScriptParticleVectorFieldSettings::InternalGetvectorField(ScriptParticleVectorFieldSettings* thisPtr)
	{
		ResourceHandle<VectorField> tmp__output;
		tmp__output = thisPtr->GetInternal()->vectorField;

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().getScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptParticleVectorFieldSettings::InternalSetvectorField(ScriptParticleVectorFieldSettings* thisPtr, MonoObject* value)
	{
		ResourceHandle<VectorField> tmpvalue;
		ScriptRRefBase* scriptvalue;
		scriptvalue = ScriptRRefBase::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = static_resource_cast<VectorField>(scriptvalue->GetHandle());
		thisPtr->GetInternal()->vectorField = tmpvalue;
	}

	float ScriptParticleVectorFieldSettings::InternalGetintensity(ScriptParticleVectorFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->intensity;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleVectorFieldSettings::InternalSetintensity(ScriptParticleVectorFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->intensity = value;
	}

	float ScriptParticleVectorFieldSettings::InternalGettightness(ScriptParticleVectorFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->tightness;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleVectorFieldSettings::InternalSettightness(ScriptParticleVectorFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->tightness = value;
	}

	void ScriptParticleVectorFieldSettings::InternalGetscale(ScriptParticleVectorFieldSettings* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->scale;

		*__output = tmp__output;


	}

	void ScriptParticleVectorFieldSettings::InternalSetscale(ScriptParticleVectorFieldSettings* thisPtr, Vector3* value)
	{
		thisPtr->GetInternal()->scale = *value;
	}

	void ScriptParticleVectorFieldSettings::InternalGetoffset(ScriptParticleVectorFieldSettings* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->offset;

		*__output = tmp__output;


	}

	void ScriptParticleVectorFieldSettings::InternalSetoffset(ScriptParticleVectorFieldSettings* thisPtr, Vector3* value)
	{
		thisPtr->GetInternal()->offset = *value;
	}

	void ScriptParticleVectorFieldSettings::InternalGetrotation(ScriptParticleVectorFieldSettings* thisPtr, Quaternion* __output)
	{
		Quaternion tmp__output;
		tmp__output = thisPtr->GetInternal()->rotation;

		*__output = tmp__output;


	}

	void ScriptParticleVectorFieldSettings::InternalSetrotation(ScriptParticleVectorFieldSettings* thisPtr, Quaternion* value)
	{
		thisPtr->GetInternal()->rotation = *value;
	}

	MonoObject* ScriptParticleVectorFieldSettings::InternalGetrotationRate(ScriptParticleVectorFieldSettings* thisPtr)
	{
		SPtr<TDistribution<Vector3>> tmp__output = bs_shared_ptr_new<TDistribution<Vector3>>();
		*tmp__output = thisPtr->GetInternal()->rotationRate;

		MonoObject* __output;
		__output = ScriptTDistributionVector3::Create(tmp__output);

		return __output;
	}

	void ScriptParticleVectorFieldSettings::InternalSetrotationRate(ScriptParticleVectorFieldSettings* thisPtr, MonoObject* value)
	{
		SPtr<TDistribution<Vector3>> tmpvalue;
		ScriptTDistributionVector3* scriptvalue;
		scriptvalue = ScriptTDistributionVector3::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->rotationRate = *tmpvalue;
	}

	bool ScriptParticleVectorFieldSettings::InternalGettilingX(ScriptParticleVectorFieldSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->tilingX;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleVectorFieldSettings::InternalSettilingX(ScriptParticleVectorFieldSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->tilingX = value;
	}

	bool ScriptParticleVectorFieldSettings::InternalGettilingY(ScriptParticleVectorFieldSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->tilingY;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleVectorFieldSettings::InternalSettilingY(ScriptParticleVectorFieldSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->tilingY = value;
	}

	bool ScriptParticleVectorFieldSettings::InternalGettilingZ(ScriptParticleVectorFieldSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->tilingZ;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleVectorFieldSettings::InternalSettilingZ(ScriptParticleVectorFieldSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->tilingZ = value;
	}
}
