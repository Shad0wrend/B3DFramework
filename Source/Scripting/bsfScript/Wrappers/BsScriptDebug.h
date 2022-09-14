//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "BsScriptObject.h"

namespace bs
{
	/** @addtogroup ScriptInteropEngine
	 *  @{
	 */

	/**	Interop class between C++ & CLR for Debug. */
	class BS_SCR_BE_EXPORT ScriptDebug : public ScriptObject<ScriptDebug>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "Debug")

		/**	Registers internal callbacks. Must be called on scripting system load. */
		static void StartUp();

		/**	Unregisters internal callbacks. Must be called on scripting system shutdown. */
		static void ShutDown();
	private:
		ScriptDebug(MonoObject* instance);

		/**	Triggered when a new entry is added to the debug log. */
		static void OnLogEntryAdded(const LogEntry& entry);

		static HEvent mOnLogEntryAddedConn;

		/************************************************************************/
		/* 								CLR HOOKS						   		*/
		/************************************************************************/
		static void InternalLog(MonoString* message, UINT32 category);
		static void InternalLogWarning(MonoString* message, UINT32 category);
		static void InternalLogError(MonoString* message, UINT32 category);
		static void InternalLogMessage(MonoString* message, LogVerbosity verbosity, UINT32 category);
		static void InternalClear(LogVerbosity verbosity, UINT32 category);
		static MonoArray* InternalGetMessages();

		typedef void(BS_THUNKCALL *OnAddedThunkDef) (MonoString*, UINT32, UINT32, MonoException**);

		static OnAddedThunkDef onAddedThunk;
	};

	/** @} */
}
