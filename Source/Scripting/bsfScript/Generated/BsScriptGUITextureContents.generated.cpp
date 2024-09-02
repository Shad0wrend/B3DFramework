//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptGUITextureContents.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
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
		ScriptSpriteImageWrapperBase* scriptWrapperObjectImage;
		scriptWrapperObjectImage = (ScriptSpriteImageWrapperBase*)ScriptSpriteImage::GetScriptObjectWrapper(value.Image);
		if(scriptWrapperObjectImage != nullptr)
			tmpImage = B3DStaticResourceCast<SpriteImage>(scriptWrapperObjectImage->GetBaseNativeObjectAsHandle());
		output.Image = tmpImage;
		output.ScaleMode = value.ScaleMode;
		output.IsTransparent = value.IsTransparent;

		return output;
	}

	__GUITextureContentsInterop ScriptGUITextureContents::ToInterop(const GUITextureContents& value)
	{
		__GUITextureContentsInterop output;
		MonoObject* tmpImage;
		MonoObject* temptmpImage = nullptr;
		if(value.Image)
		temptmpImage = ScriptResourceWrapper::GetOrCreateScriptObject(value.Image);
		tmpImage = temptmpImage;
		output.Image = tmpImage;
		output.ScaleMode = value.ScaleMode;
		output.IsTransparent = value.IsTransparent;

		return output;
	}

}
