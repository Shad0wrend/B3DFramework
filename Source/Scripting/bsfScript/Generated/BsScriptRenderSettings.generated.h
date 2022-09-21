//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"

namespace bs { struct RenderSettings; }
namespace bs
{
	class BS_SCR_BE_EXPORT ScriptRenderSettings : public TScriptReflectable<ScriptRenderSettings, RenderSettings>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "RenderSettings")

		ScriptRenderSettings(MonoObject* managedInstance, const SPtr<RenderSettings>& value);

		static MonoObject* Create(const SPtr<RenderSettings>& value);

	private:
		static void InternalRenderSettings(MonoObject* managedInstance);
		static MonoObject* InternalGetdepthOfField(ScriptRenderSettings* thisPtr);
		static void InternalSetdepthOfField(ScriptRenderSettings* thisPtr, MonoObject* value);
		static MonoObject* InternalGetchromaticAberration(ScriptRenderSettings* thisPtr);
		static void InternalSetchromaticAberration(ScriptRenderSettings* thisPtr, MonoObject* value);
		static bool InternalGetenableAutoExposure(ScriptRenderSettings* thisPtr);
		static void InternalSetenableAutoExposure(ScriptRenderSettings* thisPtr, bool value);
		static MonoObject* InternalGetautoExposure(ScriptRenderSettings* thisPtr);
		static void InternalSetautoExposure(ScriptRenderSettings* thisPtr, MonoObject* value);
		static bool InternalGetenableTonemapping(ScriptRenderSettings* thisPtr);
		static void InternalSetenableTonemapping(ScriptRenderSettings* thisPtr, bool value);
		static MonoObject* InternalGettonemapping(ScriptRenderSettings* thisPtr);
		static void InternalSettonemapping(ScriptRenderSettings* thisPtr, MonoObject* value);
		static MonoObject* InternalGetwhiteBalance(ScriptRenderSettings* thisPtr);
		static void InternalSetwhiteBalance(ScriptRenderSettings* thisPtr, MonoObject* value);
		static MonoObject* InternalGetcolorGrading(ScriptRenderSettings* thisPtr);
		static void InternalSetcolorGrading(ScriptRenderSettings* thisPtr, MonoObject* value);
		static MonoObject* InternalGetambientOcclusion(ScriptRenderSettings* thisPtr);
		static void InternalSetambientOcclusion(ScriptRenderSettings* thisPtr, MonoObject* value);
		static MonoObject* InternalGetscreenSpaceReflections(ScriptRenderSettings* thisPtr);
		static void InternalSetscreenSpaceReflections(ScriptRenderSettings* thisPtr, MonoObject* value);
		static MonoObject* InternalGetbloom(ScriptRenderSettings* thisPtr);
		static void InternalSetbloom(ScriptRenderSettings* thisPtr, MonoObject* value);
		static MonoObject* InternalGetscreenSpaceLensFlare(ScriptRenderSettings* thisPtr);
		static void InternalSetscreenSpaceLensFlare(ScriptRenderSettings* thisPtr, MonoObject* value);
		static MonoObject* InternalGetfilmGrain(ScriptRenderSettings* thisPtr);
		static void InternalSetfilmGrain(ScriptRenderSettings* thisPtr, MonoObject* value);
		static MonoObject* InternalGetmotionBlur(ScriptRenderSettings* thisPtr);
		static void InternalSetmotionBlur(ScriptRenderSettings* thisPtr, MonoObject* value);
		static MonoObject* InternalGettemporalAA(ScriptRenderSettings* thisPtr);
		static void InternalSettemporalAA(ScriptRenderSettings* thisPtr, MonoObject* value);
		static bool InternalGetenableFXAA(ScriptRenderSettings* thisPtr);
		static void InternalSetenableFXAA(ScriptRenderSettings* thisPtr, bool value);
		static float InternalGetexposureScale(ScriptRenderSettings* thisPtr);
		static void InternalSetexposureScale(ScriptRenderSettings* thisPtr, float value);
		static float InternalGetgamma(ScriptRenderSettings* thisPtr);
		static void InternalSetgamma(ScriptRenderSettings* thisPtr, float value);
		static bool InternalGetenableHDR(ScriptRenderSettings* thisPtr);
		static void InternalSetenableHDR(ScriptRenderSettings* thisPtr, bool value);
		static bool InternalGetenableLighting(ScriptRenderSettings* thisPtr);
		static void InternalSetenableLighting(ScriptRenderSettings* thisPtr, bool value);
		static bool InternalGetenableShadows(ScriptRenderSettings* thisPtr);
		static void InternalSetenableShadows(ScriptRenderSettings* thisPtr, bool value);
		static bool InternalGetenableVelocityBuffer(ScriptRenderSettings* thisPtr);
		static void InternalSetenableVelocityBuffer(ScriptRenderSettings* thisPtr, bool value);
		static MonoObject* InternalGetshadowSettings(ScriptRenderSettings* thisPtr);
		static void InternalSetshadowSettings(ScriptRenderSettings* thisPtr, MonoObject* value);
		static bool InternalGetenableIndirectLighting(ScriptRenderSettings* thisPtr);
		static void InternalSetenableIndirectLighting(ScriptRenderSettings* thisPtr, bool value);
		static bool InternalGetoverlayOnly(ScriptRenderSettings* thisPtr);
		static void InternalSetoverlayOnly(ScriptRenderSettings* thisPtr, bool value);
		static bool InternalGetenableSkybox(ScriptRenderSettings* thisPtr);
		static void InternalSetenableSkybox(ScriptRenderSettings* thisPtr, bool value);
		static float InternalGetcullDistance(ScriptRenderSettings* thisPtr);
		static void InternalSetcullDistance(ScriptRenderSettings* thisPtr, float value);
	};
}
