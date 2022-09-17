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

	void ScriptScreenSpaceLensFlareSettings::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_ScreenSpaceLensFlareSettings", (void*)&ScriptScreenSpaceLensFlareSettings::InternalScreenSpaceLensFlareSettings);
		metaData.scriptClass->AddInternalCall("Internal_getenabled", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetenabled);
		metaData.scriptClass->AddInternalCall("Internal_setenabled", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetenabled);
		metaData.scriptClass->AddInternalCall("Internal_getdownsampleCount", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetdownsampleCount);
		metaData.scriptClass->AddInternalCall("Internal_setdownsampleCount", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetdownsampleCount);
		metaData.scriptClass->AddInternalCall("Internal_getthreshold", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetthreshold);
		metaData.scriptClass->AddInternalCall("Internal_setthreshold", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetthreshold);
		metaData.scriptClass->AddInternalCall("Internal_getghostCount", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetghostCount);
		metaData.scriptClass->AddInternalCall("Internal_setghostCount", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetghostCount);
		metaData.scriptClass->AddInternalCall("Internal_getghostSpacing", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetghostSpacing);
		metaData.scriptClass->AddInternalCall("Internal_setghostSpacing", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetghostSpacing);
		metaData.scriptClass->AddInternalCall("Internal_getbrightness", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetbrightness);
		metaData.scriptClass->AddInternalCall("Internal_setbrightness", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetbrightness);
		metaData.scriptClass->AddInternalCall("Internal_getfilterSize", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetfilterSize);
		metaData.scriptClass->AddInternalCall("Internal_setfilterSize", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetfilterSize);
		metaData.scriptClass->AddInternalCall("Internal_gethalo", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGethalo);
		metaData.scriptClass->AddInternalCall("Internal_sethalo", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSethalo);
		metaData.scriptClass->AddInternalCall("Internal_gethaloRadius", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGethaloRadius);
		metaData.scriptClass->AddInternalCall("Internal_sethaloRadius", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSethaloRadius);
		metaData.scriptClass->AddInternalCall("Internal_gethaloThickness", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGethaloThickness);
		metaData.scriptClass->AddInternalCall("Internal_sethaloThickness", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSethaloThickness);
		metaData.scriptClass->AddInternalCall("Internal_gethaloThreshold", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGethaloThreshold);
		metaData.scriptClass->AddInternalCall("Internal_sethaloThreshold", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSethaloThreshold);
		metaData.scriptClass->AddInternalCall("Internal_gethaloAspectRatio", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGethaloAspectRatio);
		metaData.scriptClass->AddInternalCall("Internal_sethaloAspectRatio", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSethaloAspectRatio);
		metaData.scriptClass->AddInternalCall("Internal_getchromaticAberration", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetchromaticAberration);
		metaData.scriptClass->AddInternalCall("Internal_setchromaticAberration", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetchromaticAberration);
		metaData.scriptClass->AddInternalCall("Internal_getchromaticAberrationOffset", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetchromaticAberrationOffset);
		metaData.scriptClass->AddInternalCall("Internal_setchromaticAberrationOffset", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetchromaticAberrationOffset);
		metaData.scriptClass->AddInternalCall("Internal_getbicubicUpsampling", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetbicubicUpsampling);
		metaData.scriptClass->AddInternalCall("Internal_setbicubicUpsampling", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetbicubicUpsampling);

	}

	MonoObject* ScriptScreenSpaceLensFlareSettings::Create(const SPtr<ScreenSpaceLensFlareSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
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
