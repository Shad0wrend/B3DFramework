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
		metaData.scriptClass->AddInternalCall("Internal_getsaturation", (void*)&ScriptColorGradingSettings::InternalGetsaturation);
		metaData.scriptClass->AddInternalCall("Internal_setsaturation", (void*)&ScriptColorGradingSettings::InternalSetsaturation);
		metaData.scriptClass->AddInternalCall("Internal_getcontrast", (void*)&ScriptColorGradingSettings::InternalGetcontrast);
		metaData.scriptClass->AddInternalCall("Internal_setcontrast", (void*)&ScriptColorGradingSettings::InternalSetcontrast);
		metaData.scriptClass->AddInternalCall("Internal_getgain", (void*)&ScriptColorGradingSettings::InternalGetgain);
		metaData.scriptClass->AddInternalCall("Internal_setgain", (void*)&ScriptColorGradingSettings::InternalSetgain);
		metaData.scriptClass->AddInternalCall("Internal_getoffset", (void*)&ScriptColorGradingSettings::InternalGetoffset);
		metaData.scriptClass->AddInternalCall("Internal_setoffset", (void*)&ScriptColorGradingSettings::InternalSetoffset);

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
