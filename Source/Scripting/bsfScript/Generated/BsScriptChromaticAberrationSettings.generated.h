//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"

namespace bs { struct ChromaticAberrationSettings; }
namespace bs
{
	class BS_SCR_BE_EXPORT ScriptChromaticAberrationSettings : public TScriptReflectable<ScriptChromaticAberrationSettings, ChromaticAberrationSettings>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "ChromaticAberrationSettings")

		ScriptChromaticAberrationSettings(MonoObject* managedInstance, const SPtr<ChromaticAberrationSettings>& value);

		static MonoObject* Create(const SPtr<ChromaticAberrationSettings>& value);

	private:
		static void InternalChromaticAberrationSettings(MonoObject* managedInstance);
		static MonoObject* InternalGetfringeTexture(ScriptChromaticAberrationSettings* thisPtr);
		static void InternalSetfringeTexture(ScriptChromaticAberrationSettings* thisPtr, MonoObject* value);
		static bool InternalGetenabled(ScriptChromaticAberrationSettings* thisPtr);
		static void InternalSetenabled(ScriptChromaticAberrationSettings* thisPtr, bool value);
		static ChromaticAberrationType InternalGettype(ScriptChromaticAberrationSettings* thisPtr);
		static void InternalSettype(ScriptChromaticAberrationSettings* thisPtr, ChromaticAberrationType value);
		static float InternalGetshiftAmount(ScriptChromaticAberrationSettings* thisPtr);
		static void InternalSetshiftAmount(ScriptChromaticAberrationSettings* thisPtr, float value);
	};
}
