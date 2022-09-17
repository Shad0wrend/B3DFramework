//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptStringTable.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Localization/BsStringTable.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "../../../Foundation/bsfCore/Localization/BsStringTable.h"

namespace bs
{
	ScriptStringTable::ScriptStringTable(MonoObject* managedInstance, const ResourceHandle<StringTable>& value)
		:TScriptResource(managedInstance, value)
	{
	}

	void ScriptStringTable::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_GetRef", (void*)&ScriptStringTable::InternalGetRef);
		metaData.scriptClass->AddInternalCall("Internal_contains", (void*)&ScriptStringTable::InternalContains);
		metaData.scriptClass->AddInternalCall("Internal_getNumStrings", (void*)&ScriptStringTable::InternalGetNumStrings);
		metaData.scriptClass->AddInternalCall("Internal_getIdentifiers", (void*)&ScriptStringTable::InternalGetIdentifiers);
		metaData.scriptClass->AddInternalCall("Internal_setString", (void*)&ScriptStringTable::InternalSetString);
		metaData.scriptClass->AddInternalCall("Internal_getString", (void*)&ScriptStringTable::InternalGetString);
		metaData.scriptClass->AddInternalCall("Internal_removeString", (void*)&ScriptStringTable::InternalRemoveString);
		metaData.scriptClass->AddInternalCall("Internal_create", (void*)&ScriptStringTable::InternalCreate);

	}

	 MonoObject*ScriptStringTable::CreateInstance()
	{
		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		return metaData.scriptClass->CreateInstance("bool", ctorParams);
	}
	MonoObject* ScriptStringTable::InternalGetRef(ScriptStringTable* thisPtr)
	{
		return thisPtr->GetRRef();
	}

	bool ScriptStringTable::InternalContains(ScriptStringTable* thisPtr, MonoString* identifier)
	{
		bool tmp__output;
		String tmpidentifier;
		tmpidentifier = MonoUtil::MonoToString(identifier);
		tmp__output = thisPtr->GetHandle()->contains(tmpidentifier);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	uint32_t ScriptStringTable::InternalGetNumStrings(ScriptStringTable* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetHandle()->GetNumStrings();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	MonoArray* ScriptStringTable::InternalGetIdentifiers(ScriptStringTable* thisPtr)
	{
		Vector<String> vec__output;
		vec__output = thisPtr->GetHandle()->GetIdentifiers();

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<String>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, vec__output[i]);
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptStringTable::InternalSetString(ScriptStringTable* thisPtr, MonoString* identifier, Language language, MonoString* value)
	{
		String tmpidentifier;
		tmpidentifier = MonoUtil::MonoToString(identifier);
		String tmpvalue;
		tmpvalue = MonoUtil::MonoToString(value);
		thisPtr->GetHandle()->SetString(tmpidentifier, language, tmpvalue);
	}

	MonoString* ScriptStringTable::InternalGetString(ScriptStringTable* thisPtr, MonoString* identifier, Language language)
	{
		String tmp__output;
		String tmpidentifier;
		tmpidentifier = MonoUtil::MonoToString(identifier);
		tmp__output = thisPtr->GetHandle()->GetString(tmpidentifier, language);

		MonoString* __output;
		__output = MonoUtil::stringToMono(tmp__output);

		return __output;
	}

	void ScriptStringTable::InternalRemoveString(ScriptStringTable* thisPtr, MonoString* identifier)
	{
		String tmpidentifier;
		tmpidentifier = MonoUtil::MonoToString(identifier);
		thisPtr->GetHandle()->removeString(tmpidentifier);
	}

	void ScriptStringTable::InternalCreate(MonoObject* managedInstance)
	{
		ResourceHandle<StringTable> instance = StringTable::Create();
		ScriptResourceManager::Instance().createBuiltinScriptResource(instance, managedInstance);
	}
}
