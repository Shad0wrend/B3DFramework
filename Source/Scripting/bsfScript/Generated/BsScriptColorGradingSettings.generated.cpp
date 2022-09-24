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

	void ScriptColorGradingSettings::InitRuntimeData()
	{
		metaData.ScriptClass->AddInternalCall("Internal_Getsaturation", (void*)&ScriptColorGradingSettings::InternalGetsaturation);
		metaData.ScriptClass->AddInternalCall("Internal_Setsaturation", (void*)&ScriptColorGradingSettings::InternalSetsaturation);
		metaData.ScriptClass->AddInternalCall("Internal_Getcontrast", (void*)&ScriptColorGradingSettings::InternalGetcontrast);
		metaData.ScriptClass->AddInternalCall("Internal_Setcontrast", (void*)&ScriptColorGradingSettings::InternalSetcontrast);
		metaData.ScriptClass->AddInternalCall("Internal_Getgain", (void*)&ScriptColorGradingSettings::InternalGetgain);
		metaData.ScriptClass->AddInternalCall("Internal_Setgain", (void*)&ScriptColorGradingSettings::InternalSetgain);
		metaData.ScriptClass->AddInternalCall("Internal_Getoffset", (void*)&ScriptColorGradingSettings::InternalGetoffset);
		metaData.ScriptClass->AddInternalCall("Internal_Setoffset", (void*)&ScriptColorGradingSettings::InternalSetoffset);

	}

	MonoObject* ScriptColorGradingSettings::Create(const SPtr<ColorGradingSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.ScriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptColorGradingSettings>()) ScriptColorGradingSettings(managedInstance, value);
		return managedInstance;
	}
	void ScriptColorGradingSettings::InternalGetsaturation(ScriptColorGradingSettings* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->Saturation;

		*__output = tmp__output;


	}

	void ScriptColorGradingSettings::InternalSetsaturation(ScriptColorGradingSettings* thisPtr, Vector3* value)
	{
		thisPtr->GetInternal()->Saturation = *value;
	}

	void ScriptColorGradingSettings::InternalGetcontrast(ScriptColorGradingSettings* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->Contrast;

		*__output = tmp__output;


	}

	void ScriptColorGradingSettings::InternalSetcontrast(ScriptColorGradingSettings* thisPtr, Vector3* value)
	{
		thisPtr->GetInternal()->Contrast = *value;
	}

	void ScriptColorGradingSettings::InternalGetgain(ScriptColorGradingSettings* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->Gain;

		*__output = tmp__output;


	}

	void ScriptColorGradingSettings::InternalSetgain(ScriptColorGradingSettings* thisPtr, Vector3* value)
	{
		thisPtr->GetInternal()->Gain = *value;
	}

	void ScriptColorGradingSettings::InternalGetoffset(ScriptColorGradingSettings* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->Offset;

		*__output = tmp__output;


	}

	void ScriptColorGradingSettings::InternalSetoffset(ScriptColorGradingSettings* thisPtr, Vector3* value)
	{
		thisPtr->GetInternal()->Offset = *value;
	}
}
