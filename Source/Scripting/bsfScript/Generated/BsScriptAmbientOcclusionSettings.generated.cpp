//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptAmbientOcclusionSettings.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptAmbientOcclusionSettings::ScriptAmbientOcclusionSettings(MonoObject* managedInstance, const SPtr<AmbientOcclusionSettings>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptAmbientOcclusionSettings::InitRuntimeData()
	{
		metaData.ScriptClass->AddInternalCall("Internal_AmbientOcclusionSettings", (void*)&ScriptAmbientOcclusionSettings::InternalAmbientOcclusionSettings);
		metaData.ScriptClass->AddInternalCall("Internal_Getenabled", (void*)&ScriptAmbientOcclusionSettings::InternalGetenabled);
		metaData.ScriptClass->AddInternalCall("Internal_Setenabled", (void*)&ScriptAmbientOcclusionSettings::InternalSetenabled);
		metaData.ScriptClass->AddInternalCall("Internal_Getradius", (void*)&ScriptAmbientOcclusionSettings::InternalGetradius);
		metaData.ScriptClass->AddInternalCall("Internal_Setradius", (void*)&ScriptAmbientOcclusionSettings::InternalSetradius);
		metaData.ScriptClass->AddInternalCall("Internal_Getbias", (void*)&ScriptAmbientOcclusionSettings::InternalGetbias);
		metaData.ScriptClass->AddInternalCall("Internal_Setbias", (void*)&ScriptAmbientOcclusionSettings::InternalSetbias);
		metaData.ScriptClass->AddInternalCall("Internal_GetfadeDistance", (void*)&ScriptAmbientOcclusionSettings::InternalGetfadeDistance);
		metaData.ScriptClass->AddInternalCall("Internal_SetfadeDistance", (void*)&ScriptAmbientOcclusionSettings::InternalSetfadeDistance);
		metaData.ScriptClass->AddInternalCall("Internal_GetfadeRange", (void*)&ScriptAmbientOcclusionSettings::InternalGetfadeRange);
		metaData.ScriptClass->AddInternalCall("Internal_SetfadeRange", (void*)&ScriptAmbientOcclusionSettings::InternalSetfadeRange);
		metaData.ScriptClass->AddInternalCall("Internal_Getintensity", (void*)&ScriptAmbientOcclusionSettings::InternalGetintensity);
		metaData.ScriptClass->AddInternalCall("Internal_Setintensity", (void*)&ScriptAmbientOcclusionSettings::InternalSetintensity);
		metaData.ScriptClass->AddInternalCall("Internal_Getpower", (void*)&ScriptAmbientOcclusionSettings::InternalGetpower);
		metaData.ScriptClass->AddInternalCall("Internal_Setpower", (void*)&ScriptAmbientOcclusionSettings::InternalSetpower);
		metaData.ScriptClass->AddInternalCall("Internal_Getquality", (void*)&ScriptAmbientOcclusionSettings::InternalGetquality);
		metaData.ScriptClass->AddInternalCall("Internal_Setquality", (void*)&ScriptAmbientOcclusionSettings::InternalSetquality);

	}

	MonoObject* ScriptAmbientOcclusionSettings::Create(const SPtr<AmbientOcclusionSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.ScriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptAmbientOcclusionSettings>()) ScriptAmbientOcclusionSettings(managedInstance, value);
		return managedInstance;
	}
	void ScriptAmbientOcclusionSettings::InternalAmbientOcclusionSettings(MonoObject* managedInstance)
	{
		SPtr<AmbientOcclusionSettings> instance = bs_shared_ptr_new<AmbientOcclusionSettings>();
		new (bs_alloc<ScriptAmbientOcclusionSettings>())ScriptAmbientOcclusionSettings(managedInstance, instance);
	}

	bool ScriptAmbientOcclusionSettings::InternalGetenabled(ScriptAmbientOcclusionSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->Enabled;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAmbientOcclusionSettings::InternalSetenabled(ScriptAmbientOcclusionSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->Enabled = value;
	}

	float ScriptAmbientOcclusionSettings::InternalGetradius(ScriptAmbientOcclusionSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->Radius;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAmbientOcclusionSettings::InternalSetradius(ScriptAmbientOcclusionSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->Radius = value;
	}

	float ScriptAmbientOcclusionSettings::InternalGetbias(ScriptAmbientOcclusionSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->Bias;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAmbientOcclusionSettings::InternalSetbias(ScriptAmbientOcclusionSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->Bias = value;
	}

	float ScriptAmbientOcclusionSettings::InternalGetfadeDistance(ScriptAmbientOcclusionSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->FadeDistance;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAmbientOcclusionSettings::InternalSetfadeDistance(ScriptAmbientOcclusionSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->FadeDistance = value;
	}

	float ScriptAmbientOcclusionSettings::InternalGetfadeRange(ScriptAmbientOcclusionSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->FadeRange;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAmbientOcclusionSettings::InternalSetfadeRange(ScriptAmbientOcclusionSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->FadeRange = value;
	}

	float ScriptAmbientOcclusionSettings::InternalGetintensity(ScriptAmbientOcclusionSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->Intensity;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAmbientOcclusionSettings::InternalSetintensity(ScriptAmbientOcclusionSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->Intensity = value;
	}

	float ScriptAmbientOcclusionSettings::InternalGetpower(ScriptAmbientOcclusionSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->Power;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAmbientOcclusionSettings::InternalSetpower(ScriptAmbientOcclusionSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->Power = value;
	}

	uint32_t ScriptAmbientOcclusionSettings::InternalGetquality(ScriptAmbientOcclusionSettings* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->Quality;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAmbientOcclusionSettings::InternalSetquality(ScriptAmbientOcclusionSettings* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->Quality = value;
	}
}
