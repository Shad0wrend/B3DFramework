//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptShaderVariationParamInfo.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Material/BsShader.h"
#include "BsScriptShaderVariationParamValue.generated.h"

namespace bs
{
	ScriptShaderVariationParamInfo::ScriptShaderVariationParamInfo(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptShaderVariationParamInfo::InitRuntimeData()
	{ }

	MonoObject*ScriptShaderVariationParamInfo::Box(const __ShaderVariationParamInfoInterop& value)
	{
		return MonoUtil::Box(metaData.ScriptClass->GetInternalClassInternal(), (void*)&value);
	}

	__ShaderVariationParamInfoInterop ScriptShaderVariationParamInfo::Unbox(MonoObject* value)
	{
		return *(__ShaderVariationParamInfoInterop*)MonoUtil::Unbox(value);
	}

	ShaderVariationParamInfo ScriptShaderVariationParamInfo::FromInterop(const __ShaderVariationParamInfoInterop& value)
	{
		ShaderVariationParamInfo output;
		String tmpname;
		tmpname = MonoUtil::MonoToString(value.Name);
		output.Name = tmpname;
		String tmpidentifier;
		tmpidentifier = MonoUtil::MonoToString(value.Identifier);
		output.Identifier = tmpidentifier;
		output.IsInternal = value.IsInternal;
		SmallVector<ShaderVariationParamValue, 4> vecvalues;
		if(value.Values != nullptr)
		{
			ScriptArray arrayvalues(value.Values);
			vecvalues.resize(arrayvalues.Size());
			for(int i = 0; i < (int)arrayvalues.Size(); i++)
			{
				vecvalues[i] = ScriptShaderVariationParamValue::FromInterop(arrayvalues.Get<__ShaderVariationParamValueInterop>(i));
			}
		}
		output.Values = vecvalues;

		return output;
	}

	__ShaderVariationParamInfoInterop ScriptShaderVariationParamInfo::ToInterop(const ShaderVariationParamInfo& value)
	{
		__ShaderVariationParamInfoInterop output;
		MonoString* tmpname;
		tmpname = MonoUtil::StringToMono(value.Name);
		output.Name = tmpname;
		MonoString* tmpidentifier;
		tmpidentifier = MonoUtil::StringToMono(value.Identifier);
		output.Identifier = tmpidentifier;
		output.IsInternal = value.IsInternal;
		int arraySizevalues = (int)value.Values.size();
		MonoArray* vecvalues;
		ScriptArray arrayvalues = ScriptArray::Create<ScriptShaderVariationParamValue>(arraySizevalues);
		for(int i = 0; i < arraySizevalues; i++)
		{
			arrayvalues.Set(i, ScriptShaderVariationParamValue::ToInterop(value.Values[i]));
		}
		vecvalues = arrayvalues.GetInternal();
		output.Values = vecvalues;

		return output;
	}

}
