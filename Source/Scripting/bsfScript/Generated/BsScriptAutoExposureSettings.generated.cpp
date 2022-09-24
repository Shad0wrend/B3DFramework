//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptAutoExposureSettings.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptAutoExposureSettings::ScriptAutoExposureSettings(MonoObject* managedInstance, const SPtr<AutoExposureSettings>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptAutoExposureSettings::InitRuntimeData()
	{
		metaData.ScriptClass->AddInternalCall("Internal_AutoExposureSettings", (void*)&ScriptAutoExposureSettings::InternalAutoExposureSettings);
		metaData.ScriptClass->AddInternalCall("Internal_GethistogramLog2Min", (void*)&ScriptAutoExposureSettings::InternalGethistogramLog2Min);
		metaData.ScriptClass->AddInternalCall("Internal_SethistogramLog2Min", (void*)&ScriptAutoExposureSettings::InternalSethistogramLog2Min);
		metaData.ScriptClass->AddInternalCall("Internal_GethistogramLog2Max", (void*)&ScriptAutoExposureSettings::InternalGethistogramLog2Max);
		metaData.ScriptClass->AddInternalCall("Internal_SethistogramLog2Max", (void*)&ScriptAutoExposureSettings::InternalSethistogramLog2Max);
		metaData.ScriptClass->AddInternalCall("Internal_GethistogramPctLow", (void*)&ScriptAutoExposureSettings::InternalGethistogramPctLow);
		metaData.ScriptClass->AddInternalCall("Internal_SethistogramPctLow", (void*)&ScriptAutoExposureSettings::InternalSethistogramPctLow);
		metaData.ScriptClass->AddInternalCall("Internal_GethistogramPctHigh", (void*)&ScriptAutoExposureSettings::InternalGethistogramPctHigh);
		metaData.ScriptClass->AddInternalCall("Internal_SethistogramPctHigh", (void*)&ScriptAutoExposureSettings::InternalSethistogramPctHigh);
		metaData.ScriptClass->AddInternalCall("Internal_GetminEyeAdaptation", (void*)&ScriptAutoExposureSettings::InternalGetminEyeAdaptation);
		metaData.ScriptClass->AddInternalCall("Internal_SetminEyeAdaptation", (void*)&ScriptAutoExposureSettings::InternalSetminEyeAdaptation);
		metaData.ScriptClass->AddInternalCall("Internal_GetmaxEyeAdaptation", (void*)&ScriptAutoExposureSettings::InternalGetmaxEyeAdaptation);
		metaData.ScriptClass->AddInternalCall("Internal_SetmaxEyeAdaptation", (void*)&ScriptAutoExposureSettings::InternalSetmaxEyeAdaptation);
		metaData.ScriptClass->AddInternalCall("Internal_GeteyeAdaptationSpeedUp", (void*)&ScriptAutoExposureSettings::InternalGeteyeAdaptationSpeedUp);
		metaData.ScriptClass->AddInternalCall("Internal_SeteyeAdaptationSpeedUp", (void*)&ScriptAutoExposureSettings::InternalSeteyeAdaptationSpeedUp);
		metaData.ScriptClass->AddInternalCall("Internal_GeteyeAdaptationSpeedDown", (void*)&ScriptAutoExposureSettings::InternalGeteyeAdaptationSpeedDown);
		metaData.ScriptClass->AddInternalCall("Internal_SeteyeAdaptationSpeedDown", (void*)&ScriptAutoExposureSettings::InternalSeteyeAdaptationSpeedDown);

	}

	MonoObject* ScriptAutoExposureSettings::Create(const SPtr<AutoExposureSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.ScriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptAutoExposureSettings>()) ScriptAutoExposureSettings(managedInstance, value);
		return managedInstance;
	}
	void ScriptAutoExposureSettings::InternalAutoExposureSettings(MonoObject* managedInstance)
	{
		SPtr<AutoExposureSettings> instance = bs_shared_ptr_new<AutoExposureSettings>();
		new (bs_alloc<ScriptAutoExposureSettings>())ScriptAutoExposureSettings(managedInstance, instance);
	}

	float ScriptAutoExposureSettings::InternalGethistogramLog2Min(ScriptAutoExposureSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->HistogramLog2Min;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAutoExposureSettings::InternalSethistogramLog2Min(ScriptAutoExposureSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->HistogramLog2Min = value;
	}

	float ScriptAutoExposureSettings::InternalGethistogramLog2Max(ScriptAutoExposureSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->HistogramLog2Max;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAutoExposureSettings::InternalSethistogramLog2Max(ScriptAutoExposureSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->HistogramLog2Max = value;
	}

	float ScriptAutoExposureSettings::InternalGethistogramPctLow(ScriptAutoExposureSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->HistogramPctLow;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAutoExposureSettings::InternalSethistogramPctLow(ScriptAutoExposureSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->HistogramPctLow = value;
	}

	float ScriptAutoExposureSettings::InternalGethistogramPctHigh(ScriptAutoExposureSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->HistogramPctHigh;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAutoExposureSettings::InternalSethistogramPctHigh(ScriptAutoExposureSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->HistogramPctHigh = value;
	}

	float ScriptAutoExposureSettings::InternalGetminEyeAdaptation(ScriptAutoExposureSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->MinEyeAdaptation;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAutoExposureSettings::InternalSetminEyeAdaptation(ScriptAutoExposureSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->MinEyeAdaptation = value;
	}

	float ScriptAutoExposureSettings::InternalGetmaxEyeAdaptation(ScriptAutoExposureSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->MaxEyeAdaptation;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAutoExposureSettings::InternalSetmaxEyeAdaptation(ScriptAutoExposureSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->MaxEyeAdaptation = value;
	}

	float ScriptAutoExposureSettings::InternalGeteyeAdaptationSpeedUp(ScriptAutoExposureSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->EyeAdaptationSpeedUp;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAutoExposureSettings::InternalSeteyeAdaptationSpeedUp(ScriptAutoExposureSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->EyeAdaptationSpeedUp = value;
	}

	float ScriptAutoExposureSettings::InternalGeteyeAdaptationSpeedDown(ScriptAutoExposureSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->EyeAdaptationSpeedDown;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAutoExposureSettings::InternalSeteyeAdaptationSpeedDown(ScriptAutoExposureSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->EyeAdaptationSpeedDown = value;
	}
}
