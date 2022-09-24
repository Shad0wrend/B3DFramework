//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptGUIContent.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Localization/BsHString.h"
#include "BsScriptHString.generated.h"
#include "../../../Foundation/bsfEngine/GUI/BsGUIContent.h"
#include "BsScriptGUIContentImages.generated.h"

namespace bs
{
	ScriptGUIContent::ScriptGUIContent(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptGUIContent::InitRuntimeData()
	{ }

	MonoObject*ScriptGUIContent::Box(const __GUIContentInterop& value)
	{
		return MonoUtil::Box(metaData.ScriptClass->GetInternalClassInternal(), (void*)&value);
	}

	__GUIContentInterop ScriptGUIContent::Unbox(MonoObject* value)
	{
		return *(__GUIContentInterop*)MonoUtil::Unbox(value);
	}

	GUIContent ScriptGUIContent::FromInterop(const __GUIContentInterop& value)
	{
		GUIContent output;
		SPtr<HString> tmptext;
		ScriptHString* scripttext;
		scripttext = ScriptHString::ToNative(value.Text);
		if(scripttext != nullptr)
			tmptext = scripttext->GetInternal();
		if(tmptext != nullptr)
		output.Text = *tmptext;
		GUIContentImages tmpimages;
		tmpimages = ScriptGUIContentImages::FromInterop(value.Images);
		output.Images = tmpimages;
		SPtr<HString> tmptooltip;
		ScriptHString* scripttooltip;
		scripttooltip = ScriptHString::ToNative(value.Tooltip);
		if(scripttooltip != nullptr)
			tmptooltip = scripttooltip->GetInternal();
		if(tmptooltip != nullptr)
		output.Tooltip = *tmptooltip;

		return output;
	}

	__GUIContentInterop ScriptGUIContent::ToInterop(const GUIContent& value)
	{
		__GUIContentInterop output;
		MonoObject* tmptext;
		SPtr<HString> tmptextcopy;
		tmptextcopy = bs_shared_ptr_new<HString>(value.Text);
		tmptext = ScriptHString::Create(tmptextcopy);
		output.Text = tmptext;
		__GUIContentImagesInterop tmpimages;
		tmpimages = ScriptGUIContentImages::ToInterop(value.Images);
		output.Images = tmpimages;
		MonoObject* tmptooltip;
		SPtr<HString> tmptooltipcopy;
		tmptooltipcopy = bs_shared_ptr_new<HString>(value.Tooltip);
		tmptooltip = ScriptHString::Create(tmptooltipcopy);
		output.Tooltip = tmptooltip;

		return output;
	}

}
