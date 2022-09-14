//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "../../../Foundation/bsfCore/Text/BsFont.h"

namespace bs { struct FontBitmap; }
namespace bs { struct __CharDescInterop; }
namespace bs
{
	class BS_SCR_BE_EXPORT ScriptFontBitmap : public TScriptReflectable<ScriptFontBitmap, FontBitmap>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "FontBitmap")

		ScriptFontBitmap(MonoObject* managedInstance, const SPtr<FontBitmap>& value);

		static MonoObject* Create(const SPtr<FontBitmap>& value);

	private:
		static void InternalGetCharDesc(ScriptFontBitmap* thisPtr, uint32_t charId, __CharDescInterop* __output);
		static uint32_t InternalGetsize(ScriptFontBitmap* thisPtr);
		static void InternalSetsize(ScriptFontBitmap* thisPtr, uint32_t value);
		static int32_t InternalGetbaselineOffset(ScriptFontBitmap* thisPtr);
		static void InternalSetbaselineOffset(ScriptFontBitmap* thisPtr, int32_t value);
		static uint32_t InternalGetlineHeight(ScriptFontBitmap* thisPtr);
		static void InternalSetlineHeight(ScriptFontBitmap* thisPtr, uint32_t value);
		static void InternalGetmissingGlyph(ScriptFontBitmap* thisPtr, __CharDescInterop* __output);
		static void InternalSetmissingGlyph(ScriptFontBitmap* thisPtr, __CharDescInterop* value);
		static uint32_t InternalGetspaceWidth(ScriptFontBitmap* thisPtr);
		static void InternalSetspaceWidth(ScriptFontBitmap* thisPtr, uint32_t value);
		static MonoArray* InternalGettexturePages(ScriptFontBitmap* thisPtr);
		static void InternalSettexturePages(ScriptFontBitmap* thisPtr, MonoArray* value);
	};
}
