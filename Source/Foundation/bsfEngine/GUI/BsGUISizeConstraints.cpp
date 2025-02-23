//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsGUISizeConstraints.h"
#include "GUI/BsGUIOptions.h"
#include "StyleSheet/BsGUIStyleSheet.h"

using namespace bs;

GUISizeConstraints GUISizeConstraints::Create()
{
	return GUISizeConstraints();
}

GUISizeConstraints GUISizeConstraints::Create(const GUIOptions& options)
{
	return Create(options.mOptions);
}

GUISizeConstraints GUISizeConstraints::Create(const TInlineArray<GUIOption, 4>& options)
{
	GUISizeConstraints dimensions;

	for(auto& option : options)
	{
		switch(option.mType)
		{
		case GUIOptionType::Position:
			dimensions.ExplicitPosition = GUILogicalPoint((i32)option.mMinimum, (i32)option.mMaximum);
			break;
		case GUIOptionType::FixedWidth:
			dimensions.Flags |= GUISizeConstraintFlag::FixedWidth | GUISizeConstraintFlag::WidthOverridenAtRuntime;
			dimensions.MinimumWidth = dimensions.MaximumWidth = option.mMinimum;
			break;
		case GUIOptionType::FixedHeight:
			dimensions.Flags |= GUISizeConstraintFlag::FixedHeight | GUISizeConstraintFlag::HeightOverridenAtRuntime;
			dimensions.MinimumHeight = dimensions.MaximumHeight = option.mMinimum;
			break;
		case GUIOptionType::FlexibleWidth:
			dimensions.Flags |= GUISizeConstraintFlag::WidthOverridenAtRuntime;
			dimensions.Flags.Unset(GUISizeConstraintFlag::FixedWidth);
			dimensions.Flags.Unset(GUISizeConstraintFlag::ExpandingWidth);
			dimensions.MinimumWidth = option.mMinimum;
			dimensions.MaximumWidth = option.mMaximum;
			break;
		case GUIOptionType::FlexibleHeight:
			dimensions.Flags |= GUISizeConstraintFlag::HeightOverridenAtRuntime;
			dimensions.Flags.Unset(GUISizeConstraintFlag::FixedHeight);
			dimensions.Flags.Unset(GUISizeConstraintFlag::ExpandingHeight);
			dimensions.MinimumHeight = option.mMinimum;
			dimensions.MaximumHeight = option.mMaximum;
			break;
		case GUIOptionType::ExpandingWidth:
			dimensions.Flags |= GUISizeConstraintFlag::ExpandingWidth | GUISizeConstraintFlag::WidthOverridenAtRuntime;
			dimensions.Flags.Unset(GUISizeConstraintFlag::FixedWidth);
			dimensions.MinimumWidth = option.mMinimum;
			dimensions.MaximumWidth = option.mMaximum;
			break;
		case GUIOptionType::ExpandingHeight:
			dimensions.Flags |= GUISizeConstraintFlag::ExpandingHeight | GUISizeConstraintFlag::HeightOverridenAtRuntime;
			dimensions.Flags.Unset(GUISizeConstraintFlag::FixedHeight);
			dimensions.MinimumHeight = option.mMinimum;
			dimensions.MaximumHeight = option.mMaximum;
			break;
		}
	}

	return dimensions;
}

void GUISizeConstraints::UpdateWithStyleSheetRule(const GUIStyleSheetRules& rule)
{
	if(!Flags.IsSet(GUISizeConstraintFlag::WidthOverridenAtRuntime))
	{
		Flags.Unset(GUISizeConstraintFlag::ExpandingWidth);

		if(rule.IsPropertySet(GUIStyleSheetPropertyType::Width))
		{
			Flags |= GUISizeConstraintFlag::FixedWidth;
			MinimumWidth = MaximumWidth = rule.Size.Width;
		}
		else
		{
			Flags.Unset(GUISizeConstraintFlag::FixedWidth);
			MinimumWidth = rule.MinimumSize.Width;
			MaximumWidth = rule.MaximumSize.Width;
		}
	}

	if(!Flags.IsSet(GUISizeConstraintFlag::HeightOverridenAtRuntime))
	{
		Flags.Unset(GUISizeConstraintFlag::ExpandingHeight);

		if(rule.IsPropertySet(GUIStyleSheetPropertyType::Height))
		{
			Flags |= GUISizeConstraintFlag::FixedHeight;
			MinimumHeight = MaximumHeight = rule.Size.Height;
		}
		else
		{
			Flags.Unset(GUISizeConstraintFlag::FixedHeight);
			MinimumHeight = rule.MinimumSize.Height;
			MaximumHeight = rule.MaximumSize.Height;
		}
	}
}

GUIConstrainedSize GUISizeConstraints::CalculateConstrainedSize(const GUILogicalSize& unconstrainedOptimalSize) const
{
	GUIConstrainedSize sizeRange;

	if(IsHeightFixed())
	{
		sizeRange.Optimal.Height = Math::Max(MinimumHeight, 0);
		sizeRange.Minimum.Height = sizeRange.Optimal.Height;
		sizeRange.Maximum.Height = sizeRange.Optimal.Height;
	}
	else
	{
		sizeRange.Optimal.Height = unconstrainedOptimalSize.Height;

		if(MinimumHeight > 0)
		{
			sizeRange.Optimal.Height = Math::Max(Math::Max(MinimumHeight, 0), sizeRange.Optimal.Height);
			sizeRange.Minimum.Height = Math::Max(MinimumHeight, 0);
		}

		if(MaximumHeight > 0)
		{
			sizeRange.Optimal.Height = Math::Min(Math::Max(MaximumHeight, 0), sizeRange.Optimal.Height);
			sizeRange.Maximum.Height = Math::Max(MaximumHeight, 0);
		}
	}

	if(IsWidthFixed())
	{
		sizeRange.Optimal.Width = Math::Max(MinimumWidth, 0);
		sizeRange.Minimum.Width = sizeRange.Optimal.Width;
		sizeRange.Maximum.Width = sizeRange.Optimal.Width;
	}
	else
	{
		sizeRange.Optimal.Width = unconstrainedOptimalSize.Width;

		if(MinimumWidth > 0)
		{
			sizeRange.Optimal.Width = Math::Max(Math::Max(MinimumWidth, 0), sizeRange.Optimal.Width);
			sizeRange.Minimum.Width = Math::Max(MinimumWidth, 0);
		}

		if(MaximumWidth > 0)
		{
			sizeRange.Optimal.Width = Math::Min(Math::Max(MaximumWidth, 0), sizeRange.Optimal.Width);
			sizeRange.Maximum.Width = Math::Max(MaximumWidth, 0);
		}
	}

	return sizeRange;
}

