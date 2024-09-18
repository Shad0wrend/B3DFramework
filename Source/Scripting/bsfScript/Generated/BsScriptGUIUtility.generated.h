//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "../../../Foundation/bsfEngine/GUI/BsGUIUtility.h"
#include "BsScriptObject.h"
#include "GUI/BsGUIElement.h"
#include "../../../Foundation/bsfUtility/Math/BsVector2I.h"

namespace bs
{
	class B3D_SCRIPT_INTEROP_EXPORT ScriptGUIUtility : public ScriptObject<ScriptGUIUtility>
	{
	public:
		SCRIPT_OBJ(kEngineAssembly, kEngineNs, "GUIUtility")

		ScriptGUIUtility(MonoObject* managedInstance, const SPtr<GUIUtility>& value);

		static void SetupScriptBindings();

		SPtr<GUIUtility> GetInternal() const { return mInternal; }
		static MonoObject* Create(const SPtr<GUIUtility>& value);

	private:
		SPtr<GUIUtility> mInternal;

		static void InternalCalculateOptimalSize(MonoObject* elem, TVector2I<int32_t>* __output);
		static void InternalCalculateTextBounds(MonoString* text, MonoObject* font, float fontSize, TVector2I<int32_t>* __output);
	};
}
