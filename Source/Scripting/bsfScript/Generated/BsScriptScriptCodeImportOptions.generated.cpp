//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptScriptCodeImportOptions.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptScriptCodeImportOptions.generated.h"

namespace bs
{
#if !BS_IS_BANSHEE3D
	ScriptScriptCodeImportOptions::ScriptScriptCodeImportOptions(MonoObject* managedInstance, const SPtr<ScriptCodeImportOptions>& value)
		:TScriptReflectable(managedInstance, value)
	{
		mInternal = value;
	}

	void ScriptScriptCodeImportOptions::InitRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_GeteditorScript", (void*)&ScriptScriptCodeImportOptions::InternalGeteditorScript);
		metaData.scriptClass->AddInternalCall("Internal_SeteditorScript", (void*)&ScriptScriptCodeImportOptions::InternalSeteditorScript);
		metaData.scriptClass->AddInternalCall("Internal_Create", (void*)&ScriptScriptCodeImportOptions::InternalCreate);

	}

	MonoObject* ScriptScriptCodeImportOptions::Create(const SPtr<ScriptCodeImportOptions>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptScriptCodeImportOptions>()) ScriptScriptCodeImportOptions(managedInstance, value);
		return managedInstance;
	}
	void ScriptScriptCodeImportOptions::InternalCreate(MonoObject* managedInstance)
	{
		SPtr<ScriptCodeImportOptions> instance = ScriptCodeImportOptions::Create();
		new (bs_alloc<ScriptScriptCodeImportOptions>())ScriptScriptCodeImportOptions(managedInstance, instance);
	}
	bool ScriptScriptCodeImportOptions::InternalGeteditorScript(ScriptScriptCodeImportOptions* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->editorScript;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptScriptCodeImportOptions::InternalSeteditorScript(ScriptScriptCodeImportOptions* thisPtr, bool value)
	{
		thisPtr->GetInternal()->editorScript = value;
	}
#endif
}
