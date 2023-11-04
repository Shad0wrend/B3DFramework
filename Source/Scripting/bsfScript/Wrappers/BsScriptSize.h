//*&&&******************************** bs::framework - Copyright 2023 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "BsScriptObject.h"
#include "Math/BsVector2.h"
#include "Math/BsVector3.h"
#include "Math/BsVector4.h"

namespace bs
{
	/** @addtogroup ScriptInteropEngine
	 *  @{
	 */

	/**	Interop class between C++ & CLR for Size2. */
	class B3D_SCRIPT_INTEROP_EXPORT ScriptSize2 : public ScriptObject<ScriptSize2>
	{
	public:
		SCRIPT_OBJ(kEngineAssembly, kEngineNs, "Size2")

		/** Unboxes a boxed managed Vector2 struct and returns the native version of the structure. */
		static Size2 Unbox(MonoObject* obj);

		/**	Boxes a native Vector2 struct and returns a managed object containing it. */
		static MonoObject* Box(const Size2& value);

	private:
		ScriptSize2(MonoObject* instance);
	};

	/**	Interop class between C++ & CLR for Size2UI. */
	class B3D_SCRIPT_INTEROP_EXPORT ScriptSize2UI : public ScriptObject<ScriptSize2UI>
	{
	public:
		SCRIPT_OBJ(kEngineAssembly, kEngineNs, "Size2UI")

		/** Unboxes a boxed managed Vector2 struct and returns the native version of the structure. */
		static Size2UI Unbox(MonoObject* obj);

		/**	Boxes a native Vector2 struct and returns a managed object containing it. */
		static MonoObject* Box(const Size2UI& value);

	private:
		ScriptSize2UI(MonoObject* instance);
	};

	/** @} */
} // namespace bs
