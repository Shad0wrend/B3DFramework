//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptGUIContentImages.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptResourceManager.h"
#include "../../../Foundation/bsfCore/Image/BsSpriteImage.h"
#include "BsScriptSpriteImage.generated.h"

namespace bs
{
	ScriptGUIContentImages::ScriptGUIContentImages(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptGUIContentImages::InitRuntimeData()
	{ }

	MonoObject*ScriptGUIContentImages::Box(const __GUIContentImagesInterop& value)
	{
		return MonoUtil::Box(metaData.ScriptClass->GetInternalClassInternal(), (void*)&value);
	}

	__GUIContentImagesInterop ScriptGUIContentImages::Unbox(MonoObject* value)
	{
		return *(__GUIContentImagesInterop*)MonoUtil::Unbox(value);
	}

	GUIContentImages ScriptGUIContentImages::FromInterop(const __GUIContentImagesInterop& value)
	{
		GUIContentImages output;
		TResourceHandle<SpriteImage> tmpNormal;
		ScriptSpriteImageBase* scriptWrapperObjectNormal;
		scriptWrapperObjectNormal = (ScriptSpriteImageBase*)ScriptSpriteImage::ToNative(value.Normal);
		if(scriptWrapperObjectNormal != nullptr)
			tmpNormal = B3DStaticResourceCast<SpriteImage>(scriptWrapperObjectNormal->GetGenericHandle());
		output.Normal = tmpNormal;
		TResourceHandle<SpriteImage> tmpHover;
		ScriptSpriteImageBase* scriptWrapperObjectHover;
		scriptWrapperObjectHover = (ScriptSpriteImageBase*)ScriptSpriteImage::ToNative(value.Hover);
		if(scriptWrapperObjectHover != nullptr)
			tmpHover = B3DStaticResourceCast<SpriteImage>(scriptWrapperObjectHover->GetGenericHandle());
		output.Hover = tmpHover;
		TResourceHandle<SpriteImage> tmpActive;
		ScriptSpriteImageBase* scriptWrapperObjectActive;
		scriptWrapperObjectActive = (ScriptSpriteImageBase*)ScriptSpriteImage::ToNative(value.Active);
		if(scriptWrapperObjectActive != nullptr)
			tmpActive = B3DStaticResourceCast<SpriteImage>(scriptWrapperObjectActive->GetGenericHandle());
		output.Active = tmpActive;
		TResourceHandle<SpriteImage> tmpFocused;
		ScriptSpriteImageBase* scriptWrapperObjectFocused;
		scriptWrapperObjectFocused = (ScriptSpriteImageBase*)ScriptSpriteImage::ToNative(value.Focused);
		if(scriptWrapperObjectFocused != nullptr)
			tmpFocused = B3DStaticResourceCast<SpriteImage>(scriptWrapperObjectFocused->GetGenericHandle());
		output.Focused = tmpFocused;
		TResourceHandle<SpriteImage> tmpNormalOn;
		ScriptSpriteImageBase* scriptWrapperObjectNormalOn;
		scriptWrapperObjectNormalOn = (ScriptSpriteImageBase*)ScriptSpriteImage::ToNative(value.NormalOn);
		if(scriptWrapperObjectNormalOn != nullptr)
			tmpNormalOn = B3DStaticResourceCast<SpriteImage>(scriptWrapperObjectNormalOn->GetGenericHandle());
		output.NormalOn = tmpNormalOn;
		TResourceHandle<SpriteImage> tmpHoverOn;
		ScriptSpriteImageBase* scriptWrapperObjectHoverOn;
		scriptWrapperObjectHoverOn = (ScriptSpriteImageBase*)ScriptSpriteImage::ToNative(value.HoverOn);
		if(scriptWrapperObjectHoverOn != nullptr)
			tmpHoverOn = B3DStaticResourceCast<SpriteImage>(scriptWrapperObjectHoverOn->GetGenericHandle());
		output.HoverOn = tmpHoverOn;
		TResourceHandle<SpriteImage> tmpActiveOn;
		ScriptSpriteImageBase* scriptWrapperObjectActiveOn;
		scriptWrapperObjectActiveOn = (ScriptSpriteImageBase*)ScriptSpriteImage::ToNative(value.ActiveOn);
		if(scriptWrapperObjectActiveOn != nullptr)
			tmpActiveOn = B3DStaticResourceCast<SpriteImage>(scriptWrapperObjectActiveOn->GetGenericHandle());
		output.ActiveOn = tmpActiveOn;
		TResourceHandle<SpriteImage> tmpFocusedOn;
		ScriptSpriteImageBase* scriptWrapperObjectFocusedOn;
		scriptWrapperObjectFocusedOn = (ScriptSpriteImageBase*)ScriptSpriteImage::ToNative(value.FocusedOn);
		if(scriptWrapperObjectFocusedOn != nullptr)
			tmpFocusedOn = B3DStaticResourceCast<SpriteImage>(scriptWrapperObjectFocusedOn->GetGenericHandle());
		output.FocusedOn = tmpFocusedOn;

		return output;
	}

	__GUIContentImagesInterop ScriptGUIContentImages::ToInterop(const GUIContentImages& value)
	{
		__GUIContentImagesInterop output;
		MonoObject* tmpNormal;
		ScriptResourceBase* scriptWrapperObjectNormal;
		scriptWrapperObjectNormal = ScriptResourceManager::Instance().GetScriptResource(value.Normal, true);
		if(scriptWrapperObjectNormal != nullptr)
			tmpNormal = scriptWrapperObjectNormal->GetManagedInstance();
		else
			tmpNormal = nullptr;
		output.Normal = tmpNormal;
		MonoObject* tmpHover;
		ScriptResourceBase* scriptWrapperObjectHover;
		scriptWrapperObjectHover = ScriptResourceManager::Instance().GetScriptResource(value.Hover, true);
		if(scriptWrapperObjectHover != nullptr)
			tmpHover = scriptWrapperObjectHover->GetManagedInstance();
		else
			tmpHover = nullptr;
		output.Hover = tmpHover;
		MonoObject* tmpActive;
		ScriptResourceBase* scriptWrapperObjectActive;
		scriptWrapperObjectActive = ScriptResourceManager::Instance().GetScriptResource(value.Active, true);
		if(scriptWrapperObjectActive != nullptr)
			tmpActive = scriptWrapperObjectActive->GetManagedInstance();
		else
			tmpActive = nullptr;
		output.Active = tmpActive;
		MonoObject* tmpFocused;
		ScriptResourceBase* scriptWrapperObjectFocused;
		scriptWrapperObjectFocused = ScriptResourceManager::Instance().GetScriptResource(value.Focused, true);
		if(scriptWrapperObjectFocused != nullptr)
			tmpFocused = scriptWrapperObjectFocused->GetManagedInstance();
		else
			tmpFocused = nullptr;
		output.Focused = tmpFocused;
		MonoObject* tmpNormalOn;
		ScriptResourceBase* scriptWrapperObjectNormalOn;
		scriptWrapperObjectNormalOn = ScriptResourceManager::Instance().GetScriptResource(value.NormalOn, true);
		if(scriptWrapperObjectNormalOn != nullptr)
			tmpNormalOn = scriptWrapperObjectNormalOn->GetManagedInstance();
		else
			tmpNormalOn = nullptr;
		output.NormalOn = tmpNormalOn;
		MonoObject* tmpHoverOn;
		ScriptResourceBase* scriptWrapperObjectHoverOn;
		scriptWrapperObjectHoverOn = ScriptResourceManager::Instance().GetScriptResource(value.HoverOn, true);
		if(scriptWrapperObjectHoverOn != nullptr)
			tmpHoverOn = scriptWrapperObjectHoverOn->GetManagedInstance();
		else
			tmpHoverOn = nullptr;
		output.HoverOn = tmpHoverOn;
		MonoObject* tmpActiveOn;
		ScriptResourceBase* scriptWrapperObjectActiveOn;
		scriptWrapperObjectActiveOn = ScriptResourceManager::Instance().GetScriptResource(value.ActiveOn, true);
		if(scriptWrapperObjectActiveOn != nullptr)
			tmpActiveOn = scriptWrapperObjectActiveOn->GetManagedInstance();
		else
			tmpActiveOn = nullptr;
		output.ActiveOn = tmpActiveOn;
		MonoObject* tmpFocusedOn;
		ScriptResourceBase* scriptWrapperObjectFocusedOn;
		scriptWrapperObjectFocusedOn = ScriptResourceManager::Instance().GetScriptResource(value.FocusedOn, true);
		if(scriptWrapperObjectFocusedOn != nullptr)
			tmpFocusedOn = scriptWrapperObjectFocusedOn->GetManagedInstance();
		else
			tmpFocusedOn = nullptr;
		output.FocusedOn = tmpFocusedOn;

		return output;
	}

}
