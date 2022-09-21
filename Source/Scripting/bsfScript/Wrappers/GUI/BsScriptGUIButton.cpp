//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/GUI/BsScriptGUIButton.h"
#include "BsScriptMeta.h"
#include "BsMonoField.h"
#include "BsMonoClass.h"
#include "BsMonoManager.h"
#include "BsMonoMethod.h"
#include "Image/BsSpriteTexture.h"
#include "BsMonoUtil.h"
#include "GUI/BsGUILayout.h"
#include "GUI/BsGUIButton.h"
#include "GUI/BsGUIOptions.h"
#include "Wrappers/GUI/BsScriptGUILayout.h"

#include "Generated/BsScriptHString.generated.h"
#include "Generated/BsScriptGUIContent.generated.h"
#include "Generated/BsScriptGUIElementStyle.generated.h"

namespace bs
{
	ScriptGUIButton::OnClickThunkDef ScriptGUIButton::onClickThunk;
	ScriptGUIButton::OnHoverThunkDef ScriptGUIButton::onHoverThunk;
	ScriptGUIButton::OnOutThunkDef ScriptGUIButton::onOutThunk;
	ScriptGUIButton::OnDoubleClickThunkDef ScriptGUIButton::onDoubleClickThunk;

	ScriptGUIButton::ScriptGUIButton(MonoObject* instance, GUIButton* button)
		:TScriptGUIElement(instance, button)
	{

	}

	void ScriptGUIButton::InitRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_CreateInstance", (void*)&ScriptGUIButton::InternalCreateInstance);
		metaData.scriptClass->AddInternalCall("Internal_SetContent", (void*)&ScriptGUIButton::InternalSetContent);
		metaData.scriptClass->AddInternalCall("Internal_SetTint", (void*)&ScriptGUIButton::InternalSetTint);

		onClickThunk = (OnClickThunkDef)metaData.scriptClass->GetMethod("DoOnClick")->GetThunk();
		onDoubleClickThunk = (OnDoubleClickThunkDef)metaData.scriptClass->GetMethod("DoOnDoubleClick")->GetThunk();
		onHoverThunk = (OnHoverThunkDef)metaData.scriptClass->GetMethod("DoOnHover")->GetThunk();
		onOutThunk = (OnOutThunkDef)metaData.scriptClass->GetMethod("DoOnOut")->GetThunk();
	}

	void ScriptGUIButton::InternalCreateInstance(MonoObject* instance, __GUIContentInterop* content,
		MonoString* style, MonoArray* guiOptions)
	{
		GUIOptions options;

		ScriptArray scriptArray(guiOptions);
		UINT32 arrayLen = scriptArray.Size();
		for(UINT32 i = 0; i < arrayLen; i++)
			options.AddOption(scriptArray.Get<GUIOption>(i));

		GUIContent nativeContent = ScriptGUIContent::FromInterop(*content);
		GUIButton* guiButton = GUIButton::Create(nativeContent, options, MonoUtil::MonoToString(style));

		auto nativeInstance = new (bs_alloc<ScriptGUIButton>()) ScriptGUIButton(instance, guiButton);

		guiButton->onClick.Connect(std::bind(&::bs::ScriptGUIButton::OnClick, nativeInstance));
		guiButton->onDoubleClick.Connect(std::bind(&::bs::ScriptGUIButton::OnDoubleClick, nativeInstance));
		guiButton->onHover.Connect(std::bind(&::bs::ScriptGUIButton::OnHover, nativeInstance));
		guiButton->onOut.Connect(std::bind(&ScriptGUIButton::OnOut, nativeInstance));
	}

	void ScriptGUIButton::InternalSetContent(ScriptGUIButton* nativeInstance, __GUIContentInterop* content)
	{
		GUIContent nativeContent = ScriptGUIContent::FromInterop(*content);

		GUIButton* button = (GUIButton*)nativeInstance->GetGuiElement();
		button->SetContent(nativeContent);
	}

	void ScriptGUIButton::InternalSetTint(ScriptGUIButton* nativeInstance, Color* color)
	{
		GUIButton* button = (GUIButton*)nativeInstance->GetGuiElement();
		button->SetTint(*color);
	}

	void ScriptGUIButton::OnClick()
	{
		MonoUtil::InvokeThunk(onClickThunk, GetManagedInstance());
	}

	void ScriptGUIButton::OnDoubleClick()
	{
		MonoUtil::InvokeThunk(onDoubleClickThunk, GetManagedInstance());
	}

	void ScriptGUIButton::OnHover()
	{
		MonoUtil::InvokeThunk(onHoverThunk, GetManagedInstance());
	}

	void ScriptGUIButton::OnOut()
	{
		MonoUtil::InvokeThunk(onOutThunk, GetManagedInstance());
	}
}
