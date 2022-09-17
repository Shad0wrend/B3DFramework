//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptTonemappingSettings.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptTonemappingSettings::ScriptTonemappingSettings(MonoObject* managedInstance, const SPtr<TonemappingSettings>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptTonemappingSettings::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_TonemappingSettings", (void*)&ScriptTonemappingSettings::InternalTonemappingSettings);
		metaData.scriptClass->AddInternalCall("Internal_getfilmicCurveShoulderStrength", (void*)&ScriptTonemappingSettings::InternalGetfilmicCurveShoulderStrength);
		metaData.scriptClass->AddInternalCall("Internal_setfilmicCurveShoulderStrength", (void*)&ScriptTonemappingSettings::InternalSetfilmicCurveShoulderStrength);
		metaData.scriptClass->AddInternalCall("Internal_getfilmicCurveLinearStrength", (void*)&ScriptTonemappingSettings::InternalGetfilmicCurveLinearStrength);
		metaData.scriptClass->AddInternalCall("Internal_setfilmicCurveLinearStrength", (void*)&ScriptTonemappingSettings::InternalSetfilmicCurveLinearStrength);
		metaData.scriptClass->AddInternalCall("Internal_getfilmicCurveLinearAngle", (void*)&ScriptTonemappingSettings::InternalGetfilmicCurveLinearAngle);
		metaData.scriptClass->AddInternalCall("Internal_setfilmicCurveLinearAngle", (void*)&ScriptTonemappingSettings::InternalSetfilmicCurveLinearAngle);
		metaData.scriptClass->AddInternalCall("Internal_getfilmicCurveToeStrength", (void*)&ScriptTonemappingSettings::InternalGetfilmicCurveToeStrength);
		metaData.scriptClass->AddInternalCall("Internal_setfilmicCurveToeStrength", (void*)&ScriptTonemappingSettings::InternalSetfilmicCurveToeStrength);
		metaData.scriptClass->AddInternalCall("Internal_getfilmicCurveToeNumerator", (void*)&ScriptTonemappingSettings::InternalGetfilmicCurveToeNumerator);
		metaData.scriptClass->AddInternalCall("Internal_setfilmicCurveToeNumerator", (void*)&ScriptTonemappingSettings::InternalSetfilmicCurveToeNumerator);
		metaData.scriptClass->AddInternalCall("Internal_getfilmicCurveToeDenominator", (void*)&ScriptTonemappingSettings::InternalGetfilmicCurveToeDenominator);
		metaData.scriptClass->AddInternalCall("Internal_setfilmicCurveToeDenominator", (void*)&ScriptTonemappingSettings::InternalSetfilmicCurveToeDenominator);
		metaData.scriptClass->AddInternalCall("Internal_getfilmicCurveLinearWhitePoint", (void*)&ScriptTonemappingSettings::InternalGetfilmicCurveLinearWhitePoint);
		metaData.scriptClass->AddInternalCall("Internal_setfilmicCurveLinearWhitePoint", (void*)&ScriptTonemappingSettings::InternalSetfilmicCurveLinearWhitePoint);

	}

	MonoObject* ScriptTonemappingSettings::Create(const SPtr<TonemappingSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptTonemappingSettings>()) ScriptTonemappingSettings(managedInstance, value);
		return managedInstance;
	}
	void ScriptTonemappingSettings::InternalTonemappingSettings(MonoObject* managedInstance)
	{
		SPtr<TonemappingSettings> instance = bs_shared_ptr_new<TonemappingSettings>();
		new (bs_alloc<ScriptTonemappingSettings>())ScriptTonemappingSettings(managedInstance, instance);
	}

	float ScriptTonemappingSettings::InternalGetfilmicCurveShoulderStrength(ScriptTonemappingSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->filmicCurveShoulderStrength;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTonemappingSettings::InternalSetfilmicCurveShoulderStrength(ScriptTonemappingSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->filmicCurveShoulderStrength = value;
	}

	float ScriptTonemappingSettings::InternalGetfilmicCurveLinearStrength(ScriptTonemappingSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->filmicCurveLinearStrength;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTonemappingSettings::InternalSetfilmicCurveLinearStrength(ScriptTonemappingSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->filmicCurveLinearStrength = value;
	}

	float ScriptTonemappingSettings::InternalGetfilmicCurveLinearAngle(ScriptTonemappingSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->filmicCurveLinearAngle;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTonemappingSettings::InternalSetfilmicCurveLinearAngle(ScriptTonemappingSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->filmicCurveLinearAngle = value;
	}

	float ScriptTonemappingSettings::InternalGetfilmicCurveToeStrength(ScriptTonemappingSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->filmicCurveToeStrength;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTonemappingSettings::InternalSetfilmicCurveToeStrength(ScriptTonemappingSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->filmicCurveToeStrength = value;
	}

	float ScriptTonemappingSettings::InternalGetfilmicCurveToeNumerator(ScriptTonemappingSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->filmicCurveToeNumerator;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTonemappingSettings::InternalSetfilmicCurveToeNumerator(ScriptTonemappingSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->filmicCurveToeNumerator = value;
	}

	float ScriptTonemappingSettings::InternalGetfilmicCurveToeDenominator(ScriptTonemappingSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->filmicCurveToeDenominator;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTonemappingSettings::InternalSetfilmicCurveToeDenominator(ScriptTonemappingSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->filmicCurveToeDenominator = value;
	}

	float ScriptTonemappingSettings::InternalGetfilmicCurveLinearWhitePoint(ScriptTonemappingSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->filmicCurveLinearWhitePoint;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTonemappingSettings::InternalSetfilmicCurveLinearWhitePoint(ScriptTonemappingSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->filmicCurveLinearWhitePoint = value;
	}
}
