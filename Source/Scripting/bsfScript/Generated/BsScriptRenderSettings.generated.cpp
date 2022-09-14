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
#include "BsScriptBloomSettings.generated.h"
#include "BsScriptAmbientOcclusionSettings.generated.h"
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

	void ScriptRenderSettings::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_RenderSettings", (void*)&ScriptRenderSettings::InternalRenderSettings);
		metaData.scriptClass->AddInternalCall("Internal_getdepthOfField", (void*)&ScriptRenderSettings::InternalGetdepthOfField);
		metaData.scriptClass->AddInternalCall("Internal_setdepthOfField", (void*)&ScriptRenderSettings::InternalSetdepthOfField);
		metaData.scriptClass->AddInternalCall("Internal_getchromaticAberration", (void*)&ScriptRenderSettings::InternalGetchromaticAberration);
		metaData.scriptClass->AddInternalCall("Internal_setchromaticAberration", (void*)&ScriptRenderSettings::InternalSetchromaticAberration);
		metaData.scriptClass->AddInternalCall("Internal_getenableAutoExposure", (void*)&ScriptRenderSettings::InternalGetenableAutoExposure);
		metaData.scriptClass->AddInternalCall("Internal_setenableAutoExposure", (void*)&ScriptRenderSettings::InternalSetenableAutoExposure);
		metaData.scriptClass->AddInternalCall("Internal_getautoExposure", (void*)&ScriptRenderSettings::InternalGetautoExposure);
		metaData.scriptClass->AddInternalCall("Internal_setautoExposure", (void*)&ScriptRenderSettings::InternalSetautoExposure);
		metaData.scriptClass->AddInternalCall("Internal_getenableTonemapping", (void*)&ScriptRenderSettings::InternalGetenableTonemapping);
		metaData.scriptClass->AddInternalCall("Internal_setenableTonemapping", (void*)&ScriptRenderSettings::InternalSetenableTonemapping);
		metaData.scriptClass->AddInternalCall("Internal_gettonemapping", (void*)&ScriptRenderSettings::InternalGettonemapping);
		metaData.scriptClass->AddInternalCall("Internal_settonemapping", (void*)&ScriptRenderSettings::InternalSettonemapping);
		metaData.scriptClass->AddInternalCall("Internal_getwhiteBalance", (void*)&ScriptRenderSettings::InternalGetwhiteBalance);
		metaData.scriptClass->AddInternalCall("Internal_setwhiteBalance", (void*)&ScriptRenderSettings::InternalSetwhiteBalance);
		metaData.scriptClass->addInternalCall("Internal_getcolorGrading", (void*)&ScriptRenderSettings::InternalGetcolorGrading);
		metaData.scriptClass->addInternalCall("Internal_setcolorGrading", (void*)&ScriptRenderSettings::InternalSetcolorGrading);
		metaData.scriptClass->addInternalCall("Internal_getambientOcclusion", (void*)&ScriptRenderSettings::InternalGetambientOcclusion);
		metaData.scriptClass->addInternalCall("Internal_setambientOcclusion", (void*)&ScriptRenderSettings::InternalSetambientOcclusion);
		metaData.scriptClass->addInternalCall("Internal_getscreenSpaceReflections", (void*)&ScriptRenderSettings::InternalGetscreenSpaceReflections);
		metaData.scriptClass->addInternalCall("Internal_setscreenSpaceReflections", (void*)&ScriptRenderSettings::InternalSetscreenSpaceReflections);
		metaData.scriptClass->addInternalCall("Internal_getbloom", (void*)&ScriptRenderSettings::InternalGetbloom);
		metaData.scriptClass->addInternalCall("Internal_setbloom", (void*)&ScriptRenderSettings::InternalSetbloom);
		metaData.scriptClass->addInternalCall("Internal_getscreenSpaceLensFlare", (void*)&ScriptRenderSettings::InternalGetscreenSpaceLensFlare);
		metaData.scriptClass->addInternalCall("Internal_setscreenSpaceLensFlare", (void*)&ScriptRenderSettings::InternalSetscreenSpaceLensFlare);
		metaData.scriptClass->addInternalCall("Internal_getfilmGrain", (void*)&ScriptRenderSettings::InternalGetfilmGrain);
		metaData.scriptClass->addInternalCall("Internal_setfilmGrain", (void*)&ScriptRenderSettings::InternalSetfilmGrain);
		metaData.scriptClass->addInternalCall("Internal_getmotionBlur", (void*)&ScriptRenderSettings::InternalGetmotionBlur);
		metaData.scriptClass->addInternalCall("Internal_setmotionBlur", (void*)&ScriptRenderSettings::InternalSetmotionBlur);
		metaData.scriptClass->addInternalCall("Internal_gettemporalAA", (void*)&ScriptRenderSettings::InternalGettemporalAa);
		metaData.scriptClass->addInternalCall("Internal_settemporalAA", (void*)&ScriptRenderSettings::InternalSettemporalAa);
		metaData.scriptClass->addInternalCall("Internal_getenableFXAA", (void*)&ScriptRenderSettings::InternalGetenableFxaa);
		metaData.scriptClass->addInternalCall("Internal_setenableFXAA", (void*)&ScriptRenderSettings::InternalSetenableFxaa);
		metaData.scriptClass->addInternalCall("Internal_getexposureScale", (void*)&ScriptRenderSettings::InternalGetexposureScale);
		metaData.scriptClass->addInternalCall("Internal_setexposureScale", (void*)&ScriptRenderSettings::InternalSetexposureScale);
		metaData.scriptClass->addInternalCall("Internal_getgamma", (void*)&ScriptRenderSettings::InternalGetgamma);
		metaData.scriptClass->addInternalCall("Internal_setgamma", (void*)&ScriptRenderSettings::InternalSetgamma);
		metaData.scriptClass->addInternalCall("Internal_getenableHDR", (void*)&ScriptRenderSettings::InternalGetenableHdr);
		metaData.scriptClass->addInternalCall("Internal_setenableHDR", (void*)&ScriptRenderSettings::InternalSetenableHdr);
		metaData.scriptClass->addInternalCall("Internal_getenableLighting", (void*)&ScriptRenderSettings::InternalGetenableLighting);
		metaData.scriptClass->addInternalCall("Internal_setenableLighting", (void*)&ScriptRenderSettings::InternalSetenableLighting);
		metaData.scriptClass->addInternalCall("Internal_getenableShadows", (void*)&ScriptRenderSettings::InternalGetenableShadows);
		metaData.scriptClass->addInternalCall("Internal_setenableShadows", (void*)&ScriptRenderSettings::InternalSetenableShadows);
		metaData.scriptClass->addInternalCall("Internal_getenableVelocityBuffer", (void*)&ScriptRenderSettings::InternalGetenableVelocityBuffer);
		metaData.scriptClass->addInternalCall("Internal_setenableVelocityBuffer", (void*)&ScriptRenderSettings::InternalSetenableVelocityBuffer);
		metaData.scriptClass->addInternalCall("Internal_getshadowSettings", (void*)&ScriptRenderSettings::InternalGetshadowSettings);
		metaData.scriptClass->addInternalCall("Internal_setshadowSettings", (void*)&ScriptRenderSettings::InternalSetshadowSettings);
		metaData.scriptClass->addInternalCall("Internal_getenableIndirectLighting", (void*)&ScriptRenderSettings::InternalGetenableIndirectLighting);
		metaData.scriptClass->addInternalCall("Internal_setenableIndirectLighting", (void*)&ScriptRenderSettings::InternalSetenableIndirectLighting);
		metaData.scriptClass->addInternalCall("Internal_getoverlayOnly", (void*)&ScriptRenderSettings::InternalGetoverlayOnly);
		metaData.scriptClass->addInternalCall("Internal_setoverlayOnly", (void*)&ScriptRenderSettings::InternalSetoverlayOnly);
		metaData.scriptClass->addInternalCall("Internal_getenableSkybox", (void*)&ScriptRenderSettings::InternalGetenableSkybox);
		metaData.scriptClass->addInternalCall("Internal_setenableSkybox", (void*)&ScriptRenderSettings::InternalSetenableSkybox);
		metaData.scriptClass->addInternalCall("Internal_getcullDistance", (void*)&ScriptRenderSettings::InternalGetcullDistance);
		metaData.scriptClass->addInternalCall("Internal_setcullDistance", (void*)&ScriptRenderSettings::InternalSetcullDistance);

	}

	MonoObject* ScriptRenderSettings::Create(const SPtr<RenderSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
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
		*tmp__output = thisPtr->getInternal()->depthOfField;

		MonoObject* __output;
		__output = ScriptDepthOfFieldSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetdepthOfField(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<DepthOfFieldSettings> tmpvalue;
		ScriptDepthOfFieldSettings* scriptvalue;
		scriptvalue = ScriptDepthOfFieldSettings::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->getInternal();
		thisPtr->getInternal()->depthOfField = *tmpvalue;
	}

	MonoObject* ScriptRenderSettings::InternalGetchromaticAberration(ScriptRenderSettings* thisPtr)
	{
		SPtr<ChromaticAberrationSettings> tmp__output = bs_shared_ptr_new<ChromaticAberrationSettings>();
		*tmp__output = thisPtr->getInternal()->chromaticAberration;

		MonoObject* __output;
		__output = ScriptChromaticAberrationSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetchromaticAberration(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<ChromaticAberrationSettings> tmpvalue;
		ScriptChromaticAberrationSettings* scriptvalue;
		scriptvalue = ScriptChromaticAberrationSettings::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->getInternal();
		thisPtr->getInternal()->chromaticAberration = *tmpvalue;
	}

	bool ScriptRenderSettings::InternalGetenableAutoExposure(ScriptRenderSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->getInternal()->enableAutoExposure;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetenableAutoExposure(ScriptRenderSettings* thisPtr, bool value)
	{
		thisPtr->getInternal()->enableAutoExposure = value;
	}

	MonoObject* ScriptRenderSettings::InternalGetautoExposure(ScriptRenderSettings* thisPtr)
	{
		SPtr<AutoExposureSettings> tmp__output = bs_shared_ptr_new<AutoExposureSettings>();
		*tmp__output = thisPtr->getInternal()->autoExposure;

		MonoObject* __output;
		__output = ScriptAutoExposureSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetautoExposure(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<AutoExposureSettings> tmpvalue;
		ScriptAutoExposureSettings* scriptvalue;
		scriptvalue = ScriptAutoExposureSettings::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->getInternal();
		thisPtr->getInternal()->autoExposure = *tmpvalue;
	}

	bool ScriptRenderSettings::InternalGetenableTonemapping(ScriptRenderSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->getInternal()->enableTonemapping;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetenableTonemapping(ScriptRenderSettings* thisPtr, bool value)
	{
		thisPtr->getInternal()->enableTonemapping = value;
	}

	MonoObject* ScriptRenderSettings::InternalGettonemapping(ScriptRenderSettings* thisPtr)
	{
		SPtr<TonemappingSettings> tmp__output = bs_shared_ptr_new<TonemappingSettings>();
		*tmp__output = thisPtr->getInternal()->tonemapping;

		MonoObject* __output;
		__output = ScriptTonemappingSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSettonemapping(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<TonemappingSettings> tmpvalue;
		ScriptTonemappingSettings* scriptvalue;
		scriptvalue = ScriptTonemappingSettings::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->getInternal();
		thisPtr->getInternal()->tonemapping = *tmpvalue;
	}

	MonoObject* ScriptRenderSettings::InternalGetwhiteBalance(ScriptRenderSettings* thisPtr)
	{
		SPtr<WhiteBalanceSettings> tmp__output = bs_shared_ptr_new<WhiteBalanceSettings>();
		*tmp__output = thisPtr->getInternal()->whiteBalance;

		MonoObject* __output;
		__output = ScriptWhiteBalanceSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetwhiteBalance(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<WhiteBalanceSettings> tmpvalue;
		ScriptWhiteBalanceSettings* scriptvalue;
		scriptvalue = ScriptWhiteBalanceSettings::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->getInternal();
		thisPtr->getInternal()->whiteBalance = *tmpvalue;
	}

	MonoObject* ScriptRenderSettings::InternalGetcolorGrading(ScriptRenderSettings* thisPtr)
	{
		SPtr<ColorGradingSettings> tmp__output = bs_shared_ptr_new<ColorGradingSettings>();
		*tmp__output = thisPtr->getInternal()->colorGrading;

		MonoObject* __output;
		__output = ScriptColorGradingSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetcolorGrading(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<ColorGradingSettings> tmpvalue;
		ScriptColorGradingSettings* scriptvalue;
		scriptvalue = ScriptColorGradingSettings::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->getInternal();
		thisPtr->getInternal()->colorGrading = *tmpvalue;
	}

	MonoObject* ScriptRenderSettings::InternalGetambientOcclusion(ScriptRenderSettings* thisPtr)
	{
		SPtr<AmbientOcclusionSettings> tmp__output = bs_shared_ptr_new<AmbientOcclusionSettings>();
		*tmp__output = thisPtr->getInternal()->ambientOcclusion;

		MonoObject* __output;
		__output = ScriptAmbientOcclusionSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetambientOcclusion(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<AmbientOcclusionSettings> tmpvalue;
		ScriptAmbientOcclusionSettings* scriptvalue;
		scriptvalue = ScriptAmbientOcclusionSettings::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->getInternal();
		thisPtr->getInternal()->ambientOcclusion = *tmpvalue;
	}

	MonoObject* ScriptRenderSettings::InternalGetscreenSpaceReflections(ScriptRenderSettings* thisPtr)
	{
		SPtr<ScreenSpaceReflectionsSettings> tmp__output = bs_shared_ptr_new<ScreenSpaceReflectionsSettings>();
		*tmp__output = thisPtr->getInternal()->screenSpaceReflections;

		MonoObject* __output;
		__output = ScriptScreenSpaceReflectionsSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetscreenSpaceReflections(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<ScreenSpaceReflectionsSettings> tmpvalue;
		ScriptScreenSpaceReflectionsSettings* scriptvalue;
		scriptvalue = ScriptScreenSpaceReflectionsSettings::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->getInternal();
		thisPtr->getInternal()->screenSpaceReflections = *tmpvalue;
	}

	MonoObject* ScriptRenderSettings::InternalGetbloom(ScriptRenderSettings* thisPtr)
	{
		SPtr<BloomSettings> tmp__output = bs_shared_ptr_new<BloomSettings>();
		*tmp__output = thisPtr->getInternal()->bloom;

		MonoObject* __output;
		__output = ScriptBloomSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetbloom(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<BloomSettings> tmpvalue;
		ScriptBloomSettings* scriptvalue;
		scriptvalue = ScriptBloomSettings::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->getInternal();
		thisPtr->getInternal()->bloom = *tmpvalue;
	}

	MonoObject* ScriptRenderSettings::InternalGetscreenSpaceLensFlare(ScriptRenderSettings* thisPtr)
	{
		SPtr<ScreenSpaceLensFlareSettings> tmp__output = bs_shared_ptr_new<ScreenSpaceLensFlareSettings>();
		*tmp__output = thisPtr->getInternal()->screenSpaceLensFlare;

		MonoObject* __output;
		__output = ScriptScreenSpaceLensFlareSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetscreenSpaceLensFlare(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<ScreenSpaceLensFlareSettings> tmpvalue;
		ScriptScreenSpaceLensFlareSettings* scriptvalue;
		scriptvalue = ScriptScreenSpaceLensFlareSettings::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->getInternal();
		thisPtr->getInternal()->screenSpaceLensFlare = *tmpvalue;
	}

	MonoObject* ScriptRenderSettings::InternalGetfilmGrain(ScriptRenderSettings* thisPtr)
	{
		SPtr<FilmGrainSettings> tmp__output = bs_shared_ptr_new<FilmGrainSettings>();
		*tmp__output = thisPtr->getInternal()->filmGrain;

		MonoObject* __output;
		__output = ScriptFilmGrainSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetfilmGrain(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<FilmGrainSettings> tmpvalue;
		ScriptFilmGrainSettings* scriptvalue;
		scriptvalue = ScriptFilmGrainSettings::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->getInternal();
		thisPtr->getInternal()->filmGrain = *tmpvalue;
	}

	MonoObject* ScriptRenderSettings::InternalGetmotionBlur(ScriptRenderSettings* thisPtr)
	{
		SPtr<MotionBlurSettings> tmp__output = bs_shared_ptr_new<MotionBlurSettings>();
		*tmp__output = thisPtr->getInternal()->motionBlur;

		MonoObject* __output;
		__output = ScriptMotionBlurSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetmotionBlur(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<MotionBlurSettings> tmpvalue;
		ScriptMotionBlurSettings* scriptvalue;
		scriptvalue = ScriptMotionBlurSettings::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->getInternal();
		thisPtr->getInternal()->motionBlur = *tmpvalue;
	}

	MonoObject* ScriptRenderSettings::InternalGettemporalAa(ScriptRenderSettings* thisPtr)
	{
		SPtr<TemporalAASettings> tmp__output = bs_shared_ptr_new<TemporalAASettings>();
		*tmp__output = thisPtr->getInternal()->temporalAA;

		MonoObject* __output;
		__output = ScriptTemporalAASettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSettemporalAa(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<TemporalAASettings> tmpvalue;
		ScriptTemporalAASettings* scriptvalue;
		scriptvalue = ScriptTemporalAASettings::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->getInternal();
		thisPtr->getInternal()->temporalAA = *tmpvalue;
	}

	bool ScriptRenderSettings::InternalGetenableFxaa(ScriptRenderSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->getInternal()->enableFXAA;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetenableFxaa(ScriptRenderSettings* thisPtr, bool value)
	{
		thisPtr->getInternal()->enableFXAA = value;
	}

	float ScriptRenderSettings::InternalGetexposureScale(ScriptRenderSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getInternal()->exposureScale;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetexposureScale(ScriptRenderSettings* thisPtr, float value)
	{
		thisPtr->getInternal()->exposureScale = value;
	}

	float ScriptRenderSettings::InternalGetgamma(ScriptRenderSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getInternal()->gamma;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetgamma(ScriptRenderSettings* thisPtr, float value)
	{
		thisPtr->getInternal()->gamma = value;
	}

	bool ScriptRenderSettings::InternalGetenableHdr(ScriptRenderSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->getInternal()->enableHDR;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetenableHdr(ScriptRenderSettings* thisPtr, bool value)
	{
		thisPtr->getInternal()->enableHDR = value;
	}

	bool ScriptRenderSettings::InternalGetenableLighting(ScriptRenderSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->getInternal()->enableLighting;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetenableLighting(ScriptRenderSettings* thisPtr, bool value)
	{
		thisPtr->getInternal()->enableLighting = value;
	}

	bool ScriptRenderSettings::InternalGetenableShadows(ScriptRenderSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->getInternal()->enableShadows;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetenableShadows(ScriptRenderSettings* thisPtr, bool value)
	{
		thisPtr->getInternal()->enableShadows = value;
	}

	bool ScriptRenderSettings::InternalGetenableVelocityBuffer(ScriptRenderSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->getInternal()->enableVelocityBuffer;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetenableVelocityBuffer(ScriptRenderSettings* thisPtr, bool value)
	{
		thisPtr->getInternal()->enableVelocityBuffer = value;
	}

	MonoObject* ScriptRenderSettings::InternalGetshadowSettings(ScriptRenderSettings* thisPtr)
	{
		SPtr<ShadowSettings> tmp__output = bs_shared_ptr_new<ShadowSettings>();
		*tmp__output = thisPtr->getInternal()->shadowSettings;

		MonoObject* __output;
		__output = ScriptShadowSettings::Create(tmp__output);

		return __output;
	}

	void ScriptRenderSettings::InternalSetshadowSettings(ScriptRenderSettings* thisPtr, MonoObject* value)
	{
		SPtr<ShadowSettings> tmpvalue;
		ScriptShadowSettings* scriptvalue;
		scriptvalue = ScriptShadowSettings::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->getInternal();
		thisPtr->getInternal()->shadowSettings = *tmpvalue;
	}

	bool ScriptRenderSettings::InternalGetenableIndirectLighting(ScriptRenderSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->getInternal()->enableIndirectLighting;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetenableIndirectLighting(ScriptRenderSettings* thisPtr, bool value)
	{
		thisPtr->getInternal()->enableIndirectLighting = value;
	}

	bool ScriptRenderSettings::InternalGetoverlayOnly(ScriptRenderSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->getInternal()->overlayOnly;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetoverlayOnly(ScriptRenderSettings* thisPtr, bool value)
	{
		thisPtr->getInternal()->overlayOnly = value;
	}

	bool ScriptRenderSettings::InternalGetenableSkybox(ScriptRenderSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->getInternal()->enableSkybox;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetenableSkybox(ScriptRenderSettings* thisPtr, bool value)
	{
		thisPtr->getInternal()->enableSkybox = value;
	}

	float ScriptRenderSettings::InternalGetcullDistance(ScriptRenderSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getInternal()->cullDistance;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptRenderSettings::InternalSetcullDistance(ScriptRenderSettings* thisPtr, float value)
	{
		thisPtr->getInternal()->cullDistance = value;
	}
}
