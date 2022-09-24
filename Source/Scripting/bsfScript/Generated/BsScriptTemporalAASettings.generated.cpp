//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptTemporalAASettings.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptTemporalAASettings::ScriptTemporalAASettings(MonoObject* managedInstance, const SPtr<TemporalAASettings>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptTemporalAASettings::InitRuntimeData()
	{
		metaData.ScriptClass->AddInternalCall("Internal_TemporalAASettings", (void*)&ScriptTemporalAASettings::InternalTemporalAASettings);
		metaData.ScriptClass->AddInternalCall("Internal_Getenabled", (void*)&ScriptTemporalAASettings::InternalGetenabled);
		metaData.ScriptClass->AddInternalCall("Internal_Setenabled", (void*)&ScriptTemporalAASettings::InternalSetenabled);
		metaData.ScriptClass->AddInternalCall("Internal_GetjitteredPositionCount", (void*)&ScriptTemporalAASettings::InternalGetjitteredPositionCount);
		metaData.ScriptClass->AddInternalCall("Internal_SetjitteredPositionCount", (void*)&ScriptTemporalAASettings::InternalSetjitteredPositionCount);
		metaData.ScriptClass->AddInternalCall("Internal_Getsharpness", (void*)&ScriptTemporalAASettings::InternalGetsharpness);
		metaData.ScriptClass->AddInternalCall("Internal_Setsharpness", (void*)&ScriptTemporalAASettings::InternalSetsharpness);

	}

	MonoObject* ScriptTemporalAASettings::Create(const SPtr<TemporalAASettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.ScriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptTemporalAASettings>()) ScriptTemporalAASettings(managedInstance, value);
		return managedInstance;
	}
	void ScriptTemporalAASettings::InternalTemporalAASettings(MonoObject* managedInstance)
	{
		SPtr<TemporalAASettings> instance = bs_shared_ptr_new<TemporalAASettings>();
		new (bs_alloc<ScriptTemporalAASettings>())ScriptTemporalAASettings(managedInstance, instance);
	}

	bool ScriptTemporalAASettings::InternalGetenabled(ScriptTemporalAASettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->Enabled;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTemporalAASettings::InternalSetenabled(ScriptTemporalAASettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->Enabled = value;
	}

	uint32_t ScriptTemporalAASettings::InternalGetjitteredPositionCount(ScriptTemporalAASettings* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->JitteredPositionCount;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTemporalAASettings::InternalSetjitteredPositionCount(ScriptTemporalAASettings* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->JitteredPositionCount = value;
	}

	float ScriptTemporalAASettings::InternalGetsharpness(ScriptTemporalAASettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->Sharpness;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTemporalAASettings::InternalSetsharpness(ScriptTemporalAASettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->Sharpness = value;
	}
}
