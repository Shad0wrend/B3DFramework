//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsGUIElementStyle.h"

#include "BsGUIContent.h"
#include "Private/RTTI/BsGUIElementStyleRTTI.h"

using namespace bs;

const HSpriteImage& GUIElementStyle::GetImageForState(GUIElementState state) const
{
	switch(state)
	{
	case GUIElementState::Normal:
		return Normal.Image;
	case GUIElementState::Hover:
		return Hover.Image;
	case GUIElementState::Active:
		return Active.Image;
	case GUIElementState::Focused:
		return Focused.Image;
	case GUIElementState::FocusedHover:
		return FocusedHover.Image;
	case GUIElementState::NormalOn:
		return NormalOn.Image;
	case GUIElementState::HoverOn:
		return HoverOn.Image;
	case GUIElementState::ActiveOn:
		return ActiveOn.Image;
	case GUIElementState::FocusedOn:
		return FocusedOn.Image;
	case GUIElementState::FocusedHoverOn:
		return FocusedHoverOn.Image;
	default:
		break;
	}

	return Normal.Image;
}

const Color& GUIElementStyle::GetTextColorForState(GUIElementState state) const
{
	switch(state)
	{
	case GUIElementState::Normal:
		return Normal.TextColor;
	case GUIElementState::Hover:
		return Hover.TextColor;
	case GUIElementState::Active:
		return Active.TextColor;
	case GUIElementState::Focused:
		return Focused.TextColor;
	case GUIElementState::FocusedHover:
		return FocusedHover.TextColor;
	case GUIElementState::NormalOn:
		return NormalOn.TextColor;
	case GUIElementState::HoverOn:
		return HoverOn.TextColor;
	case GUIElementState::ActiveOn:
		return ActiveOn.TextColor;
	case GUIElementState::FocusedOn:
		return FocusedOn.TextColor;
	case GUIElementState::FocusedHoverOn:
		return FocusedHoverOn.TextColor;
	default:
		break;
	}

	return Normal.TextColor;
}
RTTITypeBase* GUIElementStyle::GetRttiStatic()
{
	return GUIElementStyleRTTI::Instance();
}

RTTITypeBase* GUIElementStyle::GetRtti() const
{
	return GUIElementStyle::GetRttiStatic();
}
