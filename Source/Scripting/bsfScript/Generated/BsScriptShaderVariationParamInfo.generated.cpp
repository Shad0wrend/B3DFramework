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

	void ScriptShaderVariationParamInfo::initRuntimeData()
	{ }

	MonoObject*ScriptShaderVariationParamInfo::Box(const __ShaderVariationParamInfoInterop& value)
	{
		return MonoUtil::Box(metaData.scriptClass->GetInternalClassInternal(), (void*)&value);
	}

	__ShaderVariationParamInfoInterop ScriptShaderVariationParamInfo::Unbox(MonoObject* value)
	{
		return *(__ShaderVariationParamInfoInterop*)MonoUtil::Unbox(value);
	}

	ShaderVariationParamInfo ScriptShaderVariationParamInfo::FromInterop(const __ShaderVariationParamInfoInterop& value)
	{
		ShaderVariationParamInfo output;
		String tmpname;
		tmpname = MonoUtil::MonoToString(value.name);
		output.name = tmpname;
		String tmpidentifier;
		tmpidentifier = MonoUtil::MonoToString(value.identifier);
		output.identifier = tmpidentifier;
		output.isInternal = value.isInternal;
		SmallVector<ShaderVariationParamValue, 4> vecvalues;
		if(value.values != nullptr)
		{
			ScriptArray arrayvalues(value.values);
			vecvalues.Resize(arrayvalues.Size());
			for(int i = 0; i < (int)arrayvalues.Size(); i++)
			{
				vecvalues[i] = ScriptShaderVariationParamValue::fromInterop(arrayvalues.get<__ShaderVariationParamValueInterop>(i));
			}
		}
		output.values = vecvalues;

		return output;
	}

	__ShaderVariationParamInfoInterop ScriptShaderVariationParamInfo::ToInterop(const ShaderVariationParamInfo& value)
	{
		__ShaderVariationParamInfoInterop output;
		MonoString* tmpname;
		tmpname = MonoUtil::StringToMono(value.name);
		output.name = tmpname;
		MonoString* tmpidentifier;
		tmpidentifier = MonoUtil::StringToMono(value.identifier);
		output.identifier = tmpidentifier;
		output.isInternal = value.isInternal;
		int arraySizevalues = (int)value.values.Size();
		MonoArray* vecvalues;
		ScriptArray arrayvalues = ScriptArray::Create<ScriptShaderVariationParamValue>(arraySizevalues);
		for(int i = 0; i < arraySizevalues; i++)
		{
			arrayvalues.Set(i, ScriptShaderVariationParamValue::ToInterop(value.values[i]));
		}
		vecvalues = arrayvalues.GetInternal();
		output.values = vecvalues;

		return output;
	}

}
