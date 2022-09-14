//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "../../../Foundation/bsfEngine/GUI/BsGUIElementStyle.h"
#include "../../../Foundation/bsfEngine/Utility/BsRectOffset.h"
#include "../../../Foundation/bsfEngine/2D/BsTextSprite.h"
#include "../../../Foundation/bsfEngine/GUI/BsGUIElementStyle.h"
#include "../../../Foundation/bsfEngine/2D/BsTextSprite.h"
#include "../../../Foundation/bsfEngine/GUI/BsGUIElementStyle.h"

namespace bs { struct GUIElementStyle; }
namespace bs { struct __GUIElementStateStyleInterop; }
namespace bs
{
	class BS_SCR_BE_EXPORT ScriptGUIElementStyle : public TScriptReflectable<ScriptGUIElementStyle, GUIElementStyle>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "GUIElementStyle")

		ScriptGUIElementStyle(MonoObject* managedInstance, const SPtr<GUIElementStyle>& value);

		static MonoObject* Create(const SPtr<GUIElementStyle>& value);

	private:
		static void InternalGuiElementStyle(MonoObject* managedInstance);
		static void InternalAddSubStyle(ScriptGUIElementStyle* thisPtr, MonoString* guiType, MonoString* styleName);
		static MonoObject* InternalGetfont(ScriptGUIElementStyle* thisPtr);
		static void InternalSetfont(ScriptGUIElementStyle* thisPtr, MonoObject* value);
		static uint32_t InternalGetfontSize(ScriptGUIElementStyle* thisPtr);
		static void InternalSetfontSize(ScriptGUIElementStyle* thisPtr, uint32_t value);
		static TextHorzAlign InternalGettextHorzAlign(ScriptGUIElementStyle* thisPtr);
		static void InternalSettextHorzAlign(ScriptGUIElementStyle* thisPtr, TextHorzAlign value);
		static TextVertAlign InternalGettextVertAlign(ScriptGUIElementStyle* thisPtr);
		static void InternalSettextVertAlign(ScriptGUIElementStyle* thisPtr, TextVertAlign value);
		static GUIImagePosition InternalGetimagePosition(ScriptGUIElementStyle* thisPtr);
		static void InternalSetimagePosition(ScriptGUIElementStyle* thisPtr, GUIImagePosition value);
		static bool InternalGetwordWrap(ScriptGUIElementStyle* thisPtr);
		static void InternalSetwordWrap(ScriptGUIElementStyle* thisPtr, bool value);
		static void InternalGetnormal(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output);
		static void InternalSetnormal(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value);
		static void InternalGethover(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output);
		static void InternalSethover(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value);
		static void InternalGetactive(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output);
		static void InternalSetactive(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value);
		static void InternalGetfocused(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output);
		static void InternalSetfocused(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value);
		static void InternalGetfocusedHover(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output);
		static void InternalSetfocusedHover(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value);
		static void InternalGetnormalOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output);
		static void InternalSetnormalOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value);
		static void InternalGethoverOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output);
		static void InternalSethoverOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value);
		static void InternalGetactiveOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output);
		static void InternalSetactiveOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value);
		static void InternalGetfocusedOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output);
		static void InternalSetfocusedOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value);
		static void InternalGetfocusedHoverOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output);
		static void InternalSetfocusedHoverOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value);
		static void InternalGetborder(ScriptGUIElementStyle* thisPtr, RectOffset* __output);
		static void InternalSetborder(ScriptGUIElementStyle* thisPtr, RectOffset* value);
		static void InternalGetmargins(ScriptGUIElementStyle* thisPtr, RectOffset* __output);
		static void InternalSetmargins(ScriptGUIElementStyle* thisPtr, RectOffset* value);
		static void InternalGetcontentOffset(ScriptGUIElementStyle* thisPtr, RectOffset* __output);
		static void InternalSetcontentOffset(ScriptGUIElementStyle* thisPtr, RectOffset* value);
		static void InternalGetpadding(ScriptGUIElementStyle* thisPtr, RectOffset* __output);
		static void InternalSetpadding(ScriptGUIElementStyle* thisPtr, RectOffset* value);
		static uint32_t InternalGetwidth(ScriptGUIElementStyle* thisPtr);
		static void InternalSetwidth(ScriptGUIElementStyle* thisPtr, uint32_t value);
		static uint32_t InternalGetheight(ScriptGUIElementStyle* thisPtr);
		static void InternalSetheight(ScriptGUIElementStyle* thisPtr, uint32_t value);
		static uint32_t InternalGetminWidth(ScriptGUIElementStyle* thisPtr);
		static void InternalSetminWidth(ScriptGUIElementStyle* thisPtr, uint32_t value);
		static uint32_t InternalGetmaxWidth(ScriptGUIElementStyle* thisPtr);
		static void InternalSetmaxWidth(ScriptGUIElementStyle* thisPtr, uint32_t value);
		static uint32_t InternalGetminHeight(ScriptGUIElementStyle* thisPtr);
		static void InternalSetminHeight(ScriptGUIElementStyle* thisPtr, uint32_t value);
		static uint32_t InternalGetmaxHeight(ScriptGUIElementStyle* thisPtr);
		static void InternalSetmaxHeight(ScriptGUIElementStyle* thisPtr, uint32_t value);
		static bool InternalGetfixedWidth(ScriptGUIElementStyle* thisPtr);
		static void InternalSetfixedWidth(ScriptGUIElementStyle* thisPtr, bool value);
		static bool InternalGetfixedHeight(ScriptGUIElementStyle* thisPtr);
		static void InternalSetfixedHeight(ScriptGUIElementStyle* thisPtr, bool value);
	};
}
