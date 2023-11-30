//************************************ bs::framework - Copyright 2023 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsGUISpriteHelper.h"
#include "BsGUIVectorPaths.h"
#include "GUI/BsGUISkin.h"
#include "Image/BsSpriteTexture.h"
#include "2D/BsTextSprite.h"
#include "Image/BsSpriteVectorPath.h"
#include "StyleSheet/BsGUIStyleSheet.h"

using namespace bs;

GUIBackgroundSprite::GUIBackgroundSprite()
	:mBackgroundPathBuilder(GUIBackgroundVectorPathBuilder::Get())
{
	
}

void GUIBackgroundSprite::BuildRenderElements(const Size2UI& size, const GUIStyleSheetRules& rules, const Color& tint, u64 batchId, TInlineArray<GUIRenderElement, 4>& outRenderElements)
{
	// Skip building the sprite if invisible
	if((tint.A * rules.BackgroundColor.A * rules.Opacity) <= 0.0f)
		return;

	mBackgroundSpriteInformation.Width = size.Width;
	mBackgroundSpriteInformation.Height = size.Height;

	if(mBackgroundPathBuilder)
	{
		SpriteVectorPathCreateInformation spriteVectorPathCreateInformation;
		spriteVectorPathCreateInformation.Size = size;
		spriteVectorPathCreateInformation.VectorPath = mBackgroundPathBuilder->BuildPath(spriteVectorPathCreateInformation.Size, rules);

		mBackgroundSpriteInformation.Image = SpriteVectorPath::Create(spriteVectorPathCreateInformation);
	}
	else
		mBackgroundSpriteInformation.Image = nullptr;

	mBackgroundSpriteInformation.Color = tint;
	mBackgroundSpriteInformation.Color.A *= rules.Opacity;

	mBackgroundSprite.Update(mBackgroundSpriteInformation, batchId);

	// Calculate content bounds
	const Rect2 backgroundImageBounds(
		0.0f, 0.0f,
		(float)size.Width, (float)size.Height);

	// Populate GUI render elements from the sprites
	{
		using T = GUIRenderElementHelper;
		T::Append({ T::SpriteInfo(&mBackgroundSprite, 1, backgroundImageBounds) }, outRenderElements );
	}
}

void GUIBackgroundSprite::BuildRenderElements(const Size2UI& size, const GUIElementStyle& style, GUIElementState state, const Color& tint, u64 batchId, TInlineArray<GUIRenderElement, 4>& outRenderElements)
{
	mBackgroundSpriteInformation.Width = size.Width;
	mBackgroundSpriteInformation.Height = size.Height;

	const HSpriteImage& activeImage = style.GetImageForState(state);
	if(SpriteImage::CheckIsLoaded(activeImage))
		mBackgroundSpriteInformation.Image = activeImage;
	else
		mBackgroundSpriteInformation.Image = nullptr;

	mBackgroundSpriteInformation.BorderLeft = style.Border.Left;
	mBackgroundSpriteInformation.BorderRight = style.Border.Right;
	mBackgroundSpriteInformation.BorderTop = style.Border.Top;
	mBackgroundSpriteInformation.BorderBottom = style.Border.Bottom;
	mBackgroundSpriteInformation.Color = tint;

	mBackgroundSprite.Update(mBackgroundSpriteInformation, batchId);

	// Calculate content bounds
	const Rect2 backgroundImageBounds(
		0.0f, 0.0f,
		(float)size.Width, (float)size.Height);

	// Populate GUI render elements from the sprites
	{
		using T = GUIRenderElementHelper;
		T::Populate({ T::SpriteInfo(&mBackgroundSprite, 1, backgroundImageBounds) }, outRenderElements );
	}
}

void GUIBackgroundSprite::SetAnimationStartTime(float time)
{
	mBackgroundSpriteInformation.AnimationStartTime = time;
}

void GUISpriteHelper::BuildSpriteRenderElements(GUIElement& element, GUIElementState state, GUIBackgroundSprite& sprite)
{
	const Size2UI size(element.GetLayoutData().Area.Width, element.GetLayoutData().Area.Height);
	const u64 batchId = (u64)element.GetParentWidget();
	const Color& tint = element.GetTint();

	const bool isUsingStyleSheets = element.IsUsingStyleSheets();
	if(isUsingStyleSheets)
	{
		const GUIStyleSheetRules& styleSheetRules = element.mStyleSheetRuleInformation.CurrentStateRuleset->Rules;
		sprite.BuildRenderElements(size, styleSheetRules, tint, batchId, element.mRenderElements);
	}
	else
	{
		sprite.BuildRenderElements(size, *element.GetStyle(), state, tint, batchId, element.mRenderElements);
	}
}
