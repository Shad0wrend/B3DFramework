//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"

namespace bs { struct TonemappingSettings; }
namespace bs
{
	class BS_SCR_BE_EXPORT ScriptTonemappingSettings : public TScriptReflectable<ScriptTonemappingSettings, TonemappingSettings>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "TonemappingSettings")

		ScriptTonemappingSettings(MonoObject* managedInstance, const SPtr<TonemappingSettings>& value);

		static MonoObject* Create(const SPtr<TonemappingSettings>& value);

	private:
		static void InternalTonemappingSettings(MonoObject* managedInstance);
		static float InternalGetfilmicCurveShoulderStrength(ScriptTonemappingSettings* thisPtr);
		static void InternalSetfilmicCurveShoulderStrength(ScriptTonemappingSettings* thisPtr, float value);
		static float InternalGetfilmicCurveLinearStrength(ScriptTonemappingSettings* thisPtr);
		static void InternalSetfilmicCurveLinearStrength(ScriptTonemappingSettings* thisPtr, float value);
		static float InternalGetfilmicCurveLinearAngle(ScriptTonemappingSettings* thisPtr);
		static void InternalSetfilmicCurveLinearAngle(ScriptTonemappingSettings* thisPtr, float value);
		static float InternalGetfilmicCurveToeStrength(ScriptTonemappingSettings* thisPtr);
		static void InternalSetfilmicCurveToeStrength(ScriptTonemappingSettings* thisPtr, float value);
		static float InternalGetfilmicCurveToeNumerator(ScriptTonemappingSettings* thisPtr);
		static void InternalSetfilmicCurveToeNumerator(ScriptTonemappingSettings* thisPtr, float value);
		static float InternalGetfilmicCurveToeDenominator(ScriptTonemappingSettings* thisPtr);
		static void InternalSetfilmicCurveToeDenominator(ScriptTonemappingSettings* thisPtr, float value);
		static float InternalGetfilmicCurveLinearWhitePoint(ScriptTonemappingSettings* thisPtr);
		static void InternalSetfilmicCurveLinearWhitePoint(ScriptTonemappingSettings* thisPtr, float value);
	};
}
