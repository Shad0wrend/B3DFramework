//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptMorphChannel.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptMorphShape.generated.h"

namespace bs
{
	ScriptMorphChannel::ScriptMorphChannel(MonoObject* managedInstance, const SPtr<MorphChannel>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptMorphChannel::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_getName", (void*)&ScriptMorphChannel::InternalGetName);
		metaData.scriptClass->AddInternalCall("Internal_getShapes", (void*)&ScriptMorphChannel::InternalGetShapes);

	}

	MonoObject* ScriptMorphChannel::Create(const SPtr<MorphChannel>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptMorphChannel>()) ScriptMorphChannel(managedInstance, value);
		return managedInstance;
	}
	MonoString* ScriptMorphChannel::InternalGetName(ScriptMorphChannel* thisPtr)
	{
		String tmp__output;
		tmp__output = thisPtr->GetInternal()->GetName();

		MonoString* __output;
		__output = MonoUtil::StringToMono(tmp__output);

		return __output;
	}

	MonoArray* ScriptMorphChannel::InternalGetShapes(ScriptMorphChannel* thisPtr)
	{
		Vector<SPtr<MorphShape>> vec__output;
		vec__output = thisPtr->GetInternal()->GetShapes();

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = create<ScriptMorphShape>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			SPtr<MorphShape> arrayElemPtr__output = vec__output[i];
			MonoObject* arrayElem__output;
			arrayElem__output = ScriptMorphShape::Create(arrayElemPtr__output);
			array__output.Set(i, arrayElem__output);
		}
		__output = array__output.GetInternal();

		return __output;
	}
}
