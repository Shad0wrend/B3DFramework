//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"

namespace bs { struct AutoExposureSettings; }
namespace bs
{
	class BS_SCR_BE_EXPORT ScriptAutoExposureSettings : public TScriptReflectable<ScriptAutoExposureSettings, AutoExposureSettings>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "AutoExposureSettings")

		ScriptAutoExposureSettings(MonoObject* managedInstance, const SPtr<AutoExposureSettings>& value);

		static MonoObject* Create(const SPtr<AutoExposureSettings>& value);

	private:
		static void InternalAutoExposureSettings(MonoObject* managedInstance);
		static float InternalGethistogramLog2Min(ScriptAutoExposureSettings* thisPtr);
		static void InternalSethistogramLog2Min(ScriptAutoExposureSettings* thisPtr, float value);
		static float InternalGethistogramLog2Max(ScriptAutoExposureSettings* thisPtr);
		static void InternalSethistogramLog2Max(ScriptAutoExposureSettings* thisPtr, float value);
		static float InternalGethistogramPctLow(ScriptAutoExposureSettings* thisPtr);
		static void InternalSethistogramPctLow(ScriptAutoExposureSettings* thisPtr, float value);
		static float InternalGethistogramPctHigh(ScriptAutoExposureSettings* thisPtr);
		static void InternalSethistogramPctHigh(ScriptAutoExposureSettings* thisPtr, float value);
		static float InternalGetminEyeAdaptation(ScriptAutoExposureSettings* thisPtr);
		static void InternalSetminEyeAdaptation(ScriptAutoExposureSettings* thisPtr, float value);
		static float InternalGetmaxEyeAdaptation(ScriptAutoExposureSettings* thisPtr);
		static void InternalSetmaxEyeAdaptation(ScriptAutoExposureSettings* thisPtr, float value);
		static float InternalGeteyeAdaptationSpeedUp(ScriptAutoExposureSettings* thisPtr);
		static void InternalSeteyeAdaptationSpeedUp(ScriptAutoExposureSettings* thisPtr, float value);
		static float InternalGeteyeAdaptationSpeedDown(ScriptAutoExposureSettings* thisPtr);
		static void InternalSeteyeAdaptationSpeedDown(ScriptAutoExposureSettings* thisPtr, float value);
	};
}
