//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"

namespace bs { struct MotionBlurSettings; }
namespace bs
{
	class BS_SCR_BE_EXPORT ScriptMotionBlurSettings : public TScriptReflectable<ScriptMotionBlurSettings, MotionBlurSettings>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "MotionBlurSettings")

		ScriptMotionBlurSettings(MonoObject* managedInstance, const SPtr<MotionBlurSettings>& value);

		static MonoObject* Create(const SPtr<MotionBlurSettings>& value);

	private:
		static void InternalMotionBlurSettings(MonoObject* managedInstance);
		static bool InternalGetenabled(ScriptMotionBlurSettings* thisPtr);
		static void InternalSetenabled(ScriptMotionBlurSettings* thisPtr, bool value);
		static MotionBlurDomain InternalGetdomain(ScriptMotionBlurSettings* thisPtr);
		static void InternalSetdomain(ScriptMotionBlurSettings* thisPtr, MotionBlurDomain value);
		static MotionBlurFilter InternalGetfilter(ScriptMotionBlurSettings* thisPtr);
		static void InternalSetfilter(ScriptMotionBlurSettings* thisPtr, MotionBlurFilter value);
		static MotionBlurQuality InternalGetquality(ScriptMotionBlurSettings* thisPtr);
		static void InternalSetquality(ScriptMotionBlurSettings* thisPtr, MotionBlurQuality value);
		static float InternalGetmaximumRadius(ScriptMotionBlurSettings* thisPtr);
		static void InternalSetmaximumRadius(ScriptMotionBlurSettings* thisPtr, float value);
	};
}
