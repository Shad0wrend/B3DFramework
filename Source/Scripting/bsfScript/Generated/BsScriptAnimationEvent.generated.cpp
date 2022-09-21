//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptAnimationEvent.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptAnimationEvent::ScriptAnimationEvent(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptAnimationEvent::InitRuntimeData()
	{ }

	MonoObject*ScriptAnimationEvent::Box(const __AnimationEventInterop& value)
	{
		return MonoUtil::Box(metaData.scriptClass->GetInternalClassInternal(), (void*)&value);
	}

	__AnimationEventInterop ScriptAnimationEvent::Unbox(MonoObject* value)
	{
		return *(__AnimationEventInterop*)MonoUtil::Unbox(value);
	}

	AnimationEvent ScriptAnimationEvent::FromInterop(const __AnimationEventInterop& value)
	{
		AnimationEvent output;
		String tmpname;
		tmpname = MonoUtil::MonoToString(value.name);
		output.name = tmpname;
		output.time = value.time;

		return output;
	}

	__AnimationEventInterop ScriptAnimationEvent::ToInterop(const AnimationEvent& value)
	{
		__AnimationEventInterop output;
		MonoString* tmpname;
		tmpname = MonoUtil::StringToMono(value.name);
		output.name = tmpname;
		output.time = value.time;

		return output;
	}

}
