//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/BsScriptVirtualButton.h"
#include "BsMonoManager.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptVirtualButton::ScriptVirtualButton(MonoObject* instance)
		:ScriptObject(instance)
	{ }

	void ScriptVirtualButton::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_InitVirtualButton", (void*)&ScriptVirtualButton::InternalInitVirtualButton);
	}

	UINT32 ScriptVirtualButton::InternalInitVirtualButton(MonoString* name)
	{
		String nameStr = MonoUtil::MonoToString(name);

		VirtualButton vb(nameStr);
		return vb.buttonIdentifier;
	}

	MonoObject* ScriptVirtualButton::Box(const VirtualButton& value)
	{
		// We're casting away const but it's fine since structs are passed by value anyway
		return MonoUtil::Box(metaData.scriptClass->GetInternalClassInternal(), (void*)&value);
	}

	VirtualButton ScriptVirtualButton::Unbox(MonoObject* obj)
	{
		return *(VirtualButton*)MonoUtil::Unbox(obj);
	}
}
