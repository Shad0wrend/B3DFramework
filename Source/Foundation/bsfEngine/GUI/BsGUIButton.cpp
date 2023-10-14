//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsGUIButton.h"
#include "GUI/BsGUISizeConstraints.h"
#include "GUI/BsGUICommandEvent.h"

using namespace bs;

const String& GUIButton::GetGuiTypeName()
{
	static String name = "Button";
	return name;
}

GUIButton::GUIButton(const String& styleName, const GUIContent& content, const GUISizeConstraints& dimensions)
	: GUIButtonBase(styleName, content, dimensions)
{}

GUIButton* GUIButton::Create(const HString& text, const String& styleName)
{
	return Create(GUIContent(text), styleName);
}

GUIButton* GUIButton::Create(const HString& text, const GUIOptions& options, const String& styleName)
{
	return Create(GUIContent(text), options, styleName);
}

GUIButton* GUIButton::Create(const GUIContent& content, const String& styleName)
{
	return new(B3DAllocate<GUIButton>()) GUIButton(GetStyleName<GUIButton>(styleName), content, GUISizeConstraints::Create());
}

GUIButton* GUIButton::Create(const GUIContent& content, const GUIOptions& options, const String& styleName)
{
	return new(B3DAllocate<GUIButton>()) GUIButton(GetStyleName<GUIButton>(styleName), content, GUISizeConstraints::Create(options));
}

bool GUIButton::DoOnCommandEvent(const GUICommandEvent& ev)
{
	const bool processed = GUIButtonBase::DoOnCommandEvent(ev);

	if(ev.GetType() == GUICommandEventType::Confirm)
	{
		if(!IsDisabled())
		{
			OnClick();
			return true;
		}
	}

	return processed;
}
