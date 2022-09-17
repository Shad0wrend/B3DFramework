//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptScreenSpaceReflectionsSettings.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptScreenSpaceReflectionsSettings::ScriptScreenSpaceReflectionsSettings(MonoObject* managedInstance, const SPtr<ScreenSpaceReflectionsSettings>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptScreenSpaceReflectionsSettings::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_ScreenSpaceReflectionsSettings", (void*)&ScriptScreenSpaceReflectionsSettings::InternalScreenSpaceReflectionsSettings);
		metaData.scriptClass->AddInternalCall("Internal_getenabled", (void*)&ScriptScreenSpaceReflectionsSettings::InternalGetenabled);
		metaData.scriptClass->AddInternalCall("Internal_setenabled", (void*)&ScriptScreenSpaceReflectionsSettings::InternalSetenabled);
		metaData.scriptClass->AddInternalCall("Internal_getquality", (void*)&ScriptScreenSpaceReflectionsSettings::InternalGetquality);
		metaData.scriptClass->AddInternalCall("Internal_setquality", (void*)&ScriptScreenSpaceReflectionsSettings::InternalSetquality);
		metaData.scriptClass->AddInternalCall("Internal_getintensity", (void*)&ScriptScreenSpaceReflectionsSettings::InternalGetintensity);
		metaData.scriptClass->AddInternalCall("Internal_setintensity", (void*)&ScriptScreenSpaceReflectionsSettings::InternalSetintensity);
		metaData.scriptClass->AddInternalCall("Internal_getmaxRoughness", (void*)&ScriptScreenSpaceReflectionsSettings::InternalGetmaxRoughness);
		metaData.scriptClass->AddInternalCall("Internal_setmaxRoughness", (void*)&ScriptScreenSpaceReflectionsSettings::InternalSetmaxRoughness);

	}

	MonoObject* ScriptScreenSpaceReflectionsSettings::Create(const SPtr<ScreenSpaceReflectionsSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptScreenSpaceReflectionsSettings>()) ScriptScreenSpaceReflectionsSettings(managedInstance, value);
		return managedInstance;
	}
	void ScriptScreenSpaceReflectionsSettings::InternalScreenSpaceReflectionsSettings(MonoObject* managedInstance)
	{
		SPtr<ScreenSpaceReflectionsSettings> instance = bs_shared_ptr_new<ScreenSpaceReflectionsSettings>();
		new (bs_alloc<ScriptScreenSpaceReflectionsSettings>())ScriptScreenSpaceReflectionsSettings(managedInstance, instance);
	}

	bool ScriptScreenSpaceReflectionsSettings::InternalGetenabled(ScriptScreenSpaceReflectionsSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->enabled;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceReflectionsSettings::InternalSetenabled(ScriptScreenSpaceReflectionsSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->enabled = value;
	}

	uint32_t ScriptScreenSpaceReflectionsSettings::InternalGetquality(ScriptScreenSpaceReflectionsSettings* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->quality;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceReflectionsSettings::InternalSetquality(ScriptScreenSpaceReflectionsSettings* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->quality = value;
	}

	float ScriptScreenSpaceReflectionsSettings::InternalGetintensity(ScriptScreenSpaceReflectionsSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->intensity;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceReflectionsSettings::InternalSetintensity(ScriptScreenSpaceReflectionsSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->intensity = value;
	}

	float ScriptScreenSpaceReflectionsSettings::InternalGetmaxRoughness(ScriptScreenSpaceReflectionsSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->maxRoughness;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceReflectionsSettings::InternalSetmaxRoughness(ScriptScreenSpaceReflectionsSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->maxRoughness = value;
	}
}
