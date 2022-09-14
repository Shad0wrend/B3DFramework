//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"
#include "Image/BsColor.h"

namespace bs { struct BloomSettings; }
namespace bs
{
	class BS_SCR_BE_EXPORT ScriptBloomSettings : public TScriptReflectable<ScriptBloomSettings, BloomSettings>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "BloomSettings")

		ScriptBloomSettings(MonoObject* managedInstance, const SPtr<BloomSettings>& value);

		static MonoObject* Create(const SPtr<BloomSettings>& value);

	private:
		static void InternalBloomSettings(MonoObject* managedInstance);
		static bool InternalGetenabled(ScriptBloomSettings* thisPtr);
		static void InternalSetenabled(ScriptBloomSettings* thisPtr, bool value);
		static uint32_t InternalGetquality(ScriptBloomSettings* thisPtr);
		static void InternalSetquality(ScriptBloomSettings* thisPtr, uint32_t value);
		static float InternalGetthreshold(ScriptBloomSettings* thisPtr);
		static void InternalSetthreshold(ScriptBloomSettings* thisPtr, float value);
		static float InternalGetintensity(ScriptBloomSettings* thisPtr);
		static void InternalSetintensity(ScriptBloomSettings* thisPtr, float value);
		static void InternalGettint(ScriptBloomSettings* thisPtr, Color* __output);
		static void InternalSettint(ScriptBloomSettings* thisPtr, Color* value);
		static float InternalGetfilterSize(ScriptBloomSettings* thisPtr);
		static void InternalSetfilterSize(ScriptBloomSettings* thisPtr, float value);
	};
}
