//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptBloomSettings.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "Wrappers/BsScriptColor.h"

namespace bs
{
	ScriptBloomSettings::ScriptBloomSettings(MonoObject* managedInstance, const SPtr<BloomSettings>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptBloomSettings::InitRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_BloomSettings", (void*)&ScriptBloomSettings::InternalBloomSettings);
		metaData.scriptClass->AddInternalCall("Internal_Getenabled", (void*)&ScriptBloomSettings::InternalGetenabled);
		metaData.scriptClass->AddInternalCall("Internal_Setenabled", (void*)&ScriptBloomSettings::InternalSetenabled);
		metaData.scriptClass->AddInternalCall("Internal_Getquality", (void*)&ScriptBloomSettings::InternalGetquality);
		metaData.scriptClass->AddInternalCall("Internal_Setquality", (void*)&ScriptBloomSettings::InternalSetquality);
		metaData.scriptClass->AddInternalCall("Internal_Getthreshold", (void*)&ScriptBloomSettings::InternalGetthreshold);
		metaData.scriptClass->AddInternalCall("Internal_Setthreshold", (void*)&ScriptBloomSettings::InternalSetthreshold);
		metaData.scriptClass->AddInternalCall("Internal_Getintensity", (void*)&ScriptBloomSettings::InternalGetintensity);
		metaData.scriptClass->AddInternalCall("Internal_Setintensity", (void*)&ScriptBloomSettings::InternalSetintensity);
		metaData.scriptClass->AddInternalCall("Internal_Gettint", (void*)&ScriptBloomSettings::InternalGettint);
		metaData.scriptClass->AddInternalCall("Internal_Settint", (void*)&ScriptBloomSettings::InternalSettint);
		metaData.scriptClass->AddInternalCall("Internal_GetfilterSize", (void*)&ScriptBloomSettings::InternalGetfilterSize);
		metaData.scriptClass->AddInternalCall("Internal_SetfilterSize", (void*)&ScriptBloomSettings::InternalSetfilterSize);

	}

	MonoObject* ScriptBloomSettings::Create(const SPtr<BloomSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptBloomSettings>()) ScriptBloomSettings(managedInstance, value);
		return managedInstance;
	}
	void ScriptBloomSettings::InternalBloomSettings(MonoObject* managedInstance)
	{
		SPtr<BloomSettings> instance = bs_shared_ptr_new<BloomSettings>();
		new (bs_alloc<ScriptBloomSettings>())ScriptBloomSettings(managedInstance, instance);
	}

	bool ScriptBloomSettings::InternalGetenabled(ScriptBloomSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->enabled;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptBloomSettings::InternalSetenabled(ScriptBloomSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->enabled = value;
	}

	uint32_t ScriptBloomSettings::InternalGetquality(ScriptBloomSettings* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->quality;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptBloomSettings::InternalSetquality(ScriptBloomSettings* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->quality = value;
	}

	float ScriptBloomSettings::InternalGetthreshold(ScriptBloomSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->threshold;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptBloomSettings::InternalSetthreshold(ScriptBloomSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->threshold = value;
	}

	float ScriptBloomSettings::InternalGetintensity(ScriptBloomSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->intensity;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptBloomSettings::InternalSetintensity(ScriptBloomSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->intensity = value;
	}

	void ScriptBloomSettings::InternalGettint(ScriptBloomSettings* thisPtr, Color* __output)
	{
		Color tmp__output;
		tmp__output = thisPtr->GetInternal()->tint;

		*__output = tmp__output;


	}

	void ScriptBloomSettings::InternalSettint(ScriptBloomSettings* thisPtr, Color* value)
	{
		thisPtr->GetInternal()->tint = *value;
	}

	float ScriptBloomSettings::InternalGetfilterSize(ScriptBloomSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->filterSize;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptBloomSettings::InternalSetfilterSize(ScriptBloomSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->filterSize = value;
	}
}
