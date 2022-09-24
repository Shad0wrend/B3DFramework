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
		metaData.ScriptClass->AddInternalCall("Internal_DepthOfFieldSettings", (void*)&ScriptDepthOfFieldSettings::InternalDepthOfFieldSettings);
		metaData.ScriptClass->AddInternalCall("Internal_GetbokehShape", (void*)&ScriptDepthOfFieldSettings::InternalGetbokehShape);
		metaData.ScriptClass->AddInternalCall("Internal_SetbokehShape", (void*)&ScriptDepthOfFieldSettings::InternalSetbokehShape);
		metaData.ScriptClass->AddInternalCall("Internal_Getenabled", (void*)&ScriptDepthOfFieldSettings::InternalGetenabled);
		metaData.ScriptClass->AddInternalCall("Internal_Setenabled", (void*)&ScriptDepthOfFieldSettings::InternalSetenabled);
		metaData.ScriptClass->AddInternalCall("Internal_Gettype", (void*)&ScriptDepthOfFieldSettings::InternalGettype);
		metaData.ScriptClass->AddInternalCall("Internal_Settype", (void*)&ScriptDepthOfFieldSettings::InternalSettype);
		metaData.ScriptClass->AddInternalCall("Internal_GetfocalDistance", (void*)&ScriptDepthOfFieldSettings::InternalGetfocalDistance);
		metaData.ScriptClass->AddInternalCall("Internal_SetfocalDistance", (void*)&ScriptDepthOfFieldSettings::InternalSetfocalDistance);
		metaData.ScriptClass->AddInternalCall("Internal_GetfocalRange", (void*)&ScriptDepthOfFieldSettings::InternalGetfocalRange);
		metaData.ScriptClass->AddInternalCall("Internal_SetfocalRange", (void*)&ScriptDepthOfFieldSettings::InternalSetfocalRange);
		metaData.ScriptClass->AddInternalCall("Internal_GetnearTransitionRange", (void*)&ScriptDepthOfFieldSettings::InternalGetnearTransitionRange);
		metaData.ScriptClass->AddInternalCall("Internal_SetnearTransitionRange", (void*)&ScriptDepthOfFieldSettings::InternalSetnearTransitionRange);
		metaData.ScriptClass->AddInternalCall("Internal_GetfarTransitionRange", (void*)&ScriptDepthOfFieldSettings::InternalGetfarTransitionRange);
		metaData.ScriptClass->AddInternalCall("Internal_SetfarTransitionRange", (void*)&ScriptDepthOfFieldSettings::InternalSetfarTransitionRange);
		metaData.ScriptClass->AddInternalCall("Internal_GetnearBlurAmount", (void*)&ScriptDepthOfFieldSettings::InternalGetnearBlurAmount);
		metaData.ScriptClass->AddInternalCall("Internal_SetnearBlurAmount", (void*)&ScriptDepthOfFieldSettings::InternalSetnearBlurAmount);
		metaData.ScriptClass->AddInternalCall("Internal_GetfarBlurAmount", (void*)&ScriptDepthOfFieldSettings::InternalGetfarBlurAmount);
		metaData.ScriptClass->AddInternalCall("Internal_SetfarBlurAmount", (void*)&ScriptDepthOfFieldSettings::InternalSetfarBlurAmount);
		metaData.ScriptClass->AddInternalCall("Internal_GetmaxBokehSize", (void*)&ScriptDepthOfFieldSettings::InternalGetmaxBokehSize);
		metaData.ScriptClass->AddInternalCall("Internal_SetmaxBokehSize", (void*)&ScriptDepthOfFieldSettings::InternalSetmaxBokehSize);
		metaData.ScriptClass->AddInternalCall("Internal_GetadaptiveColorThreshold", (void*)&ScriptDepthOfFieldSettings::InternalGetadaptiveColorThreshold);
		metaData.ScriptClass->AddInternalCall("Internal_SetadaptiveColorThreshold", (void*)&ScriptDepthOfFieldSettings::InternalSetadaptiveColorThreshold);
		metaData.ScriptClass->AddInternalCall("Internal_GetadaptiveRadiusThreshold", (void*)&ScriptDepthOfFieldSettings::InternalGetadaptiveRadiusThreshold);
		metaData.ScriptClass->AddInternalCall("Internal_SetadaptiveRadiusThreshold", (void*)&ScriptDepthOfFieldSettings::InternalSetadaptiveRadiusThreshold);
		metaData.ScriptClass->AddInternalCall("Internal_GetapertureSize", (void*)&ScriptDepthOfFieldSettings::InternalGetapertureSize);
		metaData.ScriptClass->AddInternalCall("Internal_SetapertureSize", (void*)&ScriptDepthOfFieldSettings::InternalSetapertureSize);
		metaData.ScriptClass->AddInternalCall("Internal_GetfocalLength", (void*)&ScriptDepthOfFieldSettings::InternalGetfocalLength);
		metaData.ScriptClass->AddInternalCall("Internal_SetfocalLength", (void*)&ScriptDepthOfFieldSettings::InternalSetfocalLength);
		metaData.ScriptClass->AddInternalCall("Internal_GetsensorSize", (void*)&ScriptDepthOfFieldSettings::InternalGetsensorSize);
		metaData.ScriptClass->AddInternalCall("Internal_SetsensorSize", (void*)&ScriptDepthOfFieldSettings::InternalSetsensorSize);
		metaData.ScriptClass->AddInternalCall("Internal_GetbokehOcclusion", (void*)&ScriptDepthOfFieldSettings::InternalGetbokehOcclusion);
		metaData.ScriptClass->AddInternalCall("Internal_SetbokehOcclusion", (void*)&ScriptDepthOfFieldSettings::InternalSetbokehOcclusion);
		metaData.ScriptClass->AddInternalCall("Internal_GetocclusionDepthRange", (void*)&ScriptDepthOfFieldSettings::InternalGetocclusionDepthRange);
		metaData.ScriptClass->AddInternalCall("Internal_SetocclusionDepthRange", (void*)&ScriptDepthOfFieldSettings::InternalSetocclusionDepthRange);

	}

	MonoObject* ScriptDepthOfFieldSettings::Create(const SPtr<DepthOfFieldSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.ScriptClass->CreateInstance("bool", ctorParams);
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
		tmp__output = thisPtr->GetInternal()->BokehShape;

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
		thisPtr->GetInternal()->BokehShape = tmpvalue;
	}

	bool ScriptDepthOfFieldSettings::InternalGetenabled(ScriptDepthOfFieldSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->Enabled;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetenabled(ScriptDepthOfFieldSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->Enabled = value;
	}

	DepthOfFieldType ScriptDepthOfFieldSettings::InternalGettype(ScriptDepthOfFieldSettings* thisPtr)
	{
		DepthOfFieldType tmp__output;
		tmp__output = thisPtr->GetInternal()->Type;

		DepthOfFieldType __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSettype(ScriptDepthOfFieldSettings* thisPtr, DepthOfFieldType value)
	{
		thisPtr->GetInternal()->Type = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetfocalDistance(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->FocalDistance;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetfocalDistance(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->FocalDistance = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetfocalRange(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->FocalRange;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetfocalRange(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->FocalRange = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetnearTransitionRange(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->NearTransitionRange;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetnearTransitionRange(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->NearTransitionRange = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetfarTransitionRange(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->FarTransitionRange;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetfarTransitionRange(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->FarTransitionRange = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetnearBlurAmount(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->NearBlurAmount;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetnearBlurAmount(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->NearBlurAmount = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetfarBlurAmount(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->FarBlurAmount;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetfarBlurAmount(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->FarBlurAmount = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetmaxBokehSize(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->MaxBokehSize;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetmaxBokehSize(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->MaxBokehSize = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetadaptiveColorThreshold(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->AdaptiveColorThreshold;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetadaptiveColorThreshold(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->AdaptiveColorThreshold = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetadaptiveRadiusThreshold(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->AdaptiveRadiusThreshold;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetadaptiveRadiusThreshold(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->AdaptiveRadiusThreshold = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetapertureSize(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->ApertureSize;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetapertureSize(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->ApertureSize = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetfocalLength(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->FocalLength;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetfocalLength(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->FocalLength = value;
	}

	void ScriptDepthOfFieldSettings::InternalGetsensorSize(ScriptDepthOfFieldSettings* thisPtr, Vector2* __output)
	{
		Vector2 tmp__output;
		tmp__output = thisPtr->GetInternal()->SensorSize;

		*__output = tmp__output;


	}

	void ScriptDepthOfFieldSettings::InternalSetsensorSize(ScriptDepthOfFieldSettings* thisPtr, Vector2* value)
	{
		thisPtr->GetInternal()->SensorSize = *value;
	}

	bool ScriptDepthOfFieldSettings::InternalGetbokehOcclusion(ScriptDepthOfFieldSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->BokehOcclusion;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetbokehOcclusion(ScriptDepthOfFieldSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->BokehOcclusion = value;
	}

	float ScriptDepthOfFieldSettings::InternalGetocclusionDepthRange(ScriptDepthOfFieldSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->OcclusionDepthRange;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptDepthOfFieldSettings::InternalSetocclusionDepthRange(ScriptDepthOfFieldSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->OcclusionDepthRange = value;
	}
}
