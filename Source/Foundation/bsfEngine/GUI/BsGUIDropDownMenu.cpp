//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsGUIDropDownMenu.h"
#include "GUI/BsGUIPanel.h"
#include "GUI/BsGUILayoutY.h"
#include "GUI/BsGUILayoutX.h"
#include "GUI/BsGUITexture.h"
#include "GUI/BsGUILabel.h"
#include "GUI/BsGUIButton.h"
#include "GUI/BsGUISpace.h"
#include "GUI/BsGUIContent.h"
#include "GUI/BsGUISkin.h"
#include "RenderAPI/BsViewport.h"
#include "GUI/BsGUIListBox.h"
#include "GUI/BsGUIDropDownBoxManager.h"
#include "Scene/BsSceneObject.h"
#include "GUI/BsGUIDropDownHitBox.h"
#include "GUI/BsGUIDropDownContent.h"
#include "Renderer/BsCamera.h"
#include "Debug/BsDebug.h"

using namespace std::placeholders;

namespace bs
{
	const UINT32 GUIDropDownMenu::DROP_DOWN_BOX_WIDTH = 250;

	GUIDropDownDataEntry GUIDropDownDataEntry::Separator()
	{
		GUIDropDownDataEntry data;
		data.mType = Type::Separator;
		data.mCallback = nullptr;

		return data;
	}

	GUIDropDownDataEntry GUIDropDownDataEntry::Button(const String& label, std::function<void()> callback, const String& shortcutTag)
	{
		GUIDropDownDataEntry data;
		data.mLabel = label;
		data.mType = Type::Entry;
		data.mCallback = callback;
		data.mShortcutTag = shortcutTag;

		return data;
	}

	GUIDropDownDataEntry GUIDropDownDataEntry::SubMenu(const String& label, const GUIDropDownData& data)
	{
		GUIDropDownDataEntry dataEntry;
		dataEntry.mLabel = label;
		dataEntry.mType = Type::SubMenu;
		dataEntry.mChildData = data;

		return dataEntry;
	}

	GUIDropDownMenu::GUIDropDownMenu(const HSceneObject& parent, const DROP_DOWN_BOX_DESC& desc, GUIDropDownType type)
		:CGUIWidget(parent, desc.camera), mRootMenu(nullptr), mFrontHitBox(nullptr), mBackHitBox(nullptr), mCaptureHitBox(nullptr)
	{
		String stylePrefix = "";
		switch(type)
		{
		case GUIDropDownType::ContextMenu:
			stylePrefix = "ContextMenu";
			break;
		case GUIDropDownType::ListBox:
		case GUIDropDownType::MultiListBox:
			stylePrefix = "ListBox";
			break;
		case GUIDropDownType::MenuBar:
			stylePrefix = "MenuBar";
			break;
		}

		mScrollUpStyle = stylePrefix + "ScrollUpBtn";
		mScrollDownStyle = stylePrefix + "ScrollDownBtn";
		mBackgroundStyle = stylePrefix + "Frame";
		mContentStyle = stylePrefix + "Content";
		mSideBackgroundStyle = stylePrefix + "SidebarBg";
		mHandleStyle = stylePrefix + "Handle";

		SetDepth(0); // Needs to be in front of everything
		SetSkin(desc.skin);

		mFrontHitBox = GUIDropDownHitBox::Create(false, false);
		mFrontHitBox->onFocusLost.Connect(std::bind(&GUIDropDownMenu::DropDownFocusLost, this));
		mFrontHitBox->SetFocus(true);
		GUILayoutData hitboxLayoutData = mFrontHitBox->GetLayoutDataInternal();
		hitboxLayoutData.SetWidgetDepth(0);
		hitboxLayoutData.SetPanelDepth(std::numeric_limits<INT16>::min());
		mFrontHitBox->SetLayoutDataInternal(hitboxLayoutData);
		mFrontHitBox->ChangeParentWidgetInternal(GetInternalInternal());
		mFrontHitBox->MarkLayoutAsDirtyInternal();

		mBackHitBox = GUIDropDownHitBox::Create(false, true);
		GUILayoutData backHitboxLayoutData = mBackHitBox->GetLayoutDataInternal();
		backHitboxLayoutData.SetWidgetDepth(0);
		backHitboxLayoutData.SetPanelDepth(std::numeric_limits<INT16>::max());
		mBackHitBox->SetLayoutDataInternal(backHitboxLayoutData);
		mBackHitBox->ChangeParentWidgetInternal(GetInternalInternal());
		mBackHitBox->MarkLayoutAsDirtyInternal();

		SPtr<Viewport> viewport = desc.camera->GetViewport();

		Rect2I targetBounds(0, 0, viewport->GetPixelArea().width, viewport->GetPixelArea().height);
		Vector<Rect2I> captureBounds;
		targetBounds.Cut(desc.additionalBounds, captureBounds);

		mCaptureHitBox = GUIDropDownHitBox::Create(true, false);
		mCaptureHitBox->SetBounds(captureBounds);
		GUILayoutData captureHitboxLayoutData = mCaptureHitBox->GetLayoutDataInternal();
		captureHitboxLayoutData.SetWidgetDepth(0);
		captureHitboxLayoutData.SetPanelDepth(std::numeric_limits<INT16>::max());
		mCaptureHitBox->SetLayoutDataInternal(captureHitboxLayoutData);
		mCaptureHitBox->ChangeParentWidgetInternal(GetInternalInternal());
		mCaptureHitBox->MarkLayoutAsDirtyInternal();

		mAdditionalCaptureBounds = desc.additionalBounds;

		Rect2I availableBounds = viewport->GetPixelArea();
		mRootMenu = bs_new<DropDownSubMenu>(this, nullptr, desc.placement, availableBounds, desc.dropDownData, type, 0);
	}

	GUIDropDownMenu::~GUIDropDownMenu()
	{

	}

	void GUIDropDownMenu::OnDestroyed()
	{
		GUIElement::Destroy(mFrontHitBox);
		GUIElement::Destroy(mBackHitBox);
		GUIElement::Destroy(mCaptureHitBox);
		bs_delete(mRootMenu);
		mRootMenu = nullptr;

		CGUIWidget::OnDestroyed();
	}

	void GUIDropDownMenu::DropDownFocusLost()
	{
		mRootMenu->CloseSubMenu();
		GUIDropDownBoxManager::Instance().CloseDropDownBox();
	}

	void GUIDropDownMenu::NotifySubMenuOpened(DropDownSubMenu* subMenu)
	{
		Vector<Rect2I> bounds;
		while(subMenu != nullptr)
		{
			bounds.push_back(subMenu->GetVisibleBounds());

			subMenu = subMenu->mParent;
		}

		mBackHitBox->SetBounds(bounds);

		for (auto& additionalBound : mAdditionalCaptureBounds)
			bounds.push_back(additionalBound);

		mFrontHitBox->SetBounds(bounds);
	}

	void GUIDropDownMenu::NotifySubMenuClosed(DropDownSubMenu* subMenu)
	{
		Vector<Rect2I> bounds;
		while(subMenu != nullptr)
		{
			bounds.push_back(subMenu->GetVisibleBounds());

			subMenu = subMenu->mParent;
		}

		mBackHitBox->SetBounds(bounds);
		
		for (auto& additionalBound : mAdditionalCaptureBounds)
			bounds.push_back(additionalBound);

		mFrontHitBox->SetBounds(bounds);
	}

	GUIDropDownMenu::DropDownSubMenu::DropDownSubMenu(GUIDropDownMenu* owner, DropDownSubMenu* parent,
		const DropDownAreaPlacement& placement, const Rect2I& availableBounds, const GUIDropDownData& dropDownData,
		GUIDropDownType type, UINT32 depthOffset)
		: mOwner(owner), mType(type), mData(dropDownData), mPage(0), x(0), y(0), width(0), height(0)
		, mDepthOffset(depthOffset), mOpenedUpward(false), mContent(nullptr), mBackgroundFrame(nullptr)
		, mBackgroundPanel(nullptr), mContentPanel(nullptr), mContentLayout(nullptr), mSidebarPanel(nullptr)
		, mParent(parent), mSubMenu(nullptr)
	{
		mAvailableBounds = availableBounds;

		const GUIElementStyle* backgroundStyle = mOwner->GetSkin().GetStyle(mOwner->mBackgroundStyle);
		const GUIElementStyle* sideBarStyle = mOwner->GetSkin().GetStyle(mOwner->mSideBackgroundStyle);

		// Create content GUI element
		mContent = GUIDropDownContent::Create(this, dropDownData, mOwner->mContentStyle);
		mContent->SetKeyboardFocus(true);

		// Content area
		mContentPanel = mOwner->GetPanel()->AddNewElement<GUIPanel>();
		mContentPanel->SetWidth(width);
		mContentPanel->SetHeight(height);
		mContentPanel->SetDepthRange(100 - depthOffset * 2 - 1);

		// Background frame
		mBackgroundPanel = mOwner->GetPanel()->AddNewElement<GUIPanel>();
		mBackgroundPanel->SetWidth(width);
		mBackgroundPanel->SetHeight(height);
		mBackgroundPanel->SetDepthRange(100 - depthOffset * 2);

		GUILayout* backgroundLayout = mBackgroundPanel->AddNewElement<GUILayoutX>();

		mBackgroundFrame = GUITexture::Create(TextureScaleMode::StretchToFit, mOwner->mBackgroundStyle);
		backgroundLayout->AddElement(mBackgroundFrame);

		mContentLayout = mContentPanel->AddNewElement<GUILayoutY>();
		mContentLayout->AddElement(mContent); // Note: It's important this is added to the layout before we
		// use it for size calculations, in order for its skin to be assigned

		UINT32 dropDownBoxWidth = DROP_DOWN_BOX_WIDTH + sideBarStyle->width;

		UINT32 maxNeededHeight = backgroundStyle->margins.top + backgroundStyle->margins.bottom;
		UINT32 numElements = (UINT32)dropDownData.entries.size();
		for (UINT32 i = 0; i < numElements; i++)
			maxNeededHeight += mContent->GetElementHeight(i);

		DropDownAreaPlacement::HorzDir horzDir;
		DropDownAreaPlacement::VertDir vertDir;
		Rect2I placementBounds = placement.GetOptimalBounds(dropDownBoxWidth, maxNeededHeight, availableBounds, horzDir, vertDir);

		mOpenedUpward = vertDir == DropDownAreaPlacement::VertDir::Up;

		UINT32 actualY = placementBounds.y;
		if (mOpenedUpward)
			y = placementBounds.y + placementBounds.height;
		else
			y = placementBounds.y;

		x = placementBounds.x;
		width = placementBounds.width;
		height = placementBounds.height;

		mContentPanel->SetPosition(x, actualY);
		mBackgroundPanel->SetPosition(x, actualY);

		UpdateGuiElements();

		mOwner->NotifySubMenuOpened(this);
	}

	GUIDropDownMenu::DropDownSubMenu::~DropDownSubMenu()
	{
		CloseSubMenu();

		mOwner->NotifySubMenuClosed(this);

		GUIElement::Destroy(mContent);

		GUIElement::Destroy(mBackgroundFrame);

		GUILayout::Destroy(mBackgroundPanel);
		GUILayout::Destroy(mContentPanel);

		if (mSidebarPanel != nullptr)
			GUIPanel::Destroy(mSidebarPanel);
	}

	Vector<GUIDropDownMenu::DropDownSubMenu::PageInfo> GUIDropDownMenu::DropDownSubMenu::GetPageInfos() const
	{
		const GUIElementStyle* backgroundStyle = mOwner->GetSkin().GetStyle(mOwner->mBackgroundStyle);

		INT32 numElements = (INT32)mData.entries.size();

		PageInfo curPageInfo;
		curPageInfo.start = 0;
		curPageInfo.end = 0;
		curPageInfo.idx = 0;
		curPageInfo.height = backgroundStyle->margins.top + backgroundStyle->margins.bottom;
		
		Vector<PageInfo> pageInfos;
		for (INT32 i = 0; i < numElements; i++)
		{
			curPageInfo.height += mContent->GetElementHeight((UINT32)i);
			curPageInfo.end++;

			if (curPageInfo.height > height)
			{
				// Remove last few elements until we fit again
				while (curPageInfo.height > height && i >= 0)
				{
					curPageInfo.height -= mContent->GetElementHeight((UINT32)i);
					curPageInfo.end--;

					i--;
				}

				// Nothing fits, break out of infinite loop
				if (curPageInfo.start >= curPageInfo.end)
					break;

				pageInfos.push_back(curPageInfo);

				curPageInfo.start = curPageInfo.end;
				curPageInfo.height = backgroundStyle->margins.top + backgroundStyle->margins.bottom;

				curPageInfo.idx++;
			}
		}

		if (curPageInfo.start < curPageInfo.end)
			pageInfos.push_back(curPageInfo);

		return pageInfos;
	}

	void GUIDropDownMenu::DropDownSubMenu::UpdateGuiElements()
	{
		// Remove all elements from content layout
		while(mContentLayout->GetNumChildren() > 0)
			mContentLayout->RemoveElementAt(mContentLayout->GetNumChildren() - 1);

		mContentLayout->AddElement(mContent); // Note: Needs to be added first so that size calculations have proper skin to work with

		const GUIElementStyle* backgroundStyle = mOwner->GetSkin().GetStyle(mOwner->mBackgroundStyle);
		const GUIElementStyle* sideBarStyle = mOwner->GetSkin().GetStyle(mOwner->mSideBackgroundStyle);
		const GUIElementStyle* scrollUpStyle = mOwner->GetSkin().GetStyle(mOwner->mScrollUpStyle);
		const GUIElementStyle* scrollDownStyle = mOwner->GetSkin().GetStyle(mOwner->mScrollDownStyle);

		Vector<PageInfo> pageInfos = GetPageInfos();

		UINT32 pageStart = 0, pageEnd = 0;
		UINT32 pageHeight = 0;
		UINT32 pageCount = (UINT32)pageInfos.size();
		if (pageCount > mPage)
		{
			pageStart = pageInfos[mPage].start;
			pageEnd = pageInfos[mPage].end;
			pageHeight = pageInfos[mPage].height;
		}

		INT32 actualY = y;

		if (mOpenedUpward)
			actualY -= (INT32)pageHeight;

		// Add sidebar if needed
		UINT32 contentOffset = 0;
		if (pageInfos.size() > 1)
		{
			UINT32 sidebarHeight = pageHeight - 2;
			contentOffset = sideBarStyle->width;

			if (mSidebarPanel == nullptr)
			{
				mSidebarPanel = mOwner->GetPanel()->AddNewElement<GUIPanel>();

				mScrollUpBtn = GUIButton::Create(HString(""), mOwner->mScrollUpStyle);
				mScrollUpBtn->onClick.Connect(std::bind(&DropDownSubMenu::ScrollUp, this));

				GUIElementOptions scrollUpBtnOptions = mScrollUpBtn->GetOptionFlags();
				scrollUpBtnOptions.Unset(GUIElementOption::AcceptsKeyFocus);

				mScrollUpBtn->SetOptionFlags(scrollUpBtnOptions);

				mScrollDownBtn = GUIButton::Create(HString(""), mOwner->mScrollDownStyle);
				mScrollDownBtn->onClick.Connect(std::bind(&::bs::GUIDropDownMenu::DropDownSubMenu::ScrollDown, this));

				GUIElementOptions scrollDownBtnOptions = mScrollDownBtn->GetOptionFlags();
				scrollDownBtnOptions.Unset(GUIElementOption::AcceptsKeyFocus);

				mScrollDownBtn->SetOptionFlags(scrollDownBtnOptions);

				mHandle = GUITexture::Create(mOwner->mHandleStyle);
				GUITexture* background = GUITexture::Create(mOwner->mSideBackgroundStyle);
				background->SetElementDepthInternal(2);

				mSidebarPanel->AddElement(background);
				mSidebarPanel->AddElement(mScrollUpBtn);
				mSidebarPanel->AddElement(mScrollDownBtn);
				mSidebarPanel->AddElement(mHandle);
			}

			mScrollUpBtn->SetPosition(1, 1);
			mScrollDownBtn->SetPosition(1, sidebarHeight - 1 - scrollDownStyle->height);

			UINT32 maxHandleSize = std::max(0, (INT32)sidebarHeight - (INT32)scrollDownStyle->height - (INT32)scrollUpStyle->height - 2);
			UINT32 handleSize = maxHandleSize / pageCount;

			INT32 handlePos = 1 + scrollUpStyle->height + mPage * handleSize;

			mHandle->SetPosition(1, handlePos);
			mHandle->SetHeight(handleSize);

			mSidebarPanel->SetPosition(x, actualY);
			mSidebarPanel->SetWidth(sideBarStyle->width);
			mSidebarPanel->SetHeight(sidebarHeight);
		}
		else
		{
			if (mSidebarPanel != nullptr)
			{
				GUIPanel::Destroy(mSidebarPanel);
				mSidebarPanel = nullptr;
			}
		}

		mContent->SetRange(pageStart, pageEnd);

		if (mSubMenu == nullptr)
			mContent->SetKeyboardFocus(true);

		// Resize and reposition areas
		mBackgroundPanel->SetWidth(width - contentOffset);
		mBackgroundPanel->SetHeight(pageHeight);
		mBackgroundPanel->SetPosition(x + contentOffset, actualY);

		mVisibleBounds = Rect2I(x, actualY, width, pageHeight);

		UINT32 contentWidth = (UINT32)std::max(0, (INT32)width - (INT32)backgroundStyle->margins.left - (INT32)backgroundStyle->margins.right - (INT32)contentOffset);
		UINT32 contentHeight = (UINT32)std::max(0, (INT32)pageHeight - (INT32)backgroundStyle->margins.top - (INT32)backgroundStyle->margins.bottom);

		mContentPanel->SetWidth(contentWidth);
		mContentPanel->SetHeight(contentHeight);
		mContentPanel->SetPosition(x + contentOffset + backgroundStyle->margins.left, actualY + backgroundStyle->margins.top);
	}

	void GUIDropDownMenu::DropDownSubMenu::ScrollDown()
	{
		mPage++;
		if (mPage == (UINT32)GetPageInfos().size())
			mPage = 0;

		UpdateGuiElements();

		CloseSubMenu();
	}

	void GUIDropDownMenu::DropDownSubMenu::ScrollUp()
	{
		if (mPage > 0)
			mPage--;
		else
			mPage = (UINT32)GetPageInfos().size() - 1;

		UpdateGuiElements();
		CloseSubMenu();
	}

	void GUIDropDownMenu::DropDownSubMenu::ScrollToTop()
	{
		mPage = 0;
		UpdateGuiElements();

		CloseSubMenu();
	}

	void GUIDropDownMenu::DropDownSubMenu::ScrollToBottom()
	{
		mPage = (UINT32)(GetPageInfos().size() - 1);
		UpdateGuiElements();

		CloseSubMenu();
	}

	void GUIDropDownMenu::DropDownSubMenu::CloseSubMenu()
	{
		if(mSubMenu != nullptr)
		{
			bs_delete(mSubMenu);
			mSubMenu = nullptr;

			mContent->SetKeyboardFocus(true);
		}
	}

	void GUIDropDownMenu::DropDownSubMenu::ElementActivated(UINT32 idx, const Rect2I& bounds)
	{
		CloseSubMenu();

		if (!mData.entries[idx].IsSubMenu())
		{
			auto callback = mData.entries[idx].GetCallback();
			if (callback != nullptr)
				callback();

			if (mType != GUIDropDownType::MultiListBox)
				GUIDropDownBoxManager::Instance().CloseDropDownBox();
		}
		else
		{
			mContent->SetKeyboardFocus(false);

			mSubMenu = bs_new<DropDownSubMenu>(mOwner, this, DropDownAreaPlacement::AroundBoundsVert(bounds),
				mAvailableBounds, mData.entries[idx].GetSubMenuData(), mType, mDepthOffset + 1);
		}
	}

	void GUIDropDownMenu::DropDownSubMenu::Close()
	{
		if (mParent != nullptr)
			mParent->CloseSubMenu();
		else // We're the last sub-menu, close the whole thing
			GUIDropDownBoxManager::Instance().CloseDropDownBox();
	}

	void GUIDropDownMenu::DropDownSubMenu::ElementSelected(UINT32 idx)
	{
		CloseSubMenu();
	}
}
