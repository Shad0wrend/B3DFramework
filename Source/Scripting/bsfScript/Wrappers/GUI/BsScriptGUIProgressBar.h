//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/GUI/BsScriptGUIElement.h"

namespace bs
{
	/** @addtogroup ScriptInteropEngine
	 *  @{
	 */

	/**	Interop class between C++ & CLR for GUIProgressBar.  */
	class B3D_SCRIPT_INTEROP_EXPORT ScriptGUIProgressBar : public TScriptGUIInteractable<ScriptGUIProgressBar>
	{
	public:
		SCRIPT_OBJ(kEngineAssembly, kEngineNs, "GUIProgressBar")

	private:
		ScriptGUIProgressBar(MonoObject* instance, GUIProgressBar* progressBar);

		/************************************************************************/
		/* 								CLR HOOKS						   		*/
		/************************************************************************/
		static void InternalCreateInstance(MonoObject* instance, MonoString* style, MonoArray* guiOptions);
		static void InternalSetPercent(ScriptGUIProgressBar* nativeInstance, float percent);
		static float InternalGetPercent(ScriptGUIProgressBar* nativeInstance);
	};

	/** @} */
} // namespace bs
