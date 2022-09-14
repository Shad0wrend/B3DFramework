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

	/**	Interop class between C++ & CLR for GUIContextMenu. */
	class BS_SCR_BE_EXPORT ScriptContextMenu : public ScriptObject < ScriptContextMenu >
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "ContextMenu")

		/**	Returns the internal native context menu object. */
		SPtr<GUIContextMenu> GetInternal() const { return mContextMenu; }

	private:
		ScriptContextMenu(MonoObject* instance);

		/**
		 * Triggered when an item in the context menu is clicked.
		 *
		 * @param[in]	idx		Sequential index of the item that was clicked.
		 */
		void OnContextMenuItemTriggered(UINT32 idx);

		SPtr<GUIContextMenu> mContextMenu;
		UINT32 mGCHandle = 0;

		/************************************************************************/
		/* 								CLR HOOKS						   		*/
		/************************************************************************/
		typedef void(BS_THUNKCALL *OnEntryTriggeredThunkDef) (MonoObject*, UINT32 callbackIdx, MonoException**);
		static OnEntryTriggeredThunkDef onEntryTriggered;

		static void InternalCreateInstance(MonoObject* instance);
		static void InternalOpen(ScriptContextMenu* instance, Vector2I* position, ScriptGUILayout* layoutPtr);
		static void InternalAddItem(ScriptContextMenu* instance, MonoString* path, UINT32 callbackIdx, ShortcutKey* shortcut);
		static void InternalAddSeparator(ScriptContextMenu* instance, MonoString* path);
		static void InternalSetLocalizedName(ScriptContextMenu* instance, MonoString* label, ScriptHString* name);
	};

	/** @} */
}
