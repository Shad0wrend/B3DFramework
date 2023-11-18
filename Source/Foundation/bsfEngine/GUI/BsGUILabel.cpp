//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsGUILabel.h"
#include "GUI/BsGUIElementStyle.h"
#include "2D/BsTextSprite.h"
#include "Image/BsSpriteTexture.h"
#include "GUI/BsGUISizeConstraints.h"
#include "GUI/BsGUIHelper.h"
#include "StyleSheet/BsGUIStyleSheet.h"

using namespace bs;

GUILabel::GUILabel(const String& styleName, const GUIContent& content, const GUISizeConstraints& dimensions)
	: GUIElement(styleName, dimensions), mContent(content), mBackgroundImageSprite(nullptr)
{
	mTextSprite = B3DNew<TextSprite>();
}

GUILabel::~GUILabel()
{
	B3DDelete(mTextSprite);

	if(mBackgroundImageSprite != nullptr)
		B3DDelete(mBackgroundImageSprite);
}

u32 GUILabel::GetRenderElementDepthRange() const
{
	return 2;
}

void GUILabel::UpdateRenderElements()
{
	const HSpriteImage& activeImage = GetStyle()->Normal.Image;
	if(SpriteImage::CheckIsLoaded(activeImage))
	{
		mImageSpriteInformation.Image = activeImage;

		if(mBackgroundImageSprite == nullptr)
			mBackgroundImageSprite = B3DNew<ImageSprite>();
	}
	else
	{
		mImageSpriteInformation.Image = nullptr;

		if(mBackgroundImageSprite != nullptr)
		{
			B3DDelete(mBackgroundImageSprite);
			mBackgroundImageSprite = nullptr;
		}
	}

	if(mBackgroundImageSprite != nullptr)
	{
		mImageSpriteInformation.Width = mLayoutData.Area.Width;
		mImageSpriteInformation.Height = mLayoutData.Area.Height;
	}

	mTextSpriteInformation.Width = mLayoutData.Area.Width;
	mTextSpriteInformation.Height = mLayoutData.Area.Height;
	mTextSpriteInformation.Text = mContent.Text;

	const bool isUsingStyleSheets = IsUsingStyleSheets();
	if(isUsingStyleSheets)
	{
		const GUIStyleSheetRules& styleSheetRules = mStyleSheetRuleInformation.CurrentStateRuleset->Rules;

		if(mBackgroundImageSprite != nullptr)
		{
			mImageSpriteInformation.Color = GetTint() * styleSheetRules.BackgroundColor;
			mImageSpriteInformation.Color.A *= styleSheetRules.Opacity;
		}

		mTextSpriteInformation.InitializeFromStyleSheetRules(styleSheetRules);
		mTextSpriteInformation.Color *= GetTint();
	}
	else
	{
		if(mBackgroundImageSprite != nullptr)
		{
			mImageSpriteInformation.BorderLeft = GetStyle()->Border.Left;
			mImageSpriteInformation.BorderRight = GetStyle()->Border.Right;
			mImageSpriteInformation.BorderTop = GetStyle()->Border.Top;
			mImageSpriteInformation.BorderBottom = GetStyle()->Border.Bottom;
			mImageSpriteInformation.Color = GetTint();
		}

		mTextSpriteInformation.Font = GetStyle()->Font;
		mTextSpriteInformation.FontSize = GetStyle()->FontSize;
		mTextSpriteInformation.WordWrap = GetStyle()->WordWrap;
		mTextSpriteInformation.HorzAlign = GetStyle()->TextHorzAlign;
		mTextSpriteInformation.VertAlign = GetStyle()->TextVertAlign;
		mTextSpriteInformation.Color = GetTint() * GetStyle()->Normal.TextColor;
	}

	if(mBackgroundImageSprite != nullptr)
		mBackgroundImageSprite->Update(mImageSpriteInformation, (u64)GetParentWidget());

	mTextSprite->Update(mTextSpriteInformation, (u64)GetParentWidget());

	const Rect2 backgroundBounds(
		0.0f, 0.0f,
		(float)mLayoutData.Area.Width, (float)mLayoutData.Area.Height);

	const Vector2I contentOffset = GetContentOffsetInElementSpace();
	Rect2I contentBounds = GetCachedContentBounds();

	const Rect2 textBounds(
		(float)contentOffset.X, (float)contentOffset.Y,
		(float)contentBounds.Width, (float)contentBounds.Height);

	// Populate GUI render elements from the sprites
	{
		using T = GUIRenderElementHelper;
		T::Populate({ T::SpriteInfo(mTextSprite, 0, textBounds), T::SpriteInfo(mBackgroundImageSprite, 1, backgroundBounds) }, mRenderElements);
	}

	GUIElement::UpdateRenderElements();
}

Vector2I GUILabel::CalculateUnconstrainedOptimalSize() const
{
	const bool isUsingStyleSheets = IsUsingStyleSheets();
	if(isUsingStyleSheets)
	{
		const GUIStyleSheetRules& styleSheetRules = mStyleSheetRuleInformation.CurrentStateRuleset->Rules;
		const Size2UI contentSize = GUIHelper::CalculateOptimalContentSizeWithPaddingAndBorder(mContent, styleSheetRules, GetSizeConstraints().MaxWidth);

		return Vector2I(contentSize.Width, contentSize.Height);
	}
	else
		return GUIHelper::CalculateOptimalContentSize(mContent, *GetStyle(), GetSizeConstraints());
}

void GUILabel::SetContent(const GUIContent& content)
{
	Vector2I origSize = mSizeConstraints.CalculateConstrainedSize(CalculateUnconstrainedOptimalSize()).Optimal;
	mContent = content;
	Vector2I newSize = mSizeConstraints.CalculateConstrainedSize(CalculateUnconstrainedOptimalSize()).Optimal;

	if(origSize != newSize)
		MarkLayoutAsDirty();
	else
		MarkContentAsDirty();
}

GUILabel* GUILabel::Create(const HString& text, const String& styleName)
{
	return Create(GUIContent(text), styleName);
}

GUILabel* GUILabel::Create(const HString& text, const GUIOptions& options, const String& styleName)
{
	return Create(GUIContent(text), options, styleName);
}

GUILabel* GUILabel::Create(const GUIContent& content, const String& styleName)
{
	return new(B3DAllocate<GUILabel>()) GUILabel(GetStyleName<GUILabel>(styleName), content, GUISizeConstraints::Create());
}

GUILabel* GUILabel::Create(const GUIContent& content, const GUIOptions& options, const String& styleName)
{
	return new(B3DAllocate<GUILabel>()) GUILabel(GetStyleName<GUILabel>(styleName), content, GUISizeConstraints::Create(options));
}

const String& GUILabel::GetGuiTypeName()
{
	static String typeName = "Label";
	return typeName;
}
