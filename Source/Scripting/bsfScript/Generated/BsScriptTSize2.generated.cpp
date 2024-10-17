//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptTSize2.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptSize2::ScriptSize2()
	{ }

	MonoObject* ScriptSize2::Box(const TSize2<float>& value)
	{
		return MonoUtil::Box(sInteropMetaData.ScriptClass->GetInternalClass(), (void*)&value);
	}

	TSize2<float> ScriptSize2::Unbox(MonoObject* value)
	{
		return *(TSize2<float>*)MonoUtil::Unbox(value);
	}


	ScriptSize2UI::ScriptSize2UI()
	{ }

	MonoObject* ScriptSize2UI::Box(const TSize2<uint32_t>& value)
	{
		return MonoUtil::Box(sInteropMetaData.ScriptClass->GetInternalClass(), (void*)&value);
	}

	TSize2<uint32_t> ScriptSize2UI::Unbox(MonoObject* value)
	{
		return *(TSize2<uint32_t>*)MonoUtil::Unbox(value);
	}

}
