//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsGUIDropDownBoxManager.h"
#include "Scene/BsSceneObject.h"

namespace bs
{
	GUIDropDownBoxManager::~GUIDropDownBoxManager()
	{
		CloseDropDownBox();
	}

	GameObjectHandle<GUIDropDownMenu> GUIDropDownBoxManager::OpenDropDownBox(const DROP_DOWN_BOX_DESC& desc,
		GUIDropDownType type, std::function<void()> onClosedCallback)
	{
		CloseDropDownBox();

		mDropDownSO = SceneObject::Create("DropDownBox", SOF_Internal | SOF_Persistent | SOF_DontSave);
		mDropDownBox = mDropDownSO->addComponent<GUIDropDownMenu>(desc, type);
		mOnClosedCallback = onClosedCallback;

		return mDropDownBox;
	}

	void GUIDropDownBoxManager::closeDropDownBox()
	{
		if(mDropDownSO != nullptr)
		{
			mDropDownSO->destroy();
			mDropDownSO = nullptr;

			if(mOnClosedCallback != nullptr)
				mOnClosedCallback();

			mOnClosedCallback = nullptr;
		}
	}
}
