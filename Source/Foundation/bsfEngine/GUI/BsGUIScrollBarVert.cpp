//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsGUIScrollBarVert.h"
#include "GUI/BsGUISizeConstraints.h"

using namespace bs;

GUIScrollBarVert::GUIScrollBarVert(bool resizeable, const String& styleName, const GUISizeConstraints& dimensions)
	: GUIScrollBar(false, resizeable, styleName, dimensions)
{
}

GUIScrollBarVert* GUIScrollBarVert::Create(const String& styleName)
{
	return new(B3DAllocate<GUIScrollBarVert>()) GUIScrollBarVert(false, GetStyleName<GUIScrollBarVert>(false, styleName), GUISizeConstraints::Create());
}

GUIScrollBarVert* GUIScrollBarVert::Create(bool resizeable, const String& styleName)
{
	return new(B3DAllocate<GUIScrollBarVert>()) GUIScrollBarVert(resizeable, GetStyleName<GUIScrollBarVert>(resizeable, styleName), GUISizeConstraints::Create());
}

GUIScrollBarVert* GUIScrollBarVert::Create(const GUIOptions& options, const String& styleName)
{
	return new(B3DAllocate<GUIScrollBarVert>()) GUIScrollBarVert(false, GetStyleName<GUIScrollBarVert>(false, styleName), GUISizeConstraints::Create(options));
}

GUIScrollBarVert* GUIScrollBarVert::Create(bool resizeable, const GUIOptions& options, const String& styleName)
{
	return new(B3DAllocate<GUIScrollBarVert>()) GUIScrollBarVert(resizeable, GetStyleName<GUIScrollBarVert>(resizeable, styleName), GUISizeConstraints::Create(options));
}

const String& GUIScrollBarVert::GetGuiTypeName(bool resizable)
{
	static String typeName = "ScrollBarVert";
	static String resizableTypeName = "ResizeableScrollBarVert";

	if(resizable)
		return resizableTypeName;
	else
		return typeName;
}
