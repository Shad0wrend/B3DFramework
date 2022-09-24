//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsGUIScrollArea.h"
#include "GUI/BsGUIElementStyle.h"
#include "GUI/BsGUIDimensions.h"
#include "GUI/BsGUILayoutY.h"
#include "GUI/BsGUIScrollBarVert.h"
#include "GUI/BsGUIScrollBarHorz.h"
#include "GUI/BsGUIMouseEvent.h"
#include "GUI/BsGUILayoutUtility.h"

using namespace std::placeholders;

namespace bs
{
	const UINT32 GUIScrollArea::ScrollBarWidth = 16;
	const UINT32 GUIScrollArea::WheelScrollAmount = 50;

	GUIScrollArea::GUIScrollArea(ScrollBarType vertBarType, ScrollBarType horzBarType,
		const String& scrollBarStyle, const String& scrollAreaStyle, const GUIDimensions& dimensions)
		: GUIElementContainer(dimensions), mVertBarType(vertBarType), mHorzBarType(horzBarType)
		, mScrollBarStyle(scrollBarStyle), mVertScroll(nullptr), mHorzScroll(nullptr), mVertOffset(0), mHorzOffset(0)
		, mRecalculateVertOffset(false), mRecalculateHorzOffset(false)
	{
		mContentLayout = GUILayoutY::Create();
		RegisterChildElementInternal(mContentLayout);

		mHorzScroll = GUIScrollBarHorz::Create(mScrollBarStyle);
		mVertScroll = GUIScrollBarVert::Create(mScrollBarStyle);

		RegisterChildElementInternal(mHorzScroll);
		RegisterChildElementInternal(mVertScroll);

		mHorzScroll->OnScrollOrResize.Connect(std::bind(&GUIScrollArea::HorzScrollUpdate, this, _1));
		mVertScroll->OnScrollOrResize.Connect(std::bind(&GUIScrollArea::VertScrollUpdate, this, _1));
	}

	void GUIScrollArea::UpdateClippedBounds()
	{
		mClippedBounds = mLayoutData.Area;
		mClippedBounds.Clip(mLayoutData.ClipRect);
	}

	Vector2I GUIScrollArea::GetOptimalSizeInternal() const
	{
		Vector2I optimalSize = mContentLayout->GetOptimalSizeInternal();

		// Provide 10x10 in case underlying layout is empty because
		// 0 doesn't work well with the layout system
		optimalSize.X = std::max(10, optimalSize.X);
		optimalSize.Y = std::max(10, optimalSize.Y);

		return optimalSize;
	}

	LayoutSizeRange GUIScrollArea::CalculateLayoutSizeRangeInternal() const
	{
		// I'm ignoring scroll bars here since if the content layout fits
		// then they're not needed and the range is valid. And if it doesn't
		// fit the area will get clipped anyway and including the scroll bars
		// won't change the size much, but it would complicate this method significantly.
		if (mContentLayout->IsActiveInternal())
			return mDimensions.CalculateSizeRange(GetOptimalSizeInternal());

		return mDimensions.CalculateSizeRange(Vector2I());
	}

	LayoutSizeRange GUIScrollArea::GetLayoutSizeRangeInternal() const
	{
		return mSizeRange;
	}

	void GUIScrollArea::UpdateOptimalLayoutSizesInternal()
	{
		// Update all children first, otherwise we can't determine our own optimal size
		GUIElementBase::UpdateOptimalLayoutSizesInternal();

		if (mChildren.size() != mChildSizeRanges.size())
			mChildSizeRanges.resize(mChildren.size());

		UINT32 childIdx = 0;
		for (auto& child : mChildren)
		{
			if (child->IsActiveInternal())
				mChildSizeRanges[childIdx] = child->GetLayoutSizeRangeInternal();
			else
				mChildSizeRanges[childIdx] = LayoutSizeRange();

			childIdx++;
		}

		mSizeRange = mDimensions.CalculateSizeRange(GetOptimalSizeInternal());
	}

	void GUIScrollArea::GetElementAreasInternal(const Rect2I& layoutArea, Rect2I* elementAreas, UINT32 numElements,
		const Vector<LayoutSizeRange>& sizeRanges, const LayoutSizeRange& mySizeRange) const
	{
		Vector2I visibleSize, contentSize;
		GetElementAreasInternal(layoutArea, elementAreas, numElements, sizeRanges, visibleSize, contentSize);
	}

	void GUIScrollArea::GetElementAreasInternal(const Rect2I& layoutArea, Rect2I* elementAreas, UINT32 numElements,
		const Vector<LayoutSizeRange>& sizeRanges, Vector2I& visibleSize, Vector2I& contentSize) const
	{
		assert(mChildren.size() == numElements && numElements == 3);

		UINT32 layoutIdx = 0;
		UINT32 horzScrollIdx = 0;
		UINT32 vertScrollIdx = 0;
		UINT32 idx = 0;
		for (auto& child : mChildren)
		{
			if (child == mContentLayout)
				layoutIdx = idx;

			if (child == mHorzScroll)
				horzScrollIdx = idx;

			if (child == mVertScroll)
				vertScrollIdx = idx;

			idx++;
		}

		// Calculate content layout bounds

		//// We want elements to use their optimal height, since scroll area
		//// technically provides "infinite" space
		UINT32 optimalContentWidth = layoutArea.Width;
		if (mHorzBarType != ScrollBarType::NeverShow)
			optimalContentWidth = sizeRanges[layoutIdx].Optimal.X;

		UINT32 optimalContentHeight = layoutArea.Height;
		if (mVertBarType != ScrollBarType::NeverShow)
			optimalContentHeight = sizeRanges[layoutIdx].Optimal.Y;

		UINT32 layoutWidth = std::max(optimalContentWidth, (UINT32)layoutArea.Width);
		UINT32 layoutHeight = std::max(optimalContentHeight, (UINT32)layoutArea.Height);

		contentSize = GUILayoutUtility::CalcActualSize(layoutWidth, layoutHeight, mContentLayout, false);
		visibleSize = Vector2I(layoutArea.Width, layoutArea.Height);

		bool addHorzScrollbar = (mHorzBarType == ScrollBarType::ShowIfDoesntFit && contentSize.X > visibleSize.X) ||
			mHorzBarType == ScrollBarType::AlwaysShow;

		bool hasHorzScrollbar = false;
		bool hasVertScrollbar = false;
		if (addHorzScrollbar)
		{
			// Make room for scrollbar
			visibleSize.Y = (UINT32)std::max(0, (INT32)layoutArea.Height - (INT32)ScrollBarWidth);
			optimalContentHeight = (UINT32)std::max(0, (INT32)optimalContentHeight - (INT32)ScrollBarWidth);

			if (sizeRanges[layoutIdx].Min.Y > 0)
				optimalContentHeight = std::max((UINT32)sizeRanges[layoutIdx].Min.Y, optimalContentHeight);

			layoutHeight = std::max(optimalContentHeight, (UINT32)visibleSize.Y); // Never go below optimal size

			contentSize = GUILayoutUtility::CalcActualSize(layoutWidth, layoutHeight, mContentLayout, true);
			hasHorzScrollbar = true;
		}

		bool addVertScrollbar = (mVertBarType == ScrollBarType::ShowIfDoesntFit && contentSize.Y > visibleSize.Y) ||
			mVertBarType == ScrollBarType::AlwaysShow;

		if (addVertScrollbar)
		{
			// Make room for scrollbar
			visibleSize.X = (UINT32)std::max(0, (INT32)layoutArea.Width - (INT32)ScrollBarWidth);
			optimalContentWidth = (UINT32)std::max(0, (INT32)optimalContentWidth - (INT32)ScrollBarWidth);

			if (sizeRanges[layoutIdx].Min.X > 0)
				optimalContentWidth = std::max((UINT32)sizeRanges[layoutIdx].Min.X, optimalContentWidth);

			layoutWidth = std::max(optimalContentWidth, (UINT32)visibleSize.X); // Never go below optimal size

			contentSize = GUILayoutUtility::CalcActualSize(layoutWidth, layoutHeight, mContentLayout, true);
			hasVertScrollbar = true;

			if (!hasHorzScrollbar) // Since width has been reduced, we need to check if we require the horizontal scrollbar
			{
				addHorzScrollbar = (mHorzBarType == ScrollBarType::ShowIfDoesntFit && contentSize.X > visibleSize.X) && mHorzBarType != ScrollBarType::NeverShow;

				if (addHorzScrollbar)
				{
					// Make room for scrollbar
					visibleSize.Y = (UINT32)std::max(0, (INT32)layoutArea.Height - (INT32)ScrollBarWidth);
					optimalContentHeight = (UINT32)std::max(0, (INT32)optimalContentHeight - (INT32)ScrollBarWidth);

					if (sizeRanges[layoutIdx].Min.Y > 0)
						optimalContentHeight = std::max((UINT32)sizeRanges[layoutIdx].Min.Y, optimalContentHeight);

					layoutHeight = std::max(optimalContentHeight, (UINT32)visibleSize.Y); // Never go below optimal size

					contentSize = GUILayoutUtility::CalcActualSize(layoutWidth, layoutHeight, mContentLayout, true);
					hasHorzScrollbar = true;
				}
			}
		}

		elementAreas[layoutIdx] = Rect2I(layoutArea.X, layoutArea.Y, layoutWidth, layoutHeight);

		// Calculate vertical scrollbar bounds
		if (hasVertScrollbar)
		{
			INT32 scrollBarOffset = (UINT32)std::max(0, (INT32)layoutArea.Width - (INT32)ScrollBarWidth);
			UINT32 scrollBarHeight = layoutArea.Height;
			if (hasHorzScrollbar)
				scrollBarHeight = (UINT32)std::max(0, (INT32)scrollBarHeight - (INT32)ScrollBarWidth);

			elementAreas[vertScrollIdx] = Rect2I(layoutArea.X + scrollBarOffset, layoutArea.Y, ScrollBarWidth, scrollBarHeight);
		}
		else
		{
			elementAreas[vertScrollIdx] = Rect2I(layoutArea.X + layoutWidth, layoutArea.Y, 0, 0);
		}

		// Calculate horizontal scrollbar bounds
		if (hasHorzScrollbar)
		{
			INT32 scrollBarOffset = (UINT32)std::max(0, (INT32)layoutArea.Height - (INT32)ScrollBarWidth);
			UINT32 scrollBarWidth = layoutArea.Width;
			if (hasVertScrollbar)
				scrollBarWidth = (UINT32)std::max(0, (INT32)scrollBarWidth - (INT32)ScrollBarWidth);

			elementAreas[horzScrollIdx] = Rect2I(layoutArea.X, layoutArea.Y + scrollBarOffset, scrollBarWidth, ScrollBarWidth);
		}
		else
		{
			elementAreas[horzScrollIdx] = Rect2I(layoutArea.X, layoutArea.Y + layoutHeight, 0, 0);
		}		
	}

	void GUIScrollArea::UpdateLayoutInternalInternal(const GUILayoutData& data)
	{
		UINT32 numElements = (UINT32)mChildren.size();
		Rect2I* elementAreas = nullptr;

		if (numElements > 0)
			elementAreas = bs_stack_new<Rect2I>(numElements);

		UINT32 layoutIdx = 0;
		UINT32 horzScrollIdx = 0;
		UINT32 vertScrollIdx = 0;
		for (UINT32 i = 0; i < numElements; i++)
		{
			GUIElementBase* child = GetChildInternal(i);

			if (child == mContentLayout)
				layoutIdx = i;

			if (child == mHorzScroll)
				horzScrollIdx = i;

			if (child == mVertScroll)
				vertScrollIdx = i;
		}

		GetElementAreasInternal(data.Area, elementAreas, numElements, mChildSizeRanges, mVisibleSize, mContentSize);

		Rect2I& layoutBounds = elementAreas[layoutIdx];
		Rect2I& horzScrollBounds = elementAreas[horzScrollIdx];
		Rect2I& vertScrollBounds = elementAreas[vertScrollIdx];

		// Recalculate offsets in case scroll percent got updated externally (this needs to be delayed to this point because
		// at the time of the update content and visible sizes might be out of date).
		UINT32 scrollableHeight = (UINT32)std::max(0, INT32(mContentSize.Y) - INT32(mVisibleSize.Y));
		if (mRecalculateVertOffset)
		{
			mVertOffset = scrollableHeight * Math::Clamp01(mVertScroll->GetScrollPos());

			mRecalculateVertOffset = false;
		}

		UINT32 scrollableWidth = (UINT32)std::max(0, INT32(mContentSize.X) - INT32(mVisibleSize.X));
		if (mRecalculateHorzOffset)
		{
			mHorzOffset = scrollableWidth * Math::Clamp01(mHorzScroll->GetScrollPos());

			mRecalculateHorzOffset = false;
		}

		// Reset offset in case layout size changed so everything fits
		mVertOffset = Math::Clamp(mVertOffset, 0.0f, (float)scrollableHeight);
		mHorzOffset = Math::Clamp(mHorzOffset, 0.0f, (float)scrollableWidth);

		// Layout
		if (mContentLayout->IsActiveInternal())
		{
			layoutBounds.X -= Math::FloorToInt(mHorzOffset);
			layoutBounds.Y -= Math::FloorToInt(mVertOffset);

			Rect2I layoutClipRect = data.ClipRect;
			layoutClipRect.Width = (UINT32)mVisibleSize.X;
			layoutClipRect.Height = (UINT32)mVisibleSize.Y;
			layoutClipRect.Clip(data.ClipRect);

			GUILayoutData layoutData = data;
			layoutData.Area = layoutBounds;
			layoutData.ClipRect = layoutClipRect;

			mContentLayout->SetLayoutDataInternal(layoutData);
			mContentLayout->UpdateLayoutInternalInternal(layoutData);
		}

		// Vertical scrollbar
		{
			GUILayoutData vertScrollData = data;
			vertScrollData.Area = vertScrollBounds;

			vertScrollData.ClipRect = vertScrollBounds;
			vertScrollData.ClipRect.Clip(data.ClipRect);

			mVertScroll->SetLayoutDataInternal(vertScrollData);
			mVertScroll->UpdateLayoutInternalInternal(vertScrollData);

			// Set new handle size and update position to match the new size
			UINT32 scrollableHeight = (UINT32)std::max(0, INT32(mContentSize.Y) - INT32(vertScrollBounds.Height));
			float newScrollPct = 0.0f;

			if (scrollableHeight > 0)
				newScrollPct = mVertOffset / scrollableHeight;	

			mVertScroll->SetHandleSizeInternal(vertScrollBounds.Height / (float)mContentSize.Y);
			mVertScroll->SetScrollPosInternal(newScrollPct);
		}

		// Horizontal scrollbar
		{
			GUILayoutData horzScrollData = data;
			horzScrollData.Area = horzScrollBounds;

			horzScrollData.ClipRect = horzScrollBounds;
			horzScrollData.ClipRect.Clip(data.ClipRect);

			mHorzScroll->SetLayoutDataInternal(horzScrollData);
			mHorzScroll->UpdateLayoutInternalInternal(horzScrollData);

			// Set new handle size and update position to match the new size
			UINT32 scrollableWidth = (UINT32)std::max(0, INT32(mContentSize.X) - INT32(horzScrollBounds.Width));
			float newScrollPct = 0.0f;

			if (scrollableWidth > 0)
				newScrollPct = mHorzOffset / scrollableWidth;

			mHorzScroll->SetHandleSizeInternal(horzScrollBounds.Width / (float)mContentSize.X);
			mHorzScroll->SetScrollPosInternal(newScrollPct);
		}

		if (elementAreas != nullptr)
			bs_stack_free(elementAreas);
	}

	void GUIScrollArea::VertScrollUpdate(float scrollPos)
	{
		UINT32 scrollableHeight = (UINT32)std::max(0, INT32(mContentSize.Y) - INT32(mVisibleSize.Y));
		mVertOffset = scrollableHeight * Math::Clamp01(scrollPos);

		MarkLayoutAsDirtyInternal();
	}

	void GUIScrollArea::HorzScrollUpdate(float scrollPos)
	{
		UINT32 scrollableWidth = (UINT32)std::max(0, INT32(mContentSize.X) - INT32(mVisibleSize.X));
		mHorzOffset = scrollableWidth * Math::Clamp01(scrollPos);

		MarkLayoutAsDirtyInternal();
	}

	void GUIScrollArea::ScrollToVertical(float pct)
	{
		mVertScroll->SetScrollPosInternal(pct);
		mRecalculateVertOffset = true;

		MarkLayoutAsDirtyInternal();
	}

	void GUIScrollArea::ScrollToHorizontal(float pct)
	{
		mHorzScroll->SetScrollPosInternal(pct);
		mRecalculateHorzOffset = true;

		MarkLayoutAsDirtyInternal();
	}

	float GUIScrollArea::GetVerticalScroll() const
	{
		if (mVertScroll != nullptr)
			return mVertScroll->GetScrollPos();

		return 0.0f;
	}

	float GUIScrollArea::GetHorizontalScroll() const
	{
		if (mHorzScroll != nullptr)
			return mHorzScroll->GetScrollPos();

		return 0.0f;
	}

	Rect2I GUIScrollArea::GetContentBounds()
	{
		Rect2I bounds = GetBounds();

		if (mHorzScroll)
			bounds.Height -= ScrollBarWidth;

		if (mVertScroll)
			bounds.Width -= ScrollBarWidth;

		return bounds;
	}

	void GUIScrollArea::ScrollUpPx(UINT32 pixels)
	{
		if(mVertScroll != nullptr)
		{
			UINT32 scrollableSize = (UINT32)std::max(0, INT32(mContentSize.Y) - INT32(mVisibleSize.Y));

			float offset = 0.0f;
			if(scrollableSize > 0)
				offset = pixels / (float)scrollableSize;

			mVertScroll->Scroll(offset);
		}
	}

	void GUIScrollArea::ScrollDownPx(UINT32 pixels)
	{
		if(mVertScroll != nullptr)
		{
			UINT32 scrollableSize = (UINT32)std::max(0, INT32(mContentSize.Y) - INT32(mVisibleSize.Y));

			float offset = 0.0f;
			if(scrollableSize > 0)
				offset = pixels / (float)scrollableSize;

			mVertScroll->Scroll(-offset);
		}
	}

	void GUIScrollArea::ScrollLeftPx(UINT32 pixels)
	{
		if(mHorzScroll != nullptr)
		{
			UINT32 scrollableSize = (UINT32)std::max(0, INT32(mContentSize.X) - INT32(mVisibleSize.X));

			float offset = 0.0f;
			if(scrollableSize > 0)
				offset = pixels / (float)scrollableSize;

			mHorzScroll->Scroll(offset);
		}
	}

	void GUIScrollArea::ScrollRightPx(UINT32 pixels)
	{
		if(mHorzScroll != nullptr)
		{
			UINT32 scrollableSize = (UINT32)std::max(0, INT32(mContentSize.X) - INT32(mVisibleSize.X));

			float offset = 0.0f;
			if(scrollableSize > 0)
				offset = pixels / (float)scrollableSize;

			mHorzScroll->Scroll(-offset);
		}
	}

	void GUIScrollArea::ScrollUpPct(float percent)
	{
		if(mVertScroll != nullptr)
			mVertScroll->Scroll(percent);
	}

	void GUIScrollArea::ScrollDownPct(float percent)
	{
		if(mVertScroll != nullptr)
			mVertScroll->Scroll(-percent);
	}

	void GUIScrollArea::ScrollLeftPct(float percent)
	{
		if(mHorzScroll != nullptr)
			mHorzScroll->Scroll(percent);
	}

	void GUIScrollArea::ScrollRightPct(float percent)
	{
		if(mHorzScroll != nullptr)
			mHorzScroll->Scroll(-percent);
	}

	bool GUIScrollArea::MouseEventInternal(const GUIMouseEvent& ev)
	{
		if(ev.GetType() == GUIMouseEventType::MouseWheelScroll)
		{
			// Mouse wheel only scrolls on the Y axis
			if(mVertScroll != nullptr)
			{
				UINT32 scrollableHeight = (UINT32)std::max(0, INT32(mContentSize.Y) - INT32(mVisibleSize.Y));
				float additionalScroll = (float)WheelScrollAmount / scrollableHeight;

				mVertScroll->Scroll(additionalScroll * ev.GetWheelScrollAmount());
				return true;
			}
		}

		return false;
	}

	GUIScrollArea* GUIScrollArea::Create(ScrollBarType vertBarType, ScrollBarType horzBarType,
		const String& scrollBarStyle, const String& scrollAreaStyle)
	{
		return new (bs_alloc<GUIScrollArea>()) GUIScrollArea(vertBarType, horzBarType, scrollBarStyle,
			GetStyleName<GUIScrollArea>(scrollAreaStyle), GUIDimensions::Create());
	}

	GUIScrollArea* GUIScrollArea::Create(const GUIOptions& options, const String& scrollBarStyle,
		const String& scrollAreaStyle)
	{
		return new (bs_alloc<GUIScrollArea>()) GUIScrollArea(ScrollBarType::ShowIfDoesntFit,
			ScrollBarType::ShowIfDoesntFit, scrollBarStyle, GetStyleName<GUIScrollArea>(scrollAreaStyle), GUIDimensions::Create(options));
	}

	GUIScrollArea* GUIScrollArea::Create(const String& scrollBarStyle, const String& scrollAreaStyle)
	{
		return new (bs_alloc<GUIScrollArea>()) GUIScrollArea(ScrollBarType::ShowIfDoesntFit, ScrollBarType::ShowIfDoesntFit, scrollBarStyle,
			GetStyleName<GUIScrollArea>(scrollAreaStyle), GUIDimensions::Create());
	}

	GUIScrollArea* GUIScrollArea::Create(ScrollBarType vertBarType,
		ScrollBarType horzBarType, const GUIOptions& options, const String& scrollBarStyle,
		const String& scrollAreaStyle)
	{
		return new (bs_alloc<GUIScrollArea>()) GUIScrollArea(vertBarType, horzBarType, scrollBarStyle,
			GetStyleName<GUIScrollArea>(scrollAreaStyle), GUIDimensions::Create(options));
	}

	const String& GUIScrollArea::GetGuiTypeName()
	{
		static String typeName = "ScrollArea";
		return typeName;
	}
}
