//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/GUI/BsScriptGUIListBox.h"
#include "BsScriptMeta.h"
#include "BsMonoField.h"
#include "BsMonoClass.h"
#include "BsMonoMethod.h"
#include "BsMonoManager.h"
#include "Image/BsSpriteTexture.h"
#include "BsMonoUtil.h"
#include "GUI/BsGUILayout.h"
#include "GUI/BsGUIListBox.h"
#include "GUI/BsGUIOptions.h"
#include "Wrappers/GUI/BsScriptGUILayout.h"

#include "Generated/BsScriptHString.generated.h"
#include "Generated/BsScriptGUIElementStyle.generated.h"

using namespace std::placeholders;

namespace bs
{
	ScriptGUIListBox::OnSelectionChangedThunkDef ScriptGUIListBox::onSelectionChangedThunk;

	ScriptGUIListBox::ScriptGUIListBox(MonoObject* instance, GUIListBox* listBox)
		:TScriptGUIElement(instance, listBox)
	{

	}

	void ScriptGUIListBox::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_CreateInstance", (void*)&ScriptGUIListBox::InternalCreateInstance);
		metaData.scriptClass->AddInternalCall("Internal_SetElements", (void*)&ScriptGUIListBox::InternalSetElements);
		metaData.scriptClass->AddInternalCall("Internal_SetTint", (void*)&ScriptGUIListBox::InternalSetTint);
		metaData.scriptClass->AddInternalCall("Internal_SelectElement", (void*)&ScriptGUIListBox::InternalSelectElement);
		metaData.scriptClass->AddInternalCall("Internal_DeselectElement", (void*)&ScriptGUIListBox::InternalDeselectElement);
		metaData.scriptClass->AddInternalCall("Internal_GetElementStates", (void*)&ScriptGUIListBox::InternalGetElementStates);
		metaData.scriptClass->AddInternalCall("Internal_SetElementStates", (void*)&ScriptGUIListBox::InternalSetElementStates);

		onSelectionChangedThunk = (OnSelectionChangedThunkDef)metaData.scriptClass->getMethod("DoOnSelectionChanged", 1)->getThunk();
	}

	void ScriptGUIListBox::InternalCreateInstance(MonoObject* instance, MonoArray* elements, bool multiselect,
		MonoString* style, MonoArray* guiOptions)
	{
		GUIOptions options;

		ScriptArray scriptArray(guiOptions);
		UINT32 arrayLen = scriptArray.size();
		for (UINT32 i = 0; i < arrayLen; i++)
			options.addOption(scriptArray.get<GUIOption>(i));

		ScriptArray elemsArray(elements);
		UINT32 elementsArrayLen = elemsArray.size();
		Vector<HString> nativeElements;
		for(UINT32 i = 0; i < elementsArrayLen; i++)
		{
			MonoObject* stringManaged = elemsArray.get<MonoObject*>(i);

			if(stringManaged == nullptr)
				nativeElements.push_back(HString::dummy());
			else
			{
				ScriptHString* textScript = ScriptHString::toNative(stringManaged);
				nativeElements.push_back(*textScript->getInternal());
			}
		}

		GUIListBox* guiListBox = GUIListBox::Create(nativeElements, multiselect, options, MonoUtil::monoToString(style));

		auto nativeInstance = new (bs_alloc<ScriptGUIListBox>()) ScriptGUIListBox(instance, guiListBox);

		guiListBox->onSelectionToggled.connect(std::bind(&ScriptGUIListBox::onSelectionChanged, nativeInstance, _1, _2));
	}

	void ScriptGUIListBox::InternalSetElements(ScriptGUIListBox* nativeInstance, MonoArray* elements)
	{
		ScriptArray elemsArray(elements);
		UINT32 elementsArrayLen = elemsArray.size();
		Vector<HString> nativeElements;
		for(UINT32 i = 0; i < elementsArrayLen; i++)
		{
			MonoObject* stringManaged = elemsArray.get<MonoObject*>(i);

			if(stringManaged == nullptr)
				nativeElements.push_back(HString::dummy());
			else
			{
				ScriptHString* textScript = ScriptHString::toNative(stringManaged);
				nativeElements.push_back(*textScript->getInternal());
			}
		}

		GUIListBox* listBox = (GUIListBox*)nativeInstance->getGUIElement();
		listBox->setElements(nativeElements);
	}

	void ScriptGUIListBox::InternalSetTint(ScriptGUIListBox* nativeInstance, Color* color)
	{
		GUIListBox* listBox = (GUIListBox*)nativeInstance->getGUIElement();
		listBox->setTint(*color);
	}

	void ScriptGUIListBox::InternalSelectElement(ScriptGUIListBox* nativeInstance, int idx)
	{
		GUIListBox* listBox = (GUIListBox*)nativeInstance->getGUIElement();
		listBox->selectElement(idx);
	}

	void ScriptGUIListBox::InternalDeselectElement(ScriptGUIListBox* nativeInstance, int idx)
	{
		GUIListBox* listBox = (GUIListBox*)nativeInstance->getGUIElement();
		listBox->deselectElement(idx);
	}

	MonoArray* ScriptGUIListBox::InternalGetElementStates(ScriptGUIListBox* nativeInstance)
	{
		GUIListBox* listBox = (GUIListBox*)nativeInstance->getGUIElement();
		const Vector<bool>& states = listBox->getElementStates();

		UINT32 numElements = (UINT32)states.size();
		ScriptArray outStates = ScriptArray::create<bool>(numElements);

		for (UINT32 i = 0; i < numElements; i++)
			outStates.set(i, (bool)states[i]);

		return outStates.getInternal();
	}

	void ScriptGUIListBox::InternalSetElementStates(ScriptGUIListBox* nativeInstance, MonoArray* monoStates)
	{
		if (monoStates == nullptr)
			return;

		ScriptArray inStates(monoStates);
		UINT32 numElements = inStates.size();

		Vector<bool> states(numElements);
		for (UINT32 i = 0; i < numElements; i++)
			states[i] = inStates.get<bool>(i);

		GUIListBox* listBox = (GUIListBox*)nativeInstance->getGUIElement();
		listBox->setElementStates(states);
	}

	void ScriptGUIListBox::OnSelectionChanged(UINT32 index, bool enabled)
	{
		MonoUtil::invokeThunk(onSelectionChangedThunk, getManagedInstance(), index);
	}
}
