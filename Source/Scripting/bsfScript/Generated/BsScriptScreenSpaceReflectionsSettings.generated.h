//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"

namespace bs { struct ScreenSpaceReflectionsSettings; }
namespace bs
{
	class BS_SCR_BE_EXPORT ScriptScreenSpaceReflectionsSettings : public TScriptReflectable<ScriptScreenSpaceReflectionsSettings, ScreenSpaceReflectionsSettings>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "ScreenSpaceReflectionsSettings")

		ScriptScreenSpaceReflectionsSettings(MonoObject* managedInstance, const SPtr<ScreenSpaceReflectionsSettings>& value);

		static MonoObject* Create(const SPtr<ScreenSpaceReflectionsSettings>& value);

	private:
		static void InternalScreenSpaceReflectionsSettings(MonoObject* managedInstance);
		static bool InternalGetenabled(ScriptScreenSpaceReflectionsSettings* thisPtr);
		static void InternalSetenabled(ScriptScreenSpaceReflectionsSettings* thisPtr, bool value);
		static uint32_t InternalGetquality(ScriptScreenSpaceReflectionsSettings* thisPtr);
		static void InternalSetquality(ScriptScreenSpaceReflectionsSettings* thisPtr, uint32_t value);
		static float InternalGetintensity(ScriptScreenSpaceReflectionsSettings* thisPtr);
		static void InternalSetintensity(ScriptScreenSpaceReflectionsSettings* thisPtr, float value);
		static float InternalGetmaxRoughness(ScriptScreenSpaceReflectionsSettings* thisPtr);
		static void InternalSetmaxRoughness(ScriptScreenSpaceReflectionsSettings* thisPtr, float value);
	};
}
