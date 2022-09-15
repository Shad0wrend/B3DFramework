//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsGUIElementContainer.h"
#include "GUI/BsGUISkin.h"
#include "GUI/BsGUIManager.h"

namespace bs
{
	GUIElementContainer::GUIElementContainer(const GUIDimensions& dimensions, const char* style, 
		GUIElementOptions options)
		:GUIElement(style, dimensions, options)
	{
		mOptionFlags.Set(GUIElementOption::ClickThrough);
	}

	GUIElementContainer::GUIElementContainer(const GUIDimensions& dimensions, const String& style, 
		GUIElementOptions options)
		:GUIElement(style, dimensions, options)
	{
		mOptionFlags.Set(GUIElementOption::ClickThrough);
	}

	void GUIElementContainer::FillBuffer(
		UINT8* vertices,
		UINT32* indices,
		UINT32 vertexOffset,
		UINT32 indexOffset,
		const Vector2I& offset,
		UINT32 maxNumVerts,
		UINT32 maxNumIndices,
		UINT32 renderElementIdx) const
	{ }

	Vector2I GUIElementContainer::GetOptimalSizeInternal() const
	{
		return Vector2I();
	}

	void GUIElementContainer::SetFocus(bool enabled, bool clear)
	{
		if(mFocusElement)
			mFocusElement->SetFocus(enabled, clear);
		else
			GUIElement::SetFocus(enabled, clear);
	}

	bool GUIElementContainer::CommandEventInternal(const GUICommandEvent& ev)
	{
		// Make sure to pass through focus events to elements below
		if (ev.GetType() == GUICommandEventType::FocusGained)
			return false;
		else if (ev.GetType() == GUICommandEventType::FocusLost)
			return false;

		return GUIElement::CommandEventInternal(ev);
	}
}
