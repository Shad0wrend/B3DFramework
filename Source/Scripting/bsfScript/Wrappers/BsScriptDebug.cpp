//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/BsScriptDebug.h"
#include "BsMonoManager.h"
#include "BsMonoClass.h"
#include "BsMonoMethod.h"
#include "BsMonoUtil.h"
#include "Debug/BsDebug.h"
#include "Wrappers/BsScriptLogEntry.h"

namespace bs
{
	HEvent ScriptDebug::mOnLogEntryAddedConn;
	ScriptDebug::OnAddedThunkDef ScriptDebug::onAddedThunk = nullptr;

	/**	C++ version of the managed LogEntry structure. */
	struct ScriptLogEntryData
	{
		MonoString* message;
		LogVerbosity verbosity;
		UINT32 category;
	};

	ScriptDebug::ScriptDebug(MonoObject* instance)
		:ScriptObject(instance)
	{ }

	void ScriptDebug::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_Log", (void*)&ScriptDebug::InternalLog);
		metaData.scriptClass->AddInternalCall("Internal_LogWarning", (void*)&ScriptDebug::InternalLogWarning);
		metaData.scriptClass->AddInternalCall("Internal_LogError", (void*)&ScriptDebug::InternalLogError);
		metaData.scriptClass->AddInternalCall("Internal_LogMessage", (void*)&ScriptDebug::InternalLogMessage);
		metaData.scriptClass->AddInternalCall("Internal_Clear", (void*)&ScriptDebug::InternalClear);
		metaData.scriptClass->AddInternalCall("Internal_GetMessages", (void*)&ScriptDebug::InternalGetMessages);

		onAddedThunk = (OnAddedThunkDef)metaData.scriptClass->GetMethod("Internal_OnAdded", 3)->GetThunk();
	}

	void ScriptDebug::StartUp()
	{
		mOnLogEntryAddedConn = gDebug().onLogEntryAdded.Connect(&ScriptDebug::OnLogEntryAdded);
	}

	void ScriptDebug::ShutDown()
	{
		mOnLogEntryAddedConn.Disconnect();
	}

	void ScriptDebug::OnLogEntryAdded(const LogEntry& entry)
	{
		MonoString* message = MonoUtil::StringToMono(entry.GetMessage());

		MonoUtil::InvokeThunk(onAddedThunk, message, (INT32)entry.GetVerbosity(), entry.GetCategory());
	}

	void ScriptDebug::InternalLog(MonoString* message, UINT32 category)
	{
		gDebug().Log(MonoUtil::MonoToString(message), LogVerbosity::Info, category);
	}

	void ScriptDebug::InternalLogWarning(MonoString* message, UINT32 category)
	{
		gDebug().Log(MonoUtil::MonoToString(message), LogVerbosity::Warning, category);
	}

	void ScriptDebug::InternalLogError(MonoString* message, UINT32 category)
	{
		gDebug().log(MonoUtil::monoToString(message), LogVerbosity::Error, category);
	}

	void ScriptDebug::InternalLogMessage(MonoString* message, LogVerbosity type, UINT32 category)
	{
		gDebug().log(MonoUtil::monoToString(message), type, category);
	}

	void ScriptDebug::InternalClear(LogVerbosity verbosity, UINT32 category)
	{
		gDebug().getLog().clear(verbosity, category);
	}

	MonoArray* ScriptDebug::InternalGetMessages()
	{
		Vector<LogEntry> entries = gDebug().getLog().getEntries();

		UINT32 numEntries = (UINT32)entries.size();
		ScriptArray output = ScriptArray::create<ScriptLogEntry>(numEntries);
		for (UINT32 i = 0; i < numEntries; i++)
		{
			MonoString* message = MonoUtil::stringToMono(entries[i].getMessage());

			ScriptLogEntryData scriptEntry = { message, entries[i].getVerbosity(), entries[i].getCategory() };
			output.Set(i, scriptEntry);
		}

		return output.getInternal();
	}
}
