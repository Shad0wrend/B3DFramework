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
		metaData.scriptClass->addInternalCall("Internal_getfilmicCurveShoulderStrength", (void*)&ScriptTonemappingSettings::Internal_getfilmicCurveShoulderStrength);
		metaData.scriptClass->addInternalCall("Internal_setfilmicCurveShoulderStrength", (void*)&ScriptTonemappingSettings::Internal_setfilmicCurveShoulderStrength);
		metaData.scriptClass->addInternalCall("Internal_getfilmicCurveLinearStrength", (void*)&ScriptTonemappingSettings::Internal_getfilmicCurveLinearStrength);
		metaData.scriptClass->addInternalCall("Internal_setfilmicCurveLinearStrength", (void*)&ScriptTonemappingSettings::Internal_setfilmicCurveLinearStrength);
		metaData.scriptClass->addInternalCall("Internal_getfilmicCurveLinearAngle", (void*)&ScriptTonemappingSettings::Internal_getfilmicCurveLinearAngle);
		metaData.scriptClass->addInternalCall("Internal_setfilmicCurveLinearAngle", (void*)&ScriptTonemappingSettings::Internal_setfilmicCurveLinearAngle);
		metaData.scriptClass->addInternalCall("Internal_getfilmicCurveToeStrength", (void*)&ScriptTonemappingSettings::Internal_getfilmicCurveToeStrength);
		metaData.scriptClass->addInternalCall("Internal_setfilmicCurveToeStrength", (void*)&ScriptTonemappingSettings::Internal_setfilmicCurveToeStrength);
		metaData.scriptClass->addInternalCall("Internal_getfilmicCurveToeNumerator", (void*)&ScriptTonemappingSettings::Internal_getfilmicCurveToeNumerator);
		metaData.scriptClass->addInternalCall("Internal_setfilmicCurveToeNumerator", (void*)&ScriptTonemappingSettings::Internal_setfilmicCurveToeNumerator);
		metaData.scriptClass->addInternalCall("Internal_getfilmicCurveToeDenominator", (void*)&ScriptTonemappingSettings::Internal_getfilmicCurveToeDenominator);
		metaData.scriptClass->addInternalCall("Internal_setfilmicCurveToeDenominator", (void*)&ScriptTonemappingSettings::Internal_setfilmicCurveToeDenominator);
		metaData.scriptClass->addInternalCall("Internal_getfilmicCurveLinearWhitePoint", (void*)&ScriptTonemappingSettings::Internal_getfilmicCurveLinearWhitePoint);
		metaData.scriptClass->addInternalCall("Internal_setfilmicCurveLinearWhitePoint", (void*)&ScriptTonemappingSettings::Internal_setfilmicCurveLinearWhitePoint);

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
		tmp__output = thisPtr->getInternal()->filmicCurveShoulderStrength;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTonemappingSettings::InternalSetfilmicCurveShoulderStrength(ScriptTonemappingSettings* thisPtr, float value)
	{
		thisPtr->getInternal()->filmicCurveShoulderStrength = value;
	}

	float ScriptTonemappingSettings::InternalGetfilmicCurveLinearStrength(ScriptTonemappingSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getInternal()->filmicCurveLinearStrength;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTonemappingSettings::InternalSetfilmicCurveLinearStrength(ScriptTonemappingSettings* thisPtr, float value)
	{
		thisPtr->getInternal()->filmicCurveLinearStrength = value;
	}

	float ScriptTonemappingSettings::InternalGetfilmicCurveLinearAngle(ScriptTonemappingSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getInternal()->filmicCurveLinearAngle;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTonemappingSettings::InternalSetfilmicCurveLinearAngle(ScriptTonemappingSettings* thisPtr, float value)
	{
		thisPtr->getInternal()->filmicCurveLinearAngle = value;
	}

	float ScriptTonemappingSettings::InternalGetfilmicCurveToeStrength(ScriptTonemappingSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getInternal()->filmicCurveToeStrength;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTonemappingSettings::InternalSetfilmicCurveToeStrength(ScriptTonemappingSettings* thisPtr, float value)
	{
		thisPtr->getInternal()->filmicCurveToeStrength = value;
	}

	float ScriptTonemappingSettings::InternalGetfilmicCurveToeNumerator(ScriptTonemappingSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getInternal()->filmicCurveToeNumerator;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTonemappingSettings::InternalSetfilmicCurveToeNumerator(ScriptTonemappingSettings* thisPtr, float value)
	{
		thisPtr->getInternal()->filmicCurveToeNumerator = value;
	}

	float ScriptTonemappingSettings::InternalGetfilmicCurveToeDenominator(ScriptTonemappingSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getInternal()->filmicCurveToeDenominator;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTonemappingSettings::InternalSetfilmicCurveToeDenominator(ScriptTonemappingSettings* thisPtr, float value)
	{
		thisPtr->getInternal()->filmicCurveToeDenominator = value;
	}

	float ScriptTonemappingSettings::InternalGetfilmicCurveLinearWhitePoint(ScriptTonemappingSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getInternal()->filmicCurveLinearWhitePoint;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTonemappingSettings::InternalSetfilmicCurveLinearWhitePoint(ScriptTonemappingSettings* thisPtr, float value)
	{
		thisPtr->getInternal()->filmicCurveLinearWhitePoint = value;
	}
}
