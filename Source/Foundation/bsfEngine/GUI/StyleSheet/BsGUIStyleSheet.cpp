//************************************ bs::framework - Copyright 2023 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/StyleSheet/BsGUIStyleSheet.h"
#include "BsGUIStyleSheetParser.h"
#include "FileSystem/BsFileSystem.h"
#include "FileSystem/BsDataStream.h"

using namespace bs;

void GUIStyleSheetStateStyle::Override(const GUIStyleSheetStateStyle& other)
{
#define OVERRIDE_PROPERTY(PropertyName, FieldName)                              \
	if(other.OverridenProperties[(u32)GUIStyleSheetPropertyType::PropertyName]) \
		(FieldName) = other.FieldName;

	OVERRIDE_PROPERTY(Width, Size.Width)
	OVERRIDE_PROPERTY(Height, Size.Height)
	OVERRIDE_PROPERTY(MinWidth, MinimumSize.Width)
	OVERRIDE_PROPERTY(MinHeight, MinimumSize.Height)
	OVERRIDE_PROPERTY(MaxWidth, MaximumSize.Width)
	OVERRIDE_PROPERTY(MaxHeight, MaximumSize.Height)

	OVERRIDE_PROPERTY(MarginTop, Margins.Top)
	OVERRIDE_PROPERTY(MarginBottom, Margins.Bottom)
	OVERRIDE_PROPERTY(MarginLeft, Margins.Left)
	OVERRIDE_PROPERTY(MarginRight, Margins.Right)

	OVERRIDE_PROPERTY(PaddingTop, Padding.Top)
	OVERRIDE_PROPERTY(PaddingBottom, Padding.Bottom)
	OVERRIDE_PROPERTY(PaddingLeft, Padding.Left)
	OVERRIDE_PROPERTY(PaddingRight, Padding.Right)

	OVERRIDE_PROPERTY(Color, Color)
	OVERRIDE_PROPERTY(Opacity, Opacity)
	OVERRIDE_PROPERTY(BackgroundColor, BackgroundColor)

	OVERRIDE_PROPERTY(TextAlign, HorizontalTextAlignment)
	OVERRIDE_PROPERTY(VerticalAlign, VerticalTextAlignment)
	OVERRIDE_PROPERTY(FontFamily, FontFamily)
	OVERRIDE_PROPERTY(FontSize, FontSize)

	OVERRIDE_PROPERTY(BorderTopStyle, BorderTop.Style)
	OVERRIDE_PROPERTY(BorderTopWidth, BorderTop.Width)
	OVERRIDE_PROPERTY(BorderTopColor, BorderTop.Color)

	OVERRIDE_PROPERTY(BorderBottomStyle, BorderBottom.Style)
	OVERRIDE_PROPERTY(BorderBottomWidth, BorderBottom.Width)
	OVERRIDE_PROPERTY(BorderBottomColor, BorderBottom.Color)

	OVERRIDE_PROPERTY(BorderLeftStyle, BorderLeft.Style)
	OVERRIDE_PROPERTY(BorderLeftWidth, BorderLeft.Width)
	OVERRIDE_PROPERTY(BorderLeftColor, BorderLeft.Color)

	OVERRIDE_PROPERTY(BorderRightStyle, BorderRight.Style)
	OVERRIDE_PROPERTY(BorderRightWidth, BorderRight.Width)
	OVERRIDE_PROPERTY(BorderRightColor, BorderRight.Color)

	OVERRIDE_PROPERTY(BorderTopLeftRadius, BorderTopLeftRadius);
	OVERRIDE_PROPERTY(BorderTopRightRadius, BorderTopRightRadius);
	OVERRIDE_PROPERTY(BorderBottomLeftRadius, BorderBottomLeftRadius);
	OVERRIDE_PROPERTY(BorderBottomRightRadius, BorderBottomRightRadius);

#undef OVERRIDE_PROPERTY
	
}

Optional<GUIStyleSheet> GUIStyleSheet::Parse(const Path& file)
{
	const SPtr<DataStream> fileStream = FileSystem::OpenFile(file);
	if(!fileStream)
		return {};

	GUIStyleSheetParser parser;
	parser.Parse(B3DMakeShared<SourceCode>(fileStream->GetAsString()));
	
	// TODO - Returne parsed style
	return {};
}

GUIStyleSheetStateStyle GUIStyleSheet::FindStyle(const String& elementType, const String& elementId, GUIElementState state)
{
	// TODO - Cache the style

	// TODO:
	// - Find element type style, narrow down inhertiance chain to the particular state
	// - Then find id style, override everything from previous step

	auto fnLookupStateStyle = [](const GUIStyleSheetStyle& style, GUIElementState state)
	{
		GUIStyleSheetStateStyle stateStyle = style.Normal;

		// TODO - Handle this case:
		// - Red color is always used on hover, but the border radius is used from the focus state
		// - Unless border-radius is also specified in 'hover', in which case it is used

		//input:focus
		//	{
		//		background - color : lightblue;
		//		border - radius : 25px;
		//	}

		//input:hover
		//	{
		//		background - color : red;
		//	}

		// normal -> focused -> hover
		// normal -> disabled -> hover

		switch(state)
		{
		case GUIElementState::Normal: break;
		case GUIElementState::Hover:
			if(style.Hover.has_value())
				stateStyle.Override(*style.Hover);
			break;
		case GUIElementState::Active:
			if(style.Active.has_value())
				stateStyle.Override(*style.Active);
			break;
		case GUIElementState::Focused:

			break;
		case GUIElementState::FocusedHover: break;
		case GUIElementState::NormalOn: break;
		case GUIElementState::HoverOn: break;
		case GUIElementState::ActiveOn: break;
		case GUIElementState::FocusedOn: break;
		case GUIElementState::FocusedHoverOn: break;
		case GUIElementState::TypeMask: break;
		case GUIElementState::OnFlag: break;
		default: ;
		}

		return stateStyle;

	};

	GUIStyleSheetStateStyle style;

	//if(auto it = mElementStyles.find(elementType); it != mElementStyles.end())
	//{
	//	style = it->second;
	//}

	return style;
}

