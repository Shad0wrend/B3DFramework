//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "BsScriptGUIElementWrapper.h"
#include "GUI/BsGUILayout.h"

namespace bs
{
	/** @addtogroup ScriptInteropEngine
	 *  @{
	 */

	/** Interop class between C++ & CLR for all elements inheriting from GUILayout. */
	class B3D_SCRIPT_INTEROP_EXPORT ScriptGUILayoutWrapperBase : public ScriptGUIElementWrapper
	{
	public:
		using ScriptGUIElementWrapper::ScriptGUIElementWrapper;

		/** Returns the native object that is being wrapped. */
		GUILayout* GetNativeObject() const { return static_cast<GUILayout*>(mNativeObject); }
	};

	/** @} */
} // namespace bs
