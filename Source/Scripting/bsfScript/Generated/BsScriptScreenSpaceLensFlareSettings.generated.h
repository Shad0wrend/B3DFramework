//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"

namespace bs { struct ScreenSpaceLensFlareSettings; }
namespace bs
{
	class BS_SCR_BE_EXPORT ScriptScreenSpaceLensFlareSettings : public TScriptReflectable<ScriptScreenSpaceLensFlareSettings, ScreenSpaceLensFlareSettings>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "ScreenSpaceLensFlareSettings")

		ScriptScreenSpaceLensFlareSettings(MonoObject* managedInstance, const SPtr<ScreenSpaceLensFlareSettings>& value);

		static MonoObject* Create(const SPtr<ScreenSpaceLensFlareSettings>& value);

	private:
		static void InternalScreenSpaceLensFlareSettings(MonoObject* managedInstance);
		static bool InternalGetenabled(ScriptScreenSpaceLensFlareSettings* thisPtr);
		static void InternalSetenabled(ScriptScreenSpaceLensFlareSettings* thisPtr, bool value);
		static uint32_t InternalGetdownsampleCount(ScriptScreenSpaceLensFlareSettings* thisPtr);
		static void InternalSetdownsampleCount(ScriptScreenSpaceLensFlareSettings* thisPtr, uint32_t value);
		static float InternalGetthreshold(ScriptScreenSpaceLensFlareSettings* thisPtr);
		static void InternalSetthreshold(ScriptScreenSpaceLensFlareSettings* thisPtr, float value);
		static uint32_t InternalGetghostCount(ScriptScreenSpaceLensFlareSettings* thisPtr);
		static void InternalSetghostCount(ScriptScreenSpaceLensFlareSettings* thisPtr, uint32_t value);
		static float InternalGetghostSpacing(ScriptScreenSpaceLensFlareSettings* thisPtr);
		static void InternalSetghostSpacing(ScriptScreenSpaceLensFlareSettings* thisPtr, float value);
		static float InternalGetbrightness(ScriptScreenSpaceLensFlareSettings* thisPtr);
		static void InternalSetbrightness(ScriptScreenSpaceLensFlareSettings* thisPtr, float value);
		static float InternalGetfilterSize(ScriptScreenSpaceLensFlareSettings* thisPtr);
		static void InternalSetfilterSize(ScriptScreenSpaceLensFlareSettings* thisPtr, float value);
		static bool InternalGethalo(ScriptScreenSpaceLensFlareSettings* thisPtr);
		static void InternalSethalo(ScriptScreenSpaceLensFlareSettings* thisPtr, bool value);
		static float InternalGethaloRadius(ScriptScreenSpaceLensFlareSettings* thisPtr);
		static void InternalSethaloRadius(ScriptScreenSpaceLensFlareSettings* thisPtr, float value);
		static float InternalGethaloThickness(ScriptScreenSpaceLensFlareSettings* thisPtr);
		static void InternalSethaloThickness(ScriptScreenSpaceLensFlareSettings* thisPtr, float value);
		static float InternalGethaloThreshold(ScriptScreenSpaceLensFlareSettings* thisPtr);
		static void InternalSethaloThreshold(ScriptScreenSpaceLensFlareSettings* thisPtr, float value);
		static float InternalGethaloAspectRatio(ScriptScreenSpaceLensFlareSettings* thisPtr);
		static void InternalSethaloAspectRatio(ScriptScreenSpaceLensFlareSettings* thisPtr, float value);
		static bool InternalGetchromaticAberration(ScriptScreenSpaceLensFlareSettings* thisPtr);
		static void InternalSetchromaticAberration(ScriptScreenSpaceLensFlareSettings* thisPtr, bool value);
		static float InternalGetchromaticAberrationOffset(ScriptScreenSpaceLensFlareSettings* thisPtr);
		static void InternalSetchromaticAberrationOffset(ScriptScreenSpaceLensFlareSettings* thisPtr, float value);
		static bool InternalGetbicubicUpsampling(ScriptScreenSpaceLensFlareSettings* thisPtr);
		static void InternalSetbicubicUpsampling(ScriptScreenSpaceLensFlareSettings* thisPtr, bool value);
	};
}
