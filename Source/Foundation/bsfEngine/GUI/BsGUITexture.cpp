//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsGUITexture.h"
#include "2D/BsImageSprite.h"
#include "GUI/BsGUISkin.h"
#include "Image/BsSpriteTexture.h"
#include "GUI/BsGUISizeConstraints.h"

using namespace bs;

const String& GUITexture::GetGuiTypeName()
{
	static String name = "Texture";
	return name;
}

GUITexture::GUITexture(const String& styleName, const HSpriteImage& image, TextureScaleMode scale, bool transparent, const GUISizeConstraints& dimensions)
	: GUIElement(styleName, dimensions), mScaleMode(scale), mTransparent(transparent), mUsingStyleTexture(false)
{
	mImageSprite = B3DNew<ImageSprite>();
	mDesc.AnimationStartTime = GetTime().GetTime();

	if(image != nullptr)
	{
		mActiveImage = image;
		mUsingStyleTexture = false;
	}
	else
	{
		mActiveImage = GetStyle()->Normal.Image;
		mUsingStyleTexture = true;
	}

	if(SpriteImage::CheckIsLoaded(mActiveImage))
	{
		const Size2UI& animationFrameSize = mActiveImage->GetAnimationFrameSize();
		mActiveImageWidth = animationFrameSize.Width;
		mActiveImageHeight = animationFrameSize.Height;
	}
	else
	{
		mActiveImageWidth = 0;
		mActiveImageHeight = 0;
	}
}

GUITexture::~GUITexture()
{
	B3DDelete(mImageSprite);
}

GUITexture* GUITexture::Create(const HSpriteImage& image, TextureScaleMode scale, bool transparent, const GUIOptions& options, const String& styleName)
{
	return new(B3DAllocate<GUITexture>()) GUITexture(GetStyleName<GUITexture>(styleName), image, scale, transparent, GUISizeConstraints::Create(options));
}

GUITexture* GUITexture::Create(const HSpriteImage& image, TextureScaleMode scale, bool transparent, const String& styleName)
{
	return new(B3DAllocate<GUITexture>()) GUITexture(GetStyleName<GUITexture>(styleName), image, scale, transparent, GUISizeConstraints::Create());
}

GUITexture* GUITexture::Create(const HSpriteImage& image, TextureScaleMode scale, const GUIOptions& options, const String& styleName)
{
	return new(B3DAllocate<GUITexture>()) GUITexture(GetStyleName<GUITexture>(styleName), image, scale, true, GUISizeConstraints::Create(options));
}

GUITexture* GUITexture::Create(const HSpriteImage& image, TextureScaleMode scale, const String& styleName)
{
	return new(B3DAllocate<GUITexture>()) GUITexture(GetStyleName<GUITexture>(styleName), image, scale, true, GUISizeConstraints::Create());
}

GUITexture* GUITexture::Create(const HSpriteImage& image, const GUIOptions& options, const String& styleName)
{
	return new(B3DAllocate<GUITexture>()) GUITexture(GetStyleName<GUITexture>(styleName), image, TextureScaleMode::StretchToFit, true, GUISizeConstraints::Create(options));
}

GUITexture* GUITexture::Create(const HSpriteImage& image, const String& styleName)
{
	return new(B3DAllocate<GUITexture>()) GUITexture(GetStyleName<GUITexture>(styleName), image, TextureScaleMode::StretchToFit, true, GUISizeConstraints::Create());
}

GUITexture* GUITexture::Create(TextureScaleMode scale, const GUIOptions& options, const String& styleName)
{
	return new(B3DAllocate<GUITexture>()) GUITexture(GetStyleName<GUITexture>(styleName), HSpriteImage(), scale, true, GUISizeConstraints::Create(options));
}

GUITexture* GUITexture::Create(TextureScaleMode scale, const String& styleName)
{
	return new(B3DAllocate<GUITexture>()) GUITexture(GetStyleName<GUITexture>(styleName), HSpriteImage(), scale, true, GUISizeConstraints::Create());
}

GUITexture* GUITexture::Create(const GUIOptions& options, const String& styleName)
{
	return new(B3DAllocate<GUITexture>()) GUITexture(GetStyleName<GUITexture>(styleName), HSpriteImage(), TextureScaleMode::StretchToFit, true, GUISizeConstraints::Create(options));
}

GUITexture* GUITexture::Create(const String& styleName)
{
	return new(B3DAllocate<GUITexture>()) GUITexture(GetStyleName<GUITexture>(styleName), HSpriteImage(), TextureScaleMode::StretchToFit, true, GUISizeConstraints::Create());
}

void GUITexture::SetImage(const HSpriteImage& image)
{
	Vector2I origSize = mSizeConstraints.CalculateConstrainedSize(CalculateUnconstrainedOptimalSize()).Optimal;

	mActiveImage = image;

	if(SpriteImage::CheckIsLoaded(mActiveImage))
	{
		const Size2UI& animationFrameSize = mActiveImage->GetAnimationFrameSize();
		mActiveImageWidth = animationFrameSize.Width;
		mActiveImageHeight = animationFrameSize.Height;
	}
	else
	{
		mActiveImageWidth = 0;
		mActiveImageHeight = 0;
	}

	mUsingStyleTexture = false;
	mDesc.AnimationStartTime = GetTime().GetTime();

	Vector2I newSize = mSizeConstraints.CalculateConstrainedSize(CalculateUnconstrainedOptimalSize()).Optimal;
	if(origSize != newSize)
		MarkLayoutAsDirty();
	else
		MarkContentAsDirty();
}

void GUITexture::UpdateRenderElements()
{
	Size2UI textureSize(BsZero);
	if(SpriteImage::CheckIsLoaded(mActiveImage))
	{
		mDesc.Image = mActiveImage;
		textureSize = mDesc.Image->GetAnimationFrameSize();
	}
	Vector2I destSize(mLayoutData.Area.Width, mLayoutData.Area.Height);

	// ScaleToFit is the only scaling mode that might result in the GUITexture area not being completely covered by
	// the sprite. We need the actual sprite size and offsets to center it.
	if(mScaleMode == TextureScaleMode::ScaleToFit)
	{
		if(destSize.X != 0 && destSize.Y != 0)
		{
			float aspectX = textureSize.Width / (float)destSize.X;
			float aspectY = textureSize.Height / (float)destSize.Y;

			if(aspectY > aspectX)
			{
				destSize.X = Math::RoundToU32(textureSize.Width / aspectY);
				destSize.Y = Math::RoundToU32(textureSize.Height / aspectY);
			}
			else
			{
				destSize.X = Math::RoundToU32(textureSize.Width / aspectX);
				destSize.Y = Math::RoundToU32(textureSize.Height / aspectX);
			}
		}

		mImageSpriteOffset = Vector2I(
			((i32)mLayoutData.Area.Width - destSize.X) / 2,
			((i32)mLayoutData.Area.Height - destSize.Y) / 2);
	}
	else
		mImageSpriteOffset = Vector2I();

	mDesc.Width = (u32)destSize.X;
	mDesc.Height = (u32)destSize.Y;

	mDesc.BorderLeft = GetStyle()->Border.Left;
	mDesc.BorderRight = GetStyle()->Border.Right;
	mDesc.BorderTop = GetStyle()->Border.Top;
	mDesc.BorderBottom = GetStyle()->Border.Bottom;
	mDesc.Transparent = mTransparent;
	mDesc.Color = GetTint();

	if(mScaleMode != TextureScaleMode::ScaleToFit)
		mDesc.UvScale = ImageSprite::GetTextureUvScale(textureSize, destSize, mScaleMode);
	else
		mDesc.UvScale = Vector2::kOne;

	mImageSprite->Update(mDesc, (u64)GetParentWidget());

	// Populate GUI render elements from the sprites
	{
		using T = GUIRenderElementHelper;
		T::Populate({ T::SpriteInfo(mImageSprite) }, mRenderElements);
	}

	GUIElement::UpdateRenderElements();
}

void GUITexture::NotifyStyleChanged()
{
	if(mUsingStyleTexture)
	{
		mActiveImage = GetStyle()->Normal.Image;
		mDesc.AnimationStartTime = GetTime().GetTime();

		if(SpriteImage::CheckIsLoaded(mActiveImage))
		{
			const Size2UI& animationFrameSize = mActiveImage->GetAnimationFrameSize();
			mActiveImageWidth = animationFrameSize.Width;
			mActiveImageHeight = animationFrameSize.Height;
		}
		else
		{
			mActiveImageWidth = 0;
			mActiveImageHeight = 0;
		}
	}
}

Vector2I GUITexture::CalculateUnconstrainedOptimalSize() const
{
	// TODO - Accounting for style dimensions might be redundant here, I'm pretty sure we do that on higher level anyway
	Vector2I optimalSize;

	// Note: We use cached texture size here. This is because we use this method for checking we a layout update is
	// needed (size change is detected). Sprite texture could change without us knowing and by storing the size we can
	// safely detect this. (In short, don't do mActiveTexture->getFrameWidth/Height() here)

	if(GetSizeConstraints().IsWidthFixed())
		optimalSize.X = GetSizeConstraints().MinWidth;
	else
	{
		if(SpriteImage::CheckIsLoaded(mActiveImage))
			optimalSize.X = mActiveImageWidth;
		else
			optimalSize.X = GetSizeConstraints().MaxWidth;
	}

	if(GetSizeConstraints().IsHeightFixed())
		optimalSize.Y = GetSizeConstraints().MinHeight;
	else
	{
		if(SpriteImage::CheckIsLoaded(mActiveImage))
			optimalSize.Y = mActiveImageHeight;
		else
			optimalSize.Y = GetSizeConstraints().MaxHeight;
	}

	return optimalSize;
}

void GUITexture::FillBuffer(
	u8* vertices,
	u32* indices,
	u32 vertexOffset,
	u32 indexOffset,
	const Vector2I& offset,
	u32 maxNumVerts,
	u32 maxNumIndices,
	u32 renderElementIdx) const
{
	u8* uvs = vertices + sizeof(Vector2);
	u32 vertexStride = sizeof(Vector2) * 2;
	u32 indexStride = sizeof(u32);

	Vector2I layoutOffset = Vector2I(mLayoutData.Area.X, mLayoutData.Area.Y) + mImageSpriteOffset + offset;
	mImageSprite->FillBuffer(vertices, uvs, indices, vertexOffset, indexOffset, maxNumVerts, maxNumIndices, vertexStride, indexStride, renderElementIdx, layoutOffset, mLayoutData.GetLocalClipRect());
}
