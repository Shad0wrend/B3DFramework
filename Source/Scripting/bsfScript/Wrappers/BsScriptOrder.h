//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "BsScriptObject.h"

namespace bs
{
	/**	Interop class between C++ & CLR for the Order attribute. */
	class B3D_SCRIPT_INTEROP_EXPORT ScriptOrder : public ScriptObject<ScriptOrder>
	{
	public:
		SCRIPT_OBJ(kEngineAssembly, kEngineNs, "Order")

		static MonoField* GetIndexField() { return indexField; }

	private:
		ScriptOrder(MonoObject* instance);

		static MonoField* indexField;
	};
} // namespace bs
