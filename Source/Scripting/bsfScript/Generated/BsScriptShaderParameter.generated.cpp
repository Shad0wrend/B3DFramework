//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptShaderParameter.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptShaderParameter::ScriptShaderParameter(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptShaderParameter::initRuntimeData()
	{ }

	MonoObject*ScriptShaderParameter::Box(const __ShaderParameterInterop& value)
	{
		return MonoUtil::Box(metaData.scriptClass->GetInternalClassInternal(), (void*)&value);
	}

	__ShaderParameterInterop ScriptShaderParameter::Unbox(MonoObject* value)
	{
		return *(__ShaderParameterInterop*)MonoUtil::Unbox(value);
	}

	ShaderParameter ScriptShaderParameter::FromInterop(const __ShaderParameterInterop& value)
	{
		ShaderParameter output;
		String tmpname;
		tmpname = MonoUtil::MonoToString(value.name);
		output.name = tmpname;
		String tmpidentifier;
		tmpidentifier = MonoUtil::MonoToString(value.identifier);
		output.identifier = tmpidentifier;
		output.type = value.type;
		output.flags = value.flags;

		return output;
	}

	__ShaderParameterInterop ScriptShaderParameter::ToInterop(const ShaderParameter& value)
	{
		__ShaderParameterInterop output;
		MonoString* tmpname;
		tmpname = MonoUtil::StringToMono(value.name);
		output.name = tmpname;
		MonoString* tmpidentifier;
		tmpidentifier = MonoUtil::stringToMono(value.identifier);
		output.identifier = tmpidentifier;
		output.type = value.type;
		output.flags = value.flags;

		return output;
	}

}
