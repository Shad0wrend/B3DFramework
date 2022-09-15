//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/BsScriptContextMenu.h"
#include "BsScriptMeta.h"
#include "BsMonoField.h"
#include "BsMonoClass.h"
#include "BsMonoManager.h"
#include "BsMonoMethod.h"
#include "BsMonoUtil.h"
#include "GUI/BsGUIContextMenu.h"
#include "Wrappers/GUI/BsScriptGUILayout.h"
#include "GUI/BsGUILayout.h"

#include "Generated/BsScriptHString.generated.h"

using namespace std::placeholders;

namespace bs
{
	ScriptContextMenu::OnEntryTriggeredThunkDef ScriptContextMenu::onEntryTriggered;

	ScriptContextMenu::ScriptContextMenu(MonoObject* instance)
		: ScriptObject(instance)
	{
		mGCHandle = MonoUtil::NewWeakGcHandle(instance);
		mContextMenu = bs_shared_ptr_new<GUIContextMenu>();
	}

	void ScriptContextMenu::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_CreateInstance", (void*)&ScriptContextMenu::InternalCreateInstance);
		metaData.scriptClass->AddInternalCall("Internal_Open", (void*)&ScriptContextMenu::InternalOpen);
		metaData.scriptClass->AddInternalCall("Internal_AddItem", (void*)&ScriptContextMenu::InternalAddItem);
		metaData.scriptClass->AddInternalCall("Internal_AddSeparator", (void*)&ScriptContextMenu::InternalAddSeparator);
		metaData.scriptClass->AddInternalCall("Internal_SetLocalizedName", (void*)&ScriptContextMenu::InternalSetLocalizedName);

		onEntryTriggered = (OnEntryTriggeredThunkDef)metaData.scriptClass->GetMethod("InternalDoOnEntryTriggered", 1)->GetThunk();
	}

	void ScriptContextMenu::InternalCreateInstance(MonoObject* instance)
	{
		new (bs_alloc<ScriptContextMenu>()) ScriptContextMenu(instance);
	}

	void ScriptContextMenu::internal_Open(ScriptContextMenu* instance, Vector2I* position, ScriptGUILayout* layoutPtr)
	{
		GUIElementBase* layout = layoutPtr->GetGUIElement();

		GUIWidget* widget = layout->GetParentWidgetInternal();
		if (widget == nullptr)
			return;

		Rect2I bounds = layout->GetGlobalBounds();
		Vector2I windowPosition = *position + Vector2I(bounds.x, bounds.y);

		SPtr<GUIContextMenu> contextMenu = instance->GetInternal();
		contextMenu->open(windowPosition, *widget);
	}

	void ScriptContextMenu::InternalAddItem(ScriptContextMenu* instance, MonoString* path, UINT32 callbackIdx,
		ShortcutKey* shortcut)
	{
		String nativePath = MonoUtil::monoToString(path);

		SPtr<GUIContextMenu> contextMenu = instance->GetInternal();
		contextMenu->addMenuItem(nativePath, std::bind(&ScriptContextMenu::onContextMenuItemTriggered,
			instance, callbackIdx), 0, *shortcut);
	}

	void ScriptContextMenu::InternalAddSeparator(ScriptContextMenu* instance, MonoString* path)
	{
		String nativePath = MonoUtil::monoToString(path);

		SPtr<GUIContextMenu> contextMenu = instance->GetInternal();
		contextMenu->addSeparator(nativePath, 0);
	}

	void ScriptContextMenu::InternalSetLocalizedName(ScriptContextMenu* instance, MonoString* label, ScriptHString* name)
	{
		if (label == nullptr || name == nullptr)
			return;

		String nativeLabel = MonoUtil::monoToString(label);
		SPtr<GUIContextMenu> contextMenu = instance->GetInternal();
		contextMenu->SetLocalizedName(nativeLabel, *name->GetInternal());
	}

	void ScriptContextMenu::OnContextMenuItemTriggered(UINT32 idx)
	{
		MonoObject* instance = MonoUtil::getObjectFromGCHandle(mGCHandle);
		MonoUtil::invokeThunk(onEntryTriggered, instance, idx);
	}
}
