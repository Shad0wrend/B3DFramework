//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptGUITextureContents.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptResourceManager.h"
#include "../../../Foundation/bsfCore/Image/BsSpriteImage.h"
#include "BsScriptSpriteImage.generated.h"

namespace bs
{
	ScriptGUITextureContents::ScriptGUITextureContents(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptGUITextureContents::InitRuntimeData()
	{ }

	MonoObject*ScriptGUITextureContents::Box(const __GUITextureContentsInterop& value)
	{
		return MonoUtil::Box(metaData.ScriptClass->GetInternalClass(), (void*)&value);
	}

	__GUITextureContentsInterop ScriptGUITextureContents::Unbox(MonoObject* value)
	{
		return *(__GUITextureContentsInterop*)MonoUtil::Unbox(value);
	}

	GUITextureContents ScriptGUITextureContents::FromInterop(const __GUITextureContentsInterop& value)
	{
		GUITextureContents output;
		TResourceHandle<SpriteImage> tmpImage;
		ScriptSpriteImageBase* scriptWrapperObjectImage;
		scriptWrapperObjectImage = (ScriptSpriteImageBase*)ScriptSpriteImage::ToNative(value.Image);
		if(scriptWrapperObjectImage != nullptr)
			tmpImage = B3DStaticResourceCast<SpriteImage>(scriptWrapperObjectImage->GetGenericHandle());
		output.Image = tmpImage;
		output.ScaleMode = value.ScaleMode;
		output.IsTransparent = value.IsTransparent;

		return output;
	}

	__GUITextureContentsInterop ScriptGUITextureContents::ToInterop(const GUITextureContents& value)
	{
		__GUITextureContentsInterop output;
		MonoObject* tmpImage;
		ScriptResourceBase* scriptWrapperObjectImage;
		scriptWrapperObjectImage = ScriptResourceManager::Instance().GetScriptResource(value.Image, true);
		if(scriptWrapperObjectImage != nullptr)
			tmpImage = scriptWrapperObjectImage->GetManagedInstance();
		else
			tmpImage = nullptr;
		output.Image = tmpImage;
		output.ScaleMode = value.ScaleMode;
		output.IsTransparent = value.IsTransparent;

		return output;
	}

}
