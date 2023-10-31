//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "../../../Foundation/bsfCore/Text/BsFont.h"
#include "../../../Foundation/bsfCore/Text/BsFont.h"

namespace bs { struct FontBitmap; }
namespace bs { struct __CharacterInformationInterop; }
namespace bs
{
	class B3D_SCRIPT_INTEROP_EXPORT ScriptFontBitmap : public TScriptReflectable<ScriptFontBitmap, FontBitmap>
	{
	public:
		SCRIPT_OBJ(kEngineAssembly, kEngineNs, "FontBitmap")

		ScriptFontBitmap(MonoObject* managedInstance, const SPtr<FontBitmap>& value);

		static MonoObject* Create(const SPtr<FontBitmap>& value);

	private:
		static void InternalGetCharacterInformation(ScriptFontBitmap* thisPtr, uint32_t characterId, __CharacterInformationInterop* __output);
		static uint32_t InternalGetSize(ScriptFontBitmap* thisPtr);
		static void InternalSetSize(ScriptFontBitmap* thisPtr, uint32_t value);
		static float InternalGetBaselineOffset(ScriptFontBitmap* thisPtr);
		static void InternalSetBaselineOffset(ScriptFontBitmap* thisPtr, float value);
		static float InternalGetLineHeight(ScriptFontBitmap* thisPtr);
		static void InternalSetLineHeight(ScriptFontBitmap* thisPtr, float value);
		static void InternalGetMissingGlyph(ScriptFontBitmap* thisPtr, __CharacterInformationInterop* __output);
		static void InternalSetMissingGlyph(ScriptFontBitmap* thisPtr, __CharacterInformationInterop* value);
		static float InternalGetSpaceWidth(ScriptFontBitmap* thisPtr);
		static void InternalSetSpaceWidth(ScriptFontBitmap* thisPtr, float value);
		static MonoArray* InternalGetTexturePages(ScriptFontBitmap* thisPtr);
		static void InternalSetTexturePages(ScriptFontBitmap* thisPtr, MonoArray* value);
	};
}
