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

	void ScriptGUIElementStyle::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_GUIElementStyle", (void*)&ScriptGUIElementStyle::InternalGuiElementStyle);
		metaData.scriptClass->AddInternalCall("Internal_addSubStyle", (void*)&ScriptGUIElementStyle::InternalAddSubStyle);
		metaData.scriptClass->AddInternalCall("Internal_getfont", (void*)&ScriptGUIElementStyle::InternalGetfont);
		metaData.scriptClass->AddInternalCall("Internal_setfont", (void*)&ScriptGUIElementStyle::InternalSetfont);
		metaData.scriptClass->AddInternalCall("Internal_getfontSize", (void*)&ScriptGUIElementStyle::InternalGetfontSize);
		metaData.scriptClass->AddInternalCall("Internal_setfontSize", (void*)&ScriptGUIElementStyle::InternalSetfontSize);
		metaData.scriptClass->AddInternalCall("Internal_gettextHorzAlign", (void*)&ScriptGUIElementStyle::InternalGettextHorzAlign);
		metaData.scriptClass->AddInternalCall("Internal_settextHorzAlign", (void*)&ScriptGUIElementStyle::InternalSettextHorzAlign);
		metaData.scriptClass->AddInternalCall("Internal_gettextVertAlign", (void*)&ScriptGUIElementStyle::InternalGettextVertAlign);
		metaData.scriptClass->AddInternalCall("Internal_settextVertAlign", (void*)&ScriptGUIElementStyle::InternalSettextVertAlign);
		metaData.scriptClass->AddInternalCall("Internal_getimagePosition", (void*)&ScriptGUIElementStyle::InternalGetimagePosition);
		metaData.scriptClass->AddInternalCall("Internal_setimagePosition", (void*)&ScriptGUIElementStyle::InternalSetimagePosition);
		metaData.scriptClass->AddInternalCall("Internal_getwordWrap", (void*)&ScriptGUIElementStyle::InternalGetwordWrap);
		metaData.scriptClass->AddInternalCall("Internal_setwordWrap", (void*)&ScriptGUIElementStyle::InternalSetwordWrap);
		metaData.scriptClass->AddInternalCall("Internal_getnormal", (void*)&ScriptGUIElementStyle::InternalGetnormal);
		metaData.scriptClass->AddInternalCall("Internal_setnormal", (void*)&ScriptGUIElementStyle::InternalSetnormal);
		metaData.scriptClass->AddInternalCall("Internal_gethover", (void*)&ScriptGUIElementStyle::InternalGethover);
		metaData.scriptClass->AddInternalCall("Internal_sethover", (void*)&ScriptGUIElementStyle::InternalSethover);
		metaData.scriptClass->AddInternalCall("Internal_getactive", (void*)&ScriptGUIElementStyle::InternalGetactive);
		metaData.scriptClass->AddInternalCall("Internal_setactive", (void*)&ScriptGUIElementStyle::InternalSetactive);
		metaData.scriptClass->AddInternalCall("Internal_getfocused", (void*)&ScriptGUIElementStyle::InternalGetfocused);
		metaData.scriptClass->AddInternalCall("Internal_setfocused", (void*)&ScriptGUIElementStyle::InternalSetfocused);
		metaData.scriptClass->AddInternalCall("Internal_getfocusedHover", (void*)&ScriptGUIElementStyle::InternalGetfocusedHover);
		metaData.scriptClass->AddInternalCall("Internal_setfocusedHover", (void*)&ScriptGUIElementStyle::InternalSetfocusedHover);
		metaData.scriptClass->AddInternalCall("Internal_getnormalOn", (void*)&ScriptGUIElementStyle::InternalGetnormalOn);
		metaData.scriptClass->AddInternalCall("Internal_setnormalOn", (void*)&ScriptGUIElementStyle::InternalSetnormalOn);
		metaData.scriptClass->AddInternalCall("Internal_gethoverOn", (void*)&ScriptGUIElementStyle::InternalGethoverOn);
		metaData.scriptClass->AddInternalCall("Internal_sethoverOn", (void*)&ScriptGUIElementStyle::InternalSethoverOn);
		metaData.scriptClass->AddInternalCall("Internal_getactiveOn", (void*)&ScriptGUIElementStyle::InternalGetactiveOn);
		metaData.scriptClass->AddInternalCall("Internal_setactiveOn", (void*)&ScriptGUIElementStyle::InternalSetactiveOn);
		metaData.scriptClass->AddInternalCall("Internal_getfocusedOn", (void*)&ScriptGUIElementStyle::InternalGetfocusedOn);
		metaData.scriptClass->AddInternalCall("Internal_setfocusedOn", (void*)&ScriptGUIElementStyle::InternalSetfocusedOn);
		metaData.scriptClass->AddInternalCall("Internal_getfocusedHoverOn", (void*)&ScriptGUIElementStyle::InternalGetfocusedHoverOn);
		metaData.scriptClass->AddInternalCall("Internal_setfocusedHoverOn", (void*)&ScriptGUIElementStyle::InternalSetfocusedHoverOn);
		metaData.scriptClass->AddInternalCall("Internal_getborder", (void*)&ScriptGUIElementStyle::InternalGetborder);
		metaData.scriptClass->AddInternalCall("Internal_setborder", (void*)&ScriptGUIElementStyle::InternalSetborder);
		metaData.scriptClass->AddInternalCall("Internal_getmargins", (void*)&ScriptGUIElementStyle::InternalGetmargins);
		metaData.scriptClass->AddInternalCall("Internal_setmargins", (void*)&ScriptGUIElementStyle::InternalSetmargins);
		metaData.scriptClass->AddInternalCall("Internal_getcontentOffset", (void*)&ScriptGUIElementStyle::InternalGetcontentOffset);
		metaData.scriptClass->AddInternalCall("Internal_setcontentOffset", (void*)&ScriptGUIElementStyle::InternalSetcontentOffset);
		metaData.scriptClass->AddInternalCall("Internal_getpadding", (void*)&ScriptGUIElementStyle::InternalGetpadding);
		metaData.scriptClass->AddInternalCall("Internal_setpadding", (void*)&ScriptGUIElementStyle::InternalSetpadding);
		metaData.scriptClass->AddInternalCall("Internal_getwidth", (void*)&ScriptGUIElementStyle::InternalGetwidth);
		metaData.scriptClass->AddInternalCall("Internal_setwidth", (void*)&ScriptGUIElementStyle::InternalSetwidth);
		metaData.scriptClass->AddInternalCall("Internal_getheight", (void*)&ScriptGUIElementStyle::InternalGetheight);
		metaData.scriptClass->AddInternalCall("Internal_setheight", (void*)&ScriptGUIElementStyle::InternalSetheight);
		metaData.scriptClass->AddInternalCall("Internal_getminWidth", (void*)&ScriptGUIElementStyle::InternalGetminWidth);
		metaData.scriptClass->AddInternalCall("Internal_setminWidth", (void*)&ScriptGUIElementStyle::InternalSetminWidth);
		metaData.scriptClass->AddInternalCall("Internal_getmaxWidth", (void*)&ScriptGUIElementStyle::InternalGetmaxWidth);
		metaData.scriptClass->AddInternalCall("Internal_setmaxWidth", (void*)&ScriptGUIElementStyle::InternalSetmaxWidth);
		metaData.scriptClass->AddInternalCall("Internal_getminHeight", (void*)&ScriptGUIElementStyle::InternalGetminHeight);
		metaData.scriptClass->AddInternalCall("Internal_setminHeight", (void*)&ScriptGUIElementStyle::InternalSetminHeight);
		metaData.scriptClass->AddInternalCall("Internal_getmaxHeight", (void*)&ScriptGUIElementStyle::InternalGetmaxHeight);
		metaData.scriptClass->AddInternalCall("Internal_setmaxHeight", (void*)&ScriptGUIElementStyle::InternalSetmaxHeight);
		metaData.scriptClass->AddInternalCall("Internal_getfixedWidth", (void*)&ScriptGUIElementStyle::InternalGetfixedWidth);
		metaData.scriptClass->AddInternalCall("Internal_setfixedWidth", (void*)&ScriptGUIElementStyle::InternalSetfixedWidth);
		metaData.scriptClass->AddInternalCall("Internal_getfixedHeight", (void*)&ScriptGUIElementStyle::InternalGetfixedHeight);
		metaData.scriptClass->AddInternalCall("Internal_setfixedHeight", (void*)&ScriptGUIElementStyle::InternalSetfixedHeight);

	}

	MonoObject* ScriptGUIElementStyle::Create(const SPtr<GUIElementStyle>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptGUIElementStyle>()) ScriptGUIElementStyle(managedInstance, value);
		return managedInstance;
	}
	void ScriptGUIElementStyle::InternalGuiElementStyle(MonoObject* managedInstance)
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
		thisPtr->GetInternal()->addSubStyle(tmpguiType, tmpstyleName);
	}

	MonoObject* ScriptGUIElementStyle::InternalGetfont(ScriptGUIElementStyle* thisPtr)
	{
		ResourceHandle<Font> tmp__output;
		tmp__output = thisPtr->GetInternal()->font;

		MonoObject* __output;
		ScriptResourceBase* script__output;
		script__output = ScriptResourceManager::Instance().getScriptResource(tmp__output, true);
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
		scriptvalue = ScriptFont::toNative(value);
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
		interop__output = ScriptGUIElementStateStyle::toInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptGUIElementStateStyle::getMetaData()->scriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetnormal(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::fromInterop(*value);
		thisPtr->GetInternal()->normal = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGethover(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->hover;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::toInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptGUIElementStateStyle::getMetaData()->scriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSethover(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::fromInterop(*value);
		thisPtr->GetInternal()->hover = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGetactive(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->active;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::toInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptGUIElementStateStyle::getMetaData()->scriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetactive(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::fromInterop(*value);
		thisPtr->GetInternal()->active = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGetfocused(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->focused;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::toInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptGUIElementStateStyle::getMetaData()->scriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetfocused(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::fromInterop(*value);
		thisPtr->GetInternal()->focused = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGetfocusedHover(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->focusedHover;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::toInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptGUIElementStateStyle::getMetaData()->scriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetfocusedHover(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::fromInterop(*value);
		thisPtr->GetInternal()->focusedHover = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGetnormalOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->normalOn;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::toInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptGUIElementStateStyle::getMetaData()->scriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetnormalOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::fromInterop(*value);
		thisPtr->GetInternal()->normalOn = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGethoverOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->hoverOn;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::toInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptGUIElementStateStyle::getMetaData()->scriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSethoverOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::fromInterop(*value);
		thisPtr->GetInternal()->hoverOn = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGetactiveOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->activeOn;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::toInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptGUIElementStateStyle::getMetaData()->scriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetactiveOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::fromInterop(*value);
		thisPtr->GetInternal()->activeOn = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGetfocusedOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->focusedOn;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::toInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptGUIElementStateStyle::getMetaData()->scriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetfocusedOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::fromInterop(*value);
		thisPtr->GetInternal()->focusedOn = tmpvalue;
	}

	void ScriptGUIElementStyle::InternalGetfocusedHoverOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* __output)
	{
		GUIElementStateStyle tmp__output;
		tmp__output = thisPtr->GetInternal()->focusedHoverOn;

		__GUIElementStateStyleInterop interop__output;
		interop__output = ScriptGUIElementStateStyle::toInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptGUIElementStateStyle::getMetaData()->scriptClass->GetInternalClassInternal());


	}

	void ScriptGUIElementStyle::InternalSetfocusedHoverOn(ScriptGUIElementStyle* thisPtr, __GUIElementStateStyleInterop* value)
	{
		GUIElementStateStyle tmpvalue;
		tmpvalue = ScriptGUIElementStateStyle::fromInterop(*value);
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

	bool ScriptGUIElementStyle::InternalGetFixedWidth(ScriptGUIElementStyle* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->fixedWidth;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetFixedWidth(ScriptGUIElementStyle* thisPtr, bool value)
	{
		thisPtr->GetInternal()->fixedWidth = value;
	}

	bool ScriptGUIElementStyle::InternalGetFixedHeight(ScriptGUIElementStyle* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->fixedHeight;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptGUIElementStyle::InternalSetFixedHeight(ScriptGUIElementStyle* thisPtr, bool value)
	{
		thisPtr->GetInternal()->fixedHeight = value;
	}
}
