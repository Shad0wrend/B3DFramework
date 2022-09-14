//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"

namespace bs { struct ShadowSettings; }
namespace bs
{
	class BS_SCR_BE_EXPORT ScriptShadowSettings : public TScriptReflectable<ScriptShadowSettings, ShadowSettings>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "ShadowSettings")

		ScriptShadowSettings(MonoObject* managedInstance, const SPtr<ShadowSettings>& value);

		static MonoObject* Create(const SPtr<ShadowSettings>& value);

	private:
		static void InternalShadowSettings(MonoObject* managedInstance);
		static float InternalGetdirectionalShadowDistance(ScriptShadowSettings* thisPtr);
		static void InternalSetdirectionalShadowDistance(ScriptShadowSettings* thisPtr, float value);
		static uint32_t InternalGetnumCascades(ScriptShadowSettings* thisPtr);
		static void InternalSetnumCascades(ScriptShadowSettings* thisPtr, uint32_t value);
		static float InternalGetcascadeDistributionExponent(ScriptShadowSettings* thisPtr);
		static void InternalSetcascadeDistributionExponent(ScriptShadowSettings* thisPtr, float value);
		static uint32_t InternalGetshadowFilteringQuality(ScriptShadowSettings* thisPtr);
		static void InternalSetshadowFilteringQuality(ScriptShadowSettings* thisPtr, uint32_t value);
	};
}
