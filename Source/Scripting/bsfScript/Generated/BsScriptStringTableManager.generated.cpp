//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptStringTableManager.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Localization/BsStringTableManager.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "../../../Foundation/bsfCore/Localization/BsStringTable.h"

namespace bs
{
	ScriptStringTableManager::ScriptStringTableManager(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{
	}

	void ScriptStringTableManager::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_setActiveLanguage", (void*)&ScriptStringTableManager::InternalSetActiveLanguage);
		metaData.scriptClass->AddInternalCall("Internal_getActiveLanguage", (void*)&ScriptStringTableManager::InternalGetActiveLanguage);
		metaData.scriptClass->AddInternalCall("Internal_getTable", (void*)&ScriptStringTableManager::InternalGetTable);
		metaData.scriptClass->AddInternalCall("Internal_removeTable", (void*)&ScriptStringTableManager::InternalRemoveTable);
		metaData.scriptClass->AddInternalCall("Internal_setTable", (void*)&ScriptStringTableManager::InternalSetTable);

	}

	void ScriptStringTableManager::InternalSetActiveLanguage(Language language)
	{
		StringTableManager::Instance().SetActiveLanguage(language);
	}

	Language ScriptStringTableManager::InternalGetActiveLanguage()
	{
		Language tmp__output;
		tmp__output = StringTableManager::Instance().GetActiveLanguage();

		Language __output;
		__output = tmp__output;

		return __output;
	}

	MonoObject* ScriptStringTableManager::InternalGetTable(uint32_t id)
	{
		ResourceHandle<StringTable> tmp__output;
		tmp__output = StringTableManager::Instance().getTable(id);

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().getScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptStringTableManager::InternalRemoveTable(uint32_t id)
	{
		StringTableManager::Instance().removeTable(id);
	}

	void ScriptStringTableManager::InternalSetTable(uint32_t id, MonoObject* table)
	{
		ResourceHandle<StringTable> tmptable;
		ScriptRRefBase* scripttable;
		scripttable = ScriptRRefBase::toNative(table);
		if(scripttable != nullptr)
			tmptable = static_resource_cast<StringTable>(scripttable->GetHandle());
		StringTableManager::Instance().setTable(id, tmptable);
	}
}
