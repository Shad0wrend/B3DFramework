//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsGUIButtonBase.h"
#include "2D/BsImageSprite.h"
#include "GUI/BsGUISkin.h"
#include "Image/BsSpriteTexture.h"
#include "2D/BsTextSprite.h"
#include "GUI/BsGUISizeConstraints.h"
#include "GUI/BsGUIMouseEvent.h"
#include "GUI/BsGUICommandEvent.h"
#include "GUI/BsGUIHelper.h"
#include "Image/BsSpriteVectorPath.h"
#include "StyleSheet/BsGUIStyleSheet.h"

using namespace bs;

GUIButtonBase::GUIButtonBase(const String& styleName, const GUIContent& content, const GUISizeConstraints& dimensions, GUIElementOptions options)
	: GUIElement(styleName, dimensions, options), mContent(content)
{
	mTextSprite = B3DNew<TextSprite>();

	mBackgroundSprite.SetAnimationStartTime(GetTime().GetTime());
	mContentAnimationStartTime = GetTime().GetTime();

	RefreshContentSprite();
}

GUIButtonBase::~GUIButtonBase()
{
	B3DDelete(mTextSprite);

	if(mContentImageSprite != nullptr)
		B3DDelete(mContentImageSprite);
}

void GUIButtonBase::SetContent(const GUIContent& content)
{
	Vector2I origSize = mSizeConstraints.CalculateConstrainedSize(CalculateUnconstrainedOptimalSize()).Optimal;
	mContent = content;
	mContentAnimationStartTime = GetTime().GetTime();

	RefreshContentSprite();

	Vector2I newSize = mSizeConstraints.CalculateConstrainedSize(CalculateUnconstrainedOptimalSize()).Optimal;

	if(origSize != newSize)
		MarkLayoutAsDirty();
	else
		MarkContentAsDirty();
}

void GUIButtonBase::SetOnInternal(bool on)
{
	if(on)
		AddStateFlags(GUIElementStateFlag::Checked);
	else
		RemoveStateFlags(GUIElementStateFlag::Checked);

	if(on)
		SetStateInternal((GUIElementState)((i32)mActiveState | (i32)GUIElementState::OnFlag));
	else
		SetStateInternal((GUIElementState)((i32)mActiveState & ~(i32)GUIElementState::OnFlag));
}

bool GUIButtonBase::IsOnInternal() const
{
	return ((i32)mActiveState & (i32)GUIElementState::OnFlag) != 0;
}

void GUIButtonBase::UpdateRenderElements()
{
	mRenderElements.clear();
	GUISpriteHelper::BuildSpriteRenderElements(*this, mActiveState, mBackgroundSprite);

	const bool isUsingStyleSheets = IsUsingStyleSheets();
	mTextSprite->Update(GetTextDesc(), (u64)GetParentWidget());

	if(mContentImageSprite != nullptr)
	{
		Rect2I contentBounds = GetCachedContentBounds();

		HSpriteImage image = mContent.GetImage(mActiveState);

		const Size2UI& imageSize = image->GetSize();
		u32 contentWidth = imageSize.Width;
		u32 contentHeight = imageSize.Height;

		u32 contentMaxWidth = std::min((u32)contentBounds.Width, contentWidth);
		u32 contentMaxHeight = std::min((u32)contentBounds.Height, contentHeight);

		float horzRatio = contentMaxWidth / (float)contentWidth;
		float vertRatio = contentMaxHeight / (float)contentHeight;

		if(horzRatio < vertRatio)
		{
			contentWidth = Math::RoundToI32(contentWidth * horzRatio);
			contentHeight = Math::RoundToI32(contentHeight * horzRatio);
		}
		else
		{
			contentWidth = Math::RoundToI32(contentWidth * vertRatio);
			contentHeight = Math::RoundToI32(contentHeight * vertRatio);
		}

		ImageSpriteInformation contentImgDesc;
		contentImgDesc.Image = image;
		contentImgDesc.Width = contentWidth;
		contentImgDesc.Height = contentHeight;
		contentImgDesc.Color = GetTint();
		contentImgDesc.AnimationStartTime = mContentAnimationStartTime;

		if(isUsingStyleSheets)
		{
			const GUIStyleSheetRules& styleSheetRules = mStyleSheetRuleInformation.CurrentStateRuleset->Rules;
			contentImgDesc.Color *= styleSheetRules.Color;
			contentImgDesc.Color.A *= styleSheetRules.Opacity;
		}

		mContentImageSprite->Update(contentImgDesc, (u64)GetParentWidget());
	}

	// Calculate content bounds
	Rect2 textBounds;
	Rect2 contentImageBounds;

	const Vector2I contentOffset = GetContentOffsetInElementSpace();
	Rect2I contentBounds = GetCachedContentBounds();
	Rect2I textSpriteBounds = mTextSprite->GetBounds(Vector2I(), Rect2I());

	if(mContentImageSprite != nullptr)
	{
		Rect2I contentImageSpriteBounds = mContentImageSprite->GetBounds(Vector2I(), Rect2I());
		i32 imageXOffset = 0;
		i32 textImageSpacing = 0;

		if(textSpriteBounds.Width == 0)
		{
			const u32 freeWidth = (u32)std::max(0, (i32)contentBounds.Width - (i32)textSpriteBounds.Width - (i32)contentImageSpriteBounds.Width);
			imageXOffset = (i32)(freeWidth / 2);
		}
		else
			textImageSpacing = GUIContent::kImageTextSpacing;

		if(GetStyle()->ImagePosition == GUIImagePosition::Right)
		{
			const i32 imageReservedWidth = Math::Max(0, (i32)contentBounds.Width - (i32)textSpriteBounds.Width);

			textBounds.X = (float)contentOffset.X;
			textBounds.Width = (float)Math::Max(0, (i32)contentBounds.Width - imageReservedWidth);

			contentImageBounds.X = (float)(contentOffset.X + textSpriteBounds.Width + imageXOffset + textImageSpacing);
			contentImageBounds.Width = (float)Math::Max(0, imageReservedWidth - imageXOffset);
		}
		else
		{
			const i32 imageReservedWidth = (i32)contentImageSpriteBounds.Width + imageXOffset;

			contentImageBounds.X = (float)(contentOffset.X + imageXOffset);
			contentImageBounds.Width = (float)Math::Min(imageReservedWidth - imageXOffset, (i32)contentBounds.Width);

			textBounds.X = (float)(contentOffset.X + imageReservedWidth + textImageSpacing);
			textBounds.Width = (float)Math::Max(0, (i32)contentBounds.Width - imageReservedWidth);
		}

		textBounds.Y = (float)contentOffset.Y;
		textBounds.Height = (float)contentBounds.Height;

		const float imageYOffset = Math::Max(0, (float)contentBounds.Height - (float)contentImageSpriteBounds.Height) / 2.0f;
		contentImageBounds.Y = (float)contentOffset.Y + (float)imageYOffset;
		contentImageBounds.Height = (float)contentBounds.Height - imageYOffset;
	}
	else
	{
		textBounds.X = (float)contentOffset.X;
		textBounds.Y = (float)contentOffset.Y;
		textBounds.Width = (float)contentBounds.Width;
		textBounds.Height = (float)contentBounds.Height;
	}

	// Populate GUI render elements from the sprites
	{
		using T = GUIRenderElementHelper;

		T::Append({
			T::SpriteInfo(mTextSprite, 0, textBounds),
			T::SpriteInfo(mContentImageSprite, 0, contentImageBounds) },
			mRenderElements);
	}

	GUIElement::UpdateRenderElements();
}

Vector2I GUIButtonBase::CalculateUnconstrainedOptimalSize() const
{
	u32 imageWidth = 0;
	u32 imageHeight = 0;

	const bool isUsingStyleSheets = IsUsingStyleSheets();
	if(isUsingStyleSheets)
	{
		const GUIStyleSheetRules& styleSheetRules = mStyleSheetRuleInformation.CurrentStateRuleset->Rules;
		const Size2UI contentSize = GUIHelper::CalculateOptimalContentSizeWithPaddingAndBorder(mContent, styleSheetRules, GetSizeConstraints().MaxWidth);
		
		const u32 contentWidth = std::max(imageWidth, contentSize.Width);
		const u32 contentHeight = std::max(imageHeight, contentSize.Height);

		return Vector2I(contentWidth, contentHeight);
	}
	else
	{
		const HSpriteImage& activeImage = GetStyle()->GetImageForState(mActiveState);
		if(SpriteImage::CheckIsLoaded(activeImage))
		{
			imageWidth = activeImage->GetSize().Width;
			imageHeight = activeImage->GetSize().Height;
		}

		Vector2I contentSize = GUIHelper::CalculateOptimalContentSize(mContent, *GetStyle(), GetSizeConstraints(), mActiveState);
		u32 contentWidth = std::max(imageWidth, (u32)contentSize.X);
		u32 contentHeight = std::max(imageHeight, (u32)contentSize.Y);

		return Vector2I(contentWidth, contentHeight);
	}
}

u32 GUIButtonBase::GetRenderElementDepthRange() const
{
	return 2;
}

bool GUIButtonBase::DoOnMouseEvent(const GUIMouseEvent& ev)
{
	if(mOptionFlags.IsSet(GUIElementOption::IgnorePointerEvents))
		return false;

	if(ev.GetType() == GUIMouseEventType::MouseOver)
	{
		if(!IsDisabled())
		{
			AddStateFlags(GUIElementStateFlag::Hover);

			if(mHasFocus)
				SetStateInternal(IsOnInternal() ? GUIElementState::FocusedHoverOn : GUIElementState::FocusedHover);
			else
				SetStateInternal(IsOnInternal() ? GUIElementState::HoverOn : GUIElementState::Hover);

			OnHover();
		}

		return !mOptionFlags.IsSet(GUIElementOption::ClickThrough);
	}
	else if(ev.GetType() == GUIMouseEventType::MouseOut)
	{
		if(!IsDisabled())
		{
			RemoveStateFlags(GUIElementStateFlag::Hover | GUIElementStateFlag::Active);

			if(mHasFocus)
				SetStateInternal(IsOnInternal() ? GUIElementState::FocusedOn : GUIElementState::Focused);
			else
				SetStateInternal(IsOnInternal() ? GUIElementState::NormalOn : GUIElementState::Normal);

			OnOut();
		}

		return !mOptionFlags.IsSet(GUIElementOption::ClickThrough);
	}
	else if(ev.GetType() == GUIMouseEventType::MouseDown)
	{
		if(!IsDisabled())
		{
			AddStateFlags(GUIElementStateFlag::Active);
			SetStateInternal(IsOnInternal() ? GUIElementState::ActiveOn : GUIElementState::Active);
		}

		return !mOptionFlags.IsSet(GUIElementOption::ClickThrough);
	}
	else if(ev.GetType() == GUIMouseEventType::MouseUp)
	{
		if(!IsDisabled())
		{
			RemoveStateFlags(GUIElementStateFlag::Active);

			if(mHasFocus)
				SetStateInternal(IsOnInternal() ? GUIElementState::FocusedHoverOn : GUIElementState::FocusedHover);
			else
				SetStateInternal(IsOnInternal() ? GUIElementState::HoverOn : GUIElementState::Hover);

			OnClick();
		}

		return !mOptionFlags.IsSet(GUIElementOption::ClickThrough);
	}
	else if(ev.GetType() == GUIMouseEventType::MouseDoubleClick)
	{
		if(!IsDisabled())
			OnDoubleClick();

		return !mOptionFlags.IsSet(GUIElementOption::ClickThrough);
	}

	return false;
}

bool GUIButtonBase::DoOnCommandEvent(const GUICommandEvent& ev)
{
	const bool baseReturnValue = GUIElement::DoOnCommandEvent(ev);

	GUIElementState state = (GUIElementState)((u32)mActiveState & (u32)GUIElementState::TypeMask);
	if(ev.GetType() == GUICommandEventType::FocusGained)
	{
		mHasFocus = true;

		if(!IsDisabled())
		{
			AddStateFlags(GUIElementStateFlag::Focus);

			if(state == GUIElementState::Normal)
				SetStateInternal(IsOnInternal() ? GUIElementState::FocusedOn : GUIElementState::Focused);
			else if(state == GUIElementState::Hover)
				SetStateInternal(IsOnInternal() ? GUIElementState::FocusedHoverOn : GUIElementState::FocusedHover);
		}

		return true;
	}
	else if(ev.GetType() == GUICommandEventType::FocusLost)
	{
		mHasFocus = false;
		RemoveStateFlags(GUIElementStateFlag::Focus);

		if(state == GUIElementState::Focused)
			SetStateInternal(IsOnInternal() ? GUIElementState::NormalOn : GUIElementState::Normal);
		else if(state == GUIElementState::FocusedHover)
			SetStateInternal(IsOnInternal() ? GUIElementState::HoverOn : GUIElementState::Hover);

		return true;
	}

	return baseReturnValue;
}

String GUIButtonBase::GetTooltip() const
{
	return mContent.Tooltip;
}

void GUIButtonBase::RefreshContentSprite()
{
	HSpriteImage contentImage = mContent.GetImage(mActiveState);
	if(SpriteImage::CheckIsLoaded(contentImage))
	{
		if(mContentImageSprite == nullptr)
			mContentImageSprite = B3DNew<ImageSprite>();
	}
	else
	{
		if(mContentImageSprite != nullptr)
		{
			B3DDelete(mContentImageSprite);
			mContentImageSprite = nullptr;
		}
	}
}

TextSpriteInformation GUIButtonBase::GetTextDesc() const
{
	const bool isUsingStyleSheets = IsUsingStyleSheets();
	if(isUsingStyleSheets)
	{
		const GUIStyleSheetRules& styleSheetRules = mStyleSheetRuleInformation.CurrentStateRuleset->Rules;

		TextSpriteInformation textSpriteInformation;
		textSpriteInformation.Text = mContent.Text;

		Rect2I textBounds = GetCachedContentBounds();

		textSpriteInformation.Width = textBounds.Width;
		textSpriteInformation.Height = textBounds.Height;

		textSpriteInformation.InitializeFromStyleSheetRules(styleSheetRules);
		textSpriteInformation.Color *= GetTint();

		return textSpriteInformation;
	}
	else
	{
		TextSpriteInformation textDesc;
		textDesc.Text = mContent.Text;
		textDesc.Font = GetStyle()->Font;
		textDesc.FontSize = GetStyle()->FontSize;
		textDesc.Color = GetTint() * GetStyle()->GetTextColorForState(mActiveState);

		Rect2I textBounds = GetCachedContentBounds();

		textDesc.Width = textBounds.Width;
		textDesc.Height = textBounds.Height;
		textDesc.HorzAlign = GetStyle()->TextHorzAlign;
		textDesc.VertAlign = GetStyle()->TextVertAlign;

		return textDesc;
	}
}

void GUIButtonBase::NotifyStyleChanged()
{
	mBackgroundSprite.SetAnimationStartTime(GetTime().GetTime());
}

void GUIButtonBase::SetStateInternal(GUIElementState state)
{
	Vector2I origSize = mSizeConstraints.CalculateConstrainedSize(CalculateUnconstrainedOptimalSize()).Optimal;

	if(mActiveState != state)
		mBackgroundSprite.SetAnimationStartTime(GetTime().GetTime());

	mActiveState = state;
	RefreshContentSprite();
	Vector2I newSize = mSizeConstraints.CalculateConstrainedSize(CalculateUnconstrainedOptimalSize()).Optimal;

	if(origSize != newSize)
		MarkLayoutAsDirty();
	else
		MarkContentAsDirty();
}
