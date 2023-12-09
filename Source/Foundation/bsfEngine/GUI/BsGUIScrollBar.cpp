//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsGUIScrollBar.h"
#include "Image/BsSpriteTexture.h"
#include "GUI/BsGUIElementStyle.h"
#include "GUI/BsGUISizeConstraints.h"
#include "GUI/BsGUILayoutX.h"
#include "GUI/BsGUILayoutY.h"
#include "GUI/BsGUIButton.h"
#include "GUI/BsGUISliderHandle.h"
#include "GUI/BsGUISpace.h"

using namespace std::placeholders;

using namespace bs;

const u32 GUIScrollBar::kButtonScrollAmount = 10;

const String& GUIScrollBar::GetHScrollHandleType()
{
	static String typeName = "UIScrollBarHHandle";
	return typeName;
}

const String& GUIScrollBar::GetVScrollHandleType()
{
	static String typeName = "UIScrollBarVHandle";
	return typeName;
}

GUIScrollBar::GUIScrollBar(bool horizontal, bool resizable, const String& styleName, const GUISizeConstraints& dimensions)
	: GUIElement(styleName, dimensions), mHorizontal(horizontal)
{
	mImageSprite = B3DNew<ImageSprite>();

	GUISliderHandleFlags flags;
	if(resizable)
		flags |= GUISliderHandleFlag::Resizeable;

	if(mHorizontal)
	{
		mLayout = GUILayoutX::Create();
		RegisterChildElement(mLayout);

		mUpBtn = GUIButton::Create(HString(""), "ScrollLeftBtn");
		mDownBtn = GUIButton::Create(HString(""), "ScrollRightBtn");

		mHandleBtn = GUISliderHandle::Create(flags | GUISliderHandleFlag::Horizontal, GetSubStyleName(GetHScrollHandleType()));
	}
	else
	{
		mLayout = GUILayoutY::Create();
		RegisterChildElement(mLayout);

		mUpBtn = GUIButton::Create(HString(""), "ScrollUpBtn");
		mDownBtn = GUIButton::Create(HString(""), "ScrollDownBtn");

		mHandleBtn = GUISliderHandle::Create(flags | GUISliderHandleFlag::Vertical, GetSubStyleName(GetVScrollHandleType()));
	}

	GUIElementOptions scrollUpBtnOptions = mUpBtn->GetOptionFlags();
	scrollUpBtnOptions.Unset(GUIElementOption::AcceptsKeyFocus);

	mUpBtn->SetOptionFlags(scrollUpBtnOptions);

	GUIElementOptions scrollDownBtnOptions = mDownBtn->GetOptionFlags();
	scrollDownBtnOptions.Unset(GUIElementOption::AcceptsKeyFocus);

	mDownBtn->SetOptionFlags(scrollDownBtnOptions);

	mLayout->AddNewElement<GUIFixedSpace>(2);
	mLayout->AddElement(mUpBtn);
	mLayout->AddElement(mHandleBtn);
	mLayout->AddElement(mDownBtn);
	mLayout->AddNewElement<GUIFixedSpace>(2);

	mHandleBtn->OnHandleMovedOrResized.Connect(std::bind(&::bs::GUIScrollBar::HandleMoved, this, _1, _2));

	mUpBtn->OnClick.Connect(std::bind(&::bs::GUIScrollBar::UpButtonClicked, this));
	mDownBtn->OnClick.Connect(std::bind(&::bs::GUIScrollBar::DownButtonClicked, this));
}

GUIScrollBar::~GUIScrollBar()
{
	B3DDelete(mImageSprite);

	GUIElement::Destroy(mUpBtn);
	GUIElement::Destroy(mDownBtn);
	GUIElement::Destroy(mHandleBtn);
}

void GUIScrollBar::UpdateRenderElements()
{
	ImageSpriteInformation desc;

	if(GetStyle()->Normal.Image != nullptr && GetStyle()->Normal.Image.IsLoaded())
		desc.Image = GetStyle()->Normal.Image;

	desc.Width = mLayoutData.Area.Width;
	desc.Height = mLayoutData.Area.Height;
	desc.Color = GetTint();

	mImageSprite->Update(desc, (u64)GetParentWidget());

	// Populate GUI render elements from the sprites
	{
		using T = GUIRenderElementHelper;
		T::Populate({ T::SpriteInfo(mImageSprite, 2) }, mRenderElements); // +2 depth because child buttons use +1
	}

	GUIElement::UpdateRenderElements();
}

void GUIScrollBar::UpdateClippedBounds()
{
	mClippedBounds = Rect2I(0, 0, 0, 0); // We don't want any mouse input for this element. This is just a container.
}

Vector2I GUIScrollBar::CalculateUnconstrainedOptimalSize() const
{
	return mLayout->CalculateUnconstrainedOptimalSize();
}

u32 GUIScrollBar::GetRenderElementDepthRange() const
{
	return 3;
}

void GUIScrollBar::FillBuffer(
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

	Vector2I layoutOffset = Vector2I(mLayoutData.Area.X, mLayoutData.Area.Y) + offset;
	mImageSprite->FillBuffer(vertices, uvs, indices, vertexOffset, indexOffset, maxNumVerts, maxNumIndices, vertexStride, indexStride, renderElementIdx, layoutOffset, mLayoutData.GetLocalClipRect());
}

void GUIScrollBar::NotifyStyleChanged()
{
	if(mHorizontal)
		mHandleBtn->SetStyle(GetSubStyleName(GetHScrollHandleType()));
	else
		mHandleBtn->SetStyle(GetSubStyleName(GetVScrollHandleType()));
}

void GUIScrollBar::HandleMoved(float handlePct, float sizePct)
{
	if(!OnScrollOrResize.Empty())
		OnScrollOrResize(handlePct, sizePct);
}

void GUIScrollBar::UpButtonClicked()
{
	float handleOffset = 0.0f;
	float scrollableSize = (float)mHandleBtn->GetScrollableLength();

	if(scrollableSize > 0.0f)
		handleOffset = kButtonScrollAmount / scrollableSize;

	Scroll(handleOffset);
}

void GUIScrollBar::DownButtonClicked()
{
	float handleOffset = 0.0f;
	float scrollableSize = (float)mHandleBtn->GetScrollableLength();

	if(scrollableSize > 0.0f)
		handleOffset = kButtonScrollAmount / scrollableSize;

	Scroll(-handleOffset);
}

void GUIScrollBar::Scroll(float amount)
{
	float newHandlePos = Math::Clamp01(mHandleBtn->GetHandlePositionInPercent() - amount);

	float oldHandlePos = mHandleBtn->GetHandlePositionInPercent();
	mHandleBtn->SetHandlePositionInPercent(newHandlePos);

	if(oldHandlePos != mHandleBtn->GetHandlePositionInPercent())
	{
		mHandleBtn->MarkLayoutAsDirty();

		if(!OnScrollOrResize.Empty())
			OnScrollOrResize(newHandlePos, mHandleBtn->GetHandleSizeInPercent());
	}
}

void GUIScrollBar::SetHandleSizeInternal(float pct)
{
	mHandleBtn->SetHandleSizeInPercent(pct);
}

void GUIScrollBar::SetScrollPosInternal(float pct)
{
	mHandleBtn->SetHandlePositionInPercent(pct);
}

float GUIScrollBar::GetScrollPos() const
{
	return mHandleBtn->GetHandlePositionInPercent();
}

void GUIScrollBar::SetScrollPos(float pct)
{
	float oldHandlePos = mHandleBtn->GetHandlePositionInPercent();
	mHandleBtn->SetHandlePositionInPercent(pct);

	if(oldHandlePos != mHandleBtn->GetHandlePositionInPercent())
		mHandleBtn->MarkLayoutAsDirty();
}

float GUIScrollBar::GetHandleSize() const
{
	return mHandleBtn->GetHandleSizeInPercent();
}

void GUIScrollBar::SetHandleSize(float pct)
{
	mHandleBtn->SetHandleSizeInPercent(pct);
	mHandleBtn->MarkLayoutAsDirty();
}

u32 GUIScrollBar::GetScrollableSize() const
{
	return mHandleBtn->GetScrollableLength();
}

void GUIScrollBar::SetTint(const Color& color)
{
	mUpBtn->SetTint(color);
	mDownBtn->SetTint(color);
	mHandleBtn->SetTint(color);

	GUIElement::SetTint(color);
}
