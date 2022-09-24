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

	void ScriptScreenSpaceReflectionsSettings::InitRuntimeData()
	{
		metaData.ScriptClass->AddInternalCall("Internal_ScreenSpaceReflectionsSettings", (void*)&ScriptScreenSpaceReflectionsSettings::InternalScreenSpaceReflectionsSettings);
		metaData.ScriptClass->AddInternalCall("Internal_Getenabled", (void*)&ScriptScreenSpaceReflectionsSettings::InternalGetenabled);
		metaData.ScriptClass->AddInternalCall("Internal_Setenabled", (void*)&ScriptScreenSpaceReflectionsSettings::InternalSetenabled);
		metaData.ScriptClass->AddInternalCall("Internal_Getquality", (void*)&ScriptScreenSpaceReflectionsSettings::InternalGetquality);
		metaData.ScriptClass->AddInternalCall("Internal_Setquality", (void*)&ScriptScreenSpaceReflectionsSettings::InternalSetquality);
		metaData.ScriptClass->AddInternalCall("Internal_Getintensity", (void*)&ScriptScreenSpaceReflectionsSettings::InternalGetintensity);
		metaData.ScriptClass->AddInternalCall("Internal_Setintensity", (void*)&ScriptScreenSpaceReflectionsSettings::InternalSetintensity);
		metaData.ScriptClass->AddInternalCall("Internal_GetmaxRoughness", (void*)&ScriptScreenSpaceReflectionsSettings::InternalGetmaxRoughness);
		metaData.ScriptClass->AddInternalCall("Internal_SetmaxRoughness", (void*)&ScriptScreenSpaceReflectionsSettings::InternalSetmaxRoughness);

	}

	MonoObject* ScriptScreenSpaceReflectionsSettings::Create(const SPtr<ScreenSpaceReflectionsSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.ScriptClass->CreateInstance("bool", ctorParams);
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
		tmp__output = thisPtr->GetInternal()->Enabled;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceReflectionsSettings::InternalSetenabled(ScriptScreenSpaceReflectionsSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->Enabled = value;
	}

	uint32_t ScriptScreenSpaceReflectionsSettings::InternalGetquality(ScriptScreenSpaceReflectionsSettings* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->Quality;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceReflectionsSettings::InternalSetquality(ScriptScreenSpaceReflectionsSettings* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->Quality = value;
	}

	float ScriptScreenSpaceReflectionsSettings::InternalGetintensity(ScriptScreenSpaceReflectionsSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->Intensity;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceReflectionsSettings::InternalSetintensity(ScriptScreenSpaceReflectionsSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->Intensity = value;
	}

	float ScriptScreenSpaceReflectionsSettings::InternalGetmaxRoughness(ScriptScreenSpaceReflectionsSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->MaxRoughness;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceReflectionsSettings::InternalSetmaxRoughness(ScriptScreenSpaceReflectionsSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->MaxRoughness = value;
	}
}
