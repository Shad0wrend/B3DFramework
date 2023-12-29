//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptGUIInputBoxContent.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptGUIInputBoxContent::ScriptGUIInputBoxContent(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptGUIInputBoxContent::InitRuntimeData()
	{ }

	MonoObject*ScriptGUIInputBoxContent::Box(const GUIInputBoxContent& value)
	{
		return MonoUtil::Box(metaData.ScriptClass->GetInternalClassInternal(), (void*)&value);
	}

	GUIInputBoxContent ScriptGUIInputBoxContent::Unbox(MonoObject* value)
	{
		return *(GUIInputBoxContent*)MonoUtil::Unbox(value);
	}

}
