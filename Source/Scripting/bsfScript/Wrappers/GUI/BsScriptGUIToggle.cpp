//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/GUI/BsScriptGUIToggle.h"
#include "BsScriptMeta.h"
#include "BsMonoField.h"
#include "BsMonoClass.h"
#include "BsMonoManager.h"
#include "BsMonoMethod.h"
#include "Image/BsSpriteTexture.h"
#include "BsMonoUtil.h"
#include "GUI/BsGUILayout.h"
#include "GUI/BsGUIToggle.h"
#include "GUI/BsGUIToggleGroup.h"
#include "GUI/BsGUIOptions.h"
#include "Wrappers/GUI/BsScriptGUIToggleGroup.h"
#include "Wrappers/GUI/BsScriptGUILayout.h"

#include "Generated/BsScriptHString.generated.h"
#include "Generated/BsScriptGUIContent.generated.h"
#include "Generated/BsScriptGUIElementStyle.generated.h"

using namespace std::placeholders;

namespace bs
{
	ScriptGUIToggle::OnClickThunkDef ScriptGUIToggle::onClickThunk;
	ScriptGUIToggle::OnHoverThunkDef ScriptGUIToggle::onHoverThunk;
	ScriptGUIToggle::OnOutThunkDef ScriptGUIToggle::onOutThunk;
	ScriptGUIToggle::OnToggledThunkDef ScriptGUIToggle::onToggledThunk;
	ScriptGUIToggle::OnDoubleClickThunkDef ScriptGUIToggle::onDoubleClickThunk;

	ScriptGUIToggle::ScriptGUIToggle(MonoObject* instance, GUIToggle* toggle)
		:TScriptGUIElement(instance, toggle)
	{

	}

	void ScriptGUIToggle::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_CreateInstance", (void*)&ScriptGUIToggle::InternalCreateInstance);
		metaData.scriptClass->AddInternalCall("Internal_SetContent", (void*)&ScriptGUIToggle::InternalSetContent);
		metaData.scriptClass->AddInternalCall("Internal_GetValue", (void*)&ScriptGUIToggle::InternalGetValue);
		metaData.scriptClass->AddInternalCall("Internal_SetValue", (void*)&ScriptGUIToggle::InternalSetValue);
		metaData.scriptClass->AddInternalCall("Internal_SetTint", (void*)&ScriptGUIToggle::InternalSetTint);

		onClickThunk = (OnClickThunkDef)metaData.scriptClass->GetMethod("DoOnClick")->GetThunk();
		onHoverThunk = (OnHoverThunkDef)metaData.scriptClass->getMethod("DoOnHover")->getThunk();
		onOutThunk = (OnOutThunkDef)metaData.scriptClass->getMethod("DoOnOut")->getThunk();
		onToggledThunk = (OnToggledThunkDef)metaData.scriptClass->getMethod("DoOnToggled", 1)->getThunk();
		onDoubleClickThunk = (OnDoubleClickThunkDef)metaData.scriptClass->getMethod("DoOnDoubleClick")->getThunk();
	}

	void ScriptGUIToggle::InternalCreateInstance(MonoObject* instance, __GUIContentInterop* content,
		MonoObject* monoToggleGroup, MonoString* style, MonoArray* guiOptions)
	{
		GUIOptions options;

		ScriptArray scriptArray(guiOptions);
		UINT32 arrayLen = scriptArray.size();
		for (UINT32 i = 0; i < arrayLen; i++)
			options.addOption(scriptArray.get<GUIOption>(i));

		ScriptGUIToggleGroup* scriptToggleGroup = nullptr;
		SPtr<GUIToggleGroup> toggleGroup;
		if (monoToggleGroup != nullptr)
		{
			scriptToggleGroup = ScriptGUIToggleGroup::toNative(monoToggleGroup);
			toggleGroup = scriptToggleGroup->getInternalValue();
		}

		GUIContent nativeContent = ScriptGUIContent::fromInterop(*content);
		GUIToggle* guiToggle = GUIToggle::Create(nativeContent, toggleGroup, options, MonoUtil::monoToString(style));

		auto nativeInstance = new (bs_alloc<ScriptGUIToggle>()) ScriptGUIToggle(instance, guiToggle);

		guiToggle->onClick.connect(std::bind(&ScriptGUIToggle::onClick, nativeInstance));
		guiToggle->onHover.connect(std::bind(&ScriptGUIToggle::onHover, nativeInstance));
		guiToggle->onOut.connect(std::bind(&ScriptGUIToggle::onOut, nativeInstance));
		guiToggle->onToggled.connect(std::bind(&ScriptGUIToggle::onToggled, nativeInstance, std::placeholders::_1));
		guiToggle->onDoubleClick.connect(std::bind(&ScriptGUIToggle::onDoubleClick, nativeInstance));
	}

	void ScriptGUIToggle::InternalSetContent(ScriptGUIToggle* nativeInstance, __GUIContentInterop* content)
	{
		GUIContent nativeContent = ScriptGUIContent::fromInterop(*content);

		GUIToggle* toggle = (GUIToggle*)nativeInstance->getGUIElement();
		toggle->setContent(nativeContent);
	}

	bool ScriptGUIToggle::InternalGetValue(ScriptGUIToggle* nativeInstance)
	{
		GUIToggle* toggle = (GUIToggle*)nativeInstance->getGUIElement();
		return toggle->isToggled();
	}

	void ScriptGUIToggle::InternalSetValue(ScriptGUIToggle* nativeInstance, bool value)
	{
		GUIToggle* toggle = (GUIToggle*)nativeInstance->getGUIElement();

		if (value)
			toggle->toggleOn();
		else
			toggle->toggleOff();
	}

	void ScriptGUIToggle::InternalSetTint(ScriptGUIToggle* nativeInstance, Color* color)
	{
		GUIToggle* toggle = (GUIToggle*)nativeInstance->getGUIElement();
		toggle->setTint(*color);
	}

	void ScriptGUIToggle::OnClick()
	{
		MonoUtil::invokeThunk(onClickThunk, getManagedInstance());
	}

	void ScriptGUIToggle::OnHover()
	{
		MonoUtil::invokeThunk(onHoverThunk, getManagedInstance());
	}

	void ScriptGUIToggle::OnOut()
	{
		MonoUtil::invokeThunk(onOutThunk, getManagedInstance());
	}

	void ScriptGUIToggle::OnToggled(bool toggled)
	{
		MonoUtil::invokeThunk(onToggledThunk, getManagedInstance(), toggled);
	}

	void ScriptGUIToggle::OnDoubleClick()
	{
		MonoUtil::invokeThunk(onDoubleClickThunk, getManagedInstance());
	}
}
