//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/GUI/BsScriptGUILayout.h"
#include "BsScriptMeta.h"
#include "BsMonoField.h"
#include "BsMonoClass.h"
#include "BsMonoManager.h"
#include "Wrappers/GUI/BsScriptGUIScrollArea.h"
#include "GUI/BsGUILayout.h"
#include "GUI/BsGUILayoutX.h"
#include "GUI/BsGUILayoutY.h"
#include "GUI/BsGUIPanel.h"
#include "GUI/BsGUIScrollArea.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptGUILayout::ScriptGUILayout(MonoObject* instance, GUILayout* layout, bool ownsNative)
		:TScriptGUIElementBase(instance, layout), mLayout(layout), mOwnsNative(ownsNative)
	{ }

	void ScriptGUILayout::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_CreateInstanceX", (void*)&ScriptGUILayout::InternalCreateInstanceX);
		metaData.scriptClass->AddInternalCall("Internal_CreateInstanceY", (void*)&ScriptGUILayout::InternalCreateInstanceY);
		metaData.scriptClass->AddInternalCall("Internal_CreateInstancePanel", (void*)&ScriptGUILayout::InternalCreateInstancePanel);
		metaData.scriptClass->AddInternalCall("Internal_CreateInstanceYFromScrollArea", (void*)&ScriptGUILayout::InternalCreateInstanceYFromScrollArea);
		metaData.scriptClass->AddInternalCall("Internal_AddElement", (void*)&ScriptGUILayout::InternalAddElement);
		metaData.scriptClass->AddInternalCall("Internal_InsertElement", (void*)&ScriptGUILayout::InternalInsertElement);
		metaData.scriptClass->addInternalCall("Internal_GetChildCount", (void*)&ScriptGUILayout::internal_getChildCount);
		metaData.scriptClass->addInternalCall("Internal_GetChild", (void*)&ScriptGUILayout::internal_getChild);
		metaData.scriptClass->addInternalCall("Internal_Clear", (void*)&ScriptGUILayout::internal_clear);
	}

	void ScriptGUILayout::Destroy()
	{
		if(!mIsDestroyed)
		{
			if (mParent != nullptr)
				mParent->removeChild(this);

			while (mChildren.size() > 0)
			{
				ChildInfo childInfo = mChildren[0];
				childInfo.element->Destroy();
			}

			if (mOwnsNative)
				GUILayout::Destroy(mLayout);

			mLayout = nullptr;
			mIsDestroyed = true;
		}
	}

	void ScriptGUILayout::AddChild(ScriptGUIElementBaseTBase* element)
	{
		ChildInfo childInfo;

		childInfo.element = element;
		childInfo.gcHandle = MonoUtil::newGCHandle(element->GetManagedInstance(), false);

		mChildren.push_back(childInfo);
	}

	void ScriptGUILayout::InsertChild(UINT32 idx, ScriptGUIElementBaseTBase* element)
	{
		ChildInfo childInfo;

		childInfo.element = element;
		childInfo.gcHandle = MonoUtil::newGCHandle(element->GetManagedInstance(), false);

		mChildren.insert(mChildren.begin() + idx, childInfo);
	}

	void ScriptGUILayout::RemoveChild(ScriptGUIElementBaseTBase* element)
	{
		auto iterFind = std::find_if(mChildren.begin(), mChildren.end(),
			[&](const ChildInfo& x)
		{
			return x.element == element;
		});

		if (iterFind != mChildren.end())
		{
			assert(iterFind->gcHandle != 0);

			MonoUtil::freeGCHandle(iterFind->gcHandle);
			iterFind->gcHandle = 0;

			mChildren.erase(iterFind);
		}
	}

	void ScriptGUILayout::InternalCreateInstanceX(MonoObject* instance, MonoArray* guiOptions)
	{
		GUIOptions options;

		ScriptArray scriptArray(guiOptions);
		UINT32 arrayLen = scriptArray.size();
		for (UINT32 i = 0; i < arrayLen; i++)
			options.addOption(scriptArray.get<GUIOption>(i));

		GUILayout* layout = GUILayoutX::Create(options);

		new (bs_alloc<ScriptGUILayout>()) ScriptGUILayout(instance, layout);
	}

	void ScriptGUILayout::InternalCreateInstanceY(MonoObject* instance, MonoArray* guiOptions)
	{
		GUIOptions options;

		ScriptArray scriptArray(guiOptions);
		UINT32 arrayLen = scriptArray.size();
		for (UINT32 i = 0; i < arrayLen; i++)
			options.addOption(scriptArray.get<GUIOption>(i));

		GUILayout* layout = GUILayoutY::Create(options);

		new (bs_alloc<ScriptGUILayout>()) ScriptGUILayout(instance, layout);
	}

	void ScriptGUILayout::InternalCreateInstancePanel(MonoObject* instance, INT16 depth, UINT16 depthRangeMin, UINT32 depthRangeMax, MonoArray* guiOptions)
	{
		GUIOptions options;

		ScriptArray scriptArray(guiOptions);
		UINT32 arrayLen = scriptArray.size();
		for (UINT32 i = 0; i < arrayLen; i++)
			options.addOption(scriptArray.get<GUIOption>(i));

		GUILayout* layout = GUIPanel::Create(depth, depthRangeMin, depthRangeMax, options);

		new (bs_alloc<ScriptGUILayout>()) ScriptGUILayout(instance, layout);
	}

	void ScriptGUILayout::InternalCreateInstanceYFromScrollArea(MonoObject* instance, MonoObject* parentScrollArea)
	{
		ScriptGUIScrollArea* scriptScrollArea = ScriptGUIScrollArea::toNative(parentScrollArea);
		GUIScrollArea* scrollArea = (GUIScrollArea*)scriptScrollArea->GetGUIElement();

		GUILayout* nativeLayout = &scrollArea->GetLayout();

		ScriptGUIScrollAreaLayout* nativeInstance = new (bs_alloc<ScriptGUIScrollAreaLayout>())
			ScriptGUIScrollAreaLayout(instance, nativeLayout);

		// This method is expected to be called during GUIScrollArea construction, so we finish its initialization
		scriptScrollArea->Initialize(nativeInstance);
	}

	void ScriptGUILayout::InternalAddElement(ScriptGUILayout* instance, ScriptGUIElementBaseTBase* element)
	{
		if (instance->isDestroyed() || element->isDestroyed())
			return;

		instance->GetInternalValue()->AddElement(element->GetGUIElement());

		if (element->GetParent() != nullptr)
			element->GetParent()->removeChild(element);

		element->SetParent(instance);
		instance->addChild(element);
	}

	void ScriptGUILayout::InternalInsertElement(ScriptGUILayout* instance, UINT32 index, ScriptGUIElementBaseTBase* element)
	{
		if (instance->isDestroyed() || element->isDestroyed())
			return;

		instance->GetInternalValue()->insertElement(index, element->GetGUIElement());

		if (element->GetParent() != nullptr)
			element->GetParent()->removeChild(element);

		element->SetParent(instance);
		instance->insertChild(index, element);
	}

	UINT32 ScriptGUILayout::InternalGetChildCount(ScriptGUILayout* instance)
	{
		if (instance->isDestroyed())
			return 0;

		return instance->mLayout->GetNumChildren();
	}

	MonoObject* ScriptGUILayout::InternalGetChild(ScriptGUILayout* instance, UINT32 index)
	{
		if (instance->isDestroyed() || index >= instance->mChildren.size())
			return nullptr;

		return instance->mChildren[index].element->GetManagedInstance();
	}

	void ScriptGUILayout::InternalClear(ScriptGUILayout* instance)
	{
		if (instance->isDestroyed())
			return;

		for (auto& child : instance->mChildren)
		{
			instance->GetInternalValue()->removeElement(child.element->GetGUIElement());

			assert(child.gcHandle != 0);

			MonoUtil::freeGCHandle(child.gcHandle);
			child.gcHandle = 0;

			child.element->SetParent(nullptr);
		}

		instance->mChildren.clear();
	}

	ScriptGUIPanel::ScriptGUIPanel(MonoObject* instance)
		:ScriptObject(instance)
	{ }

	void ScriptGUIPanel::initRuntimeData()
	{ }

	MonoObject* ScriptGUIPanel::CreateFromExisting(GUIPanel* panel)
	{
		MonoObject* managedInstance = metaData.scriptClass->createInstance();
		new (bs_alloc<ScriptGUILayout>()) ScriptGUILayout(managedInstance, panel, false);

		return managedInstance;
	}

	ScriptGUIScrollAreaLayout::ScriptGUIScrollAreaLayout(MonoObject* instance, GUILayout* layout)
		:ScriptGUILayout(instance, layout, false), mParentScrollArea(nullptr)
	{
		
	}

	void ScriptGUIScrollAreaLayout::Destroy()
	{
		if (!mIsDestroyed)
		{
			if (mParentScrollArea != nullptr)
				mParentScrollArea->NotifyLayoutDestroyed();

			while (mChildren.size() > 0)
			{
				ChildInfo childInfo = mChildren[0];
				childInfo.element->Destroy();
			}

			mLayout = nullptr;
			mIsDestroyed = true;
		}
	}
}
