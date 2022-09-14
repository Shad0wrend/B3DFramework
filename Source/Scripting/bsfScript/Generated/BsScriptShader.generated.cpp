//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptShader.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Material/BsShader.h"
#include "BsScriptShaderVariationParamInfo.generated.h"
#include "BsScriptShaderParameter.generated.h"
#include "../Extensions/BsShaderEx.h"

namespace bs
{
	ScriptShader::ScriptShader(MonoObject* managedInstance, const ResourceHandle<Shader>& value)
		:TScriptResource(managedInstance, value)
	{
	}

	void ScriptShader::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_GetRef", (void*)&ScriptShader::InternalGetRef);
		metaData.scriptClass->AddInternalCall("Internal_getVariationParams", (void*)&ScriptShader::InternalGetVariationParams);
		metaData.scriptClass->AddInternalCall("Internal_getParameters", (void*)&ScriptShader::InternalGetParameters);

	}

	 MonoObject*ScriptShader::CreateInstance()
	{
		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		return metaData.scriptClass->CreateInstance("bool", ctorParams);
	}
	MonoObject* ScriptShader::InternalGetRef(ScriptShader* thisPtr)
	{
		return thisPtr->GetRRef();
	}

	MonoArray* ScriptShader::Internal_getVariationParams(ScriptShader* thisPtr)
	{
		Vector<ShaderVariationParamInfo> vec__output;
		vec__output = thisPtr->getHandle()->getVariationParams();

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptShaderVariationParamInfo>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.set(i, ScriptShaderVariationParamInfo::toInterop(vec__output[i]));
		}
		__output = array__output.getInternal();

		return __output;
	}

	MonoArray* ScriptShader::InternalGetParameters(ScriptShader* thisPtr)
	{
		Vector<ShaderParameter> vec__output;
		vec__output = ShaderEx::getParameters(thisPtr->getHandle());

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptShaderParameter>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.set(i, ScriptShaderParameter::toInterop(vec__output[i]));
		}
		__output = array__output.getInternal();

		return __output;
	}
}
