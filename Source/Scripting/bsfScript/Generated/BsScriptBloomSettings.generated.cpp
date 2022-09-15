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

	void ScriptBloomSettings::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_BloomSettings", (void*)&ScriptBloomSettings::InternalBloomSettings);
		metaData.scriptClass->addInternalCall("Internal_getenabled", (void*)&ScriptBloomSettings::Internal_getenabled);
		metaData.scriptClass->addInternalCall("Internal_setenabled", (void*)&ScriptBloomSettings::Internal_setenabled);
		metaData.scriptClass->addInternalCall("Internal_getquality", (void*)&ScriptBloomSettings::Internal_getquality);
		metaData.scriptClass->addInternalCall("Internal_setquality", (void*)&ScriptBloomSettings::Internal_setquality);
		metaData.scriptClass->addInternalCall("Internal_getthreshold", (void*)&ScriptBloomSettings::Internal_getthreshold);
		metaData.scriptClass->addInternalCall("Internal_setthreshold", (void*)&ScriptBloomSettings::Internal_setthreshold);
		metaData.scriptClass->addInternalCall("Internal_getintensity", (void*)&ScriptBloomSettings::Internal_getintensity);
		metaData.scriptClass->addInternalCall("Internal_setintensity", (void*)&ScriptBloomSettings::Internal_setintensity);
		metaData.scriptClass->addInternalCall("Internal_gettint", (void*)&ScriptBloomSettings::Internal_gettint);
		metaData.scriptClass->addInternalCall("Internal_settint", (void*)&ScriptBloomSettings::Internal_settint);
		metaData.scriptClass->addInternalCall("Internal_getfilterSize", (void*)&ScriptBloomSettings::Internal_getfilterSize);
		metaData.scriptClass->addInternalCall("Internal_setfilterSize", (void*)&ScriptBloomSettings::Internal_setfilterSize);

	}

	MonoObject* ScriptBloomSettings::Create(const SPtr<BloomSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
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
