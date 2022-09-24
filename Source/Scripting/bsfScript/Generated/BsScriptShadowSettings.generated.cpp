//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptShadowSettings.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptShadowSettings::ScriptShadowSettings(MonoObject* managedInstance, const SPtr<ShadowSettings>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptShadowSettings::InitRuntimeData()
	{
		metaData.ScriptClass->AddInternalCall("Internal_ShadowSettings", (void*)&ScriptShadowSettings::InternalShadowSettings);
		metaData.ScriptClass->AddInternalCall("Internal_GetdirectionalShadowDistance", (void*)&ScriptShadowSettings::InternalGetdirectionalShadowDistance);
		metaData.ScriptClass->AddInternalCall("Internal_SetdirectionalShadowDistance", (void*)&ScriptShadowSettings::InternalSetdirectionalShadowDistance);
		metaData.ScriptClass->AddInternalCall("Internal_GetnumCascades", (void*)&ScriptShadowSettings::InternalGetnumCascades);
		metaData.ScriptClass->AddInternalCall("Internal_SetnumCascades", (void*)&ScriptShadowSettings::InternalSetnumCascades);
		metaData.ScriptClass->AddInternalCall("Internal_GetcascadeDistributionExponent", (void*)&ScriptShadowSettings::InternalGetcascadeDistributionExponent);
		metaData.ScriptClass->AddInternalCall("Internal_SetcascadeDistributionExponent", (void*)&ScriptShadowSettings::InternalSetcascadeDistributionExponent);
		metaData.ScriptClass->AddInternalCall("Internal_GetshadowFilteringQuality", (void*)&ScriptShadowSettings::InternalGetshadowFilteringQuality);
		metaData.ScriptClass->AddInternalCall("Internal_SetshadowFilteringQuality", (void*)&ScriptShadowSettings::InternalSetshadowFilteringQuality);

	}

	MonoObject* ScriptShadowSettings::Create(const SPtr<ShadowSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.ScriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptShadowSettings>()) ScriptShadowSettings(managedInstance, value);
		return managedInstance;
	}
	void ScriptShadowSettings::InternalShadowSettings(MonoObject* managedInstance)
	{
		SPtr<ShadowSettings> instance = bs_shared_ptr_new<ShadowSettings>();
		new (bs_alloc<ScriptShadowSettings>())ScriptShadowSettings(managedInstance, instance);
	}

	float ScriptShadowSettings::InternalGetdirectionalShadowDistance(ScriptShadowSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->DirectionalShadowDistance;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptShadowSettings::InternalSetdirectionalShadowDistance(ScriptShadowSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->DirectionalShadowDistance = value;
	}

	uint32_t ScriptShadowSettings::InternalGetnumCascades(ScriptShadowSettings* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->NumCascades;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptShadowSettings::InternalSetnumCascades(ScriptShadowSettings* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->NumCascades = value;
	}

	float ScriptShadowSettings::InternalGetcascadeDistributionExponent(ScriptShadowSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->CascadeDistributionExponent;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptShadowSettings::InternalSetcascadeDistributionExponent(ScriptShadowSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->CascadeDistributionExponent = value;
	}

	uint32_t ScriptShadowSettings::InternalGetshadowFilteringQuality(ScriptShadowSettings* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->ShadowFilteringQuality;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptShadowSettings::InternalSetshadowFilteringQuality(ScriptShadowSettings* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->ShadowFilteringQuality = value;
	}
}
