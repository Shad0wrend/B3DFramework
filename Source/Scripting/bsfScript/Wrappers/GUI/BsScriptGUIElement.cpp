//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/GUI/BsScriptGUIElement.h"
#include "BsScriptMeta.h"
#include "BsMonoField.h"
#include "BsMonoClass.h"
#include "BsMonoManager.h"
#include "BsMonoMethod.h"
#include "BsMonoUtil.h"
#include "GUI/BsGUIElement.h"
#include "Wrappers/GUI/BsScriptGUILayout.h"
#include "Wrappers/BsScriptContextMenu.h"
#include "GUI/BsGUIElement.h"

using namespace std::placeholders;

namespace bs
{
	ScriptGUIElementBaseTBase::ScriptGUIElementBaseTBase(MonoObject* instance)
		:ScriptObjectBase(instance), mIsDestroyed(false), mElement(nullptr), mParent(nullptr)
	{
		mGCHandle = MonoUtil::NewWeakGcHandle(instance);
	}

	void ScriptGUIElementBaseTBase::Initialize(GUIElementBase* element)
	{
		mElement = element;

		if (mElement != nullptr && mElement->GetTypeInternal() == GUIElementBase::Type::Element)
		{
			GUIElement* guiElem = static_cast<GUIElement*>(element);
			guiElem->onFocusChanged.Connect(std::bind(&ScriptGUIElementBaseTBase::OnFocusChanged, this, _1));
		}
	}

	void ScriptGUIElementBaseTBase::OnFocusChanged(ScriptGUIElementBaseTBase* thisPtr, bool focus)
	{
		MonoObject* instance = MonoUtil::GetObjectFromGcHandle(thisPtr->mGCHandle);

		if (focus)
			MonoUtil::InvokeThunk(ScriptGUIElement::onFocusGainedThunk, instance);
		else
			MonoUtil::InvokeThunk(ScriptGUIElement::onFocusLostThunk, instance);
	}

	MonoObject* ScriptGUIElementBaseTBase::GetManagedInstance() const
	{
		return MonoUtil::GetObjectFromGcHandle(mGCHandle);
	}

	void ScriptGUIElementBaseTBase::OnManagedInstanceDeletedInternal(bool assemblyRefresh)
	{
		Destroy();

		ScriptObjectBase::OnManagedInstanceDeletedInternal(assemblyRefresh);
	}

	void ScriptGUIElementBaseTBase::ClearManagedInstanceInternal()
	{
		// Need to call destroy here because we need to release any GC handles before the domain is unloaded

		Destroy();
	}

	ScriptGUIElementTBase::ScriptGUIElementTBase(MonoObject* instance)
		:ScriptGUIElementBaseTBase(instance)
	{

	}

	void ScriptGUIElementTBase::Destroy()
	{
		if(!mIsDestroyed)
		{
			if (mParent != nullptr)
				mParent->removeChild(this);

			if (mElement->GetTypeInternal() == GUIElementBase::Type::Element)
			{
				GUIElement::destroy((GUIElement*)mElement);
				mElement = nullptr;

				mIsDestroyed = true;
			}
		}
	}

	ScriptGUIElement::OnFocusChangedThunkDef ScriptGUIElement::onFocusGainedThunk;
	ScriptGUIElement::OnFocusChangedThunkDef ScriptGUIElement::onFocusLostThunk;

	ScriptGUIElement::ScriptGUIElement(MonoObject* instance)
		:ScriptObject(instance)
	{

	}

	void ScriptGUIElement::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_Destroy", (void*)&ScriptGUIElement::internal_destroy);
		metaData.scriptClass->addInternalCall("Internal_SetVisible", (void*)&ScriptGUIElement::internal_setVisible);
		metaData.scriptClass->addInternalCall("Internal_SetActive", (void*)&ScriptGUIElement::internal_setActive);
		metaData.scriptClass->addInternalCall("Internal_SetDisabled", (void*)&ScriptGUIElement::internal_setDisabled);
		metaData.scriptClass->addInternalCall("Internal_GetVisible", (void*)&ScriptGUIElement::internal_getVisible);
		metaData.scriptClass->addInternalCall("Internal_GetActive", (void*)&ScriptGUIElement::internal_getActive);
		metaData.scriptClass->addInternalCall("Internal_GetDisabled", (void*)&ScriptGUIElement::internal_getDisabled);
		metaData.scriptClass->addInternalCall("Internal_SetFocus", (void*)&ScriptGUIElement::internal_setFocus);
		metaData.scriptClass->addInternalCall("Internal_SetFocus", (void*)&ScriptGUIElement::internal_setFocus);
		metaData.scriptClass->addInternalCall("Internal_GetBlocking", (void*)&ScriptGUIElement::internal_getBlocking);
		metaData.scriptClass->addInternalCall("Internal_SetBlocking", (void*)&ScriptGUIElement::internal_setBlocking);
		metaData.scriptClass->addInternalCall("Internal_GetAcceptsKeyFocus", (void*)&ScriptGUIElement::internal_getAcceptsKeyFocus);
		metaData.scriptClass->addInternalCall("Internal_SetAcceptsKeyFocus", (void*)&ScriptGUIElement::internal_setAcceptsKeyFocus);
		metaData.scriptClass->addInternalCall("Internal_GetBounds", (void*)&ScriptGUIElement::internal_getBounds);
		metaData.scriptClass->addInternalCall("Internal_SetBounds", (void*)&ScriptGUIElement::internal_setBounds);
		metaData.scriptClass->addInternalCall("Internal_GetVisibleBounds", (void*)&ScriptGUIElement::internal_getVisibleBounds);
		metaData.scriptClass->addInternalCall("Internal_GetScreenBounds", (void*)&ScriptGUIElement::internal_getScreenBounds);
		metaData.scriptClass->addInternalCall("Internal_SetPosition", (void*)&ScriptGUIElement::internal_SetPosition);
		metaData.scriptClass->addInternalCall("Internal_SetWidth", (void*)&ScriptGUIElement::internal_SetWidth);
		metaData.scriptClass->addInternalCall("Internal_SetFlexibleWidth", (void*)&ScriptGUIElement::internal_SetFlexibleWidth);
		metaData.scriptClass->addInternalCall("Internal_SetHeight", (void*)&ScriptGUIElement::internal_SetHeight);
		metaData.scriptClass->addInternalCall("Internal_SetFlexibleHeight", (void*)&ScriptGUIElement::internal_SetFlexibleHeight);
		metaData.scriptClass->addInternalCall("Internal_ResetDimensions", (void*)&ScriptGUIElement::internal_ResetDimensions);
		metaData.scriptClass->addInternalCall("Internal_SetContextMenu", (void*)&ScriptGUIElement::internal_SetContextMenu);
		metaData.scriptClass->addInternalCall("Internal_GetStyle", (void*)&ScriptGUIElement::internal_GetStyle);
		metaData.scriptClass->addInternalCall("Internal_SetStyle", (void*)&ScriptGUIElement::internal_SetStyle);
		metaData.scriptClass->addInternalCall("Internal_GetParent", (void*)&ScriptGUIElement::internal_getParent);

		onFocusGainedThunk = (OnFocusChangedThunkDef)metaData.scriptClass->getMethod("Internal_OnFocusGained", 0)->getThunk();
		onFocusLostThunk = (OnFocusChangedThunkDef)metaData.scriptClass->getMethod("Internal_OnFocusLost", 0)->getThunk();
	}

	void ScriptGUIElement::InternalDestroy(ScriptGUIElementBaseTBase* nativeInstance)
	{
		nativeInstance->destroy();
	}

	void ScriptGUIElement::InternalSetVisible(ScriptGUIElementBaseTBase* nativeInstance, bool visible)
	{
		if (nativeInstance->isDestroyed())
			return;

		nativeInstance->getGUIElement()->setVisible(visible);
	}

	void ScriptGUIElement::InternalSetActive(ScriptGUIElementBaseTBase* nativeInstance, bool enabled)
	{
		if (nativeInstance->isDestroyed())
			return;

		nativeInstance->getGUIElement()->setActive(enabled);
	}

	void ScriptGUIElement::InternalSetDisabled(ScriptGUIElementBaseTBase* nativeInstance, bool disabled)
	{
		if (nativeInstance->isDestroyed())
			return;

		nativeInstance->getGUIElement()->setDisabled(disabled);
	}

	void ScriptGUIElement::InternalSetFocus(ScriptGUIElementBaseTBase* nativeInstance, bool focus)
	{
		if (nativeInstance->isDestroyed())
			return;

		GUIElementBase* guiElemBase = nativeInstance->getGUIElement();
		if (guiElemBase->GetTypeInternal() == GUIElementBase::Type::Element)
		{
			GUIElement* guiElem = static_cast<GUIElement*>(guiElemBase);
			guiElem->setFocus(focus, true);
		}		
	}

	bool ScriptGUIElement::InternalGetVisible(ScriptGUIElementBaseTBase* nativeInstance)
	{
		if (nativeInstance->isDestroyed())
			return false;

		GUIElementBase* guiElemBase = nativeInstance->getGUIElement();
		return guiElemBase->IsVisibleInternal();
	}

	bool ScriptGUIElement::InternalGetActive(ScriptGUIElementBaseTBase* nativeInstance)
	{
		if (nativeInstance->isDestroyed())
			return false;

		GUIElementBase* guiElemBase = nativeInstance->getGUIElement();
		return guiElemBase->IsActiveInternal();
	}

	bool ScriptGUIElement::InternalGetDisabled(ScriptGUIElementBaseTBase* nativeInstance)
	{
		if (nativeInstance->isDestroyed())
			return false;

		GUIElementBase* guiElemBase = nativeInstance->getGUIElement();
		return guiElemBase->IsDisabledInternal();
	}

	bool ScriptGUIElement::InternalGetBlocking(ScriptGUIElementBaseTBase* nativeInstance)
	{
		if (nativeInstance->isDestroyed())
			return false;

		GUIElementBase* guiElemBase = nativeInstance->getGUIElement();
		if (guiElemBase->GetTypeInternal() == GUIElementBase::Type::Element)
		{
			GUIElement* guiElem = static_cast<GUIElement*>(guiElemBase);
			return !guiElem->getOptionFlags().isSet(GUIElementOption::ClickThrough);
		}

		return false;
	}

	void ScriptGUIElement::InternalSetBlocking(ScriptGUIElementBaseTBase* nativeInstance, bool blocking)
	{
		if (nativeInstance->isDestroyed())
			return;

		GUIElementBase* guiElemBase = nativeInstance->getGUIElement();
		if (guiElemBase->GetTypeInternal() == GUIElementBase::Type::Element)
		{
			GUIElement* guiElem = static_cast<GUIElement*>(guiElemBase);

			GUIElementOptions options = guiElem->getOptionFlags();
			if(blocking)
				options.unset(GUIElementOption::ClickThrough);
			else
				options.set(GUIElementOption::ClickThrough);

			guiElem->setOptionFlags(options);
		}
	}

	bool ScriptGUIElement::InternalGetAcceptsKeyFocus(ScriptGUIElementBaseTBase* nativeInstance)
	{
		if (nativeInstance->isDestroyed())
			return false;

		GUIElementBase* guiElemBase = nativeInstance->getGUIElement();
		if (guiElemBase->GetTypeInternal() == GUIElementBase::Type::Element)
		{
			GUIElement* guiElem = static_cast<GUIElement*>(guiElemBase);
			return guiElem->getOptionFlags().isSet(GUIElementOption::AcceptsKeyFocus);
		}

		return false;
	}

	void ScriptGUIElement::InternalSetAcceptsKeyFocus(ScriptGUIElementBaseTBase* nativeInstance, bool accepts)
	{
		if (nativeInstance->isDestroyed())
			return;

		GUIElementBase* guiElemBase = nativeInstance->getGUIElement();
		if (guiElemBase->GetTypeInternal() == GUIElementBase::Type::Element)
		{
			GUIElement* guiElem = static_cast<GUIElement*>(guiElemBase);

			GUIElementOptions options = guiElem->getOptionFlags();
			if(accepts)
				options.set(GUIElementOption::AcceptsKeyFocus);
			else
				options.unset(GUIElementOption::AcceptsKeyFocus);

			guiElem->setOptionFlags(options);
		}
	}

	MonoObject* ScriptGUIElement::InternalGetParent(ScriptGUIElementBaseTBase* nativeInstance)
	{
		if (nativeInstance->isDestroyed())
			return nullptr;

		if (nativeInstance->getParent() != nullptr)
			return nativeInstance->getParent()->getManagedInstance();

		return nullptr;
	}

	void ScriptGUIElement::InternalGetBounds(ScriptGUIElementBaseTBase* nativeInstance, Rect2I* bounds)
	{
		if (nativeInstance->isDestroyed())
		{
			*bounds = Rect2I();
			return;
		}

		*bounds = nativeInstance->getGUIElement()->getBounds();
	}

	void ScriptGUIElement::InternalSetBounds(ScriptGUIElementBaseTBase* nativeInstance, Rect2I* bounds)
	{
		if (nativeInstance->isDestroyed())
			return;

		nativeInstance->getGUIElement()->setPosition(bounds->x, bounds->y);
		nativeInstance->getGUIElement()->setWidth(bounds->width);
		nativeInstance->getGUIElement()->setHeight(bounds->height);
	}

	void ScriptGUIElement::InternalGetVisibleBounds(ScriptGUIElementBaseTBase* nativeInstance, Rect2I* bounds)
	{
		if (nativeInstance->isDestroyed())
		{
			*bounds = Rect2I();
			return;
		}

		*bounds = nativeInstance->getGUIElement()->getVisibleBounds();
	}

	void ScriptGUIElement::InternalGetScreenBounds(ScriptGUIElementBaseTBase* nativeInstance, Rect2I* bounds)
	{
		if (nativeInstance->isDestroyed())
		{
			*bounds = Rect2I();
			return;
		}

		*bounds = nativeInstance->getGUIElement()->getScreenBounds();
	}

	void ScriptGUIElement::InternalSetPosition(ScriptGUIElementBaseTBase* nativeInstance, INT32 x, INT32 y)
	{
		if (nativeInstance->isDestroyed())
			return;

		nativeInstance->getGUIElement()->setPosition(x, y);
	}

	void ScriptGUIElement::InternalSetWidth(ScriptGUIElementBaseTBase* nativeInstance, UINT32 width)
	{
		if (nativeInstance->isDestroyed())
			return;

		nativeInstance->getGUIElement()->setWidth(width);
	}

	void ScriptGUIElement::InternalSetFlexibleWidth(ScriptGUIElementBaseTBase* nativeInstance, UINT32 minWidth, UINT32 maxWidth)
	{
		if (nativeInstance->isDestroyed())
			return;

		nativeInstance->getGUIElement()->setFlexibleWidth(minWidth, maxWidth);
	}

	void ScriptGUIElement::InternalSetHeight(ScriptGUIElementBaseTBase* nativeInstance, UINT32 height)
	{
		if (nativeInstance->isDestroyed())
			return;

		nativeInstance->getGUIElement()->setHeight(height);
	}

	void ScriptGUIElement::InternalSetFlexibleHeight(ScriptGUIElementBaseTBase* nativeInstance, UINT32 minHeight, UINT32 maxHeight)
	{
		if (nativeInstance->isDestroyed())
			return;

		nativeInstance->getGUIElement()->setFlexibleHeight(minHeight, maxHeight);
	}

	void ScriptGUIElement::InternalResetDimensions(ScriptGUIElementBaseTBase* nativeInstance)
	{
		if (nativeInstance->isDestroyed())
			return;

		nativeInstance->getGUIElement()->resetDimensions();
	}

	void ScriptGUIElement::InternalSetContextMenu(ScriptGUIElementBaseTBase* nativeInstance, ScriptContextMenu* contextMenu)
	{
		if (nativeInstance->isDestroyed())
			return;

		GUIElementBase* guiElemBase = nativeInstance->getGUIElement();
		if (guiElemBase->GetTypeInternal() == GUIElementBase::Type::Element)
		{
			GUIElement* guiElem = static_cast<GUIElement*>(guiElemBase);

			SPtr<GUIContextMenu> nativeContextMenu;
			if (contextMenu != nullptr)
				nativeContextMenu = contextMenu->getInternal();

			guiElem->setContextMenu(nativeContextMenu);
		}
	}

	MonoString* ScriptGUIElement::InternalGetStyle(ScriptGUIElementBaseTBase* nativeInstance)
	{
		if (!nativeInstance->isDestroyed())
		{
			GUIElementBase* guiElemBase = nativeInstance->getGUIElement();
			if (guiElemBase->GetTypeInternal() == GUIElementBase::Type::Element)
			{
				GUIElement* guiElem = static_cast<GUIElement*>(guiElemBase);
				return MonoUtil::stringToMono(guiElem->getStyleName());
			}
		}

		return MonoUtil::stringToMono(StringUtil::BLANK);
	}

	void ScriptGUIElement::InternalSetStyle(ScriptGUIElementBaseTBase* nativeInstance, MonoString* style)
	{
		if (!nativeInstance->isDestroyed())
		{
			GUIElementBase* guiElemBase = nativeInstance->getGUIElement();
			if (guiElemBase->GetTypeInternal() == GUIElementBase::Type::Element)
			{
				GUIElement* guiElem = static_cast<GUIElement*>(guiElemBase);
				guiElem->setStyle(MonoUtil::monoToString(style));
			}
		}
	}
}
