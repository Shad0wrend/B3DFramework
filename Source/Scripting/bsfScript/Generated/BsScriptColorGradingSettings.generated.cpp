//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptColorGradingSettings.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "Wrappers/BsScriptVector.h"

namespace bs
{
	ScriptColorGradingSettings::ScriptColorGradingSettings(MonoObject* managedInstance, const SPtr<ColorGradingSettings>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptColorGradingSettings::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_getsaturation", (void*)&ScriptColorGradingSettings::Internal_getsaturation);
		metaData.scriptClass->addInternalCall("Internal_setsaturation", (void*)&ScriptColorGradingSettings::Internal_setsaturation);
		metaData.scriptClass->addInternalCall("Internal_getcontrast", (void*)&ScriptColorGradingSettings::Internal_getcontrast);
		metaData.scriptClass->addInternalCall("Internal_setcontrast", (void*)&ScriptColorGradingSettings::Internal_setcontrast);
		metaData.scriptClass->addInternalCall("Internal_getgain", (void*)&ScriptColorGradingSettings::Internal_getgain);
		metaData.scriptClass->addInternalCall("Internal_setgain", (void*)&ScriptColorGradingSettings::Internal_setgain);
		metaData.scriptClass->addInternalCall("Internal_getoffset", (void*)&ScriptColorGradingSettings::Internal_getoffset);
		metaData.scriptClass->addInternalCall("Internal_setoffset", (void*)&ScriptColorGradingSettings::Internal_setoffset);

	}

	MonoObject* ScriptColorGradingSettings::Create(const SPtr<ColorGradingSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptColorGradingSettings>()) ScriptColorGradingSettings(managedInstance, value);
		return managedInstance;
	}
	void ScriptColorGradingSettings::InternalGetsaturation(ScriptColorGradingSettings* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->saturation;

		*__output = tmp__output;


	}

	void ScriptColorGradingSettings::InternalSetsaturation(ScriptColorGradingSettings* thisPtr, Vector3* value)
	{
		thisPtr->GetInternal()->saturation = *value;
	}

	void ScriptColorGradingSettings::InternalGetcontrast(ScriptColorGradingSettings* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->contrast;

		*__output = tmp__output;


	}

	void ScriptColorGradingSettings::InternalSetcontrast(ScriptColorGradingSettings* thisPtr, Vector3* value)
	{
		thisPtr->GetInternal()->contrast = *value;
	}

	void ScriptColorGradingSettings::InternalGetgain(ScriptColorGradingSettings* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->gain;

		*__output = tmp__output;


	}

	void ScriptColorGradingSettings::InternalSetgain(ScriptColorGradingSettings* thisPtr, Vector3* value)
	{
		thisPtr->GetInternal()->gain = *value;
	}

	void ScriptColorGradingSettings::InternalGetoffset(ScriptColorGradingSettings* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->offset;

		*__output = tmp__output;


	}

	void ScriptColorGradingSettings::InternalSetoffset(ScriptColorGradingSettings* thisPtr, Vector3* value)
	{
		thisPtr->GetInternal()->offset = *value;
	}
}
