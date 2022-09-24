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
		metaData.ScriptClass->AddInternalCall("Internal_ScreenSpaceLensFlareSettings", (void*)&ScriptScreenSpaceLensFlareSettings::InternalScreenSpaceLensFlareSettings);
		metaData.ScriptClass->AddInternalCall("Internal_Getenabled", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetenabled);
		metaData.ScriptClass->AddInternalCall("Internal_Setenabled", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetenabled);
		metaData.ScriptClass->AddInternalCall("Internal_GetdownsampleCount", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetdownsampleCount);
		metaData.ScriptClass->AddInternalCall("Internal_SetdownsampleCount", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetdownsampleCount);
		metaData.ScriptClass->AddInternalCall("Internal_Getthreshold", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetthreshold);
		metaData.ScriptClass->AddInternalCall("Internal_Setthreshold", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetthreshold);
		metaData.ScriptClass->AddInternalCall("Internal_GetghostCount", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetghostCount);
		metaData.ScriptClass->AddInternalCall("Internal_SetghostCount", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetghostCount);
		metaData.ScriptClass->AddInternalCall("Internal_GetghostSpacing", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetghostSpacing);
		metaData.ScriptClass->AddInternalCall("Internal_SetghostSpacing", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetghostSpacing);
		metaData.ScriptClass->AddInternalCall("Internal_Getbrightness", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetbrightness);
		metaData.ScriptClass->AddInternalCall("Internal_Setbrightness", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetbrightness);
		metaData.ScriptClass->AddInternalCall("Internal_GetfilterSize", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetfilterSize);
		metaData.ScriptClass->AddInternalCall("Internal_SetfilterSize", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetfilterSize);
		metaData.ScriptClass->AddInternalCall("Internal_Gethalo", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGethalo);
		metaData.ScriptClass->AddInternalCall("Internal_Sethalo", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSethalo);
		metaData.ScriptClass->AddInternalCall("Internal_GethaloRadius", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGethaloRadius);
		metaData.ScriptClass->AddInternalCall("Internal_SethaloRadius", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSethaloRadius);
		metaData.ScriptClass->AddInternalCall("Internal_GethaloThickness", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGethaloThickness);
		metaData.ScriptClass->AddInternalCall("Internal_SethaloThickness", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSethaloThickness);
		metaData.ScriptClass->AddInternalCall("Internal_GethaloThreshold", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGethaloThreshold);
		metaData.ScriptClass->AddInternalCall("Internal_SethaloThreshold", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSethaloThreshold);
		metaData.ScriptClass->AddInternalCall("Internal_GethaloAspectRatio", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGethaloAspectRatio);
		metaData.ScriptClass->AddInternalCall("Internal_SethaloAspectRatio", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSethaloAspectRatio);
		metaData.ScriptClass->AddInternalCall("Internal_GetchromaticAberration", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetchromaticAberration);
		metaData.ScriptClass->AddInternalCall("Internal_SetchromaticAberration", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetchromaticAberration);
		metaData.ScriptClass->AddInternalCall("Internal_GetchromaticAberrationOffset", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetchromaticAberrationOffset);
		metaData.ScriptClass->AddInternalCall("Internal_SetchromaticAberrationOffset", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetchromaticAberrationOffset);
		metaData.ScriptClass->AddInternalCall("Internal_GetbicubicUpsampling", (void*)&ScriptScreenSpaceLensFlareSettings::InternalGetbicubicUpsampling);
		metaData.ScriptClass->AddInternalCall("Internal_SetbicubicUpsampling", (void*)&ScriptScreenSpaceLensFlareSettings::InternalSetbicubicUpsampling);

	}

	MonoObject* ScriptScreenSpaceLensFlareSettings::Create(const SPtr<ScreenSpaceLensFlareSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.ScriptClass->CreateInstance("bool", ctorParams);
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
		tmp__output = thisPtr->GetInternal()->Enabled;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSetenabled(ScriptScreenSpaceLensFlareSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->Enabled = value;
	}

	uint32_t ScriptScreenSpaceLensFlareSettings::InternalGetdownsampleCount(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->DownsampleCount;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSetdownsampleCount(ScriptScreenSpaceLensFlareSettings* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->DownsampleCount = value;
	}

	float ScriptScreenSpaceLensFlareSettings::InternalGetthreshold(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->Threshold;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSetthreshold(ScriptScreenSpaceLensFlareSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->Threshold = value;
	}

	uint32_t ScriptScreenSpaceLensFlareSettings::InternalGetghostCount(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->GhostCount;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSetghostCount(ScriptScreenSpaceLensFlareSettings* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->GhostCount = value;
	}

	float ScriptScreenSpaceLensFlareSettings::InternalGetghostSpacing(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->GhostSpacing;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSetghostSpacing(ScriptScreenSpaceLensFlareSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->GhostSpacing = value;
	}

	float ScriptScreenSpaceLensFlareSettings::InternalGetbrightness(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->Brightness;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSetbrightness(ScriptScreenSpaceLensFlareSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->Brightness = value;
	}

	float ScriptScreenSpaceLensFlareSettings::InternalGetfilterSize(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->FilterSize;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSetfilterSize(ScriptScreenSpaceLensFlareSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->FilterSize = value;
	}

	bool ScriptScreenSpaceLensFlareSettings::InternalGethalo(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->Halo;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSethalo(ScriptScreenSpaceLensFlareSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->Halo = value;
	}

	float ScriptScreenSpaceLensFlareSettings::InternalGethaloRadius(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->HaloRadius;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSethaloRadius(ScriptScreenSpaceLensFlareSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->HaloRadius = value;
	}

	float ScriptScreenSpaceLensFlareSettings::InternalGethaloThickness(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->HaloThickness;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSethaloThickness(ScriptScreenSpaceLensFlareSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->HaloThickness = value;
	}

	float ScriptScreenSpaceLensFlareSettings::InternalGethaloThreshold(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->HaloThreshold;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSethaloThreshold(ScriptScreenSpaceLensFlareSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->HaloThreshold = value;
	}

	float ScriptScreenSpaceLensFlareSettings::InternalGethaloAspectRatio(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->HaloAspectRatio;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSethaloAspectRatio(ScriptScreenSpaceLensFlareSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->HaloAspectRatio = value;
	}

	bool ScriptScreenSpaceLensFlareSettings::InternalGetchromaticAberration(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->ChromaticAberration;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSetchromaticAberration(ScriptScreenSpaceLensFlareSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->ChromaticAberration = value;
	}

	float ScriptScreenSpaceLensFlareSettings::InternalGetchromaticAberrationOffset(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->ChromaticAberrationOffset;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSetchromaticAberrationOffset(ScriptScreenSpaceLensFlareSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->ChromaticAberrationOffset = value;
	}

	bool ScriptScreenSpaceLensFlareSettings::InternalGetbicubicUpsampling(ScriptScreenSpaceLensFlareSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->BicubicUpsampling;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScreenSpaceLensFlareSettings::InternalSetbicubicUpsampling(ScriptScreenSpaceLensFlareSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->BicubicUpsampling = value;
	}
}
