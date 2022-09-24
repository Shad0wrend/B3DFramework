//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptBlend1DInfo.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Animation/BsAnimation.h"
#include "BsScriptBlendClipInfo.generated.h"

namespace bs
{
	ScriptBlend1DInfo::ScriptBlend1DInfo(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptBlend1DInfo::InitRuntimeData()
	{ }

	MonoObject*ScriptBlend1DInfo::Box(const __Blend1DInfoInterop& value)
	{
		return MonoUtil::Box(metaData.ScriptClass->GetInternalClassInternal(), (void*)&value);
	}

	__Blend1DInfoInterop ScriptBlend1DInfo::Unbox(MonoObject* value)
	{
		return *(__Blend1DInfoInterop*)MonoUtil::Unbox(value);
	}

	Blend1DInfo ScriptBlend1DInfo::FromInterop(const __Blend1DInfoInterop& value)
	{
		Blend1DInfo output;
		Vector<BlendClipInfo> vecclips;
		if(value.Clips != nullptr)
		{
			ScriptArray arrayclips(value.Clips);
			vecclips.resize(arrayclips.Size());
			for(int i = 0; i < (int)arrayclips.Size(); i++)
			{
				vecclips[i] = ScriptBlendClipInfo::FromInterop(arrayclips.Get<__BlendClipInfoInterop>(i));
			}
		}
		output.Clips = vecclips;

		return output;
	}

	__Blend1DInfoInterop ScriptBlend1DInfo::ToInterop(const Blend1DInfo& value)
	{
		__Blend1DInfoInterop output;
		int arraySizeclips = (int)value.Clips.size();
		MonoArray* vecclips;
		ScriptArray arrayclips = ScriptArray::Create<ScriptBlendClipInfo>(arraySizeclips);
		for(int i = 0; i < arraySizeclips; i++)
		{
			arrayclips.Set(i, ScriptBlendClipInfo::ToInterop(value.Clips[i]));
		}
		vecclips = arrayclips.GetInternal();
		output.Clips = vecclips;

		return output;
	}

}
