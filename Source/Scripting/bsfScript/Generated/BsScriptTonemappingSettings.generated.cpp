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
		metaData.ScriptClass->AddInternalCall("Internal_TonemappingSettings", (void*)&ScriptTonemappingSettings::InternalTonemappingSettings);
		metaData.ScriptClass->AddInternalCall("Internal_GetfilmicCurveShoulderStrength", (void*)&ScriptTonemappingSettings::InternalGetfilmicCurveShoulderStrength);
		metaData.ScriptClass->AddInternalCall("Internal_SetfilmicCurveShoulderStrength", (void*)&ScriptTonemappingSettings::InternalSetfilmicCurveShoulderStrength);
		metaData.ScriptClass->AddInternalCall("Internal_GetfilmicCurveLinearStrength", (void*)&ScriptTonemappingSettings::InternalGetfilmicCurveLinearStrength);
		metaData.ScriptClass->AddInternalCall("Internal_SetfilmicCurveLinearStrength", (void*)&ScriptTonemappingSettings::InternalSetfilmicCurveLinearStrength);
		metaData.ScriptClass->AddInternalCall("Internal_GetfilmicCurveLinearAngle", (void*)&ScriptTonemappingSettings::InternalGetfilmicCurveLinearAngle);
		metaData.ScriptClass->AddInternalCall("Internal_SetfilmicCurveLinearAngle", (void*)&ScriptTonemappingSettings::InternalSetfilmicCurveLinearAngle);
		metaData.ScriptClass->AddInternalCall("Internal_GetfilmicCurveToeStrength", (void*)&ScriptTonemappingSettings::InternalGetfilmicCurveToeStrength);
		metaData.ScriptClass->AddInternalCall("Internal_SetfilmicCurveToeStrength", (void*)&ScriptTonemappingSettings::InternalSetfilmicCurveToeStrength);
		metaData.ScriptClass->AddInternalCall("Internal_GetfilmicCurveToeNumerator", (void*)&ScriptTonemappingSettings::InternalGetfilmicCurveToeNumerator);
		metaData.ScriptClass->AddInternalCall("Internal_SetfilmicCurveToeNumerator", (void*)&ScriptTonemappingSettings::InternalSetfilmicCurveToeNumerator);
		metaData.ScriptClass->AddInternalCall("Internal_GetfilmicCurveToeDenominator", (void*)&ScriptTonemappingSettings::InternalGetfilmicCurveToeDenominator);
		metaData.ScriptClass->AddInternalCall("Internal_SetfilmicCurveToeDenominator", (void*)&ScriptTonemappingSettings::InternalSetfilmicCurveToeDenominator);
		metaData.ScriptClass->AddInternalCall("Internal_GetfilmicCurveLinearWhitePoint", (void*)&ScriptTonemappingSettings::InternalGetfilmicCurveLinearWhitePoint);
		metaData.ScriptClass->AddInternalCall("Internal_SetfilmicCurveLinearWhitePoint", (void*)&ScriptTonemappingSettings::InternalSetfilmicCurveLinearWhitePoint);

	}

	MonoObject* ScriptTonemappingSettings::Create(const SPtr<TonemappingSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.ScriptClass->CreateInstance("bool", ctorParams);
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
		tmp__output = thisPtr->GetInternal()->FilmicCurveShoulderStrength;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTonemappingSettings::InternalSetfilmicCurveShoulderStrength(ScriptTonemappingSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->FilmicCurveShoulderStrength = value;
	}

	float ScriptTonemappingSettings::InternalGetfilmicCurveLinearStrength(ScriptTonemappingSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->FilmicCurveLinearStrength;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTonemappingSettings::InternalSetfilmicCurveLinearStrength(ScriptTonemappingSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->FilmicCurveLinearStrength = value;
	}

	float ScriptTonemappingSettings::InternalGetfilmicCurveLinearAngle(ScriptTonemappingSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->FilmicCurveLinearAngle;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTonemappingSettings::InternalSetfilmicCurveLinearAngle(ScriptTonemappingSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->FilmicCurveLinearAngle = value;
	}

	float ScriptTonemappingSettings::InternalGetfilmicCurveToeStrength(ScriptTonemappingSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->FilmicCurveToeStrength;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTonemappingSettings::InternalSetfilmicCurveToeStrength(ScriptTonemappingSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->FilmicCurveToeStrength = value;
	}

	float ScriptTonemappingSettings::InternalGetfilmicCurveToeNumerator(ScriptTonemappingSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->FilmicCurveToeNumerator;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTonemappingSettings::InternalSetfilmicCurveToeNumerator(ScriptTonemappingSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->FilmicCurveToeNumerator = value;
	}

	float ScriptTonemappingSettings::InternalGetfilmicCurveToeDenominator(ScriptTonemappingSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->FilmicCurveToeDenominator;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTonemappingSettings::InternalSetfilmicCurveToeDenominator(ScriptTonemappingSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->FilmicCurveToeDenominator = value;
	}

	float ScriptTonemappingSettings::InternalGetfilmicCurveLinearWhitePoint(ScriptTonemappingSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->FilmicCurveLinearWhitePoint;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTonemappingSettings::InternalSetfilmicCurveLinearWhitePoint(ScriptTonemappingSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->FilmicCurveLinearWhitePoint = value;
	}
}
