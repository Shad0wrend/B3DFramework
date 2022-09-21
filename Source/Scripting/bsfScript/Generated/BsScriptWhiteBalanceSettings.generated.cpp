//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptWhiteBalanceSettings.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptWhiteBalanceSettings::ScriptWhiteBalanceSettings(MonoObject* managedInstance, const SPtr<WhiteBalanceSettings>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptWhiteBalanceSettings::InitRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_WhiteBalanceSettings", (void*)&ScriptWhiteBalanceSettings::InternalWhiteBalanceSettings);
		metaData.scriptClass->AddInternalCall("Internal_Gettemperature", (void*)&ScriptWhiteBalanceSettings::InternalGettemperature);
		metaData.scriptClass->AddInternalCall("Internal_Settemperature", (void*)&ScriptWhiteBalanceSettings::InternalSettemperature);
		metaData.scriptClass->AddInternalCall("Internal_Gettint", (void*)&ScriptWhiteBalanceSettings::InternalGettint);
		metaData.scriptClass->AddInternalCall("Internal_Settint", (void*)&ScriptWhiteBalanceSettings::InternalSettint);

	}

	MonoObject* ScriptWhiteBalanceSettings::Create(const SPtr<WhiteBalanceSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptWhiteBalanceSettings>()) ScriptWhiteBalanceSettings(managedInstance, value);
		return managedInstance;
	}
	void ScriptWhiteBalanceSettings::InternalWhiteBalanceSettings(MonoObject* managedInstance)
	{
		SPtr<WhiteBalanceSettings> instance = bs_shared_ptr_new<WhiteBalanceSettings>();
		new (bs_alloc<ScriptWhiteBalanceSettings>())ScriptWhiteBalanceSettings(managedInstance, instance);
	}

	float ScriptWhiteBalanceSettings::InternalGettemperature(ScriptWhiteBalanceSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->temperature;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptWhiteBalanceSettings::InternalSettemperature(ScriptWhiteBalanceSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->temperature = value;
	}

	float ScriptWhiteBalanceSettings::InternalGettint(ScriptWhiteBalanceSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->tint;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptWhiteBalanceSettings::InternalSettint(ScriptWhiteBalanceSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->tint = value;
	}
}
