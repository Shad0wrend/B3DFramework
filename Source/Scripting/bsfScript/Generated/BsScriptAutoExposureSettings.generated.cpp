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

	void ScriptAutoExposureSettings::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_AutoExposureSettings", (void*)&ScriptAutoExposureSettings::InternalAutoExposureSettings);
		metaData.scriptClass->addInternalCall("Internal_gethistogramLog2Min", (void*)&ScriptAutoExposureSettings::Internal_gethistogramLog2Min);
		metaData.scriptClass->addInternalCall("Internal_sethistogramLog2Min", (void*)&ScriptAutoExposureSettings::Internal_sethistogramLog2Min);
		metaData.scriptClass->addInternalCall("Internal_gethistogramLog2Max", (void*)&ScriptAutoExposureSettings::Internal_gethistogramLog2Max);
		metaData.scriptClass->addInternalCall("Internal_sethistogramLog2Max", (void*)&ScriptAutoExposureSettings::Internal_sethistogramLog2Max);
		metaData.scriptClass->addInternalCall("Internal_gethistogramPctLow", (void*)&ScriptAutoExposureSettings::Internal_gethistogramPctLow);
		metaData.scriptClass->addInternalCall("Internal_sethistogramPctLow", (void*)&ScriptAutoExposureSettings::Internal_sethistogramPctLow);
		metaData.scriptClass->addInternalCall("Internal_gethistogramPctHigh", (void*)&ScriptAutoExposureSettings::Internal_gethistogramPctHigh);
		metaData.scriptClass->addInternalCall("Internal_sethistogramPctHigh", (void*)&ScriptAutoExposureSettings::Internal_sethistogramPctHigh);
		metaData.scriptClass->addInternalCall("Internal_getminEyeAdaptation", (void*)&ScriptAutoExposureSettings::Internal_getminEyeAdaptation);
		metaData.scriptClass->addInternalCall("Internal_setminEyeAdaptation", (void*)&ScriptAutoExposureSettings::Internal_setminEyeAdaptation);
		metaData.scriptClass->addInternalCall("Internal_getmaxEyeAdaptation", (void*)&ScriptAutoExposureSettings::Internal_getmaxEyeAdaptation);
		metaData.scriptClass->addInternalCall("Internal_setmaxEyeAdaptation", (void*)&ScriptAutoExposureSettings::Internal_setmaxEyeAdaptation);
		metaData.scriptClass->addInternalCall("Internal_geteyeAdaptationSpeedUp", (void*)&ScriptAutoExposureSettings::Internal_geteyeAdaptationSpeedUp);
		metaData.scriptClass->addInternalCall("Internal_seteyeAdaptationSpeedUp", (void*)&ScriptAutoExposureSettings::Internal_seteyeAdaptationSpeedUp);
		metaData.scriptClass->addInternalCall("Internal_geteyeAdaptationSpeedDown", (void*)&ScriptAutoExposureSettings::Internal_geteyeAdaptationSpeedDown);
		metaData.scriptClass->addInternalCall("Internal_seteyeAdaptationSpeedDown", (void*)&ScriptAutoExposureSettings::Internal_seteyeAdaptationSpeedDown);

	}

	MonoObject* ScriptAutoExposureSettings::Create(const SPtr<AutoExposureSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
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
		tmp__output = thisPtr->GetInternal()->histogramLog2Min;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAutoExposureSettings::InternalSethistogramLog2Min(ScriptAutoExposureSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->histogramLog2Min = value;
	}

	float ScriptAutoExposureSettings::InternalGethistogramLog2Max(ScriptAutoExposureSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->histogramLog2Max;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAutoExposureSettings::InternalSethistogramLog2Max(ScriptAutoExposureSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->histogramLog2Max = value;
	}

	float ScriptAutoExposureSettings::InternalGethistogramPctLow(ScriptAutoExposureSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->histogramPctLow;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAutoExposureSettings::InternalSethistogramPctLow(ScriptAutoExposureSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->histogramPctLow = value;
	}

	float ScriptAutoExposureSettings::InternalGethistogramPctHigh(ScriptAutoExposureSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->histogramPctHigh;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAutoExposureSettings::InternalSethistogramPctHigh(ScriptAutoExposureSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->histogramPctHigh = value;
	}

	float ScriptAutoExposureSettings::InternalGetminEyeAdaptation(ScriptAutoExposureSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->minEyeAdaptation;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAutoExposureSettings::InternalSetminEyeAdaptation(ScriptAutoExposureSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->minEyeAdaptation = value;
	}

	float ScriptAutoExposureSettings::InternalGetmaxEyeAdaptation(ScriptAutoExposureSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->maxEyeAdaptation;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAutoExposureSettings::InternalSetmaxEyeAdaptation(ScriptAutoExposureSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->maxEyeAdaptation = value;
	}

	float ScriptAutoExposureSettings::InternalGeteyeAdaptationSpeedUp(ScriptAutoExposureSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->eyeAdaptationSpeedUp;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAutoExposureSettings::InternalSeteyeAdaptationSpeedUp(ScriptAutoExposureSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->eyeAdaptationSpeedUp = value;
	}

	float ScriptAutoExposureSettings::InternalGeteyeAdaptationSpeedDown(ScriptAutoExposureSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->eyeAdaptationSpeedDown;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAutoExposureSettings::InternalSeteyeAdaptationSpeedDown(ScriptAutoExposureSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->eyeAdaptationSpeedDown = value;
	}
}
