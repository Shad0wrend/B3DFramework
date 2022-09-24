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

	void ScriptParticleVectorFieldSettings::InitRuntimeData()
	{
		metaData.ScriptClass->AddInternalCall("Internal_GetvectorField", (void*)&ScriptParticleVectorFieldSettings::InternalGetvectorField);
		metaData.ScriptClass->AddInternalCall("Internal_SetvectorField", (void*)&ScriptParticleVectorFieldSettings::InternalSetvectorField);
		metaData.ScriptClass->AddInternalCall("Internal_Getintensity", (void*)&ScriptParticleVectorFieldSettings::InternalGetintensity);
		metaData.ScriptClass->AddInternalCall("Internal_Setintensity", (void*)&ScriptParticleVectorFieldSettings::InternalSetintensity);
		metaData.ScriptClass->AddInternalCall("Internal_Gettightness", (void*)&ScriptParticleVectorFieldSettings::InternalGettightness);
		metaData.ScriptClass->AddInternalCall("Internal_Settightness", (void*)&ScriptParticleVectorFieldSettings::InternalSettightness);
		metaData.ScriptClass->AddInternalCall("Internal_Getscale", (void*)&ScriptParticleVectorFieldSettings::InternalGetscale);
		metaData.ScriptClass->AddInternalCall("Internal_Setscale", (void*)&ScriptParticleVectorFieldSettings::InternalSetscale);
		metaData.ScriptClass->AddInternalCall("Internal_Getoffset", (void*)&ScriptParticleVectorFieldSettings::InternalGetoffset);
		metaData.ScriptClass->AddInternalCall("Internal_Setoffset", (void*)&ScriptParticleVectorFieldSettings::InternalSetoffset);
		metaData.ScriptClass->AddInternalCall("Internal_Getrotation", (void*)&ScriptParticleVectorFieldSettings::InternalGetrotation);
		metaData.ScriptClass->AddInternalCall("Internal_Setrotation", (void*)&ScriptParticleVectorFieldSettings::InternalSetrotation);
		metaData.ScriptClass->AddInternalCall("Internal_GetrotationRate", (void*)&ScriptParticleVectorFieldSettings::InternalGetrotationRate);
		metaData.ScriptClass->AddInternalCall("Internal_SetrotationRate", (void*)&ScriptParticleVectorFieldSettings::InternalSetrotationRate);
		metaData.ScriptClass->AddInternalCall("Internal_GettilingX", (void*)&ScriptParticleVectorFieldSettings::InternalGettilingX);
		metaData.ScriptClass->AddInternalCall("Internal_SettilingX", (void*)&ScriptParticleVectorFieldSettings::InternalSettilingX);
		metaData.ScriptClass->AddInternalCall("Internal_GettilingY", (void*)&ScriptParticleVectorFieldSettings::InternalGettilingY);
		metaData.ScriptClass->AddInternalCall("Internal_SettilingY", (void*)&ScriptParticleVectorFieldSettings::InternalSettilingY);
		metaData.ScriptClass->AddInternalCall("Internal_GettilingZ", (void*)&ScriptParticleVectorFieldSettings::InternalGettilingZ);
		metaData.ScriptClass->AddInternalCall("Internal_SettilingZ", (void*)&ScriptParticleVectorFieldSettings::InternalSettilingZ);

	}

	MonoObject* ScriptParticleVectorFieldSettings::Create(const SPtr<ParticleVectorFieldSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.ScriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptParticleVectorFieldSettings>()) ScriptParticleVectorFieldSettings(managedInstance, value);
		return managedInstance;
	}
	MonoObject* ScriptParticleVectorFieldSettings::InternalGetvectorField(ScriptParticleVectorFieldSettings* thisPtr)
	{
		ResourceHandle<VectorField> tmp__output;
		tmp__output = thisPtr->GetInternal()->VectorField;

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().GetScriptRRef(tmp__output);
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
		scriptvalue = ScriptRRefBase::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = static_resource_cast<VectorField>(scriptvalue->GetHandle());
		thisPtr->GetInternal()->VectorField = tmpvalue;
	}

	float ScriptParticleVectorFieldSettings::InternalGetintensity(ScriptParticleVectorFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->Intensity;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleVectorFieldSettings::InternalSetintensity(ScriptParticleVectorFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->Intensity = value;
	}

	float ScriptParticleVectorFieldSettings::InternalGettightness(ScriptParticleVectorFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->Tightness;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleVectorFieldSettings::InternalSettightness(ScriptParticleVectorFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->Tightness = value;
	}

	void ScriptParticleVectorFieldSettings::InternalGetscale(ScriptParticleVectorFieldSettings* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->Scale;

		*__output = tmp__output;


	}

	void ScriptParticleVectorFieldSettings::InternalSetscale(ScriptParticleVectorFieldSettings* thisPtr, Vector3* value)
	{
		thisPtr->GetInternal()->Scale = *value;
	}

	void ScriptParticleVectorFieldSettings::InternalGetoffset(ScriptParticleVectorFieldSettings* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->Offset;

		*__output = tmp__output;


	}

	void ScriptParticleVectorFieldSettings::InternalSetoffset(ScriptParticleVectorFieldSettings* thisPtr, Vector3* value)
	{
		thisPtr->GetInternal()->Offset = *value;
	}

	void ScriptParticleVectorFieldSettings::InternalGetrotation(ScriptParticleVectorFieldSettings* thisPtr, Quaternion* __output)
	{
		Quaternion tmp__output;
		tmp__output = thisPtr->GetInternal()->Rotation;

		*__output = tmp__output;


	}

	void ScriptParticleVectorFieldSettings::InternalSetrotation(ScriptParticleVectorFieldSettings* thisPtr, Quaternion* value)
	{
		thisPtr->GetInternal()->Rotation = *value;
	}

	MonoObject* ScriptParticleVectorFieldSettings::InternalGetrotationRate(ScriptParticleVectorFieldSettings* thisPtr)
	{
		SPtr<TDistribution<Vector3>> tmp__output = bs_shared_ptr_new<TDistribution<Vector3>>();
		*tmp__output = thisPtr->GetInternal()->RotationRate;

		MonoObject* __output;
		__output = ScriptTDistributionVector3::Create(tmp__output);

		return __output;
	}

	void ScriptParticleVectorFieldSettings::InternalSetrotationRate(ScriptParticleVectorFieldSettings* thisPtr, MonoObject* value)
	{
		SPtr<TDistribution<Vector3>> tmpvalue;
		ScriptTDistributionVector3* scriptvalue;
		scriptvalue = ScriptTDistributionVector3::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->RotationRate = *tmpvalue;
	}

	bool ScriptParticleVectorFieldSettings::InternalGettilingX(ScriptParticleVectorFieldSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->TilingX;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleVectorFieldSettings::InternalSettilingX(ScriptParticleVectorFieldSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->TilingX = value;
	}

	bool ScriptParticleVectorFieldSettings::InternalGettilingY(ScriptParticleVectorFieldSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->TilingY;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleVectorFieldSettings::InternalSettilingY(ScriptParticleVectorFieldSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->TilingY = value;
	}

	bool ScriptParticleVectorFieldSettings::InternalGettilingZ(ScriptParticleVectorFieldSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->TilingZ;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptParticleVectorFieldSettings::InternalSettilingZ(ScriptParticleVectorFieldSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->TilingZ = value;
	}
}
