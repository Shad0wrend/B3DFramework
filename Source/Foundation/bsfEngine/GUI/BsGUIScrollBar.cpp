//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsGUIScrollBar.h"
#include "Image/BsSpriteTexture.h"
#include "GUI/BsGUIElementStyle.h"
#include "GUI/BsGUIDimensions.h"
#include "GUI/BsGUILayoutX.h"
#include "GUI/BsGUILayoutY.h"
#include "GUI/BsGUIButton.h"
#include "GUI/BsGUISliderHandle.h"
#include "GUI/BsGUISpace.h"

using namespace std::placeholders;

namespace bs
{
	const UINT32 GUIScrollBar::ButtonScrollAmount = 10;

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

	GUIScrollBar::GUIScrollBar(bool horizontal, bool resizable, const String& styleName, const GUIDimensions& dimensions)
		:GUIElement(styleName, dimensions), mHorizontal(horizontal)
	{
		mImageSprite = bs_new<ImageSprite>();

		GUISliderHandleFlags flags;
		if (resizable)
			flags |= GUISliderHandleFlag::Resizeable;

		if(mHorizontal)
		{
			mLayout = GUILayoutX::Create();
			RegisterChildElementInternal(mLayout);

			mUpBtn = GUIButton::Create(HString(""), "ScrollLeftBtn");
			mDownBtn = GUIButton::Create(HString(""), "ScrollRightBtn");

			mHandleBtn = GUISliderHandle::Create(flags | GUISliderHandleFlag::Horizontal, GetSubStyleName(GetHScrollHandleType()));
		}
		else
		{
			mLayout = GUILayoutY::Create();
			RegisterChildElementInternal(mLayout);

			mUpBtn = GUIButton::Create(HString(""), "ScrollUpBtn");
			mDownBtn = GUIButton::Create(HString(""), "ScrollDownBtn");

			mHandleBtn = GUISliderHandle::Create(flags | GUISliderHandleFlag::Vertical, getSubStyleName(getVScrollHandleType()));
		}

		GUIElementOptions scrollUpBtnOptions = mUpBtn->getOptionFlags();
		scrollUpBtnOptions.unset(GUIElementOption::AcceptsKeyFocus);

		mUpBtn->setOptionFlags(scrollUpBtnOptions);

		GUIElementOptions scrollDownBtnOptions = mDownBtn->getOptionFlags();
		scrollDownBtnOptions.unset(GUIElementOption::AcceptsKeyFocus);

		mDownBtn->setOptionFlags(scrollDownBtnOptions);

		mLayout->addNewElement<GUIFixedSpace>(2);
		mLayout->addElement(mUpBtn);
		mLayout->addElement(mHandleBtn);
		mLayout->addElement(mDownBtn);
		mLayout->addNewElement<GUIFixedSpace>(2);

		mHandleBtn->onHandleMovedOrResized.connect(std::bind(&GUIScrollBar::handleMoved, this, _1, _2));

		mUpBtn->onClick.connect(std::bind(&GUIScrollBar::upButtonClicked, this));
		mDownBtn->onClick.connect(std::bind(&GUIScrollBar::downButtonClicked, this));
	}

	GUIScrollBar::~GUIScrollBar()
	{
		bs_delete(mImageSprite);

		GUIElement::destroy(mUpBtn);
		GUIElement::destroy(mDownBtn);
		GUIElement::destroy(mHandleBtn);
	}

	void GUIScrollBar::UpdateRenderElementsInternal()
	{
		IMAGE_SPRITE_DESC desc;

		if(GetStyleInternal()->normal.texture != nullptr && GetStyleInternal()->normal.texture.isLoaded())
			desc.texture = GetStyleInternal()->normal.texture;

		desc.width = mLayoutData.area.width;
		desc.height = mLayoutData.area.height;
		desc.color = getTint();

		mImageSprite->update(desc, (UINT64)GetParentWidgetInternal());

		// Populate GUI render elements from the sprites
		{
			using T = impl::GUIRenderElementHelper;
			T::populate({ T::SpriteInfo(mImageSprite, 2) }, mRenderElements); // +2 depth because child buttons use +1
		}

		GUIElement::updateRenderElementsInternal();
	}

	void GUIScrollBar::UpdateClippedBounds()
	{
		mClippedBounds = Rect2I(0, 0, 0, 0); // We don't want any mouse input for this element. This is just a container.
	}

	Vector2I GUIScrollBar::GetOptimalSizeInternal() const
	{
		return mLayout->GetOptimalSizeInternal();
	}

	UINT32 GUIScrollBar::GetRenderElementDepthRangeInternal() const
	{
		return 3;
	}

	void GUIScrollBar::FillBuffer(
		UINT8* vertices,
		UINT32* indices,
		UINT32 vertexOffset,
		UINT32 indexOffset,
		const Vector2I& offset,
		UINT32 maxNumVerts,
		UINT32 maxNumIndices,
		UINT32 renderElementIdx) const
	{
		UINT8* uvs = vertices + sizeof(Vector2);
		UINT32 vertexStride = sizeof(Vector2) * 2;
		UINT32 indexStride = sizeof(UINT32);

		Vector2I layoutOffset = Vector2I(mLayoutData.area.x, mLayoutData.area.y) + offset;
		mImageSprite->fillBuffer(vertices, uvs, indices, vertexOffset, indexOffset, maxNumVerts, maxNumIndices,
			vertexStride, indexStride, renderElementIdx, layoutOffset, mLayoutData.getLocalClipRect());
	}

	void GUIScrollBar::StyleUpdated()
	{
		if (mHorizontal)
			mHandleBtn->setStyle(getSubStyleName(getHScrollHandleType()));
		else
			mHandleBtn->setStyle(getSubStyleName(getVScrollHandleType()));
	}

	void GUIScrollBar::HandleMoved(float handlePct, float sizePct)
	{
		if(!onScrollOrResize.empty())
			onScrollOrResize(handlePct, sizePct);
	}

	void GUIScrollBar::UpButtonClicked()
	{
		float handleOffset = 0.0f;
		float scrollableSize = (float)mHandleBtn->getScrollableSize();
		
		if(scrollableSize > 0.0f)
			handleOffset = ButtonScrollAmount / scrollableSize;

		scroll(handleOffset);
	}

	void GUIScrollBar::DownButtonClicked()
	{
		float handleOffset = 0.0f;
		float scrollableSize = (float)mHandleBtn->getScrollableSize();

		if(scrollableSize > 0.0f)
			handleOffset = ButtonScrollAmount / scrollableSize;

		scroll(-handleOffset);
	}

	void GUIScrollBar::Scroll(float amount)
	{
		float newHandlePos = Math::clamp01(mHandleBtn->getHandlePos() - amount);

		float oldHandlePos = mHandleBtn->getHandlePos();
		mHandleBtn->SetHandlePosInternal(newHandlePos);

		if (oldHandlePos != mHandleBtn->getHandlePos())
		{
			mHandleBtn->MarkLayoutAsDirtyInternal();

			if (!onScrollOrResize.empty())
				onScrollOrResize(newHandlePos, mHandleBtn->GetHandleSizePctInternal());
		}
	}

	void GUIScrollBar::SetHandleSizeInternal(float pct)
	{
		mHandleBtn->SetHandleSizeInternal(pct);
	}

	void GUIScrollBar::SetScrollPosInternal(float pct)
	{
		mHandleBtn->SetHandlePosInternal(pct);
	}

	float GUIScrollBar::GetScrollPos() const
	{
		return mHandleBtn->getHandlePos();
	}

	void GUIScrollBar::SetScrollPos(float pct)
	{
		float oldHandlePos = mHandleBtn->getHandlePos();
		mHandleBtn->SetHandlePosInternal(pct);

		if (oldHandlePos != mHandleBtn->getHandlePos())
			mHandleBtn->MarkLayoutAsDirtyInternal();
	}

	float GUIScrollBar::GetHandleSize() const
	{
		return mHandleBtn->GetHandleSizePctInternal();
	}

	void GUIScrollBar::SetHandleSize(float pct)
	{
		mHandleBtn->SetHandleSizeInternal(pct);
		mHandleBtn->MarkLayoutAsDirtyInternal();
	}

	UINT32 GUIScrollBar::GetScrollableSize() const
	{
		return mHandleBtn->getScrollableSize();
	}

	void GUIScrollBar::SetTint(const Color& color)
	{
		mUpBtn->setTint(color);
		mDownBtn->setTint(color);
		mHandleBtn->setTint(color);

		GUIElement::setTint(color);
	}
}
