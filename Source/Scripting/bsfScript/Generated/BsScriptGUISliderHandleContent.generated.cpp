//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptGUISliderHandleContent.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptGUISliderHandleContent::ScriptGUISliderHandleContent(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptGUISliderHandleContent::InitRuntimeData()
	{ }

	MonoObject*ScriptGUISliderHandleContent::Box(const GUISliderHandleContent& value)
	{
		return MonoUtil::Box(metaData.ScriptClass->GetInternalClass(), (void*)&value);
	}

	GUISliderHandleContent ScriptGUISliderHandleContent::Unbox(MonoObject* value)
	{
		return *(GUISliderHandleContent*)MonoUtil::Unbox(value);
	}

}
