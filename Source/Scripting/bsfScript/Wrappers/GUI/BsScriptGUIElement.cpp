//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/GUI/BsScriptGUIElement.h"
#include "BsScriptMeta.h"
#include "BsMonoField.h"
#include "BsMonoClass.h"
#include "BsMonoManager.h"
#include "BsMonoMethod.h"
#include "BsMonoUtil.h"
#include "GUI/BsGUIInteractable.h"
#include "Wrappers/GUI/BsScriptGUILayout.h"
#include "Wrappers/BsScriptContextMenu.h"
#include "GUI/BsGUIInteractable.h"

using namespace std::placeholders;

using namespace bs;
ScriptGUIElementBase::ScriptGUIElementBase(MonoObject* instance)
	: ScriptObjectBase(instance), mIsDestroyed(false), mElement(nullptr), mParent(nullptr)
{
	mGCHandle = MonoUtil::NewWeakGcHandle(instance);
}

void ScriptGUIElementBase::Initialize(GUIElementBase* element)
{
	mElement = element;

	if(mElement != nullptr && mElement->GetType() == GUIElementBase::Type::Interactable)
	{
		GUIInteractable* guiElem = static_cast<GUIInteractable*>(element);
		guiElem->OnFocusChanged.Connect(std::bind(&ScriptGUIElementBase::OnFocusChanged, this, _1));
	}
}

void ScriptGUIElementBase::OnFocusChanged(ScriptGUIElementBase* thisPtr, bool focus)
{
	MonoObject* instance = MonoUtil::GetObjectFromGcHandle(thisPtr->mGCHandle);

	if(focus)
		MonoUtil::InvokeThunk(ScriptGUIInteractable::onFocusGainedThunk, instance);
	else
		MonoUtil::InvokeThunk(ScriptGUIInteractable::onFocusLostThunk, instance);
}

MonoObject* ScriptGUIElementBase::GetManagedInstance() const
{
	return MonoUtil::GetObjectFromGcHandle(mGCHandle);
}

void ScriptGUIElementBase::OnManagedInstanceDeletedInternal(bool assemblyRefresh)
{
	Destroy();

	ScriptObjectBase::OnManagedInstanceDeletedInternal(assemblyRefresh);
}

void ScriptGUIElementBase::ClearManagedInstanceInternal()
{
	// Need to call destroy here because we need to release any GC handles before the domain is unloaded

	Destroy();
}

ScriptGUIInteractableBase::ScriptGUIInteractableBase(MonoObject* instance)
	: ScriptGUIElementBase(instance)
{
}

void ScriptGUIInteractableBase::Destroy()
{
	if(!mIsDestroyed)
	{
		if(mParent != nullptr)
			mParent->RemoveChild(this);

		mElement->Destroy();
		mElement = nullptr;

		mIsDestroyed = true;
	}
}

ScriptGUIInteractable::OnFocusChangedThunkDef ScriptGUIInteractable::onFocusGainedThunk;
ScriptGUIInteractable::OnFocusChangedThunkDef ScriptGUIInteractable::onFocusLostThunk;

ScriptGUIInteractable::ScriptGUIInteractable(MonoObject* instance)
	: ScriptObject(instance)
{
}

void ScriptGUIInteractable::InitRuntimeData()
{
	metaData.ScriptClass->AddInternalCall("Internal_Destroy", (void*)&ScriptGUIInteractable::InternalDestroy);
	metaData.ScriptClass->AddInternalCall("Internal_SetVisible", (void*)&ScriptGUIInteractable::InternalSetVisible);
	metaData.ScriptClass->AddInternalCall("Internal_SetActive", (void*)&ScriptGUIInteractable::InternalSetActive);
	metaData.ScriptClass->AddInternalCall("Internal_SetDisabled", (void*)&ScriptGUIInteractable::InternalSetDisabled);
	metaData.ScriptClass->AddInternalCall("Internal_GetVisible", (void*)&ScriptGUIInteractable::InternalGetVisible);
	metaData.ScriptClass->AddInternalCall("Internal_GetActive", (void*)&ScriptGUIInteractable::InternalGetActive);
	metaData.ScriptClass->AddInternalCall("Internal_GetDisabled", (void*)&ScriptGUIInteractable::InternalGetDisabled);
	metaData.ScriptClass->AddInternalCall("Internal_SetFocus", (void*)&ScriptGUIInteractable::InternalSetFocus);
	metaData.ScriptClass->AddInternalCall("Internal_SetFocus", (void*)&ScriptGUIInteractable::InternalSetFocus);
	metaData.ScriptClass->AddInternalCall("Internal_GetBlocking", (void*)&ScriptGUIInteractable::InternalGetBlocking);
	metaData.ScriptClass->AddInternalCall("Internal_SetBlocking", (void*)&ScriptGUIInteractable::InternalSetBlocking);
	metaData.ScriptClass->AddInternalCall("Internal_GetAcceptsKeyFocus", (void*)&ScriptGUIInteractable::InternalGetAcceptsKeyFocus);
	metaData.ScriptClass->AddInternalCall("Internal_SetAcceptsKeyFocus", (void*)&ScriptGUIInteractable::InternalSetAcceptsKeyFocus);
	metaData.ScriptClass->AddInternalCall("Internal_GetBounds", (void*)&ScriptGUIInteractable::InternalGetBounds);
	metaData.ScriptClass->AddInternalCall("Internal_SetBounds", (void*)&ScriptGUIInteractable::InternalSetBounds);
	metaData.ScriptClass->AddInternalCall("Internal_GetVisibleBounds", (void*)&ScriptGUIInteractable::InternalGetVisibleBounds);
	metaData.ScriptClass->AddInternalCall("Internal_GetScreenBounds", (void*)&ScriptGUIInteractable::InternalGetScreenBounds);
	metaData.ScriptClass->AddInternalCall("Internal_SetPosition", (void*)&ScriptGUIInteractable::InternalSetPosition);
	metaData.ScriptClass->AddInternalCall("Internal_SetWidth", (void*)&ScriptGUIInteractable::InternalSetWidth);
	metaData.ScriptClass->AddInternalCall("Internal_SetFlexibleWidth", (void*)&ScriptGUIInteractable::InternalSetFlexibleWidth);
	metaData.ScriptClass->AddInternalCall("Internal_SetHeight", (void*)&ScriptGUIInteractable::InternalSetHeight);
	metaData.ScriptClass->AddInternalCall("Internal_SetFlexibleHeight", (void*)&ScriptGUIInteractable::InternalSetFlexibleHeight);
	metaData.ScriptClass->AddInternalCall("Internal_ResetDimensions", (void*)&ScriptGUIInteractable::InternalResetDimensions);
	metaData.ScriptClass->AddInternalCall("Internal_SetContextMenu", (void*)&ScriptGUIInteractable::InternalSetContextMenu);
	metaData.ScriptClass->AddInternalCall("Internal_GetStyle", (void*)&ScriptGUIInteractable::InternalGetStyle);
	metaData.ScriptClass->AddInternalCall("Internal_SetStyle", (void*)&ScriptGUIInteractable::InternalSetStyle);
	metaData.ScriptClass->AddInternalCall("Internal_GetParent", (void*)&ScriptGUIInteractable::InternalGetParent);
	metaData.ScriptClass->AddInternalCall("Internal_SetTint", (void*)&ScriptGUIInteractable::InternalSetTint);

	onFocusGainedThunk = (OnFocusChangedThunkDef)metaData.ScriptClass->GetMethod("Internal_OnFocusGained", 0)->GetThunk();
	onFocusLostThunk = (OnFocusChangedThunkDef)metaData.ScriptClass->GetMethod("Internal_OnFocusLost", 0)->GetThunk();
}

void ScriptGUIInteractable::InternalDestroy(ScriptGUIElementBase* nativeInstance)
{
	nativeInstance->Destroy();
}

void ScriptGUIInteractable::InternalSetVisible(ScriptGUIElementBase* nativeInstance, bool visible)
{
	if(nativeInstance->IsDestroyed())
		return;

	nativeInstance->GetGuiElement()->SetVisible(visible);
}

void ScriptGUIInteractable::InternalSetActive(ScriptGUIElementBase* nativeInstance, bool enabled)
{
	if(nativeInstance->IsDestroyed())
		return;

	nativeInstance->GetGuiElement()->SetActive(enabled);
}

void ScriptGUIInteractable::InternalSetDisabled(ScriptGUIElementBase* nativeInstance, bool disabled)
{
	if(nativeInstance->IsDestroyed())
		return;

	nativeInstance->GetGuiElement()->SetDisabled(disabled);
}

void ScriptGUIInteractable::InternalSetFocus(ScriptGUIElementBase* nativeInstance, bool focus)
{
	if(nativeInstance->IsDestroyed())
		return;

	GUIElementBase* guiElemBase = nativeInstance->GetGuiElement();
	if(guiElemBase->GetType() == GUIElementBase::Type::Interactable)
	{
		GUIInteractable* guiElem = static_cast<GUIInteractable*>(guiElemBase);
		guiElem->SetFocus(focus, true);
	}
}

bool ScriptGUIInteractable::InternalGetVisible(ScriptGUIElementBase* nativeInstance)
{
	if(nativeInstance->IsDestroyed())
		return false;

	GUIElementBase* guiElemBase = nativeInstance->GetGuiElement();
	return guiElemBase->IsVisible();
}

bool ScriptGUIInteractable::InternalGetActive(ScriptGUIElementBase* nativeInstance)
{
	if(nativeInstance->IsDestroyed())
		return false;

	GUIElementBase* guiElemBase = nativeInstance->GetGuiElement();
	return guiElemBase->IsActive();
}

bool ScriptGUIInteractable::InternalGetDisabled(ScriptGUIElementBase* nativeInstance)
{
	if(nativeInstance->IsDestroyed())
		return false;

	GUIElementBase* guiElemBase = nativeInstance->GetGuiElement();
	return guiElemBase->IsDisabled();
}

bool ScriptGUIInteractable::InternalGetBlocking(ScriptGUIElementBase* nativeInstance)
{
	if(nativeInstance->IsDestroyed())
		return false;

	GUIElementBase* guiElemBase = nativeInstance->GetGuiElement();
	if(guiElemBase->GetType() == GUIElementBase::Type::Interactable)
	{
		GUIInteractable* guiElem = static_cast<GUIInteractable*>(guiElemBase);
		return !guiElem->GetOptionFlags().IsSet(GUIElementOption::ClickThrough);
	}

	return false;
}

void ScriptGUIInteractable::InternalSetBlocking(ScriptGUIElementBase* nativeInstance, bool blocking)
{
	if(nativeInstance->IsDestroyed())
		return;

	GUIElementBase* guiElemBase = nativeInstance->GetGuiElement();
	if(guiElemBase->GetType() == GUIElementBase::Type::Interactable)
	{
		GUIInteractable* guiElem = static_cast<GUIInteractable*>(guiElemBase);

		GUIElementOptions options = guiElem->GetOptionFlags();
		if(blocking)
			options.Unset(GUIElementOption::ClickThrough);
		else
			options.Set(GUIElementOption::ClickThrough);

		guiElem->SetOptionFlags(options);
	}
}

bool ScriptGUIInteractable::InternalGetAcceptsKeyFocus(ScriptGUIElementBase* nativeInstance)
{
	if(nativeInstance->IsDestroyed())
		return false;

	GUIElementBase* guiElemBase = nativeInstance->GetGuiElement();
	if(guiElemBase->GetType() == GUIElementBase::Type::Interactable)
	{
		GUIInteractable* guiElem = static_cast<GUIInteractable*>(guiElemBase);
		return guiElem->GetOptionFlags().IsSet(GUIElementOption::AcceptsKeyFocus);
	}

	return false;
}

void ScriptGUIInteractable::InternalSetAcceptsKeyFocus(ScriptGUIElementBase* nativeInstance, bool accepts)
{
	if(nativeInstance->IsDestroyed())
		return;

	GUIElementBase* guiElemBase = nativeInstance->GetGuiElement();
	if(guiElemBase->GetType() == GUIElementBase::Type::Interactable)
	{
		GUIInteractable* guiElem = static_cast<GUIInteractable*>(guiElemBase);

		GUIElementOptions options = guiElem->GetOptionFlags();
		if(accepts)
			options.Set(GUIElementOption::AcceptsKeyFocus);
		else
			options.Unset(GUIElementOption::AcceptsKeyFocus);

		guiElem->SetOptionFlags(options);
	}
}

MonoObject* ScriptGUIInteractable::InternalGetParent(ScriptGUIElementBase* nativeInstance)
{
	if(nativeInstance->IsDestroyed())
		return nullptr;

	if(nativeInstance->GetParent() != nullptr)
		return nativeInstance->GetParent()->GetManagedInstance();

	return nullptr;
}

void ScriptGUIInteractable::InternalGetBounds(ScriptGUIElementBase* nativeInstance, Rect2I* bounds)
{
	if(nativeInstance->IsDestroyed())
	{
		*bounds = Rect2I();
		return;
	}

	*bounds = nativeInstance->GetGuiElement()->GetBoundsRelativeTo();
}

void ScriptGUIInteractable::InternalSetBounds(ScriptGUIElementBase* nativeInstance, Rect2I* bounds)
{
	if(nativeInstance->IsDestroyed())
		return;

	nativeInstance->GetGuiElement()->SetPosition(bounds->X, bounds->Y);
	nativeInstance->GetGuiElement()->SetWidth(bounds->Width);
	nativeInstance->GetGuiElement()->SetHeight(bounds->Height);
}

void ScriptGUIInteractable::InternalGetVisibleBounds(ScriptGUIElementBase* nativeInstance, Rect2I* bounds)
{
	if(nativeInstance->IsDestroyed())
	{
		*bounds = Rect2I();
		return;
	}

	*bounds = nativeInstance->GetGuiElement()->GetBoundsRelativeTo();
}

void ScriptGUIInteractable::InternalGetScreenBounds(ScriptGUIElementBase* nativeInstance, Rect2I* bounds)
{
	if(nativeInstance->IsDestroyed())
	{
		*bounds = Rect2I();
		return;
	}

	*bounds = nativeInstance->GetGuiElement()->GetScreenBounds();
}

void ScriptGUIInteractable::InternalSetPosition(ScriptGUIElementBase* nativeInstance, i32 x, i32 y)
{
	if(nativeInstance->IsDestroyed())
		return;

	nativeInstance->GetGuiElement()->SetPosition(x, y);
}

void ScriptGUIInteractable::InternalSetWidth(ScriptGUIElementBase* nativeInstance, u32 width)
{
	if(nativeInstance->IsDestroyed())
		return;

	nativeInstance->GetGuiElement()->SetWidth(width);
}

void ScriptGUIInteractable::InternalSetFlexibleWidth(ScriptGUIElementBase* nativeInstance, u32 minWidth, u32 maxWidth)
{
	if(nativeInstance->IsDestroyed())
		return;

	nativeInstance->GetGuiElement()->SetFlexibleWidth(minWidth, maxWidth);
}

void ScriptGUIInteractable::InternalSetHeight(ScriptGUIElementBase* nativeInstance, u32 height)
{
	if(nativeInstance->IsDestroyed())
		return;

	nativeInstance->GetGuiElement()->SetHeight(height);
}

void ScriptGUIInteractable::InternalSetFlexibleHeight(ScriptGUIElementBase* nativeInstance, u32 minHeight, u32 maxHeight)
{
	if(nativeInstance->IsDestroyed())
		return;

	nativeInstance->GetGuiElement()->SetFlexibleHeight(minHeight, maxHeight);
}

void ScriptGUIInteractable::InternalSetTint(ScriptGUIElementBase* nativeInstance, Color* tint)
{
	if(nativeInstance->IsDestroyed())
		return;

	GUIElementBase* const guiElementBase = nativeInstance->GetGuiElement();
	if(guiElementBase->GetType() != GUIElementBase::Type::Interactable)
		return;

	GUIInteractable* const guiElement = static_cast<GUIInteractable*>(guiElementBase);
	guiElement->SetTint(*tint);
}

void ScriptGUIInteractable::InternalResetDimensions(ScriptGUIElementBase* nativeInstance)
{
	if(nativeInstance->IsDestroyed())
		return;

	nativeInstance->GetGuiElement()->ResetDimensions();
}

void ScriptGUIInteractable::InternalSetContextMenu(ScriptGUIElementBase* nativeInstance, ScriptContextMenu* contextMenu)
{
	if(nativeInstance->IsDestroyed())
		return;

	GUIElementBase* guiElemBase = nativeInstance->GetGuiElement();
	if(guiElemBase->GetType() == GUIElementBase::Type::Interactable)
	{
		GUIInteractable* guiElem = static_cast<GUIInteractable*>(guiElemBase);

		SPtr<GUIContextMenu> nativeContextMenu;
		if(contextMenu != nullptr)
			nativeContextMenu = contextMenu->GetInternal();

		guiElem->SetContextMenu(nativeContextMenu);
	}
}

MonoString* ScriptGUIInteractable::InternalGetStyle(ScriptGUIElementBase* nativeInstance)
{
	if(!nativeInstance->IsDestroyed())
	{
		GUIElementBase* guiElemBase = nativeInstance->GetGuiElement();
		if(guiElemBase->GetType() == GUIElementBase::Type::Interactable)
		{
			GUIInteractable* guiElem = static_cast<GUIInteractable*>(guiElemBase);
			return MonoUtil::StringToMono(guiElem->GetStyleSheetClass());
		}
	}

	return MonoUtil::StringToMono(StringUtil::kBlank);
}

void ScriptGUIInteractable::InternalSetStyle(ScriptGUIElementBase* nativeInstance, MonoString* style)
{
	if(!nativeInstance->IsDestroyed())
	{
		GUIElementBase* guiElemBase = nativeInstance->GetGuiElement();
		if(guiElemBase->GetType() == GUIElementBase::Type::Interactable)
		{
			GUIInteractable* guiElem = static_cast<GUIInteractable*>(guiElemBase);
			guiElem->SetStyleSheetClass(MonoUtil::MonoToString(style));
		}
	}
}
