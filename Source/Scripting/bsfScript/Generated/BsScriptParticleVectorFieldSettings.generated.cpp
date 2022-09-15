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
		metaData.scriptClass->addInternalCall("Internal_getvectorField", (void*)&ScriptParticleVectorFieldSettings::Internal_getvectorField);
		metaData.scriptClass->addInternalCall("Internal_setvectorField", (void*)&ScriptParticleVectorFieldSettings::Internal_setvectorField);
		metaData.scriptClass->addInternalCall("Internal_getintensity", (void*)&ScriptParticleVectorFieldSettings::Internal_getintensity);
		metaData.scriptClass->addInternalCall("Internal_setintensity", (void*)&ScriptParticleVectorFieldSettings::Internal_setintensity);
		metaData.scriptClass->addInternalCall("Internal_gettightness", (void*)&ScriptParticleVectorFieldSettings::Internal_gettightness);
		metaData.scriptClass->addInternalCall("Internal_settightness", (void*)&ScriptParticleVectorFieldSettings::Internal_settightness);
		metaData.scriptClass->addInternalCall("Internal_getscale", (void*)&ScriptParticleVectorFieldSettings::Internal_getscale);
		metaData.scriptClass->addInternalCall("Internal_setscale", (void*)&ScriptParticleVectorFieldSettings::Internal_setscale);
		metaData.scriptClass->addInternalCall("Internal_getoffset", (void*)&ScriptParticleVectorFieldSettings::Internal_getoffset);
		metaData.scriptClass->addInternalCall("Internal_setoffset", (void*)&ScriptParticleVectorFieldSettings::Internal_setoffset);
		metaData.scriptClass->addInternalCall("Internal_getrotation", (void*)&ScriptParticleVectorFieldSettings::Internal_getrotation);
		metaData.scriptClass->addInternalCall("Internal_setrotation", (void*)&ScriptParticleVectorFieldSettings::Internal_setrotation);
		metaData.scriptClass->addInternalCall("Internal_getrotationRate", (void*)&ScriptParticleVectorFieldSettings::Internal_getrotationRate);
		metaData.scriptClass->addInternalCall("Internal_setrotationRate", (void*)&ScriptParticleVectorFieldSettings::Internal_setrotationRate);
		metaData.scriptClass->addInternalCall("Internal_gettilingX", (void*)&ScriptParticleVectorFieldSettings::Internal_gettilingX);
		metaData.scriptClass->addInternalCall("Internal_settilingX", (void*)&ScriptParticleVectorFieldSettings::Internal_settilingX);
		metaData.scriptClass->addInternalCall("Internal_gettilingY", (void*)&ScriptParticleVectorFieldSettings::Internal_gettilingY);
		metaData.scriptClass->addInternalCall("Internal_settilingY", (void*)&ScriptParticleVectorFieldSettings::Internal_settilingY);
		metaData.scriptClass->addInternalCall("Internal_gettilingZ", (void*)&ScriptParticleVectorFieldSettings::Internal_gettilingZ);
		metaData.scriptClass->addInternalCall("Internal_settilingZ", (void*)&ScriptParticleVectorFieldSettings::Internal_settilingZ);

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
