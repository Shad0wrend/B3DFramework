//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"
#include "../../../Foundation/bsfCore/Renderer/BsRenderSettings.h"
#include "Math/BsVector2.h"

namespace bs { struct DepthOfFieldSettings; }
namespace bs
{
	class BS_SCR_BE_EXPORT ScriptDepthOfFieldSettings : public TScriptReflectable<ScriptDepthOfFieldSettings, DepthOfFieldSettings>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "DepthOfFieldSettings")

		ScriptDepthOfFieldSettings(MonoObject* managedInstance, const SPtr<DepthOfFieldSettings>& value);

		static MonoObject* Create(const SPtr<DepthOfFieldSettings>& value);

	private:
		static void InternalDepthOfFieldSettings(MonoObject* managedInstance);
		static MonoObject* InternalGetbokehShape(ScriptDepthOfFieldSettings* thisPtr);
		static void InternalSetbokehShape(ScriptDepthOfFieldSettings* thisPtr, MonoObject* value);
		static bool InternalGetenabled(ScriptDepthOfFieldSettings* thisPtr);
		static void InternalSetenabled(ScriptDepthOfFieldSettings* thisPtr, bool value);
		static DepthOfFieldType InternalGettype(ScriptDepthOfFieldSettings* thisPtr);
		static void InternalSettype(ScriptDepthOfFieldSettings* thisPtr, DepthOfFieldType value);
		static float InternalGetfocalDistance(ScriptDepthOfFieldSettings* thisPtr);
		static void InternalSetfocalDistance(ScriptDepthOfFieldSettings* thisPtr, float value);
		static float InternalGetfocalRange(ScriptDepthOfFieldSettings* thisPtr);
		static void InternalSetfocalRange(ScriptDepthOfFieldSettings* thisPtr, float value);
		static float InternalGetnearTransitionRange(ScriptDepthOfFieldSettings* thisPtr);
		static void InternalSetnearTransitionRange(ScriptDepthOfFieldSettings* thisPtr, float value);
		static float InternalGetfarTransitionRange(ScriptDepthOfFieldSettings* thisPtr);
		static void InternalSetfarTransitionRange(ScriptDepthOfFieldSettings* thisPtr, float value);
		static float InternalGetnearBlurAmount(ScriptDepthOfFieldSettings* thisPtr);
		static void InternalSetnearBlurAmount(ScriptDepthOfFieldSettings* thisPtr, float value);
		static float InternalGetfarBlurAmount(ScriptDepthOfFieldSettings* thisPtr);
		static void InternalSetfarBlurAmount(ScriptDepthOfFieldSettings* thisPtr, float value);
		static float InternalGetmaxBokehSize(ScriptDepthOfFieldSettings* thisPtr);
		static void InternalSetmaxBokehSize(ScriptDepthOfFieldSettings* thisPtr, float value);
		static float InternalGetadaptiveColorThreshold(ScriptDepthOfFieldSettings* thisPtr);
		static void InternalSetadaptiveColorThreshold(ScriptDepthOfFieldSettings* thisPtr, float value);
		static float InternalGetadaptiveRadiusThreshold(ScriptDepthOfFieldSettings* thisPtr);
		static void InternalSetadaptiveRadiusThreshold(ScriptDepthOfFieldSettings* thisPtr, float value);
		static float InternalGetapertureSize(ScriptDepthOfFieldSettings* thisPtr);
		static void InternalSetapertureSize(ScriptDepthOfFieldSettings* thisPtr, float value);
		static float InternalGetfocalLength(ScriptDepthOfFieldSettings* thisPtr);
		static void InternalSetfocalLength(ScriptDepthOfFieldSettings* thisPtr, float value);
		static void InternalGetsensorSize(ScriptDepthOfFieldSettings* thisPtr, Vector2* __output);
		static void InternalSetsensorSize(ScriptDepthOfFieldSettings* thisPtr, Vector2* value);
		static bool InternalGetbokehOcclusion(ScriptDepthOfFieldSettings* thisPtr);
		static void InternalSetbokehOcclusion(ScriptDepthOfFieldSettings* thisPtr, bool value);
		static float InternalGetocclusionDepthRange(ScriptDepthOfFieldSettings* thisPtr);
		static void InternalSetocclusionDepthRange(ScriptDepthOfFieldSettings* thisPtr, float value);
	};
}
