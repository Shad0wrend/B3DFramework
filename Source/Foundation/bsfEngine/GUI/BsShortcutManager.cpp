//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsShortcutManager.h"
#include "Input/BsInput.h"

using namespace std::placeholders;

namespace bs
{
	ShortcutManager::ShortcutManager()
	{
		mOnButtonDownConn = Input::Instance().onButtonDown.Connect(std::bind(&::bs::ShortcutManager::OnButtonDown, this, _1));
	}

	ShortcutManager::~ShortcutManager()
	{
		mOnButtonDownConn.Disconnect();
	}

	void ShortcutManager::AddShortcut(const ShortcutKey& key, std::function<void()> callback)
	{
		mShortcuts[key] = callback;
	}

	void ShortcutManager::RemoveShortcut(const ShortcutKey& key)
	{
		mShortcuts.erase(key);
	}

	void ShortcutManager::OnButtonDown(const ButtonEvent& event)
	{
		UINT32 modifiers = 0;
		if (Input::Instance().IsButtonHeld(BC_LSHIFT) || Input::Instance().IsButtonHeld(BC_RSHIFT))
			modifiers |= (UINT32)ButtonModifier::Shift;

		if (Input::Instance().IsButtonHeld(BC_LCONTROL) || Input::Instance().IsButtonHeld(BC_RCONTROL))
			modifiers |= (UINT32)ButtonModifier::Ctrl;

		if (Input::Instance().IsButtonHeld(BC_LMENU) || Input::Instance().IsButtonHeld(BC_RMENU))
			modifiers |= (UINT32)ButtonModifier::Alt;

		ShortcutKey searchKey((ButtonModifier)modifiers, event.buttonCode);

		auto iterFind = mShortcuts.find(searchKey);
		if (iterFind != mShortcuts.end())
		{
			if (iterFind->second != nullptr)
				iterFind->second();
		}
	}
}
