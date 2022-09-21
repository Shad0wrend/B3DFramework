//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"

namespace bs { struct TemporalAASettings; }
namespace bs
{
	class BS_SCR_BE_EXPORT ScriptTemporalAASettings : public TScriptReflectable<ScriptTemporalAASettings, TemporalAASettings>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "TemporalAASettings")

		ScriptTemporalAASettings(MonoObject* managedInstance, const SPtr<TemporalAASettings>& value);

		static MonoObject* Create(const SPtr<TemporalAASettings>& value);

	private:
		static void InternalTemporalAASettings(MonoObject* managedInstance);
		static bool InternalGetenabled(ScriptTemporalAASettings* thisPtr);
		static void InternalSetenabled(ScriptTemporalAASettings* thisPtr, bool value);
		static uint32_t InternalGetjitteredPositionCount(ScriptTemporalAASettings* thisPtr);
		static void InternalSetjitteredPositionCount(ScriptTemporalAASettings* thisPtr, uint32_t value);
		static float InternalGetsharpness(ScriptTemporalAASettings* thisPtr);
		static void InternalSetsharpness(ScriptTemporalAASettings* thisPtr, float value);
	};
}
