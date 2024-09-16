//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
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

	/**	Interop class between C++ & CLR for Vector2. */
	class B3D_SCRIPT_INTEROP_EXPORT ScriptVector2 : public ScriptObject<ScriptVector2>
	{
	public:
		SCRIPT_OBJ(kEngineAssembly, kEngineNs, "Vector2")

		/** Unboxes a boxed managed Vector2 struct and returns the native version of the structure. */
		static Vector2 Unbox(MonoObject* obj);

		/**	Boxes a native Vector2 struct and returns a managed object containing it. */
		static MonoObject* Box(const Vector2& value);

	private:
		ScriptVector2(MonoObject* instance);
	};

	/**	Interop class between C++ & CLR for Vector4. */
	class B3D_SCRIPT_INTEROP_EXPORT ScriptVector4 : public ScriptObject<ScriptVector4>
	{
	public:
		SCRIPT_OBJ(kEngineAssembly, kEngineNs, "Vector4")

		/**	Unboxes a boxed managed Vector4 struct and returns the native version of the structure. */
		static Vector4 Unbox(MonoObject* obj);

		/**	Boxes a native Vector4 struct and returns a managed object containing it. */
		static MonoObject* Box(const Vector4& value);

	private:
		ScriptVector4(MonoObject* instance);
	};

	/** @} */
} // namespace bs
