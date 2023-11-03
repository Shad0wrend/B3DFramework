//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "BsScriptObject.h"
#include "../../../Foundation/bsfCore/Text/BsFont.h"

namespace bs
{
	struct __FontBitmapPageInterop
	{
		MonoObject* Texture;
		bool IsDynamic;
	};

	class B3D_SCRIPT_INTEROP_EXPORT ScriptFontBitmapPage : public ScriptObject<ScriptFontBitmapPage>
	{
	public:
		SCRIPT_OBJ(kEngineAssembly, kEngineNs, "FontBitmapPage")

		static MonoObject* Box(const __FontBitmapPageInterop& value);
		static __FontBitmapPageInterop Unbox(MonoObject* value);
		static FontBitmapPage FromInterop(const __FontBitmapPageInterop& value);
		static __FontBitmapPageInterop ToInterop(const FontBitmapPage& value);

	private:
		ScriptFontBitmapPage(MonoObject* managedInstance);

	};
}
