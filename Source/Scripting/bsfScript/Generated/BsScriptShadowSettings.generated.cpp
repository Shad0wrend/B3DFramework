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

	void ScriptShadowSettings::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_ShadowSettings", (void*)&ScriptShadowSettings::InternalShadowSettings);
		metaData.scriptClass->addInternalCall("Internal_getdirectionalShadowDistance", (void*)&ScriptShadowSettings::Internal_getdirectionalShadowDistance);
		metaData.scriptClass->addInternalCall("Internal_setdirectionalShadowDistance", (void*)&ScriptShadowSettings::Internal_setdirectionalShadowDistance);
		metaData.scriptClass->addInternalCall("Internal_getnumCascades", (void*)&ScriptShadowSettings::Internal_getnumCascades);
		metaData.scriptClass->addInternalCall("Internal_setnumCascades", (void*)&ScriptShadowSettings::Internal_setnumCascades);
		metaData.scriptClass->addInternalCall("Internal_getcascadeDistributionExponent", (void*)&ScriptShadowSettings::Internal_getcascadeDistributionExponent);
		metaData.scriptClass->addInternalCall("Internal_setcascadeDistributionExponent", (void*)&ScriptShadowSettings::Internal_setcascadeDistributionExponent);
		metaData.scriptClass->addInternalCall("Internal_getshadowFilteringQuality", (void*)&ScriptShadowSettings::Internal_getshadowFilteringQuality);
		metaData.scriptClass->addInternalCall("Internal_setshadowFilteringQuality", (void*)&ScriptShadowSettings::Internal_setshadowFilteringQuality);

	}

	MonoObject* ScriptShadowSettings::Create(const SPtr<ShadowSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
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
		tmp__output = thisPtr->GetInternal()->directionalShadowDistance;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptShadowSettings::InternalSetdirectionalShadowDistance(ScriptShadowSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->directionalShadowDistance = value;
	}

	uint32_t ScriptShadowSettings::InternalGetnumCascades(ScriptShadowSettings* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->numCascades;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptShadowSettings::InternalSetnumCascades(ScriptShadowSettings* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->numCascades = value;
	}

	float ScriptShadowSettings::InternalGetcascadeDistributionExponent(ScriptShadowSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->cascadeDistributionExponent;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptShadowSettings::InternalSetcascadeDistributionExponent(ScriptShadowSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->cascadeDistributionExponent = value;
	}

	uint32_t ScriptShadowSettings::InternalGetshadowFilteringQuality(ScriptShadowSettings* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->shadowFilteringQuality;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptShadowSettings::InternalSetshadowFilteringQuality(ScriptShadowSettings* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->shadowFilteringQuality = value;
	}
}
