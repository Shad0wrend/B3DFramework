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

	void ScriptDepthOfFieldSettings::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_DepthOfFieldSettings", (void*)&ScriptDepthOfFieldSettings::InternalDepthOfFieldSettings);
		metaData.scriptClass->AddInternalCall("Internal_getbokehShape", (void*)&ScriptDepthOfFieldSettings::InternalGetbokehShape);
		metaData.scriptClass->AddInternalCall("Internal_setbokehShape", (void*)&ScriptDepthOfFieldSettings::InternalSetbokehShape);
		metaData.scriptClass->AddInternalCall("Internal_getenabled", (void*)&ScriptDepthOfFieldSettings::InternalGetenabled);
		metaData.scriptClass->AddInternalCall("Internal_setenabled", (void*)&ScriptDepthOfFieldSettings::InternalSetenabled);
		metaData.scriptClass->AddInternalCall("Internal_gettype", (void*)&ScriptDepthOfFieldSettings::InternalGettype);
		metaData.scriptClass->AddInternalCall("Internal_settype", (void*)&ScriptDepthOfFieldSettings::InternalSettype);
		metaData.scriptClass->AddInternalCall("Internal_getfocalDistance", (void*)&ScriptDepthOfFieldSettings::InternalGetfocalDistance);
		metaData.scriptClass->AddInternalCall("Internal_setfocalDistance", (void*)&ScriptDepthOfFieldSettings::InternalSetfocalDistance);
		metaData.scriptClass->AddInternalCall("Internal_getfocalRange", (void*)&ScriptDepthOfFieldSettings::InternalGetfocalRange);
		metaData.scriptClass->AddInternalCall("Internal_setfocalRange", (void*)&ScriptDepthOfFieldSettings::InternalSetfocalRange);
		metaData.scriptClass->AddInternalCall("Internal_getnearTransitionRange", (void*)&ScriptDepthOfFieldSettings::InternalGetnearTransitionRange);
		metaData.scriptClass->AddInternalCall("Internal_setnearTransitionRange", (void*)&ScriptDepthOfFieldSettings::InternalSetnearTransitionRange);
		metaData.scriptClass->AddInternalCall("Internal_getfarTransitionRange", (void*)&ScriptDepthOfFieldSettings::InternalGetfarTransitionRange);
		metaData.scriptClass->AddInternalCall("Internal_setfarTransitionRange", (void*)&ScriptDepthOfFieldSettings::InternalSetfarTransitionRange);
		metaData.scriptClass->AddInternalCall("Internal_getnearBlurAmount", (void*)&ScriptDepthOfFieldSettings::InternalGetnearBlurAmount);
		metaData.scriptClass->AddInternalCall("Internal_setnearBlurAmount", (void*)&ScriptDepthOfFieldSettings::InternalSetnearBlurAmount);
		metaData.scriptClass->AddInternalCall("Internal_getfarBlurAmount", (void*)&ScriptDepthOfFieldSettings::InternalGetfarBlurAmount);
		metaData.scriptClass->AddInternalCall("Internal_setfarBlurAmount", (void*)&ScriptDepthOfFieldSettings::InternalSetfarBlurAmount);
		metaData.scriptClass->AddInternalCall("Internal_getmaxBokehSize", (void*)&ScriptDepthOfFieldSettings::InternalGetmaxBokehSize);
		metaData.scriptClass->AddInternalCall("Internal_setmaxBokehSize", (void*)&ScriptDepthOfFieldSettings::InternalSetmaxBokehSize);
		metaData.scriptClass->AddInternalCall("Internal_getadaptiveColorThreshold", (void*)&ScriptDepthOfFieldSettings::InternalGetadaptiveColorThreshold);
		metaData.scriptClass->AddInternalCall("Internal_setadaptiveColorThreshold", (void*)&ScriptDepthOfFieldSettings::InternalSetadaptiveColorThreshold);
		metaData.scriptClass->AddInternalCall("Internal_getadaptiveRadiusThreshold", (void*)&ScriptDepthOfFieldSettings::InternalGetadaptiveRadiusThreshold);
		metaData.scriptClass->AddInternalCall("Internal_setadaptiveRadiusThreshold", (void*)&ScriptDepthOfFieldSettings::InternalSetadaptiveRadiusThreshold);
		metaData.scriptClass->AddInternalCall("Internal_getapertureSize", (void*)&ScriptDepthOfFieldSettings::InternalGetapertureSize);
		metaData.scriptClass->AddInternalCall("Internal_setapertureSize", (void*)&ScriptDepthOfFieldSettings::InternalSetapertureSize);
		metaData.scriptClass->AddInternalCall("Internal_getfocalLength", (void*)&ScriptDepthOfFieldSettings::InternalGetfocalLength);
		metaData.scriptClass->AddInternalCall("Internal_setfocalLength", (void*)&ScriptDepthOfFieldSettings::InternalSetfocalLength);
		metaData.scriptClass->AddInternalCall("Internal_getsensorSize", (void*)&ScriptDepthOfFieldSettings::InternalGetsensorSize);
		metaData.scriptClass->AddInternalCall("Internal_setsensorSize", (void*)&ScriptDepthOfFieldSettings::InternalSetsensorSize);
		metaData.scriptClass->AddInternalCall("Internal_getbokehOcclusion", (void*)&ScriptDepthOfFieldSettings::InternalGetbokehOcclusion);
		metaData.scriptClass->AddInternalCall("Internal_setbokehOcclusion", (void*)&ScriptDepthOfFieldSettings::InternalSetbokehOcclusion);
		metaData.scriptClass->AddInternalCall("Internal_getocclusionDepthRange", (void*)&ScriptDepthOfFieldSettings::InternalGetocclusionDepthRange);
		metaData.scriptClass->AddInternalCall("Internal_setocclusionDepthRange", (void*)&ScriptDepthOfFieldSettings::InternalSetocclusionDepthRange);

	}

	MonoObject* ScriptDepthOfFieldSettings::Create(const SPtr<DepthOfFieldSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
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
		script__output = ScriptResourceManager::Instance().getScriptRRef(tmp__output);
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
		scriptvalue = ScriptRRefBase::toNative(value);
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
