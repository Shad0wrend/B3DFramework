//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptDepthOfFieldSettings.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "../../../Foundation/bsfCore/Image/BsTexture.h"
#include "Wrappers/BsScriptVector.h"

namespace bs
{
	ScriptDepthOfFieldSettings::ScriptDepthOfFieldSettings(MonoObject* managedInstance, const SPtr<DepthOfFieldSettings>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptDepthOfFieldSettings::InitRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_DepthOfFieldSettings", (void*)&ScriptDepthOfFieldSettings::InternalDepthOfFieldSettings);
		metaData.scriptClass->AddInternalCall("Internal_GetbokehShape", (void*)&ScriptDepthOfFieldSettings::InternalGetbokehShape);
		metaData.scriptClass->AddInternalCall("Internal_SetbokehShape", (void*)&ScriptDepthOfFieldSettings::InternalSetbokehShape);
		metaData.scriptClass->AddInternalCall("Internal_Getenabled", (void*)&ScriptDepthOfFieldSettings::InternalGetenabled);
		metaData.scriptClass->AddInternalCall("Internal_Setenabled", (void*)&ScriptDepthOfFieldSettings::InternalSetenabled);
		metaData.scriptClass->AddInternalCall("Internal_Gettype", (void*)&ScriptDepthOfFieldSettings::InternalGettype);
		metaData.scriptClass->AddInternalCall("Internal_Settype", (void*)&ScriptDepthOfFieldSettings::InternalSettype);
		metaData.scriptClass->AddInternalCall("Internal_GetfocalDistance", (void*)&ScriptDepthOfFieldSettings::InternalGetfocalDistance);
		metaData.scriptClass->AddInternalCall("Internal_SetfocalDistance", (void*)&ScriptDepthOfFieldSettings::InternalSetfocalDistance);
		metaData.scriptClass->AddInternalCall("Internal_GetfocalRange", (void*)&ScriptDepthOfFieldSettings::InternalGetfocalRange);
		metaData.scriptClass->AddInternalCall("Internal_SetfocalRange", (void*)&ScriptDepthOfFieldSettings::InternalSetfocalRange);
		metaData.scriptClass->AddInternalCall("Internal_GetnearTransitionRange", (void*)&ScriptDepthOfFieldSettings::InternalGetnearTransitionRange);
		metaData.scriptClass->AddInternalCall("Internal_SetnearTransitionRange", (void*)&ScriptDepthOfFieldSettings::InternalSetnearTransitionRange);
		metaData.scriptClass->AddInternalCall("Internal_GetfarTransitionRange", (void*)&ScriptDepthOfFieldSettings::InternalGetfarTransitionRange);
		metaData.scriptClass->AddInternalCall("Internal_SetfarTransitionRange", (void*)&ScriptDepthOfFieldSettings::InternalSetfarTransitionRange);
		metaData.scriptClass->AddInternalCall("Internal_GetnearBlurAmount", (void*)&ScriptDepthOfFieldSettings::InternalGetnearBlurAmount);
		metaData.scriptClass->AddInternalCall("Internal_SetnearBlurAmount", (void*)&ScriptDepthOfFieldSettings::InternalSetnearBlurAmount);
		metaData.scriptClass->AddInternalCall("Internal_GetfarBlurAmount", (void*)&ScriptDepthOfFieldSettings::InternalGetfarBlurAmount);
		metaData.scriptClass->AddInternalCall("Internal_SetfarBlurAmount", (void*)&ScriptDepthOfFieldSettings::InternalSetfarBlurAmount);
		metaData.scriptClass->AddInternalCall("Internal_GetmaxBokehSize", (void*)&ScriptDepthOfFieldSettings::InternalGetmaxBokehSize);
		metaData.scriptClass->AddInternalCall("Internal_SetmaxBokehSize", (void*)&ScriptDepthOfFieldSettings::InternalSetmaxBokehSize);
		metaData.scriptClass->AddInternalCall("Internal_GetadaptiveColorThreshold", (void*)&ScriptDepthOfFieldSettings::InternalGetadaptiveColorThreshold);
		metaData.scriptClass->AddInternalCall("Internal_SetadaptiveColorThreshold", (void*)&ScriptDepthOfFieldSettings::InternalSetadaptiveColorThreshold);
		metaData.scriptClass->AddInternalCall("Internal_GetadaptiveRadiusThreshold", (void*)&ScriptDepthOfFieldSettings::InternalGetadaptiveRadiusThreshold);
		metaData.scriptClass->AddInternalCall("Internal_SetadaptiveRadiusThreshold", (void*)&ScriptDepthOfFieldSettings::InternalSetadaptiveRadiusThreshold);
		metaData.scriptClass->AddInternalCall("Internal_GetapertureSize", (void*)&ScriptDepthOfFieldSettings::InternalGetapertureSize);
		metaData.scriptClass->AddInternalCall("Internal_SetapertureSize", (void*)&ScriptDepthOfFieldSettings::InternalSetapertureSize);
		metaData.scriptClass->AddInternalCall("Internal_GetfocalLength", (void*)&ScriptDepthOfFieldSettings::InternalGetfocalLength);
		metaData.scriptClass->AddInternalCall("Internal_SetfocalLength", (void*)&ScriptDepthOfFieldSettings::InternalSetfocalLength);
		metaData.scriptClass->AddInternalCall("Internal_GetsensorSize", (void*)&ScriptDepthOfFieldSettings::InternalGetsensorSize);
		metaData.scriptClass->AddInternalCall("Internal_SetsensorSize", (void*)&ScriptDepthOfFieldSettings::InternalSetsensorSize);
		metaData.scriptClass->AddInternalCall("Internal_GetbokehOcclusion", (void*)&ScriptDepthOfFieldSettings::InternalGetbokehOcclusion);
		metaData.scriptClass->AddInternalCall("Internal_SetbokehOcclusion", (void*)&ScriptDepthOfFieldSettings::InternalSetbokehOcclusion);
		metaData.scriptClass->AddInternalCall("Internal_GetocclusionDepthRange", (void*)&ScriptDepthOfFieldSettings::InternalGetocclusionDepthRange);
		metaData.scriptClass->AddInternalCall("Internal_SetocclusionDepthRange", (void*)&ScriptDepthOfFieldSettings::InternalSetocclusionDepthRange);

	}

	MonoObject* ScriptDepthOfFieldSettings::Create(const SPtr<DepthOfFieldSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptDepthOfFieldSettings>()) ScriptDepthOfFieldSettings(managedInstance, value);
		return managedInstance;
	}
	void ScriptDepthOfFieldSettings::InternalDepthOfFieldSettings(MonoObject* managedInstance)
	{
		SPtr<DepthOfFieldSettings> instance = bs_shared_ptr_new<DepthOfFieldSettings>();
		new (bs_alloc<ScriptDepthOfFieldSettings>())ScriptDepthOfFieldSettings(managedInstance, instance);
	}

	MonoObject* ScriptDepthOfFieldSettings::InternalGetbokehShape(ScriptDepthOfFieldSettings* thisPtr)
	{
		ResourceHandle<Texture> tmp__output;
		tmp__output = thisPtr->GetInternal()->bokehShape;

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().GetScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetbokehShape(ScriptDepthOfFieldSettings* thisPtr, MonoObject* value)
	{
		ResourceHandle<Texture> tmpvalue;
		ScriptRRefBase* scriptvalue;
		scriptvalue = ScriptRRefBase::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = static_resource_cast<Texture>(scriptvalue->GetHandle());
		thisPtr->GetInternal()->bokehShape = tmpvalue;
	}

	bool ScriptDepthOfFieldSettings::InternalGetenabled(ScriptDepthOfFieldSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->enabled;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetenabled(ScriptDepthOfFieldSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->enabled = value;
	}

	DepthOfFieldType ScriptDepthOfFieldSettings::InternalGettype(ScriptDepthOfFieldSettings* thisPtr)
	{
		DepthOfFieldType tmp__output;
		tmp__output = thisPtr->GetInternal()->type;

		DepthOfFieldType __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSettype(ScriptDepthOfFieldSettings* thisPtr, DepthOfFieldType value)
	{
		thisPtr->GetInternal()->type = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetfocalDistance(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->focalDistance;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetfocalDistance(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->focalDistance = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetfocalRange(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->focalRange;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetfocalRange(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->focalRange = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetnearTransitionRange(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->nearTransitionRange;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetnearTransitionRange(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->nearTransitionRange = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetfarTransitionRange(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->farTransitionRange;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetfarTransitionRange(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->farTransitionRange = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetnearBlurAmount(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->nearBlurAmount;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetnearBlurAmount(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->nearBlurAmount = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetfarBlurAmount(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->farBlurAmount;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetfarBlurAmount(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->farBlurAmount = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetmaxBokehSize(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->maxBokehSize;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetmaxBokehSize(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->maxBokehSize = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetadaptiveColorThreshold(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->adaptiveColorThreshold;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetadaptiveColorThreshold(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->adaptiveColorThreshold = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetadaptiveRadiusThreshold(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->adaptiveRadiusThreshold;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetadaptiveRadiusThreshold(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->adaptiveRadiusThreshold = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetapertureSize(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->apertureSize;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetapertureSize(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->apertureSize = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetfocalLength(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->focalLength;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetfocalLength(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->focalLength = value;
	}

	void ScriptDepthOfFieldSettings::InternalGetsensorSize(ScriptDepthOfFieldSettings* thisPtr, Vector2* __output)
	{
		Vector2 tmp__output;
		tmp__output = thisPtr->GetInternal()->sensorSize;

		*__output = tmp__output;


	}

	void ScriptDepthOfFieldSettings::InternalSetsensorSize(ScriptDepthOfFieldSettings* thisPtr, Vector2* value)
	{
		thisPtr->GetInternal()->sensorSize = *value;
	}

	bool ScriptDepthOfFieldSettings::InternalGetbokehOcclusion(ScriptDepthOfFieldSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->bokehOcclusion;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetbokehOcclusion(ScriptDepthOfFieldSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->bokehOcclusion = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetocclusionDepthRange(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->occlusionDepthRange;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetocclusionDepthRange(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->occlusionDepthRange = value;
	}
}
