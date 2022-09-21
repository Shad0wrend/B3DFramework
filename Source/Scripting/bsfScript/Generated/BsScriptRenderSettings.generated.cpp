//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptRenderSettings.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptDepthOfFieldSettings.generated.h"
#include "BsScriptWhiteBalanceSettings.generated.h"
#include "BsScriptChromaticAberrationSettings.generated.h"
#include "BsScriptAutoExposureSettings.generated.h"
#include "BsScriptTonemappingSettings.generated.h"
#include "BsScriptScreenSpaceReflectionsSettings.generated.h"
#include "BsScriptColorGradingSettings.generated.h"
#include "BsScriptAmbientOcclusionSettings.generated.h"
#include "BsScriptBloomSettings.generated.h"
#include "BsScriptScreenSpaceLensFlareSettings.generated.h"
#include "BsScriptFilmGrainSettings.generated.h"
#include "BsScriptMotionBlurSettings.generated.h"
#include "BsScriptTemporalAASettings.generated.h"
#include "BsScriptShadowSettings.generated.h"

namespace bs
{
	ScriptRenderSettings::ScriptRenderSettings(MonoObject* managedInstance, const SPtr<RenderSettings>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptRenderSettings::InitRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_RenderSettings", (void*)&ScriptRenderSettings::InternalRenderSettings);
		metaData.scriptClass->AddInternalCall("Internal_GetdepthOfField", (void*)&ScriptRenderSettings::InternalGetdepthOfField);
		metaData.scriptClass->AddInternalCall("Internal_SetdepthOfField", (void*)&ScriptRenderSettings::InternalSetdepthOfField);
		metaData.scriptClass->AddInternalCall("Internal_GetchromaticAberration", (void*)&ScriptRenderSettings::InternalGetchromaticAberration);
		metaData.scriptClass->AddInternalCall("Internal_SetchromaticAberration", (void*)&ScriptRenderSettings::InternalSetchromaticAberration);
		metaData.scriptClass->AddInternalCall("Internal_GetenableAutoExposure", (void*)&ScriptRenderSettings::InternalGetenableAutoExposure);
		metaData.scriptClass->AddInternalCall("Internal_SetenableAutoExposure", (void*)&ScriptRenderSettings::InternalSetenableAutoExposure);
		metaData.scriptClass->AddInternalCall("Internal_GetautoExposure", (void*)&ScriptRenderSettings::InternalGetautoExposure);
		metaData.scriptClass->AddInternalCall("Internal_SetautoExposure", (void*)&ScriptRenderSettings::InternalSetautoExposure);
		metaData.scriptClass->AddInternalCall("Internal_GetenableTonemapping", (void*)&ScriptRenderSettings::InternalGetenableTonemapping);
		metaData.scriptClass->AddInternalCall("Internal_SetenableTonemapping", (void*)&ScriptRenderSettings::InternalSetenableTonemapping);
		metaData.scriptClass->AddInternalCall("Internal_Gettonemapping", (void*)&ScriptRenderSettings::InternalGettonemapping);
		metaData.scriptClass->AddInternalCall("Internal_Settonemapping", (void*)&ScriptRenderSettings::InternalSettonemapping);
		metaData.scriptClass->AddInternalCall("Internal_GetwhiteBalance", (void*)&ScriptRenderSettings::InternalGetwhiteBalance);
		metaData.scriptClass->AddInternalCall("Internal_SetwhiteBalance", (void*)&ScriptRenderSettings::InternalSetwhiteBalance);
		metaData.scriptClass->AddInternalCall("Internal_GetcolorGrading", (void*)&ScriptRenderSettings::InternalGetcolorGrading);
		metaData.scriptClass->AddInternalCall("Internal_SetcolorGrading", (void*)&ScriptRenderSettings::InternalSetcolorGrading);
		metaData.scriptClass->AddInternalCall("Internal_GetambientOcclusion", (void*)&ScriptRenderSettings::InternalGetambientOcclusion);
		metaData.scriptClass->AddInternalCall("Internal_SetambientOcclusion", (void*)&ScriptRenderSettings::InternalSetambientOcclusion);
		metaData.scriptClass->AddInternalCall("Internal_GetscreenSpaceReflections", (void*)&ScriptRenderSettings::InternalGetscreenSpaceReflections);
		metaData.scriptClass->AddInternalCall("Internal_SetscreenSpaceReflections", (void*)&ScriptRenderSettings::InternalSetscreenSpaceReflections);
		metaData.scriptClass->AddInternalCall("Internal_Getbloom", (void*)&ScriptRenderSettings::InternalGetbloom);
		metaData.scriptClass->AddInternalCall("Internal_Setbloom", (void*)&ScriptRenderSettings::InternalSetbloom);
		metaData.scriptClass->AddInternalCall("Internal_GetscreenSpaceLensFlare", (void*)&ScriptRenderSettings::InternalGetscreenSpaceLensFlare);
		metaData.scriptClass->AddInternalCall("Internal_SetscreenSpaceLensFlare", (void*)&ScriptRenderSettings::InternalSetscreenSpaceLensFlare);
		metaData.scriptClass->AddInternalCall("Internal_GetfilmGrain", (void*)&ScriptRenderSettings::InternalGetfilmGrain);
		metaData.scriptClass->AddInternalCall("Internal_SetfilmGrain", (void*)&ScriptRenderSettings::InternalSetfilmGrain);
		metaData.scriptClass->AddInternalCall("Internal_GetmotionBlur", (void*)&ScriptRenderSettings::InternalGetmotionBlur);
		metaData.scriptClass->AddInternalCall("Internal_SetmotionBlur", (void*)&ScriptRenderSettings::InternalSetmotionBlur);
		metaData.scriptClass->AddInternalCall("Internal_GettemporalAA", (void*)&ScriptRenderSettings::InternalGettemporalAA);
		metaData.scriptClass->AddInternalCall("Internal_SettemporalAA", (void*)&ScriptRenderSettings::InternalSettemporalAA);
		metaData.scriptClass->AddInternalCall("Internal_GetenableFXAA", (void*)&ScriptRenderSettings::InternalGetenableFXAA);
		metaData.scriptClass->AddInternalCall("Internal_SetenableFXAA", (void*)&ScriptRenderSettings::InternalSetenableFXAA);
		metaData.scriptClass->AddInternalCall("Internal_GetexposureScale", (void*)&ScriptRenderSettings::InternalGetexposureScale);
		metaData.scriptClass->AddInternalCall("Internal_SetexposureScale", (void*)&ScriptRenderSettings::InternalSetexposureScale);
		metaData.scriptClass->AddInternalCall("Internal_Getgamma", (void*)&ScriptRenderSettings::InternalGetgamma);
		metaData.scriptClass->AddInternalCall("Internal_Setgamma", (void*)&ScriptRenderSettings::InternalSetgamma);
		metaData.scriptClass->AddInternalCall("Internal_GetenableHDR", (void*)&ScriptRenderSettings::InternalGetenableHDR);
		metaData.scriptClass->AddInternalCall("Internal_SetenableHDR", (void*)&ScriptRenderSettings::InternalSetenableHDR);
		metaData.scriptClass->AddInternalCall("Internal_GetenableLighting", (void*)&ScriptRenderSettings::InternalGetenableLighting);
		metaData.scriptClass->AddInternalCall("Internal_SetenableLighting", (void*)&ScriptRenderSettings::InternalSetenableLighting);
		metaData.scriptClass->AddInternalCall("Internal_GetenableShadows", (void*)&ScriptRenderSettings::InternalGetenableShadows);
		metaData.scriptClass->AddInternalCall("Internal_SetenableShadows", (void*)&ScriptRenderSettings::InternalSetenableShadows);
		metaData.scriptClass->AddInternalCall("Internal_GetenableVelocityBuffer", (void*)&ScriptRenderSettings::InternalGetenableVelocityBuffer);
		metaData.scriptClass->AddInternalCall("Internal_SetenableVelocityBuffer", (void*)&ScriptRenderSettings::InternalSetenableVelocityBuffer);
		metaData.scriptClass->AddInternalCall("Internal_GetshadowSettings", (void*)&ScriptRenderSettings::InternalGetshadowSettings);
		metaData.scriptClass->AddInternalCall("Internal_SetshadowSettings", (void*)&ScriptRenderSettings::InternalSetshadowSettings);
		metaData.scriptClass->AddInternalCall("Internal_GetenableIndirectLighting", (void*)&ScriptRenderSettings::InternalGetenableIndirectLighting);
		metaData.scriptClass->AddInternalCall("Internal_SetenableIndirectLighting", (void*)&ScriptRenderSettings::InternalSetenableIndirectLighting);
		metaData.scriptClass->AddInternalCall("Internal_GetoverlayOnly", (void*)&ScriptRenderSettings::InternalGetoverlayOnly);
		metaData.scriptClass->AddInternalCall("Internal_SetoverlayOnly", (void*)&ScriptRenderSettings::InternalSetoverlayOnly);
		metaData.scriptClass->AddInternalCall("Internal_GetenableSkybox", (void*)&ScriptRenderSettings::InternalGetenableSkybox);
		metaData.scriptClass->AddInternalCall("Internal_SetenableSkybox", (void*)&ScriptRenderSettings::InternalSetenableSkybox);
		metaData.scriptClass->AddInternalCall("Internal_GetcullDistance", (void*)&ScriptRenderSettings::InternalGetcullDistance);
		metaData.scriptClass->AddInternalCall("Internal_SetcullDistance", (void*)&ScriptRenderSettings::InternalSetcullDistance);

	}

	MonoObject* ScriptRenderSettings::Create(const SPtr<RenderSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptRenderSettings>()) ScriptRenderSettings(managedInstance, value);
		return managedInstance;
	}
	void ScriptRenderSettings::InternalRenderSettings(MonoObject* managedInstance)
	{
		SPtr<RenderSettings> instance = bs_shared_ptr_new<RenderSettings>();
		new (bs_alloc<ScriptRenderSettings>())ScriptRenderSettings(managedInstance, instance);
	}

	MonoObject* ScriptRenderSettings::InternalGetdepthOfField(ScriptRenderSettings* thisPtr)
	{
		SPtr<DepthOfFieldSettings> tmp__output = bs_shared_ptr_new<DepthOfFieldSettings>();
		*tmp__output = thisPtr->GetInternal()->depthOfField;

		MonoObject* __output;
		__output = ScriptDepthOfFieldSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetdepthOfField(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<DepthOfFieldSettings> tmpvalue;
		ScriptDepthOfFieldSettings* scriptvalue;
		scriptvalue = ScriptDepthOfFieldSettings::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->depthOfField = *tmpvalue;
	}

	MonoObject* ScriptRenderSettings::InternalGetchromaticAberration(ScriptRenderSettings* thisPtr)
	{
		SPtr<ChromaticAberrationSettings> tmp__output = bs_shared_ptr_new<ChromaticAberrationSettings>();
		*tmp__output = thisPtr->GetInternal()->chromaticAberration;

		MonoObject* __output;
		__output = ScriptChromaticAberrationSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetchromaticAberration(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<ChromaticAberrationSettings> tmpvalue;
		ScriptChromaticAberrationSettings* scriptvalue;
		scriptvalue = ScriptChromaticAberrationSettings::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->chromaticAberration = *tmpvalue;
	}

	bool ScriptRenderSettings::InternalGetenableAutoExposure(ScriptRenderSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->enableAutoExposure;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetenableAutoExposure(ScriptRenderSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->enableAutoExposure = value;
	}

	MonoObject* ScriptRenderSettings::InternalGetautoExposure(ScriptRenderSettings* thisPtr)
	{
		SPtr<AutoExposureSettings> tmp__output = bs_shared_ptr_new<AutoExposureSettings>();
		*tmp__output = thisPtr->GetInternal()->autoExposure;

		MonoObject* __output;
		__output = ScriptAutoExposureSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetautoExposure(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<AutoExposureSettings> tmpvalue;
		ScriptAutoExposureSettings* scriptvalue;
		scriptvalue = ScriptAutoExposureSettings::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->autoExposure = *tmpvalue;
	}

	bool ScriptRenderSettings::InternalGetenableTonemapping(ScriptRenderSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->enableTonemapping;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetenableTonemapping(ScriptRenderSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->enableTonemapping = value;
	}

	MonoObject* ScriptRenderSettings::InternalGettonemapping(ScriptRenderSettings* thisPtr)
	{
		SPtr<TonemappingSettings> tmp__output = bs_shared_ptr_new<TonemappingSettings>();
		*tmp__output = thisPtr->GetInternal()->tonemapping;

		MonoObject* __output;
		__output = ScriptTonemappingSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSettonemapping(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<TonemappingSettings> tmpvalue;
		ScriptTonemappingSettings* scriptvalue;
		scriptvalue = ScriptTonemappingSettings::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->tonemapping = *tmpvalue;
	}

	MonoObject* ScriptRenderSettings::InternalGetwhiteBalance(ScriptRenderSettings* thisPtr)
	{
		SPtr<WhiteBalanceSettings> tmp__output = bs_shared_ptr_new<WhiteBalanceSettings>();
		*tmp__output = thisPtr->GetInternal()->whiteBalance;

		MonoObject* __output;
		__output = ScriptWhiteBalanceSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetwhiteBalance(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<WhiteBalanceSettings> tmpvalue;
		ScriptWhiteBalanceSettings* scriptvalue;
		scriptvalue = ScriptWhiteBalanceSettings::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->whiteBalance = *tmpvalue;
	}

	MonoObject* ScriptRenderSettings::InternalGetcolorGrading(ScriptRenderSettings* thisPtr)
	{
		SPtr<ColorGradingSettings> tmp__output = bs_shared_ptr_new<ColorGradingSettings>();
		*tmp__output = thisPtr->GetInternal()->colorGrading;

		MonoObject* __output;
		__output = ScriptColorGradingSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetcolorGrading(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<ColorGradingSettings> tmpvalue;
		ScriptColorGradingSettings* scriptvalue;
		scriptvalue = ScriptColorGradingSettings::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->colorGrading = *tmpvalue;
	}

	MonoObject* ScriptRenderSettings::InternalGetambientOcclusion(ScriptRenderSettings* thisPtr)
	{
		SPtr<AmbientOcclusionSettings> tmp__output = bs_shared_ptr_new<AmbientOcclusionSettings>();
		*tmp__output = thisPtr->GetInternal()->ambientOcclusion;

		MonoObject* __output;
		__output = ScriptAmbientOcclusionSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetambientOcclusion(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<AmbientOcclusionSettings> tmpvalue;
		ScriptAmbientOcclusionSettings* scriptvalue;
		scriptvalue = ScriptAmbientOcclusionSettings::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->ambientOcclusion = *tmpvalue;
	}

	MonoObject* ScriptRenderSettings::InternalGetscreenSpaceReflections(ScriptRenderSettings* thisPtr)
	{
		SPtr<ScreenSpaceReflectionsSettings> tmp__output = bs_shared_ptr_new<ScreenSpaceReflectionsSettings>();
		*tmp__output = thisPtr->GetInternal()->screenSpaceReflections;

		MonoObject* __output;
		__output = ScriptScreenSpaceReflectionsSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetscreenSpaceReflections(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<ScreenSpaceReflectionsSettings> tmpvalue;
		ScriptScreenSpaceReflectionsSettings* scriptvalue;
		scriptvalue = ScriptScreenSpaceReflectionsSettings::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->screenSpaceReflections = *tmpvalue;
	}

	MonoObject* ScriptRenderSettings::InternalGetbloom(ScriptRenderSettings* thisPtr)
	{
		SPtr<BloomSettings> tmp__output = bs_shared_ptr_new<BloomSettings>();
		*tmp__output = thisPtr->GetInternal()->bloom;

		MonoObject* __output;
		__output = ScriptBloomSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetbloom(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<BloomSettings> tmpvalue;
		ScriptBloomSettings* scriptvalue;
		scriptvalue = ScriptBloomSettings::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->bloom = *tmpvalue;
	}

	MonoObject* ScriptRenderSettings::InternalGetscreenSpaceLensFlare(ScriptRenderSettings* thisPtr)
	{
		SPtr<ScreenSpaceLensFlareSettings> tmp__output = bs_shared_ptr_new<ScreenSpaceLensFlareSettings>();
		*tmp__output = thisPtr->GetInternal()->screenSpaceLensFlare;

		MonoObject* __output;
		__output = ScriptScreenSpaceLensFlareSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetscreenSpaceLensFlare(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<ScreenSpaceLensFlareSettings> tmpvalue;
		ScriptScreenSpaceLensFlareSettings* scriptvalue;
		scriptvalue = ScriptScreenSpaceLensFlareSettings::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->screenSpaceLensFlare = *tmpvalue;
	}

	MonoObject* ScriptRenderSettings::InternalGetfilmGrain(ScriptRenderSettings* thisPtr)
	{
		SPtr<FilmGrainSettings> tmp__output = bs_shared_ptr_new<FilmGrainSettings>();
		*tmp__output = thisPtr->GetInternal()->filmGrain;

		MonoObject* __output;
		__output = ScriptFilmGrainSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetfilmGrain(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<FilmGrainSettings> tmpvalue;
		ScriptFilmGrainSettings* scriptvalue;
		scriptvalue = ScriptFilmGrainSettings::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->filmGrain = *tmpvalue;
	}

	MonoObject* ScriptRenderSettings::InternalGetmotionBlur(ScriptRenderSettings* thisPtr)
	{
		SPtr<MotionBlurSettings> tmp__output = bs_shared_ptr_new<MotionBlurSettings>();
		*tmp__output = thisPtr->GetInternal()->motionBlur;

		MonoObject* __output;
		__output = ScriptMotionBlurSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetmotionBlur(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<MotionBlurSettings> tmpvalue;
		ScriptMotionBlurSettings* scriptvalue;
		scriptvalue = ScriptMotionBlurSettings::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->motionBlur = *tmpvalue;
	}

	MonoObject* ScriptRenderSettings::InternalGettemporalAA(ScriptRenderSettings* thisPtr)
	{
		SPtr<TemporalAASettings> tmp__output = bs_shared_ptr_new<TemporalAASettings>();
		*tmp__output = thisPtr->GetInternal()->temporalAA;

		MonoObject* __output;
		__output = ScriptTemporalAASettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSettemporalAA(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<TemporalAASettings> tmpvalue;
		ScriptTemporalAASettings* scriptvalue;
		scriptvalue = ScriptTemporalAASettings::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->temporalAA = *tmpvalue;
	}

	bool ScriptRenderSettings::InternalGetenableFXAA(ScriptRenderSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->enableFXAA;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetenableFXAA(ScriptRenderSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->enableFXAA = value;
	}

	float ScriptRenderSettings::InternalGetexposureScale(ScriptRenderSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->exposureScale;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetexposureScale(ScriptRenderSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->exposureScale = value;
	}

	float ScriptRenderSettings::InternalGetgamma(ScriptRenderSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->gamma;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetgamma(ScriptRenderSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->gamma = value;
	}

	bool ScriptRenderSettings::InternalGetenableHDR(ScriptRenderSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->enableHDR;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetenableHDR(ScriptRenderSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->enableHDR = value;
	}

	bool ScriptRenderSettings::InternalGetenableLighting(ScriptRenderSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->enableLighting;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetenableLighting(ScriptRenderSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->enableLighting = value;
	}

	bool ScriptRenderSettings::InternalGetenableShadows(ScriptRenderSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->enableShadows;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetenableShadows(ScriptRenderSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->enableShadows = value;
	}

	bool ScriptRenderSettings::InternalGetenableVelocityBuffer(ScriptRenderSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->enableVelocityBuffer;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetenableVelocityBuffer(ScriptRenderSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->enableVelocityBuffer = value;
	}

	MonoObject* ScriptRenderSettings::InternalGetshadowSettings(ScriptRenderSettings* thisPtr)
	{
		SPtr<ShadowSettings> tmp__output = bs_shared_ptr_new<ShadowSettings>();
		*tmp__output = thisPtr->GetInternal()->shadowSettings;

		MonoObject* __output;
		__output = ScriptShadowSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetshadowSettings(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<ShadowSettings> tmpvalue;
		ScriptShadowSettings* scriptvalue;
		scriptvalue = ScriptShadowSettings::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetInternal();
		thisPtr->GetInternal()->shadowSettings = *tmpvalue;
	}

	bool ScriptRenderSettings::InternalGetenableIndirectLighting(ScriptRenderSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->enableIndirectLighting;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetenableIndirectLighting(ScriptRenderSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->enableIndirectLighting = value;
	}

	bool ScriptRenderSettings::InternalGetoverlayOnly(ScriptRenderSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->overlayOnly;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetoverlayOnly(ScriptRenderSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->overlayOnly = value;
	}

	bool ScriptRenderSettings::InternalGetenableSkybox(ScriptRenderSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->enableSkybox;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetenableSkybox(ScriptRenderSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->enableSkybox = value;
	}

	float ScriptRenderSettings::InternalGetcullDistance(ScriptRenderSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->cullDistance;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetcullDistance(ScriptRenderSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->cullDistance = value;
	}
}
