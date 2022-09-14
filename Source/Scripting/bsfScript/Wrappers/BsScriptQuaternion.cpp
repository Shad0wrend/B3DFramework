//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/BsScriptQuaternion.h"
#include "BsMonoManager.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptQuaternion::ScriptQuaternion(MonoObject* instance)
		:ScriptObject(instance)
	{ }

	void ScriptQuaternion::initRuntimeData()
	{ }

	MonoObject* ScriptQuaternion::Box(const Quaternion& value)
	{
		// We're casting away const but it's fine since structs are passed by value anyway
		return MonoUtil::Box(metaData.scriptClass->GetInternalClassInternal(), (void*)&value);
	}

	Quaternion ScriptQuaternion::Unbox(MonoObject* obj)
	{
		return *(Quaternion*)MonoUtil::Unbox(obj);
	}
}
