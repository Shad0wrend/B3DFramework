//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/GUI/BsScriptGUI.h"
#include "Wrappers/GUI/BsScriptGUILayout.h"
#include "GUI/BsGUIWidget.h"
#include "Scene/BsSceneManager.h"
#include "BsScriptObjectManager.h"
#include "Resources/BsBuiltinResources.h"
#include "BsMonoMethod.h"

#include "Generated/BsScriptGUISkin.generated.h"

namespace bs
{
	SPtr<GUIWidget> ScriptGUI::sGUIWidget;
	ScriptGUILayout* ScriptGUI::sPanel = nullptr;
	HEvent ScriptGUI::sDomainUnloadConn;
	HEvent ScriptGUI::sDomainLoadConn;
	MonoMethod* ScriptGUI::sGUIPanelMethod = nullptr;

	ScriptGUI::ScriptGUI(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	ScriptGUI::~ScriptGUI()
	{ }

	void ScriptGUI::StartUp()
	{
		SPtr<Camera> mainCamera = gSceneManager().GetMainCamera();
		sGUIWidget = GUIWidget::Create(mainCamera);
		sGUIWidget->SetSkin(BuiltinResources::Instance().GetGuiSkin());

		auto createPanel = [] ()
		{
			assert(sPanel == nullptr);

			MonoObject* guiPanel = ScriptGUIPanel::createFromExisting(sGUIWidget->getPanel());
			sPanel = ScriptGUILayout::toNative(guiPanel);

			void* params[1];
			params[0] = guiPanel;

			sGUIPanelMethod->invoke(nullptr, params);
		};

		auto clearPanel = [] ()
		{
			sPanel = nullptr;
		};

		createPanel();

		sDomainLoadConn = ScriptObjectManager::Instance().onRefreshDomainLoaded.connect(createPanel);
		sDomainUnloadConn = MonoManager::Instance().onDomainUnload.connect(clearPanel);
	}

	void ScriptGUI::Update()
	{
		if (sGUIWidget == nullptr)
			return;

		SPtr<Camera> mainCamera = gSceneManager().getMainCamera();
		if (mainCamera != sGUIWidget->getCamera())
			sGUIWidget->setCamera(mainCamera);

		sGUIWidget->UpdateRTInternal();
	}

	void ScriptGUI::ShutDown()
	{
		sDomainLoadConn.disconnect();
		sDomainUnloadConn.disconnect();

		if (sPanel != nullptr)
		{
			sPanel->destroy();
			sPanel = nullptr;
		}

		if (sGUIWidget != nullptr)
		{
			sGUIWidget->DestroyInternal();
			sGUIWidget = nullptr;
		}
	}

	void ScriptGUI::initRuntimeData()
	{
		sGUIPanelMethod = metaData.scriptClass->getMethod("SetPanel", 1);

		metaData.scriptClass->addInternalCall("Internal_SetSkin", (void*)&ScriptGUI::internal_SetSkin);
	}

	void ScriptGUI::InternalSetSkin(ScriptGUISkin* skin)
	{
		HGUISkin guiSkin;
		if (skin != nullptr)
			guiSkin = skin->getHandle();

		if (!guiSkin.isLoaded())
			guiSkin = BuiltinResources::Instance().getGUISkin();

		if(sGUIWidget != nullptr)
			sGUIWidget->setSkin(guiSkin);
	}
}
