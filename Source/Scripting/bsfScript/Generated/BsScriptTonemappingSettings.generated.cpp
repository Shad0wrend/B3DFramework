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

	void ScriptTonemappingSettings::InitRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_TonemappingSettings", (void*)&ScriptTonemappingSettings::InternalTonemappingSettings);
		metaData.scriptClass->AddInternalCall("Internal_GetfilmicCurveShoulderStrength", (void*)&ScriptTonemappingSettings::InternalGetfilmicCurveShoulderStrength);
		metaData.scriptClass->AddInternalCall("Internal_SetfilmicCurveShoulderStrength", (void*)&ScriptTonemappingSettings::InternalSetfilmicCurveShoulderStrength);
		metaData.scriptClass->AddInternalCall("Internal_GetfilmicCurveLinearStrength", (void*)&ScriptTonemappingSettings::InternalGetfilmicCurveLinearStrength);
		metaData.scriptClass->AddInternalCall("Internal_SetfilmicCurveLinearStrength", (void*)&ScriptTonemappingSettings::InternalSetfilmicCurveLinearStrength);
		metaData.scriptClass->AddInternalCall("Internal_GetfilmicCurveLinearAngle", (void*)&ScriptTonemappingSettings::InternalGetfilmicCurveLinearAngle);
		metaData.scriptClass->AddInternalCall("Internal_SetfilmicCurveLinearAngle", (void*)&ScriptTonemappingSettings::InternalSetfilmicCurveLinearAngle);
		metaData.scriptClass->AddInternalCall("Internal_GetfilmicCurveToeStrength", (void*)&ScriptTonemappingSettings::InternalGetfilmicCurveToeStrength);
		metaData.scriptClass->AddInternalCall("Internal_SetfilmicCurveToeStrength", (void*)&ScriptTonemappingSettings::InternalSetfilmicCurveToeStrength);
		metaData.scriptClass->AddInternalCall("Internal_GetfilmicCurveToeNumerator", (void*)&ScriptTonemappingSettings::InternalGetfilmicCurveToeNumerator);
		metaData.scriptClass->AddInternalCall("Internal_SetfilmicCurveToeNumerator", (void*)&ScriptTonemappingSettings::InternalSetfilmicCurveToeNumerator);
		metaData.scriptClass->AddInternalCall("Internal_GetfilmicCurveToeDenominator", (void*)&ScriptTonemappingSettings::InternalGetfilmicCurveToeDenominator);
		metaData.scriptClass->AddInternalCall("Internal_SetfilmicCurveToeDenominator", (void*)&ScriptTonemappingSettings::InternalSetfilmicCurveToeDenominator);
		metaData.scriptClass->AddInternalCall("Internal_GetfilmicCurveLinearWhitePoint", (void*)&ScriptTonemappingSettings::InternalGetfilmicCurveLinearWhitePoint);
		metaData.scriptClass->AddInternalCall("Internal_SetfilmicCurveLinearWhitePoint", (void*)&ScriptTonemappingSettings::InternalSetfilmicCurveLinearWhitePoint);

	}

	MonoObject* ScriptTonemappingSettings::Create(const SPtr<TonemappingSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->CreateInstance("bool", ctorParams);
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
