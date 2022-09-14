//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptPixelVolume.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptPixelVolume::ScriptPixelVolume(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptPixelVolume::initRuntimeData()
	{ }

	MonoObject*ScriptPixelVolume::Box(const PixelVolume& value)
	{
		return MonoUtil::Box(metaData.scriptClass->GetInternalClassInternal(), (void*)&value);
	}

	PixelVolume ScriptPixelVolume::Unbox(MonoObject* value)
	{
		return *(PixelVolume*)MonoUtil::Unbox(value);
	}

}
