//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptGUIElementStyle.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptResourceManager.h"
#include "BsScriptFont.generated.h"
#include "BsScriptRectOffset.generated.h"
#include "BsScriptGUIElementStateStyle.generated.h"

namespace bs
{
	ScriptGUIElementStyle::ScriptGUIElementStyle(MonoObject* managedInstance, const SPtr<GUIElementStyle>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptGUIElementStyle::InitRuntimeData()
	{
		metaData.ScriptClass->AddInternalCall("Internal_GUIElementStyle", (void*)&ScriptGUIElementStyle::InternalGUIElementStyle);
		metaData.ScriptClass->AddInternalCall("Internal_AddSubStyle", (void*)&ScriptGUIElementStyle::InternalAddSubStyle);
		metaData.ScriptClass->AddInternalCall("Internal_Getfont", (void*)&ScriptGUIElementStyle::InternalGetfont);
		metaData.ScriptClass->AddInternalCall("Internal_Setfont", (void*)&ScriptGUIElementStyle::InternalSetfont);
		metaData.ScriptClass->AddInternalCall("Internal_GetfontSize", (void*)&ScriptGUIElementStyle::InternalGetfontSize);
		metaData.ScriptClass->AddInternalCall("Internal_SetfontSize", (void*)&ScriptGUIElementStyle::InternalSetfontSize);
		metaData.ScriptClass->AddInternalCall("Internal_GettextHorzAlign", (void*)&ScriptGUIElementStyle::InternalGettextHorzAlign);
		metaData.ScriptClass->AddInternalCall("Internal_SettextHorzAlign", (void*)&ScriptGUIElementStyle::InternalSettextHorzAlign);
		metaData.ScriptClass->AddInternalCall("Internal_GettextVertAlign", (void*)&ScriptGUIElementStyle::InternalGettextVertAlign);
		metaData.ScriptClass->AddInternalCall("Internal_SettextVertAlign", (void*)&ScriptGUIElementStyle::InternalSettextVertAlign);
		metaData.ScriptClass->AddInternalCall("Internal_GetimagePosition", (void*)&ScriptGUIElementStyle::InternalGetimagePosition);
		metaData.ScriptClass->AddInternalCall("Internal_SetimagePosition", (void*)&ScriptGUIElementStyle::InternalSetimagePosition);
		metaData.ScriptClass->AddInternalCall("Internal_GetwordWrap", (void*)&ScriptGUIElementStyle::InternalGetwordWrap);
		metaData.ScriptClass->AddInternalCall("Internal_SetwordWrap", (void*)&ScriptGUIElementStyle::InternalSetwordWrap);
		metaData.ScriptClass->AddInternalCall("Internal_Getnormal", (void*)&ScriptGUIElementStyle::InternalGetnormal);
		metaData.ScriptClass->AddInternalCall("Internal_Setnormal", (void*)&ScriptGUIElementStyle::InternalSetnormal);
		metaData.ScriptClass->AddInternalCall("Internal_Gethover", (void*)&ScriptGUIElementStyle::InternalGethover);
		metaData.ScriptClass->AddInternalCall("Internal_Sethover", (void*)&ScriptGUIElementStyle::InternalSethover);
		metaData.ScriptClass->AddInternalCall("Internal_Getactive", (void*)&ScriptGUIElementStyle::InternalGetactive);
		metaData.ScriptClass->AddInternalCall("Internal_Setactive", (void*)&ScriptGUIElementStyle::InternalSetactive);
		metaData.ScriptClass->AddInternalCall("Internal_Getfocused", (void*)&ScriptGUIElementStyle::InternalGetfocused);
		metaData.ScriptClass->AddInternalCall("Internal_Setfocused", (void*)&ScriptGUIElementStyle::InternalSetfocused);
		metaData.ScriptClass->AddInternalCall("Internal_GetfocusedHover", (void*)&ScriptGUIElementStyle::InternalGetfocusedHover);
		metaData.ScriptClass->AddInternalCall("Internal_SetfocusedHover", (void*)&ScriptGUIElementStyle::InternalSetfocusedHover);
		metaData.ScriptClass->AddInternalCall("Internal_GetnormalOn", (void*)&ScriptGUIElementStyle::InternalGetnormalOn);
		metaData.ScriptClass->AddInternalCall("Internal_SetnormalOn", (void*)&ScriptGUIElementStyle::InternalSetnormalOn);
		metaData.ScriptClass->AddInternalCall("Internal_GethoverOn", (void*)&ScriptGUIElementStyle::InternalGethoverOn);
		metaData.ScriptClass->AddInternalCall("Internal_SethoverOn", (void*)&ScriptGUIElementStyle::InternalSethoverOn);
		metaData.ScriptClass->AddInternalCall("Internal_GetactiveOn", (void*)&ScriptGUIElementStyle::InternalGetactiveOn);
		metaData.ScriptClass->AddInternalCall("Internal_SetactiveOn", (void*)&ScriptGUIElementStyle::InternalSetactiveOn);
		metaData.ScriptClass->AddInternalCall("Internal_GetfocusedOn", (void*)&ScriptGUIElementStyle::InternalGetfocusedOn);
		metaData.ScriptClass->AddInternalCall("Internal_SetfocusedOn", (void*)&ScriptGUIElementStyle::InternalSetfocusedOn);
		metaData.ScriptClass->AddInternalCall("Internal_GetfocusedHoverOn", (void*)&ScriptGUIElementStyle::InternalGetfocusedHoverOn);
		metaData.ScriptClass->AddInternalCall("Internal_SetfocusedHoverOn", (void*)&ScriptGUIElementStyle::InternalSetfocusedHoverOn);
		metaData.ScriptClass->AddInternalCall("Internal_Getborder", (void*)&ScriptGUIElementStyle::InternalGetborder);
		metaData.ScriptClass->AddInternalCall("Internal_Setborder", (void*)&ScriptGUIElementStyle::InternalSetborder);
		metaData.ScriptClass->AddInternalCall("Internal_Getmargins", (void*)&ScriptGUIElementStyle::InternalGetmargins);
		metaData.ScriptClass->AddInternalCall("Internal_Setmargins", (void*)&ScriptGUIElementStyle::InternalSetmargins);
		metaData.ScriptClass->AddInternalCall("Internal_GetcontentOffset", (void*)&ScriptGUIElementStyle::InternalGetcontentOffset);
		metaData.ScriptClass->AddInternalCall("Internal_SetcontentOffset", (void*)&ScriptGUIElementStyle::InternalSetcontentOffset);
		metaData.ScriptClass->AddInternalCall("Internal_Getpadding", (void*)&ScriptGUIElementStyle::InternalGetpadding);
		metaData.ScriptClass->AddInternalCall("Internal_Setpadding", (void*)&ScriptGUIElementStyle::InternalSetpadding);
		metaData.ScriptClass->AddInternalCall("Internal_Getwidth", (void*)&ScriptGUIElementStyle::InternalGetwidth);
		metaData.ScriptClass->AddInternalCall("Internal_Setwidth", (void*)&ScriptGUIElementStyle::InternalSetwidth);
		metaData.ScriptClass->AddInternalCall("Internal_Getheight", (void*)&ScriptGUIElementStyle::InternalGetheight);
		metaData.ScriptClass->AddInternalCall("Internal_Setheight", (void*)&ScriptGUIElementStyle::InternalSetheight);
		metaData.ScriptClass->AddInternalCall("Internal_GetminWidth", (void*)&ScriptGUIElementStyle::InternalGetminWidth);
		metaData.ScriptClass->AddInternalCall("Internal_SetminWidth", (void*)&ScriptGUIElementStyle::InternalSetminWidth);
		metaData.ScriptClass->AddInternalCall("Internal_GetmaxWidth", (void*)&ScriptGUIElementStyle::InternalGetmaxWidth);
		metaData.ScriptClass->AddInternalCall("Internal_SetmaxWidth", (void*)&ScriptGUIElementStyle::InternalSetmaxWidth);
		metaData.ScriptClass->AddInternalCall("Internal_GetminHeight", (void*)&ScriptGUIElementStyle::InternalGetminHeight);
		metaData.ScriptClass->AddInternalCall("Internal_SetminHeight", (void*)&ScriptGUIElementStyle::InternalSetminHeight);
		metaData.ScriptClass->AddInternalCall("Internal_GetmaxHeight", (void*)&ScriptGUIElementStyle::InternalGetmaxHeight);
		metaData.ScriptClass->AddInternalCall("Internal_SetmaxHeight", (void*)&ScriptGUIElementStyle::InternalSetmaxHeight);
		metaData.ScriptClass->AddInternalCall("Internal_GetfixedWidth", (void*)&ScriptGUIElementStyle::InternalGetfixedWidth);
		metaData.ScriptClass->AddInternalCall("Internal_SetfixedWidth", (void*)&ScriptGUIElementStyle::InternalSetfixedWidth);
		metaData.ScriptClass->AddInternalCall("Internal_GetfixedHeight", (void*)&ScriptGUIElementStyle::InternalGetfixedHeight);
		metaData.ScriptClass->AddInternalCall("Internal_SetfixedHeight", (void*)&ScriptGUIElementStyle::InternalSetfixedHeight);

	}

	MonoObject* ScriptGUIElementStyle::Create(const SPtr<GUIElementStyle>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.ScriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptGUIElementStyle>()) ScriptGUIElementStyle(managedInstance, value);
		return managedInstance;
	}
	void ScriptGUIElementStyle::InternalGUIElementStyle(MonoObject* managedInstance)
	{
		SPtr<GUIElementStyle> instance = bs_shared_ptr_new<GUIElementStyle>();
		new (bs_alloc<ScriptGUIElementStyle>())ScriptGUIElementStyle(managedInstance, instance);
	}

	void ScriptGUIElementStyle::InternalAddSubStyle(ScriptGUIElementStyle* thisPtr, MonoString* guiType, MonoString* styleName)
	{
		String tmpguiType;
		tmpguiType = MonoUtil::MonoToString(guiType);
		String tmpstyleName;
		tmpstyleName = MonoUtil::MonoToString(styleName);
		thisPtr->GetInternal()->AddSubStyle(tmpguiType, tmpstyleName);
	}

	MonoObject* ScriptGUIElementStyle::InternalGetfont(ScriptGUIElementStyle* thisPtr)
	{
		ResourceHandle<Font> tmp__output;
		tmp__output = thisPtr->GetInternal()->Font;

		MonoObject* __output;
		ScriptResourceBase* script__output;
		script__output = ScriptResourceManager::Instance().GetScriptResource(tmp__output, true);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetfont(ScriptGUIElementStyle* thisPtr, MonoObject* value)
	{
		ResourceHandle<Font> tmpvalue;
		ScriptFont* scriptvalue;
		scriptvalue = ScriptFont::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = scriptvalue->GetHandle();
		thisPtr->GetInternal()->Font = tmpvalue;
	}

	uint32_t ScriptGUIElementStyle::InternalGetfontSize(ScriptGUIElementStyle* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->FontSize;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetfontSize(ScriptGUIElementStyle* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->FontSize = value;
	}

	TextHorzAlign ScriptGUIElementStyle::InternalGettextHorzAlign(ScriptGUIElementStyle* thisPtr)
	{
		TextHorzAlign tmp__output;
		tmp__output = thisPtr->GetInternal()->TextHorzAlign;

		TextHorzAlign __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSettextHorzAlign(ScriptGUIElementStyle* thisPtr, TextHorzAlign value)
	{
		thisPtr->GetInternal()->TextHorzAlign = value;
	}

	TextVertAlign ScriptGUIElementStyle::InternalGettextVertAlign(ScriptGUIElementStyle* thisPtr)
	{
		TextVertAlign tmp__output;
		tmp__output = thisPtr->GetInternal()->TextVertAlign;

		TextVertAlign __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSettextVertAlign(ScriptGUIElementStyle* thisPtr, TextVertAlign value)
	{
		thisPtr->GetInternal()->TextVertAlign = value;
	}

	GUIImagePosition ScriptGUIElementStyle::InternalGetimagePosition(ScriptGUIElementStyle* thisPtr)
	{
		GUIImagePosition tmp__output;
		tmp__output = thisPtr->GetInternal()->ImagePosition;

		GUIImagePosition __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetimagePosition(ScriptGUIElementStyle* thisPtr, GUIImagePosition value)
	{
		thisPtr->GetInternal()->ImagePosition = value;
	}

	bool ScriptGUIElementStyle::InternalGetwordWrap(ScriptGUIElementStyle* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->WordWrap;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetwordWrap(ScriptGUIElementStyle* thisPtr, bool value)
	{
		thisPtr->GetInternal()->WordWrap = value;
	}

	void ScriptGUIElementStyle::InternalGetnormal(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->Normal;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptGUIElementStateStyle::GetMetaData()->ScriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetnormal(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::FromInterop(*value);
		thisPtr->GetInternal()->Normal = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGethover(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->Hover;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptGUIElementStateStyle::GetMetaData()->ScriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSethover(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::FromInterop(*value);
		thisPtr->GetInternal()->Hover = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGetactive(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->Active;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptGUIElementStateStyle::GetMetaData()->ScriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetactive(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::FromInterop(*value);
		thisPtr->GetInternal()->Active = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGetfocused(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->Focused;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptGUIElementStateStyle::GetMetaData()->ScriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetfocused(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::FromInterop(*value);
		thisPtr->GetInternal()->Focused = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGetfocusedHover(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->FocusedHover;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptGUIElementStateStyle::GetMetaData()->ScriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetfocusedHover(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::FromInterop(*value);
		thisPtr->GetInternal()->FocusedHover = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGetnormalOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->NormalOn;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptGUIElementStateStyle::GetMetaData()->ScriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetnormalOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::FromInterop(*value);
		thisPtr->GetInternal()->NormalOn = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGethoverOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->HoverOn;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptGUIElementStateStyle::GetMetaData()->ScriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSethoverOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::FromInterop(*value);
		thisPtr->GetInternal()->HoverOn = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGetactiveOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->ActiveOn;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptGUIElementStateStyle::GetMetaData()->ScriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetactiveOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::FromInterop(*value);
		thisPtr->GetInternal()->ActiveOn = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGetfocusedOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->FocusedOn;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptGUIElementStateStyle::GetMetaData()->ScriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetfocusedOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::FromInterop(*value);
		thisPtr->GetInternal()->FocusedOn = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGetfocusedHoverOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->FocusedHoverOn;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptGUIElementStateStyle::GetMetaData()->ScriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetfocusedHoverOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::FromInterop(*value);
		thisPtr->GetInternal()->FocusedHoverOn = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGetborder(ScriptGUIElementStyle* thisPtr, RectOffset* __output)
	{
		RectOffset tmp__output;
		tmp__output = thisPtr->GetInternal()->Border;

		*__output = tmp__output;


	}

	void ScriptGUIElementStyle::InternalSetborder(ScriptGUIElementStyle* thisPtr, RectOffset* value)
	{
		thisPtr->GetInternal()->Border = *value;
	}

	void ScriptGUIElementStyle::InternalGetmargins(ScriptGUIElementStyle* thisPtr, RectOffset* __output)
	{
		RectOffset tmp__output;
		tmp__output = thisPtr->GetInternal()->Margins;

		*__output = tmp__output;


	}

	void ScriptGUIElementStyle::InternalSetmargins(ScriptGUIElementStyle* thisPtr, RectOffset* value)
	{
		thisPtr->GetInternal()->Margins = *value;
	}

	void ScriptGUIElementStyle::InternalGetcontentOffset(ScriptGUIElementStyle* thisPtr, RectOffset* __output)
	{
		RectOffset tmp__output;
		tmp__output = thisPtr->GetInternal()->ContentOffset;

		*__output = tmp__output;


	}

	void ScriptGUIElementStyle::InternalSetcontentOffset(ScriptGUIElementStyle* thisPtr, RectOffset* value)
	{
		thisPtr->GetInternal()->ContentOffset = *value;
	}

	void ScriptGUIElementStyle::InternalGetpadding(ScriptGUIElementStyle* thisPtr, RectOffset* __output)
	{
		RectOffset tmp__output;
		tmp__output = thisPtr->GetInternal()->Padding;

		*__output = tmp__output;


	}

	void ScriptGUIElementStyle::InternalSetpadding(ScriptGUIElementStyle* thisPtr, RectOffset* value)
	{
		thisPtr->GetInternal()->Padding = *value;
	}

	uint32_t ScriptGUIElementStyle::InternalGetwidth(ScriptGUIElementStyle* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->Width;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetwidth(ScriptGUIElementStyle* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->Width = value;
	}

	uint32_t ScriptGUIElementStyle::InternalGetheight(ScriptGUIElementStyle* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->Height;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetheight(ScriptGUIElementStyle* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->Height = value;
	}

	uint32_t ScriptGUIElementStyle::InternalGetminWidth(ScriptGUIElementStyle* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->MinWidth;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetminWidth(ScriptGUIElementStyle* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->MinWidth = value;
	}

	uint32_t ScriptGUIElementStyle::InternalGetmaxWidth(ScriptGUIElementStyle* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->MaxWidth;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetmaxWidth(ScriptGUIElementStyle* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->MaxWidth = value;
	}

	uint32_t ScriptGUIElementStyle::InternalGetminHeight(ScriptGUIElementStyle* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->MinHeight;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetminHeight(ScriptGUIElementStyle* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->MinHeight = value;
	}

	uint32_t ScriptGUIElementStyle::InternalGetmaxHeight(ScriptGUIElementStyle* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->MaxHeight;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetmaxHeight(ScriptGUIElementStyle* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->MaxHeight = value;
	}

	bool ScriptGUIElementStyle::InternalGetfixedWidth(ScriptGUIElementStyle* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->FixedWidth;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetfixedWidth(ScriptGUIElementStyle* thisPtr, bool value)
	{
		thisPtr->GetInternal()->FixedWidth = value;
	}

	bool ScriptGUIElementStyle::InternalGetfixedHeight(ScriptGUIElementStyle* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->FixedHeight;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetfixedHeight(ScriptGUIElementStyle* thisPtr, bool value)
	{
		thisPtr->GetInternal()->FixedHeight = value;
	}
}
