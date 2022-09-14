//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"

namespace bs { struct AmbientOcclusionSettings; }
namespace bs
{
	class BS_SCR_BE_EXPORT ScriptAmbientOcclusionSettings : public TScriptReflectable<ScriptAmbientOcclusionSettings, AmbientOcclusionSettings>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "AmbientOcclusionSettings")

		ScriptAmbientOcclusionSettings(MonoObject* managedInstance, const SPtr<AmbientOcclusionSettings>& value);

		static MonoObject* Create(const SPtr<AmbientOcclusionSettings>& value);

	private:
		static void InternalAmbientOcclusionSettings(MonoObject* managedInstance);
		static bool InternalGetenabled(ScriptAmbientOcclusionSettings* thisPtr);
		static void InternalSetenabled(ScriptAmbientOcclusionSettings* thisPtr, bool value);
		static float InternalGetradius(ScriptAmbientOcclusionSettings* thisPtr);
		static void InternalSetradius(ScriptAmbientOcclusionSettings* thisPtr, float value);
		static float InternalGetbias(ScriptAmbientOcclusionSettings* thisPtr);
		static void InternalSetbias(ScriptAmbientOcclusionSettings* thisPtr, float value);
		static float InternalGetfadeDistance(ScriptAmbientOcclusionSettings* thisPtr);
		static void InternalSetfadeDistance(ScriptAmbientOcclusionSettings* thisPtr, float value);
		static float InternalGetfadeRange(ScriptAmbientOcclusionSettings* thisPtr);
		static void InternalSetfadeRange(ScriptAmbientOcclusionSettings* thisPtr, float value);
		static float InternalGetintensity(ScriptAmbientOcclusionSettings* thisPtr);
		static void InternalSetintensity(ScriptAmbientOcclusionSettings* thisPtr, float value);
		static float InternalGetpower(ScriptAmbientOcclusionSettings* thisPtr);
		static void InternalSetpower(ScriptAmbientOcclusionSettings* thisPtr, float value);
		static uint32_t InternalGetquality(ScriptAmbientOcclusionSettings* thisPtr);
		static void InternalSetquality(ScriptAmbientOcclusionSettings* thisPtr, uint32_t value);
	};
}
