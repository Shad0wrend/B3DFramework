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

	void ScriptTemporalAASettings::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_TemporalAASettings", (void*)&ScriptTemporalAASettings::InternalTemporalAaSettings);
		metaData.scriptClass->AddInternalCall("Internal_getenabled", (void*)&ScriptTemporalAASettings::InternalGetenabled);
		metaData.scriptClass->AddInternalCall("Internal_setenabled", (void*)&ScriptTemporalAASettings::InternalSetenabled);

	}

	MonoObject* ScriptTemporalAASettings::Create(const SPtr<TemporalAASettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptTemporalAASettings>()) ScriptTemporalAASettings(managedInstance, value);
		return managedInstance;
	}
	void ScriptTemporalAASettings::InternalTemporalAaSettings(MonoObject* managedInstance)
	{
		SPtr<TemporalAASettings> instance = bs_shared_ptr_new<TemporalAASettings>();
		new (bs_alloc<ScriptTemporalAASettings>())ScriptTemporalAASettings(managedInstance, instance);
	}

	bool ScriptTemporalAASettings::InternalGetenabled(ScriptTemporalAASettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->enabled;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTemporalAASettings::InternalSetenabled(ScriptTemporalAASettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->enabled = value;
	}
}
