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
		metaData.scriptClass->addInternalCall("Internal_getenabled", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_getenabled);
		metaData.scriptClass->addInternalCall("Internal_setenabled", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_setenabled);
		metaData.scriptClass->addInternalCall("Internal_getdownsampleCount", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_getdownsampleCount);
		metaData.scriptClass->addInternalCall("Internal_setdownsampleCount", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_setdownsampleCount);
		metaData.scriptClass->addInternalCall("Internal_getthreshold", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_getthreshold);
		metaData.scriptClass->addInternalCall("Internal_setthreshold", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_setthreshold);
		metaData.scriptClass->addInternalCall("Internal_getghostCount", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_getghostCount);
		metaData.scriptClass->addInternalCall("Internal_setghostCount", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_setghostCount);
		metaData.scriptClass->addInternalCall("Internal_getghostSpacing", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_getghostSpacing);
		metaData.scriptClass->addInternalCall("Internal_setghostSpacing", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_setghostSpacing);
		metaData.scriptClass->addInternalCall("Internal_getbrightness", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_getbrightness);
		metaData.scriptClass->addInternalCall("Internal_setbrightness", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_setbrightness);
		metaData.scriptClass->addInternalCall("Internal_getfilterSize", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_getfilterSize);
		metaData.scriptClass->addInternalCall("Internal_setfilterSize", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_setfilterSize);
		metaData.scriptClass->addInternalCall("Internal_gethalo", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_gethalo);
		metaData.scriptClass->addInternalCall("Internal_sethalo", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_sethalo);
		metaData.scriptClass->addInternalCall("Internal_gethaloRadius", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_gethaloRadius);
		metaData.scriptClass->addInternalCall("Internal_sethaloRadius", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_sethaloRadius);
		metaData.scriptClass->addInternalCall("Internal_gethaloThickness", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_gethaloThickness);
		metaData.scriptClass->addInternalCall("Internal_sethaloThickness", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_sethaloThickness);
		metaData.scriptClass->addInternalCall("Internal_gethaloThreshold", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_gethaloThreshold);
		metaData.scriptClass->addInternalCall("Internal_sethaloThreshold", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_sethaloThreshold);
		metaData.scriptClass->addInternalCall("Internal_gethaloAspectRatio", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_gethaloAspectRatio);
		metaData.scriptClass->addInternalCall("Internal_sethaloAspectRatio", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_sethaloAspectRatio);
		metaData.scriptClass->addInternalCall("Internal_getchromaticAberration", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_getchromaticAberration);
		metaData.scriptClass->addInternalCall("Internal_setchromaticAberration", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_setchromaticAberration);
		metaData.scriptClass->addInternalCall("Internal_getchromaticAberrationOffset", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_getchromaticAberrationOffset);
		metaData.scriptClass->addInternalCall("Internal_setchromaticAberrationOffset", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_setchromaticAberrationOffset);
		metaData.scriptClass->addInternalCall("Internal_getbicubicUpsampling", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_getbicubicUpsampling);
		metaData.scriptClass->addInternalCall("Internal_setbicubicUpsampling", (void*)&ScriptScreenSpaceLensFlareSettings::Internal_setbicubicUpsampling);

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
