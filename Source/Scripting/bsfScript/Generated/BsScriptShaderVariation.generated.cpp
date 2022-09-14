//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptShaderVariation.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptShaderVariation::ScriptShaderVariation(MonoObject* managedInstance, const SPtr<ShaderVariation>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptShaderVariation::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_ShaderVariation", (void*)&ScriptShaderVariation::InternalShaderVariation);
		metaData.scriptClass->AddInternalCall("Internal_getInt", (void*)&ScriptShaderVariation::InternalGetInt);
		metaData.scriptClass->AddInternalCall("Internal_getUInt", (void*)&ScriptShaderVariation::InternalGetUInt);
		metaData.scriptClass->AddInternalCall("Internal_getFloat", (void*)&ScriptShaderVariation::InternalGetFloat);
		metaData.scriptClass->AddInternalCall("Internal_getBool", (void*)&ScriptShaderVariation::InternalGetBool);
		metaData.scriptClass->AddInternalCall("Internal_setInt", (void*)&ScriptShaderVariation::InternalSetInt);
		metaData.scriptClass->AddInternalCall("Internal_setUInt", (void*)&ScriptShaderVariation::InternalSetUInt);
		metaData.scriptClass->AddInternalCall("Internal_setFloat", (void*)&ScriptShaderVariation::InternalSetFloat);
		metaData.scriptClass->AddInternalCall("Internal_setBool", (void*)&ScriptShaderVariation::InternalSetBool);
		metaData.scriptClass->AddInternalCall("Internal_removeParam", (void*)&ScriptShaderVariation::InternalRemoveParam);
		metaData.scriptClass->AddInternalCall("Internal_hasParam", (void*)&ScriptShaderVariation::InternalHasParam);
		metaData.scriptClass->AddInternalCall("Internal_clearParams", (void*)&ScriptShaderVariation::InternalClearParams);
		metaData.scriptClass->AddInternalCall("Internal_getParamNames", (void*)&ScriptShaderVariation::InternalGetParamNames);

	}

	MonoObject* ScriptShaderVariation::Create(const SPtr<ShaderVariation>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptShaderVariation>()) ScriptShaderVariation(managedInstance, value);
		return managedInstance;
	}
	void ScriptShaderVariation::InternalShaderVariation(MonoObject* managedInstance)
	{
		SPtr<ShaderVariation> instance = bs_shared_ptr_new<ShaderVariation>();
		new (bs_alloc<ScriptShaderVariation>())ScriptShaderVariation(managedInstance, instance);
	}

	int32_t ScriptShaderVariation::InternalGetInt(ScriptShaderVariation* thisPtr, MonoString* name)
	{
		int32_t tmp__output;
		String tmpname;
		tmpname = MonoUtil::MonoToString(name);
		tmp__output = thisPtr->GetInternal()->GetInt(tmpname);

		int32_t __output;
		__output = tmp__output;

		return __output;
	}

	uint32_t ScriptShaderVariation::InternalGetUInt(ScriptShaderVariation* thisPtr, MonoString* name)
	{
		uint32_t tmp__output;
		String tmpname;
		tmpname = MonoUtil::MonoToString(name);
		tmp__output = thisPtr->GetInternal()->getUInt(tmpname);

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	float ScriptShaderVariation::InternalGetFloat(ScriptShaderVariation* thisPtr, MonoString* name)
	{
		float tmp__output;
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		tmp__output = thisPtr->getInternal()->getFloat(tmpname);

		float __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptShaderVariation::InternalGetBool(ScriptShaderVariation* thisPtr, MonoString* name)
	{
		bool tmp__output;
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		tmp__output = thisPtr->getInternal()->getBool(tmpname);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptShaderVariation::InternalSetInt(ScriptShaderVariation* thisPtr, MonoString* name, int32_t value)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		thisPtr->getInternal()->setInt(tmpname, value);
	}

	void ScriptShaderVariation::InternalSetUInt(ScriptShaderVariation* thisPtr, MonoString* name, uint32_t value)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		thisPtr->getInternal()->setUInt(tmpname, value);
	}

	void ScriptShaderVariation::InternalSetFloat(ScriptShaderVariation* thisPtr, MonoString* name, float value)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		thisPtr->getInternal()->setFloat(tmpname, value);
	}

	void ScriptShaderVariation::InternalSetBool(ScriptShaderVariation* thisPtr, MonoString* name, bool value)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		thisPtr->getInternal()->setBool(tmpname, value);
	}

	void ScriptShaderVariation::InternalRemoveParam(ScriptShaderVariation* thisPtr, MonoString* paramName)
	{
		String tmpparamName;
		tmpparamName = MonoUtil::monoToString(paramName);
		thisPtr->getInternal()->removeParam(tmpparamName);
	}

	bool ScriptShaderVariation::InternalHasParam(ScriptShaderVariation* thisPtr, MonoString* paramName)
	{
		bool tmp__output;
		String tmpparamName;
		tmpparamName = MonoUtil::monoToString(paramName);
		tmp__output = thisPtr->getInternal()->hasParam(tmpparamName);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptShaderVariation::InternalClearParams(ScriptShaderVariation* thisPtr)
	{
		thisPtr->getInternal()->clearParams();
	}

	MonoArray* ScriptShaderVariation::InternalGetParamNames(ScriptShaderVariation* thisPtr)
	{
		Vector<String> vec__output;
		vec__output = thisPtr->getInternal()->getParamNames();

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<String>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.set(i, vec__output[i]);
		}
		__output = array__output.getInternal();

		return __output;
	}
}
