//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "BsScriptImportOptions.generated.h"
#include "../../../Foundation/bsfCore/Text/BsFontImportOptions.h"
#include "../../../Foundation/bsfCore/Text/BsFontImportOptions.h"
#include "../../../Foundation/bsfCore/Text/BsFontImportOptions.h"

namespace bs { class FontImportOptions; }
namespace bs
{
#if !BS_IS_BANSHEE3D
	class BS_SCR_BE_EXPORT ScriptFontImportOptions : public TScriptReflectable<ScriptFontImportOptions, FontImportOptions, ScriptImportOptionsBase>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "FontImportOptions")

		ScriptFontImportOptions(MonoObject* managedInstance, const SPtr<FontImportOptions>& value);

		static MonoObject* Create(const SPtr<FontImportOptions>& value);

	private:
		static MonoArray* InternalGetfontSizes(ScriptFontImportOptions* thisPtr);
		static void InternalSetfontSizes(ScriptFontImportOptions* thisPtr, MonoArray* value);
		static MonoArray* InternalGetcharIndexRanges(ScriptFontImportOptions* thisPtr);
		static void InternalSetcharIndexRanges(ScriptFontImportOptions* thisPtr, MonoArray* value);
		static uint32_t InternalGetdpi(ScriptFontImportOptions* thisPtr);
		static void InternalSetdpi(ScriptFontImportOptions* thisPtr, uint32_t value);
		static FontRenderMode InternalGetrenderMode(ScriptFontImportOptions* thisPtr);
		static void InternalSetrenderMode(ScriptFontImportOptions* thisPtr, FontRenderMode value);
		static bool InternalGetbold(ScriptFontImportOptions* thisPtr);
		static void InternalSetbold(ScriptFontImportOptions* thisPtr, bool value);
		static bool InternalGetitalic(ScriptFontImportOptions* thisPtr);
		static void InternalSetitalic(ScriptFontImportOptions* thisPtr, bool value);
		static void InternalCreate(MonoObject* managedInstance);
	};
#endif
}
