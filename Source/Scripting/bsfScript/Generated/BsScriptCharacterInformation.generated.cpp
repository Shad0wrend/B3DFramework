//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptCharacterInformation.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Text/BsFont.h"
#include "BsScriptKerningPair.generated.h"

namespace bs
{
	ScriptCharacterInformation::ScriptCharacterInformation(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptCharacterInformation::InitRuntimeData()
	{ }

	MonoObject*ScriptCharacterInformation::Box(const __CharacterInformationInterop& value)
	{
		return MonoUtil::Box(metaData.ScriptClass->GetInternalClassInternal(), (void*)&value);
	}

	__CharacterInformationInterop ScriptCharacterInformation::Unbox(MonoObject* value)
	{
		return *(__CharacterInformationInterop*)MonoUtil::Unbox(value);
	}

	CharacterInformation ScriptCharacterInformation::FromInterop(const __CharacterInformationInterop& value)
	{
		CharacterInformation output;
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
		Vector<KerningPair> vecKerningPairs;
		if(value.KerningPairs != nullptr)
		{
			ScriptArray arrayKerningPairs(value.KerningPairs);
			vecKerningPairs.resize(arrayKerningPairs.Size());
			for(int i = 0; i < (int)arrayKerningPairs.Size(); i++)
			{
				vecKerningPairs[i] = arrayKerningPairs.Get<KerningPair>(i);
			}
		}
		output.KerningPairs = vecKerningPairs;

		return output;
	}

	__CharacterInformationInterop ScriptCharacterInformation::ToInterop(const CharacterInformation& value)
	{
		__CharacterInformationInterop output;
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
		int arraySizeKerningPairs = (int)value.KerningPairs.size();
		MonoArray* vecKerningPairs;
		ScriptArray arrayKerningPairs = ScriptArray::Create<ScriptKerningPair>(arraySizeKerningPairs);
		for(int i = 0; i < arraySizeKerningPairs; i++)
		{
			arrayKerningPairs.Set(i, value.KerningPairs[i]);
		}
		vecKerningPairs = arrayKerningPairs.GetInternal();
		output.KerningPairs = vecKerningPairs;

		return output;
	}

}
