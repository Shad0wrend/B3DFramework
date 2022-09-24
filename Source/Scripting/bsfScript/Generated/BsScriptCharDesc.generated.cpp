//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptCharDesc.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Text/BsFontDesc.h"
#include "BsScriptKerningPair.generated.h"

namespace bs
{
	ScriptCharDesc::ScriptCharDesc(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptCharDesc::InitRuntimeData()
	{ }

	MonoObject*ScriptCharDesc::Box(const __CharDescInterop& value)
	{
		return MonoUtil::Box(metaData.ScriptClass->GetInternalClassInternal(), (void*)&value);
	}

	__CharDescInterop ScriptCharDesc::Unbox(MonoObject* value)
	{
		return *(__CharDescInterop*)MonoUtil::Unbox(value);
	}

	CharDesc ScriptCharDesc::FromInterop(const __CharDescInterop& value)
	{
		CharDesc output;
		output.CharId = value.CharId;
		output.Page = value.Page;
		output.UvX = value.UvX;
		output.UvY = value.UvY;
		output.UvWidth = value.UvWidth;
		output.UvHeight = value.UvHeight;
		output.Width = value.Width;
		output.Height = value.Height;
		output.XOffset = value.XOffset;
		output.YOffset = value.YOffset;
		output.XAdvance = value.XAdvance;
		output.YAdvance = value.YAdvance;
		Vector<KerningPair> veckerningPairs;
		if(value.KerningPairs != nullptr)
		{
			ScriptArray arraykerningPairs(value.KerningPairs);
			veckerningPairs.resize(arraykerningPairs.Size());
			for(int i = 0; i < (int)arraykerningPairs.Size(); i++)
			{
				veckerningPairs[i] = arraykerningPairs.Get<KerningPair>(i);
			}
		}
		output.KerningPairs = veckerningPairs;

		return output;
	}

	__CharDescInterop ScriptCharDesc::ToInterop(const CharDesc& value)
	{
		__CharDescInterop output;
		output.CharId = value.CharId;
		output.Page = value.Page;
		output.UvX = value.UvX;
		output.UvY = value.UvY;
		output.UvWidth = value.UvWidth;
		output.UvHeight = value.UvHeight;
		output.Width = value.Width;
		output.Height = value.Height;
		output.XOffset = value.XOffset;
		output.YOffset = value.YOffset;
		output.XAdvance = value.XAdvance;
		output.YAdvance = value.YAdvance;
		int arraySizekerningPairs = (int)value.KerningPairs.size();
		MonoArray* veckerningPairs;
		ScriptArray arraykerningPairs = ScriptArray::Create<ScriptKerningPair>(arraySizekerningPairs);
		for(int i = 0; i < arraySizekerningPairs; i++)
		{
			arraykerningPairs.Set(i, value.KerningPairs[i]);
		}
		veckerningPairs = arraykerningPairs.GetInternal();
		output.KerningPairs = veckerningPairs;

		return output;
	}

}
