//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/GUI/BsScriptGUIWidget.h"
#include "Wrappers/BsScriptSceneObject.h"
#include "Wrappers/GUI/BsScriptGUILayout.h"
#include "GUI/BsGUIWidget.h"
#include "Scene/BsSceneObject.h"
#include "Scene/BsSceneManager.h"
#include "Resources/BsBuiltinResources.h"
#include "Components/BsCCamera.h"

#include "Generated/BsScriptCCamera.generated.h"
#include "Generated/BsScriptGUISkin.generated.h"

namespace bs
{
	MonoField* ScriptGUIWidget::sGUIPanelField = nullptr;

	ScriptGUIWidget::ScriptGUIWidget(MonoObject* managedInstance)
		:ScriptObject(managedInstance), mGUIWidget(nullptr)
	{
		SPtr<Camera> mainCamera = gSceneManager().getMainCamera();

		mGUIWidget = GUIWidget::Create(mainCamera);
		mGUIWidget->setSkin(BuiltinResources::Instance().getGUISkin());

		MonoObject* guiPanel = ScriptGUIPanel::createFromExisting(mGUIWidget->getPanel());
		mPanel = ScriptGUILayout::toNative(guiPanel);

		sGUIPanelField->set(managedInstance, guiPanel);
	}

	ScriptGUIWidget::~ScriptGUIWidget()
	{ }

	void ScriptGUIWidget::initRuntimeData()
	{
		sGUIPanelField = metaData.scriptClass->getField("panel");

		metaData.scriptClass->addInternalCall("Internal_Create", (void*)&ScriptGUIWidget::InternalCreate);
		metaData.scriptClass->addInternalCall("Internal_UpdateTransform", (void*)&ScriptGUIWidget::InternalUpdateTransform);
		metaData.scriptClass->addInternalCall("Internal_UpdateMainCamera", (void*)&ScriptGUIWidget::InternalUpdateMainCamera);
		metaData.scriptClass->addInternalCall("Internal_SetSkin", (void*)&ScriptGUIWidget::InternalSetSkin);
		metaData.scriptClass->addInternalCall("Internal_SetCamera", (void*)&ScriptGUIWidget::InternalSetCamera);
		metaData.scriptClass->addInternalCall("Internal_SetDepth", (void*)&ScriptGUIWidget::InternalSetDepth);
		metaData.scriptClass->addInternalCall("Internal_GetDepth", (void*)&ScriptGUIWidget::InternalGetDepth);
		metaData.scriptClass->addInternalCall("Internal_Destroy", (void*)&ScriptGUIWidget::InternalDestroy);
	}

	void ScriptGUIWidget::InternalCreate(MonoObject* managedInstance)
	{
		new (bs_alloc<ScriptGUIWidget>()) ScriptGUIWidget(managedInstance);
	}

	void ScriptGUIWidget::InternalUpdateTransform(ScriptGUIWidget* thisPtr, ScriptSceneObject* parent)
	{
		HSceneObject parentSO = parent->getHandle();

		SPtr<GUIWidget> widget = thisPtr->getInternal();
		if (!parentSO.isDestroyed() && widget != nullptr)
		{
			widget->UpdateTransformInternal(parentSO);
			widget->UpdateRTInternal();

			if (parentSO->getActive() != widget->getIsActive())
				widget->setIsActive(parentSO->getActive());
		}
	}

	void ScriptGUIWidget::InternalUpdateMainCamera(ScriptGUIWidget* instance, ScriptCCamera* camera)
	{
		SPtr<GUIWidget> widget = instance->getInternal();

		if (widget != nullptr)
		{
			SPtr<Camera> nativeCamera;
			if (camera != nullptr)
				nativeCamera = camera->getHandle()->GetCameraInternal();

			widget->setCamera(nativeCamera);
		}
	}

	void ScriptGUIWidget::InternalSetSkin(ScriptGUIWidget* instance, ScriptGUISkin* skin)
	{
		HGUISkin guiSkin;
		if (skin != nullptr)
			guiSkin = skin->getHandle();

		if (!guiSkin.isLoaded())
			guiSkin = BuiltinResources::Instance().getGUISkin();

		SPtr<GUIWidget> widget = instance->getInternal();
		if (widget != nullptr)
			widget->setSkin(guiSkin);
	}

	void ScriptGUIWidget::InternalSetCamera(ScriptGUIWidget* instance, ScriptCCamera* camera)
	{
		SPtr<Camera> nativeCamera;
		if (camera != nullptr)
			nativeCamera = camera->getHandle()->GetCameraInternal();

		if(nativeCamera == nullptr)
			nativeCamera = gSceneManager().getMainCamera();

		SPtr<GUIWidget> widget = instance->getInternal();
		if(widget != nullptr)
			widget->setCamera(nativeCamera);
	}

	void ScriptGUIWidget::InternalSetDepth(ScriptGUIWidget* instance, INT8 value)
	{
		instance->getInternal()->setDepth(value);
	}

	INT8 ScriptGUIWidget::InternalGetDepth(ScriptGUIWidget* instance)
	{
		return instance->getInternal()->getDepth();
	}

	void ScriptGUIWidget::InternalDestroy(ScriptGUIWidget* instance)
	{
		instance->Destroy();
	}

	void ScriptGUIWidget::Destroy(bool destroyPanel)
	{
		if(mPanel != nullptr && destroyPanel)
		{
			mPanel->destroy();
			mPanel = nullptr;
		}

		if (mGUIWidget != nullptr)
		{
			mGUIWidget->DestroyInternal();
			mGUIWidget = nullptr;
		}
	}

	void ScriptGUIWidget::OnManagedInstanceDeletedInternal(bool assemblyRefresh)
	{
		Destroy(false);

		ScriptObject::OnManagedInstanceDeletedInternal(assemblyRefresh);
	}
}
