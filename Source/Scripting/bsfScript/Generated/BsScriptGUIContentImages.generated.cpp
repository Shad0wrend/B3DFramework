//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptGUIContentImages.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptResourceManager.h"
#include "../../../Foundation/bsfCore/Image/BsSpriteTexture.h"
#include "BsScriptSpriteTexture.generated.h"

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
		ResourceHandle<SpriteTexture> tmpnormal;
		ScriptSpriteTexture* scriptnormal;
		scriptnormal = ScriptSpriteTexture::ToNative(value.Normal);
		if(scriptnormal != nullptr)
			tmpnormal = scriptnormal->GetHandle();
		output.Normal = tmpnormal;
		ResourceHandle<SpriteTexture> tmphover;
		ScriptSpriteTexture* scripthover;
		scripthover = ScriptSpriteTexture::ToNative(value.Hover);
		if(scripthover != nullptr)
			tmphover = scripthover->GetHandle();
		output.Hover = tmphover;
		ResourceHandle<SpriteTexture> tmpactive;
		ScriptSpriteTexture* scriptactive;
		scriptactive = ScriptSpriteTexture::ToNative(value.Active);
		if(scriptactive != nullptr)
			tmpactive = scriptactive->GetHandle();
		output.Active = tmpactive;
		ResourceHandle<SpriteTexture> tmpfocused;
		ScriptSpriteTexture* scriptfocused;
		scriptfocused = ScriptSpriteTexture::ToNative(value.Focused);
		if(scriptfocused != nullptr)
			tmpfocused = scriptfocused->GetHandle();
		output.Focused = tmpfocused;
		ResourceHandle<SpriteTexture> tmpnormalOn;
		ScriptSpriteTexture* scriptnormalOn;
		scriptnormalOn = ScriptSpriteTexture::ToNative(value.NormalOn);
		if(scriptnormalOn != nullptr)
			tmpnormalOn = scriptnormalOn->GetHandle();
		output.NormalOn = tmpnormalOn;
		ResourceHandle<SpriteTexture> tmphoverOn;
		ScriptSpriteTexture* scripthoverOn;
		scripthoverOn = ScriptSpriteTexture::ToNative(value.HoverOn);
		if(scripthoverOn != nullptr)
			tmphoverOn = scripthoverOn->GetHandle();
		output.HoverOn = tmphoverOn;
		ResourceHandle<SpriteTexture> tmpactiveOn;
		ScriptSpriteTexture* scriptactiveOn;
		scriptactiveOn = ScriptSpriteTexture::ToNative(value.ActiveOn);
		if(scriptactiveOn != nullptr)
			tmpactiveOn = scriptactiveOn->GetHandle();
		output.ActiveOn = tmpactiveOn;
		ResourceHandle<SpriteTexture> tmpfocusedOn;
		ScriptSpriteTexture* scriptfocusedOn;
		scriptfocusedOn = ScriptSpriteTexture::ToNative(value.FocusedOn);
		if(scriptfocusedOn != nullptr)
			tmpfocusedOn = scriptfocusedOn->GetHandle();
		output.FocusedOn = tmpfocusedOn;

		return output;
	}

	__GUIContentImagesInterop ScriptGUIContentImages::ToInterop(const GUIContentImages& value)
	{
		__GUIContentImagesInterop output;
		ScriptResourceBase* scriptnormal;
		scriptnormal = ScriptResourceManager::Instance().GetScriptResource(value.Normal, true);
		MonoObject* tmpnormal;
		if(scriptnormal != nullptr)
			tmpnormal = scriptnormal->GetManagedInstance();
		else
			tmpnormal = nullptr;
		output.Normal = tmpnormal;
		ScriptResourceBase* scripthover;
		scripthover = ScriptResourceManager::Instance().GetScriptResource(value.Hover, true);
		MonoObject* tmphover;
		if(scripthover != nullptr)
			tmphover = scripthover->GetManagedInstance();
		else
			tmphover = nullptr;
		output.Hover = tmphover;
		ScriptResourceBase* scriptactive;
		scriptactive = ScriptResourceManager::Instance().GetScriptResource(value.Active, true);
		MonoObject* tmpactive;
		if(scriptactive != nullptr)
			tmpactive = scriptactive->GetManagedInstance();
		else
			tmpactive = nullptr;
		output.Active = tmpactive;
		ScriptResourceBase* scriptfocused;
		scriptfocused = ScriptResourceManager::Instance().GetScriptResource(value.Focused, true);
		MonoObject* tmpfocused;
		if(scriptfocused != nullptr)
			tmpfocused = scriptfocused->GetManagedInstance();
		else
			tmpfocused = nullptr;
		output.Focused = tmpfocused;
		ScriptResourceBase* scriptnormalOn;
		scriptnormalOn = ScriptResourceManager::Instance().GetScriptResource(value.NormalOn, true);
		MonoObject* tmpnormalOn;
		if(scriptnormalOn != nullptr)
			tmpnormalOn = scriptnormalOn->GetManagedInstance();
		else
			tmpnormalOn = nullptr;
		output.NormalOn = tmpnormalOn;
		ScriptResourceBase* scripthoverOn;
		scripthoverOn = ScriptResourceManager::Instance().GetScriptResource(value.HoverOn, true);
		MonoObject* tmphoverOn;
		if(scripthoverOn != nullptr)
			tmphoverOn = scripthoverOn->GetManagedInstance();
		else
			tmphoverOn = nullptr;
		output.HoverOn = tmphoverOn;
		ScriptResourceBase* scriptactiveOn;
		scriptactiveOn = ScriptResourceManager::Instance().GetScriptResource(value.ActiveOn, true);
		MonoObject* tmpactiveOn;
		if(scriptactiveOn != nullptr)
			tmpactiveOn = scriptactiveOn->GetManagedInstance();
		else
			tmpactiveOn = nullptr;
		output.ActiveOn = tmpactiveOn;
		ScriptResourceBase* scriptfocusedOn;
		scriptfocusedOn = ScriptResourceManager::Instance().GetScriptResource(value.FocusedOn, true);
		MonoObject* tmpfocusedOn;
		if(scriptfocusedOn != nullptr)
			tmpfocusedOn = scriptfocusedOn->GetManagedInstance();
		else
			tmpfocusedOn = nullptr;
		output.FocusedOn = tmpfocusedOn;

		return output;
	}

}
