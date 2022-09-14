//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"
#include "Math/BsVector3.h"

namespace bs { struct ColorGradingSettings; }
namespace bs
{
	class BS_SCR_BE_EXPORT ScriptColorGradingSettings : public TScriptReflectable<ScriptColorGradingSettings, ColorGradingSettings>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "ColorGradingSettings")

		ScriptColorGradingSettings(MonoObject* managedInstance, const SPtr<ColorGradingSettings>& value);

		static MonoObject* Create(const SPtr<ColorGradingSettings>& value);

	private:
		static void InternalGetsaturation(ScriptColorGradingSettings* thisPtr, Vector3* __output);
		static void InternalSetsaturation(ScriptColorGradingSettings* thisPtr, Vector3* value);
		static void InternalGetcontrast(ScriptColorGradingSettings* thisPtr, Vector3* __output);
		static void InternalSetcontrast(ScriptColorGradingSettings* thisPtr, Vector3* value);
		static void InternalGetgain(ScriptColorGradingSettings* thisPtr, Vector3* __output);
		static void InternalSetgain(ScriptColorGradingSettings* thisPtr, Vector3* value);
		static void InternalGetoffset(ScriptColorGradingSettings* thisPtr, Vector3* __output);
		static void InternalSetoffset(ScriptColorGradingSettings* thisPtr, Vector3* value);
	};
}
