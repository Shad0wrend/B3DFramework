//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/GUI/BsScriptGUIInputBox.h"
#include "BsScriptMeta.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoManager.h"
#include "BsMonoUtil.h"
#include "GUI/BsGUIInputBox.h"
#include "GUI/BsGUIOptions.h"

using namespace std::placeholders;

namespace bs
{
	ScriptGUIInputBox::OnChangedThunkDef ScriptGUIInputBox::onChangedThunk;
	ScriptGUIInputBox::OnConfirmedThunkDef ScriptGUIInputBox::onConfirmedThunk;

	ScriptGUIInputBox::ScriptGUIInputBox(MonoObject* instance, GUIInputBox* inputBox)
		:TScriptGUIElement(instance, inputBox)
	{

	}

	void ScriptGUIInputBox::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_CreateInstance", (void*)&ScriptGUIInputBox::InternalCreateInstance);
		metaData.scriptClass->AddInternalCall("Internal_GetText", (void*)&ScriptGUIInputBox::InternalGetText);
		metaData.scriptClass->AddInternalCall("Internal_SetText", (void*)&ScriptGUIInputBox::InternalSetText);
		metaData.scriptClass->AddInternalCall("Internal_SetTint", (void*)&ScriptGUIInputBox::InternalSetTint);

		onChangedThunk = (OnChangedThunkDef)metaData.scriptClass->GetMethod("Internal_DoOnChanged", 1)->GetThunk();
		onConfirmedThunk = (OnConfirmedThunkDef)metaData.scriptClass->GetMethod("Internal_DoOnConfirmed", 0)->GetThunk();
	}

	void ScriptGUIInputBox::InternalCreateInstance(MonoObject* instance, bool multiline, MonoString* style, MonoArray* guiOptions)
	{
		GUIOptions options;

		ScriptArray scriptArray(guiOptions);
		UINT32 arrayLen = scriptArray.size();
		for(UINT32 i = 0; i < arrayLen; i++)
			options.addOption(scriptArray.get<GUIOption>(i));

		GUIInputBox* guiInputBox = GUIInputBox::Create(multiline, options, MonoUtil::monoToString(style));

		auto nativeInstance = new (bs_alloc<ScriptGUIInputBox>()) ScriptGUIInputBox(instance, guiInputBox);

		guiInputBox->onValueChanged.Connect(std::bind(&ScriptGUIInputBox::onChanged, nativeInstance, _1));
	}

	void ScriptGUIInputBox::InternalGetText(ScriptGUIInputBox* nativeInstance, MonoString** text)
	{
		GUIInputBox* inputBox = (GUIInputBox*)nativeInstance->GetGUIElement();
		MonoUtil::referenceCopy(text, (MonoObject*)MonoUtil::stringToMono(inputBox->GetText()));
	}

	void ScriptGUIInputBox::InternalSetText(ScriptGUIInputBox* nativeInstance, MonoString* text)
	{
		GUIInputBox* inputBox = (GUIInputBox*)nativeInstance->GetGUIElement();
		inputBox->SetText(MonoUtil::monoToString(text));
	}

	void ScriptGUIInputBox::InternalSetTint(ScriptGUIInputBox* nativeInstance, Color* color)
	{
		GUIInputBox* inputBox = (GUIInputBox*)nativeInstance->GetGUIElement();
		inputBox->SetTint(*color);
	}

	void ScriptGUIInputBox::OnChanged(const String& newValue)
	{
		MonoString* monoValue = MonoUtil::stringToMono(newValue);
		MonoUtil::invokeThunk(onChangedThunk, getManagedInstance(), monoValue);
	}

	void ScriptGUIInputBox::OnConfirmed()
	{
		MonoUtil::invokeThunk(onConfirmedThunk, getManagedInstance());
	}
}
