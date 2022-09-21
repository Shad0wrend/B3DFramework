//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptScreenSpaceLensFlareSettings.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptScreenSpaceLensFlareSettings::ScriptScreenSpaceLensFlareSettings(MonoObject* managedInstance, const SPtr<ScreenSpaceLensFlareSettings>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptScreenSpaceLensFlareSettings::InitRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_ScreenSpaceLensFlareSettings", (void*)&ScriptScreenSpaceLensFlareSettings::InternalScreenSpaceLensFlareSettings);
		metaData.scriptClass->AddInternalCall("Internal_Getenabled", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetenabled);
		metaData.scriptClass->AddInternalCall("Internal_Setenabled", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetenabled);
		metaData.scriptClass->AddInternalCall("Internal_GetdownsampleCount", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetdownsampleCount);
		metaData.scriptClass->AddInternalCall("Internal_SetdownsampleCount", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetdownsampleCount);
		metaData.scriptClass->AddInternalCall("Internal_Getthreshold", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetthreshold);
		metaData.scriptClass->AddInternalCall("Internal_Setthreshold", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetthreshold);
		metaData.scriptClass->AddInternalCall("Internal_GetghostCount", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetghostCount);
		metaData.scriptClass->AddInternalCall("Internal_SetghostCount", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetghostCount);
		metaData.scriptClass->AddInternalCall("Internal_GetghostSpacing", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetghostSpacing);
		metaData.scriptClass->AddInternalCall("Internal_SetghostSpacing", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetghostSpacing);
		metaData.scriptClass->AddInternalCall("Internal_Getbrightness", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetbrightness);
		metaData.scriptClass->AddInternalCall("Internal_Setbrightness", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetbrightness);
		metaData.scriptClass->AddInternalCall("Internal_GetfilterSize", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetfilterSize);
		metaData.scriptClass->AddInternalCall("Internal_SetfilterSize", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetfilterSize);
		metaData.scriptClass->AddInternalCall("Internal_Gethalo", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGethalo);
		metaData.scriptClass->AddInternalCall("Internal_Sethalo", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSethalo);
		metaData.scriptClass->AddInternalCall("Internal_GethaloRadius", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGethaloRadius);
		metaData.scriptClass->AddInternalCall("Internal_SethaloRadius", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSethaloRadius);
		metaData.scriptClass->AddInternalCall("Internal_GethaloThickness", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGethaloThickness);
		metaData.scriptClass->AddInternalCall("Internal_SethaloThickness", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSethaloThickness);
		metaData.scriptClass->AddInternalCall("Internal_GethaloThreshold", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGethaloThreshold);
		metaData.scriptClass->AddInternalCall("Internal_SethaloThreshold", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSethaloThreshold);
		metaData.scriptClass->AddInternalCall("Internal_GethaloAspectRatio", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGethaloAspectRatio);
		metaData.scriptClass->AddInternalCall("Internal_SethaloAspectRatio", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSethaloAspectRatio);
		metaData.scriptClass->AddInternalCall("Internal_GetchromaticAberration", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetchromaticAberration);
		metaData.scriptClass->AddInternalCall("Internal_SetchromaticAberration", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetchromaticAberration);
		metaData.scriptClass->AddInternalCall("Internal_GetchromaticAberrationOffset", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetchromaticAberrationOffset);
		metaData.scriptClass->AddInternalCall("Internal_SetchromaticAberrationOffset", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetchromaticAberrationOffset);
		metaData.scriptClass->AddInternalCall("Internal_GetbicubicUpsampling", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetbicubicUpsampling);
		metaData.scriptClass->AddInternalCall("Internal_SetbicubicUpsampling", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetbicubicUpsampling);

	}

	MonoObject* ScriptScreenSpaceLensFlareSettings::Create(const SPtr<ScreenSpaceLensFlareSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptScreenSpaceLensFlareSettings>()) ScriptScreenSpaceLensFlareSettings(managedInstance, value);
		return managedInstance;
	}
	void ScriptScreenSpaceLensFlareSettings::InternalScreenSpaceLensFlareSettings(MonoObject* managedInstance)
	{
		SPtr<ScreenSpaceLensFlareSettings> instance = bs_shared_ptr_new<ScreenSpaceLensFlareSettings>();
		new (bs_alloc<ScriptScreenSpaceLensFlareSettings>())ScriptScreenSpaceLensFlareSettings(managedInstance, instance);
	}

	bool ScriptScreenSpaceLensFlareSettings::InternalGetenabled(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->enabled;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSetenabled(ScriptScreenSpaceLensFlareSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->enabled = value;
	}

	uint32_t ScriptScreenSpaceLensFlareSettings::InternalGetdownsampleCount(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->downsampleCount;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSetdownsampleCount(ScriptScreenSpaceLensFlareSettings* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->downsampleCount = value;
	}

	float ScriptScreenSpaceLensFlareSettings::InternalGetthreshold(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->threshold;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSetthreshold(ScriptScreenSpaceLensFlareSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->threshold = value;
	}

	uint32_t ScriptScreenSpaceLensFlareSettings::InternalGetghostCount(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->ghostCount;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSetghostCount(ScriptScreenSpaceLensFlareSettings* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->ghostCount = value;
	}

	float ScriptScreenSpaceLensFlareSettings::InternalGetghostSpacing(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->ghostSpacing;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSetghostSpacing(ScriptScreenSpaceLensFlareSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->ghostSpacing = value;
	}

	float ScriptScreenSpaceLensFlareSettings::InternalGetbrightness(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->brightness;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSetbrightness(ScriptScreenSpaceLensFlareSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->brightness = value;
	}

	float ScriptScreenSpaceLensFlareSettings::InternalGetfilterSize(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->filterSize;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSetfilterSize(ScriptScreenSpaceLensFlareSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->filterSize = value;
	}

	bool ScriptScreenSpaceLensFlareSettings::InternalGethalo(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->halo;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSethalo(ScriptScreenSpaceLensFlareSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->halo = value;
	}

	float ScriptScreenSpaceLensFlareSettings::InternalGethaloRadius(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->haloRadius;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSethaloRadius(ScriptScreenSpaceLensFlareSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->haloRadius = value;
	}

	float ScriptScreenSpaceLensFlareSettings::InternalGethaloThickness(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->haloThickness;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSethaloThickness(ScriptScreenSpaceLensFlareSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->haloThickness = value;
	}

	float ScriptScreenSpaceLensFlareSettings::InternalGethaloThreshold(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->haloThreshold;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSethaloThreshold(ScriptScreenSpaceLensFlareSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->haloThreshold = value;
	}

	float ScriptScreenSpaceLensFlareSettings::InternalGethaloAspectRatio(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->haloAspectRatio;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSethaloAspectRatio(ScriptScreenSpaceLensFlareSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->haloAspectRatio = value;
	}

	bool ScriptScreenSpaceLensFlareSettings::InternalGetchromaticAberration(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->chromaticAberration;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSetchromaticAberration(ScriptScreenSpaceLensFlareSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->chromaticAberration = value;
	}

	float ScriptScreenSpaceLensFlareSettings::InternalGetchromaticAberrationOffset(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->chromaticAberrationOffset;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSetchromaticAberrationOffset(ScriptScreenSpaceLensFlareSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->chromaticAberrationOffset = value;
	}

	bool ScriptScreenSpaceLensFlareSettings::InternalGetbicubicUpsampling(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->bicubicUpsampling;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSetbicubicUpsampling(ScriptScreenSpaceLensFlareSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->bicubicUpsampling = value;
	}
}
