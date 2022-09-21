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
		metaData.scriptClass->AddInternalCall("Internal_GUIElementStyle", (void*)&ScriptGUIElementStyle::InternalGUIElementStyle);
		metaData.scriptClass->AddInternalCall("Internal_AddSubStyle", (void*)&ScriptGUIElementStyle::InternalAddSubStyle);
		metaData.scriptClass->AddInternalCall("Internal_Getfont", (void*)&ScriptGUIElementStyle::InternalGetfont);
		metaData.scriptClass->AddInternalCall("Internal_Setfont", (void*)&ScriptGUIElementStyle::InternalSetfont);
		metaData.scriptClass->AddInternalCall("Internal_GetfontSize", (void*)&ScriptGUIElementStyle::InternalGetfontSize);
		metaData.scriptClass->AddInternalCall("Internal_SetfontSize", (void*)&ScriptGUIElementStyle::InternalSetfontSize);
		metaData.scriptClass->AddInternalCall("Internal_GettextHorzAlign", (void*)&ScriptGUIElementStyle::InternalGettextHorzAlign);
		metaData.scriptClass->AddInternalCall("Internal_SettextHorzAlign", (void*)&ScriptGUIElementStyle::InternalSettextHorzAlign);
		metaData.scriptClass->AddInternalCall("Internal_GettextVertAlign", (void*)&ScriptGUIElementStyle::InternalGettextVertAlign);
		metaData.scriptClass->AddInternalCall("Internal_SettextVertAlign", (void*)&ScriptGUIElementStyle::InternalSettextVertAlign);
		metaData.scriptClass->AddInternalCall("Internal_GetimagePosition", (void*)&ScriptGUIElementStyle::InternalGetimagePosition);
		metaData.scriptClass->AddInternalCall("Internal_SetimagePosition", (void*)&ScriptGUIElementStyle::InternalSetimagePosition);
		metaData.scriptClass->AddInternalCall("Internal_GetwordWrap", (void*)&ScriptGUIElementStyle::InternalGetwordWrap);
		metaData.scriptClass->AddInternalCall("Internal_SetwordWrap", (void*)&ScriptGUIElementStyle::InternalSetwordWrap);
		metaData.scriptClass->AddInternalCall("Internal_Getnormal", (void*)&ScriptGUIElementStyle::InternalGetnormal);
		metaData.scriptClass->AddInternalCall("Internal_Setnormal", (void*)&ScriptGUIElementStyle::InternalSetnormal);
		metaData.scriptClass->AddInternalCall("Internal_Gethover", (void*)&ScriptGUIElementStyle::InternalGethover);
		metaData.scriptClass->AddInternalCall("Internal_Sethover", (void*)&ScriptGUIElementStyle::InternalSethover);
		metaData.scriptClass->AddInternalCall("Internal_Getactive", (void*)&ScriptGUIElementStyle::InternalGetactive);
		metaData.scriptClass->AddInternalCall("Internal_Setactive", (void*)&ScriptGUIElementStyle::InternalSetactive);
		metaData.scriptClass->AddInternalCall("Internal_Getfocused", (void*)&ScriptGUIElementStyle::InternalGetfocused);
		metaData.scriptClass->AddInternalCall("Internal_Setfocused", (void*)&ScriptGUIElementStyle::InternalSetfocused);
		metaData.scriptClass->AddInternalCall("Internal_GetfocusedHover", (void*)&ScriptGUIElementStyle::InternalGetfocusedHover);
		metaData.scriptClass->AddInternalCall("Internal_SetfocusedHover", (void*)&ScriptGUIElementStyle::InternalSetfocusedHover);
		metaData.scriptClass->AddInternalCall("Internal_GetnormalOn", (void*)&ScriptGUIElementStyle::InternalGetnormalOn);
		metaData.scriptClass->AddInternalCall("Internal_SetnormalOn", (void*)&ScriptGUIElementStyle::InternalSetnormalOn);
		metaData.scriptClass->AddInternalCall("Internal_GethoverOn", (void*)&ScriptGUIElementStyle::InternalGethoverOn);
		metaData.scriptClass->AddInternalCall("Internal_SethoverOn", (void*)&ScriptGUIElementStyle::InternalSethoverOn);
		metaData.scriptClass->AddInternalCall("Internal_GetactiveOn", (void*)&ScriptGUIElementStyle::InternalGetactiveOn);
		metaData.scriptClass->AddInternalCall("Internal_SetactiveOn", (void*)&ScriptGUIElementStyle::InternalSetactiveOn);
		metaData.scriptClass->AddInternalCall("Internal_GetfocusedOn", (void*)&ScriptGUIElementStyle::InternalGetfocusedOn);
		metaData.scriptClass->AddInternalCall("Internal_SetfocusedOn", (void*)&ScriptGUIElementStyle::InternalSetfocusedOn);
		metaData.scriptClass->AddInternalCall("Internal_GetfocusedHoverOn", (void*)&ScriptGUIElementStyle::InternalGetfocusedHoverOn);
		metaData.scriptClass->AddInternalCall("Internal_SetfocusedHoverOn", (void*)&ScriptGUIElementStyle::InternalSetfocusedHoverOn);
		metaData.scriptClass->AddInternalCall("Internal_Getborder", (void*)&ScriptGUIElementStyle::InternalGetborder);
		metaData.scriptClass->AddInternalCall("Internal_Setborder", (void*)&ScriptGUIElementStyle::InternalSetborder);
		metaData.scriptClass->AddInternalCall("Internal_Getmargins", (void*)&ScriptGUIElementStyle::InternalGetmargins);
		metaData.scriptClass->AddInternalCall("Internal_Setmargins", (void*)&ScriptGUIElementStyle::InternalSetmargins);
		metaData.scriptClass->AddInternalCall("Internal_GetcontentOffset", (void*)&ScriptGUIElementStyle::InternalGetcontentOffset);
		metaData.scriptClass->AddInternalCall("Internal_SetcontentOffset", (void*)&ScriptGUIElementStyle::InternalSetcontentOffset);
		metaData.scriptClass->AddInternalCall("Internal_Getpadding", (void*)&ScriptGUIElementStyle::InternalGetpadding);
		metaData.scriptClass->AddInternalCall("Internal_Setpadding", (void*)&ScriptGUIElementStyle::InternalSetpadding);
		metaData.scriptClass->AddInternalCall("Internal_Getwidth", (void*)&ScriptGUIElementStyle::InternalGetwidth);
		metaData.scriptClass->AddInternalCall("Internal_Setwidth", (void*)&ScriptGUIElementStyle::InternalSetwidth);
		metaData.scriptClass->AddInternalCall("Internal_Getheight", (void*)&ScriptGUIElementStyle::InternalGetheight);
		metaData.scriptClass->AddInternalCall("Internal_Setheight", (void*)&ScriptGUIElementStyle::InternalSetheight);
		metaData.scriptClass->AddInternalCall("Internal_GetminWidth", (void*)&ScriptGUIElementStyle::InternalGetminWidth);
		metaData.scriptClass->AddInternalCall("Internal_SetminWidth", (void*)&ScriptGUIElementStyle::InternalSetminWidth);
		metaData.scriptClass->AddInternalCall("Internal_GetmaxWidth", (void*)&ScriptGUIElementStyle::InternalGetmaxWidth);
		metaData.scriptClass->AddInternalCall("Internal_SetmaxWidth", (void*)&ScriptGUIElementStyle::InternalSetmaxWidth);
		metaData.scriptClass->AddInternalCall("Internal_GetminHeight", (void*)&ScriptGUIElementStyle::InternalGetminHeight);
		metaData.scriptClass->AddInternalCall("Internal_SetminHeight", (void*)&ScriptGUIElementStyle::InternalSetminHeight);
		metaData.scriptClass->AddInternalCall("Internal_GetmaxHeight", (void*)&ScriptGUIElementStyle::InternalGetmaxHeight);
		metaData.scriptClass->AddInternalCall("Internal_SetmaxHeight", (void*)&ScriptGUIElementStyle::InternalSetmaxHeight);
		metaData.scriptClass->AddInternalCall("Internal_GetfixedWidth", (void*)&ScriptGUIElementStyle::InternalGetfixedWidth);
		metaData.scriptClass->AddInternalCall("Internal_SetfixedWidth", (void*)&ScriptGUIElementStyle::InternalSetfixedWidth);
		metaData.scriptClass->AddInternalCall("Internal_GetfixedHeight", (void*)&ScriptGUIElementStyle::InternalGetfixedHeight);
		metaData.scriptClass->AddInternalCall("Internal_SetfixedHeight", (void*)&ScriptGUIElementStyle::InternalSetfixedHeight);

	}

	MonoObject* ScriptGUIElementStyle::Create(const SPtr<GUIElementStyle>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->CreateInstance("bool", ctorParams);
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
		tmp__output = thisPtr->GetInternal()->font;

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
		thisPtr->GetInternal()->font = tmpvalue;
	}

	uint32_t ScriptGUIElementStyle::InternalGetfontSize(ScriptGUIElementStyle* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->fontSize;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetfontSize(ScriptGUIElementStyle* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->fontSize = value;
	}

	TextHorzAlign ScriptGUIElementStyle::InternalGettextHorzAlign(ScriptGUIElementStyle* thisPtr)
	{
		TextHorzAlign tmp__output;
		tmp__output = thisPtr->GetInternal()->textHorzAlign;

		TextHorzAlign __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSettextHorzAlign(ScriptGUIElementStyle* thisPtr, TextHorzAlign value)
	{
		thisPtr->GetInternal()->textHorzAlign = value;
	}

	TextVertAlign ScriptGUIElementStyle::InternalGettextVertAlign(ScriptGUIElementStyle* thisPtr)
	{
		TextVertAlign tmp__output;
		tmp__output = thisPtr->GetInternal()->textVertAlign;

		TextVertAlign __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSettextVertAlign(ScriptGUIElementStyle* thisPtr, TextVertAlign value)
	{
		thisPtr->GetInternal()->textVertAlign = value;
	}

	GUIImagePosition ScriptGUIElementStyle::InternalGetimagePosition(ScriptGUIElementStyle* thisPtr)
	{
		GUIImagePosition tmp__output;
		tmp__output = thisPtr->GetInternal()->imagePosition;

		GUIImagePosition __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetimagePosition(ScriptGUIElementStyle* thisPtr, GUIImagePosition value)
	{
		thisPtr->GetInternal()->imagePosition = value;
	}

	bool ScriptGUIElementStyle::InternalGetwordWrap(ScriptGUIElementStyle* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->wordWrap;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetwordWrap(ScriptGUIElementStyle* thisPtr, bool value)
	{
		thisPtr->GetInternal()->wordWrap = value;
	}

	void ScriptGUIElementStyle::InternalGetnormal(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->normal;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptGUIElementStateStyle::GetMetaData()->scriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetnormal(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::FromInterop(*value);
		thisPtr->GetInternal()->normal = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGethover(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->hover;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptGUIElementStateStyle::GetMetaData()->scriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSethover(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::FromInterop(*value);
		thisPtr->GetInternal()->hover = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGetactive(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->active;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptGUIElementStateStyle::GetMetaData()->scriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetactive(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::FromInterop(*value);
		thisPtr->GetInternal()->active = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGetfocused(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->focused;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptGUIElementStateStyle::GetMetaData()->scriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetfocused(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::FromInterop(*value);
		thisPtr->GetInternal()->focused = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGetfocusedHover(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->focusedHover;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptGUIElementStateStyle::GetMetaData()->scriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetfocusedHover(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::FromInterop(*value);
		thisPtr->GetInternal()->focusedHover = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGetnormalOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->normalOn;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptGUIElementStateStyle::GetMetaData()->scriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetnormalOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::FromInterop(*value);
		thisPtr->GetInternal()->normalOn = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGethoverOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->hoverOn;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptGUIElementStateStyle::GetMetaData()->scriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSethoverOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::FromInterop(*value);
		thisPtr->GetInternal()->hoverOn = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGetactiveOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->activeOn;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptGUIElementStateStyle::GetMetaData()->scriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetactiveOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::FromInterop(*value);
		thisPtr->GetInternal()->activeOn = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGetfocusedOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->focusedOn;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptGUIElementStateStyle::GetMetaData()->scriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetfocusedOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::FromInterop(*value);
		thisPtr->GetInternal()->focusedOn = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGetfocusedHoverOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->focusedHoverOn;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptGUIElementStateStyle::GetMetaData()->scriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetfocusedHoverOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::FromInterop(*value);
		thisPtr->GetInternal()->focusedHoverOn = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGetborder(ScriptGUIElementStyle* thisPtr, RectOffset* __output)
	{
		RectOffset tmp__output;
		tmp__output = thisPtr->GetInternal()->border;

		*__output = tmp__output;


	}

	void ScriptGUIElementStyle::InternalSetborder(ScriptGUIElementStyle* thisPtr, RectOffset* value)
	{
		thisPtr->GetInternal()->border = *value;
	}

	void ScriptGUIElementStyle::InternalGetmargins(ScriptGUIElementStyle* thisPtr, RectOffset* __output)
	{
		RectOffset tmp__output;
		tmp__output = thisPtr->GetInternal()->margins;

		*__output = tmp__output;


	}

	void ScriptGUIElementStyle::InternalSetmargins(ScriptGUIElementStyle* thisPtr, RectOffset* value)
	{
		thisPtr->GetInternal()->margins = *value;
	}

	void ScriptGUIElementStyle::InternalGetcontentOffset(ScriptGUIElementStyle* thisPtr, RectOffset* __output)
	{
		RectOffset tmp__output;
		tmp__output = thisPtr->GetInternal()->contentOffset;

		*__output = tmp__output;


	}

	void ScriptGUIElementStyle::InternalSetcontentOffset(ScriptGUIElementStyle* thisPtr, RectOffset* value)
	{
		thisPtr->GetInternal()->contentOffset = *value;
	}

	void ScriptGUIElementStyle::InternalGetpadding(ScriptGUIElementStyle* thisPtr, RectOffset* __output)
	{
		RectOffset tmp__output;
		tmp__output = thisPtr->GetInternal()->padding;

		*__output = tmp__output;


	}

	void ScriptGUIElementStyle::InternalSetpadding(ScriptGUIElementStyle* thisPtr, RectOffset* value)
	{
		thisPtr->GetInternal()->padding = *value;
	}

	uint32_t ScriptGUIElementStyle::InternalGetwidth(ScriptGUIElementStyle* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->width;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetwidth(ScriptGUIElementStyle* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->width = value;
	}

	uint32_t ScriptGUIElementStyle::InternalGetheight(ScriptGUIElementStyle* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->height;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetheight(ScriptGUIElementStyle* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->height = value;
	}

	uint32_t ScriptGUIElementStyle::InternalGetminWidth(ScriptGUIElementStyle* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->minWidth;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetminWidth(ScriptGUIElementStyle* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->minWidth = value;
	}

	uint32_t ScriptGUIElementStyle::InternalGetmaxWidth(ScriptGUIElementStyle* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->maxWidth;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetmaxWidth(ScriptGUIElementStyle* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->maxWidth = value;
	}

	uint32_t ScriptGUIElementStyle::InternalGetminHeight(ScriptGUIElementStyle* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->minHeight;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetminHeight(ScriptGUIElementStyle* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->minHeight = value;
	}

	uint32_t ScriptGUIElementStyle::InternalGetmaxHeight(ScriptGUIElementStyle* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->maxHeight;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetmaxHeight(ScriptGUIElementStyle* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->maxHeight = value;
	}

	bool ScriptGUIElementStyle::InternalGetfixedWidth(ScriptGUIElementStyle* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->fixedWidth;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetfixedWidth(ScriptGUIElementStyle* thisPtr, bool value)
	{
		thisPtr->GetInternal()->fixedWidth = value;
	}

	bool ScriptGUIElementStyle::InternalGetfixedHeight(ScriptGUIElementStyle* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->fixedHeight;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetfixedHeight(ScriptGUIElementStyle* thisPtr, bool value)
	{
		thisPtr->GetInternal()->fixedHeight = value;
	}
}
