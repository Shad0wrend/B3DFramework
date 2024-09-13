//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptGUIContentImages.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Image/BsSpriteImage.h"
#include "BsScriptSpriteImage.generated.h"

namespace bs
{
	ScriptGUIContentImages::ScriptGUIContentImages()
	{ }

	MonoObject* ScriptGUIContentImages::Box(const __GUIContentImagesInterop& value)
	{
		return MonoUtil::Box(sInteropMetaData.ScriptClass->GetInternalClass(), (void*)&value);
	}

	__GUIContentImagesInterop ScriptGUIContentImages::Unbox(MonoObject* value)
	{
		return *(__GUIContentImagesInterop*)MonoUtil::Unbox(value);
	}

	GUIContentImages ScriptGUIContentImages::FromInterop(const __GUIContentImagesInterop& value)
	{
		GUIContentImages output;
		TResourceHandle<SpriteImage> tmpNormal;
		ScriptSpriteImageWrapperBase* scriptWrapperObjectNormal;
		scriptWrapperObjectNormal = (ScriptSpriteImageWrapperBase*)ScriptSpriteImage::GetScriptObjectWrapper(value.Normal);
		if(scriptWrapperObjectNormal != nullptr)
			tmpNormal = B3DStaticResourceCast<SpriteImage>(scriptWrapperObjectNormal->GetBaseNativeObjectAsHandle());
		output.Normal = tmpNormal;
		TResourceHandle<SpriteImage> tmpHover;
		ScriptSpriteImageWrapperBase* scriptWrapperObjectHover;
		scriptWrapperObjectHover = (ScriptSpriteImageWrapperBase*)ScriptSpriteImage::GetScriptObjectWrapper(value.Hover);
		if(scriptWrapperObjectHover != nullptr)
			tmpHover = B3DStaticResourceCast<SpriteImage>(scriptWrapperObjectHover->GetBaseNativeObjectAsHandle());
		output.Hover = tmpHover;
		TResourceHandle<SpriteImage> tmpActive;
		ScriptSpriteImageWrapperBase* scriptWrapperObjectActive;
		scriptWrapperObjectActive = (ScriptSpriteImageWrapperBase*)ScriptSpriteImage::GetScriptObjectWrapper(value.Active);
		if(scriptWrapperObjectActive != nullptr)
			tmpActive = B3DStaticResourceCast<SpriteImage>(scriptWrapperObjectActive->GetBaseNativeObjectAsHandle());
		output.Active = tmpActive;
		TResourceHandle<SpriteImage> tmpFocused;
		ScriptSpriteImageWrapperBase* scriptWrapperObjectFocused;
		scriptWrapperObjectFocused = (ScriptSpriteImageWrapperBase*)ScriptSpriteImage::GetScriptObjectWrapper(value.Focused);
		if(scriptWrapperObjectFocused != nullptr)
			tmpFocused = B3DStaticResourceCast<SpriteImage>(scriptWrapperObjectFocused->GetBaseNativeObjectAsHandle());
		output.Focused = tmpFocused;
		TResourceHandle<SpriteImage> tmpNormalOn;
		ScriptSpriteImageWrapperBase* scriptWrapperObjectNormalOn;
		scriptWrapperObjectNormalOn = (ScriptSpriteImageWrapperBase*)ScriptSpriteImage::GetScriptObjectWrapper(value.NormalOn);
		if(scriptWrapperObjectNormalOn != nullptr)
			tmpNormalOn = B3DStaticResourceCast<SpriteImage>(scriptWrapperObjectNormalOn->GetBaseNativeObjectAsHandle());
		output.NormalOn = tmpNormalOn;
		TResourceHandle<SpriteImage> tmpHoverOn;
		ScriptSpriteImageWrapperBase* scriptWrapperObjectHoverOn;
		scriptWrapperObjectHoverOn = (ScriptSpriteImageWrapperBase*)ScriptSpriteImage::GetScriptObjectWrapper(value.HoverOn);
		if(scriptWrapperObjectHoverOn != nullptr)
			tmpHoverOn = B3DStaticResourceCast<SpriteImage>(scriptWrapperObjectHoverOn->GetBaseNativeObjectAsHandle());
		output.HoverOn = tmpHoverOn;
		TResourceHandle<SpriteImage> tmpActiveOn;
		ScriptSpriteImageWrapperBase* scriptWrapperObjectActiveOn;
		scriptWrapperObjectActiveOn = (ScriptSpriteImageWrapperBase*)ScriptSpriteImage::GetScriptObjectWrapper(value.ActiveOn);
		if(scriptWrapperObjectActiveOn != nullptr)
			tmpActiveOn = B3DStaticResourceCast<SpriteImage>(scriptWrapperObjectActiveOn->GetBaseNativeObjectAsHandle());
		output.ActiveOn = tmpActiveOn;
		TResourceHandle<SpriteImage> tmpFocusedOn;
		ScriptSpriteImageWrapperBase* scriptWrapperObjectFocusedOn;
		scriptWrapperObjectFocusedOn = (ScriptSpriteImageWrapperBase*)ScriptSpriteImage::GetScriptObjectWrapper(value.FocusedOn);
		if(scriptWrapperObjectFocusedOn != nullptr)
			tmpFocusedOn = B3DStaticResourceCast<SpriteImage>(scriptWrapperObjectFocusedOn->GetBaseNativeObjectAsHandle());
		output.FocusedOn = tmpFocusedOn;

		return output;
	}

	__GUIContentImagesInterop ScriptGUIContentImages::ToInterop(const GUIContentImages& value)
	{
		__GUIContentImagesInterop output;
		MonoObject* tmpNormal;
		MonoObject* temptmpNormal = nullptr;
		if(value.Normal)
		temptmpNormal = ScriptResourceWrapper::GetOrCreateScriptObject(value.Normal);
		tmpNormal = temptmpNormal;
		output.Normal = tmpNormal;
		MonoObject* tmpHover;
		MonoObject* temptmpHover = nullptr;
		if(value.Hover)
		temptmpHover = ScriptResourceWrapper::GetOrCreateScriptObject(value.Hover);
		tmpHover = temptmpHover;
		output.Hover = tmpHover;
		MonoObject* tmpActive;
		MonoObject* temptmpActive = nullptr;
		if(value.Active)
		temptmpActive = ScriptResourceWrapper::GetOrCreateScriptObject(value.Active);
		tmpActive = temptmpActive;
		output.Active = tmpActive;
		MonoObject* tmpFocused;
		MonoObject* temptmpFocused = nullptr;
		if(value.Focused)
		temptmpFocused = ScriptResourceWrapper::GetOrCreateScriptObject(value.Focused);
		tmpFocused = temptmpFocused;
		output.Focused = tmpFocused;
		MonoObject* tmpNormalOn;
		MonoObject* temptmpNormalOn = nullptr;
		if(value.NormalOn)
		temptmpNormalOn = ScriptResourceWrapper::GetOrCreateScriptObject(value.NormalOn);
		tmpNormalOn = temptmpNormalOn;
		output.NormalOn = tmpNormalOn;
		MonoObject* tmpHoverOn;
		MonoObject* temptmpHoverOn = nullptr;
		if(value.HoverOn)
		temptmpHoverOn = ScriptResourceWrapper::GetOrCreateScriptObject(value.HoverOn);
		tmpHoverOn = temptmpHoverOn;
		output.HoverOn = tmpHoverOn;
		MonoObject* tmpActiveOn;
		MonoObject* temptmpActiveOn = nullptr;
		if(value.ActiveOn)
		temptmpActiveOn = ScriptResourceWrapper::GetOrCreateScriptObject(value.ActiveOn);
		tmpActiveOn = temptmpActiveOn;
		output.ActiveOn = tmpActiveOn;
		MonoObject* tmpFocusedOn;
		MonoObject* temptmpFocusedOn = nullptr;
		if(value.FocusedOn)
		temptmpFocusedOn = ScriptResourceWrapper::GetOrCreateScriptObject(value.FocusedOn);
		tmpFocusedOn = temptmpFocusedOn;
		output.FocusedOn = tmpFocusedOn;

		return output;
	}

}
