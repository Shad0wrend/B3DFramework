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
		onHoverThunk = (OnHoverThunkDef)metaData.scriptClass->GetMethod("DoOnHover")->GetThunk();
		onOutThunk = (OnOutThunkDef)metaData.scriptClass->GetMethod("DoOnOut")->GetThunk();
		onToggledThunk = (OnToggledThunkDef)metaData.scriptClass->GetMethod("DoOnToggled", 1)->GetThunk();
		onDoubleClickThunk = (OnDoubleClickThunkDef)metaData.scriptClass->GetMethod("DoOnDoubleClick")->GetThunk();
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
			toggleGroup = scriptToggleGroup->GetInternalValue();
		}

		GUIContent nativeContent = ScriptGUIContent::fromInterop(*content);
		GUIToggle* guiToggle = GUIToggle::Create(nativeContent, toggleGroup, options, MonoUtil::monoToString(style));

		auto nativeInstance = new (bs_alloc<ScriptGUIToggle>()) ScriptGUIToggle(instance, guiToggle);

		guiToggle->onClick.Connect(std::bind(&ScriptGUIToggle::onClick, nativeInstance));
		guiToggle->onHover.Connect(std::bind(&ScriptGUIToggle::onHover, nativeInstance));
		guiToggle->onOut.Connect(std::bind(&ScriptGUIToggle::onOut, nativeInstance));
		guiToggle->onToggled.Connect(std::bind(&ScriptGUIToggle::onToggled, nativeInstance, std::placeholders::_1));
		guiToggle->onDoubleClick.Connect(std::bind(&ScriptGUIToggle::onDoubleClick, nativeInstance));
	}

	void ScriptGUIToggle::InternalSetContent(ScriptGUIToggle* nativeInstance, __GUIContentInterop* content)
	{
		GUIContent nativeContent = ScriptGUIContent::fromInterop(*content);

		GUIToggle* toggle = (GUIToggle*)nativeInstance->GetGUIElement();
		toggle->SetContent(nativeContent);
	}

	bool ScriptGUIToggle::InternalGetValue(ScriptGUIToggle* nativeInstance)
	{
		GUIToggle* toggle = (GUIToggle*)nativeInstance->GetGUIElement();
		return toggle->isToggled();
	}

	void ScriptGUIToggle::InternalSetValue(ScriptGUIToggle* nativeInstance, bool value)
	{
		GUIToggle* toggle = (GUIToggle*)nativeInstance->GetGUIElement();

		if (value)
			toggle->toggleOn();
		else
			toggle->toggleOff();
	}

	void ScriptGUIToggle::InternalSetTint(ScriptGUIToggle* nativeInstance, Color* color)
	{
		GUIToggle* toggle = (GUIToggle*)nativeInstance->GetGUIElement();
		toggle->SetTint(*color);
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
