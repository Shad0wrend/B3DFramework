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
		mGUIWidget->SetSkin(BuiltinResources::Instance().getGUISkin());

		MonoObject* guiPanel = ScriptGUIPanel::createFromExisting(mGUIWidget->GetPanel());
		mPanel = ScriptGUILayout::toNative(guiPanel);

		sGUIPanelField->set(managedInstance, guiPanel);
	}

	ScriptGUIWidget::~ScriptGUIWidget()
	{ }

	void ScriptGUIWidget::initRuntimeData()
	{
		sGUIPanelField = metaData.scriptClass->GetField("panel");

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
		HSceneObject parentSO = parent->GetHandle();

		SPtr<GUIWidget> widget = thisPtr->GetInternal();
		if (!parentSO.isDestroyed() && widget != nullptr)
		{
			widget->UpdateTransformInternal(parentSO);
			widget->UpdateRTInternal();

			if (parentSO->GetActive() != widget->GetIsActive())
				widget->SetIsActive(parentSO->GetActive());
		}
	}

	void ScriptGUIWidget::InternalUpdateMainCamera(ScriptGUIWidget* instance, ScriptCCamera* camera)
	{
		SPtr<GUIWidget> widget = instance->GetInternal();

		if (widget != nullptr)
		{
			SPtr<Camera> nativeCamera;
			if (camera != nullptr)
				nativeCamera = camera->GetHandle()->GetCameraInternal();

			widget->SetCamera(nativeCamera);
		}
	}

	void ScriptGUIWidget::InternalSetSkin(ScriptGUIWidget* instance, ScriptGUISkin* skin)
	{
		HGUISkin guiSkin;
		if (skin != nullptr)
			guiSkin = skin->GetHandle();

		if (!guiSkin.IsLoaded())
			guiSkin = BuiltinResources::Instance().getGUISkin();

		SPtr<GUIWidget> widget = instance->GetInternal();
		if (widget != nullptr)
			widget->SetSkin(guiSkin);
	}

	void ScriptGUIWidget::InternalSetCamera(ScriptGUIWidget* instance, ScriptCCamera* camera)
	{
		SPtr<Camera> nativeCamera;
		if (camera != nullptr)
			nativeCamera = camera->GetHandle()->GetCameraInternal();

		if(nativeCamera == nullptr)
			nativeCamera = gSceneManager().getMainCamera();

		SPtr<GUIWidget> widget = instance->GetInternal();
		if(widget != nullptr)
			widget->SetCamera(nativeCamera);
	}

	void ScriptGUIWidget::InternalSetDepth(ScriptGUIWidget* instance, INT8 value)
	{
		instance->GetInternal()->SetDepth(value);
	}

	INT8 ScriptGUIWidget::InternalGetDepth(ScriptGUIWidget* instance)
	{
		return instance->GetInternal()->GetDepth();
	}

	void ScriptGUIWidget::InternalDestroy(ScriptGUIWidget* instance)
	{
		instance->Destroy();
	}

	void ScriptGUIWidget::Destroy(bool destroyPanel)
	{
		if(mPanel != nullptr && destroyPanel)
		{
			mPanel->Destroy();
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
