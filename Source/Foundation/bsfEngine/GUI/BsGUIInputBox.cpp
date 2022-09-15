//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsGUIInputBox.h"
#include "GUI/BsGUIManager.h"
#include "2D/BsImageSprite.h"
#include "GUI/BsGUISkin.h"
#include "Image/BsSpriteTexture.h"
#include "2D/BsTextSprite.h"
#include "GUI/BsGUIDimensions.h"
#include "GUI/BsGUITextInputEvent.h"
#include "GUI/BsGUIMouseEvent.h"
#include "GUI/BsGUICommandEvent.h"
#include "GUI/BsGUIInputCaret.h"
#include "GUI/BsGUIInputSelection.h"
#include "GUI/BsGUIContextMenu.h"
#include "GUI/BsGUIHelper.h"
#include "Utility/BsTime.h"
#include "Platform/BsPlatform.h"
#include "String/BsUnicode.h"

namespace bs
{
	VirtualButton GUIInputBox::mCopyVB = VirtualButton("Copy");
	VirtualButton GUIInputBox::mPasteVB = VirtualButton("Paste");
	VirtualButton GUIInputBox::mCutVB = VirtualButton("Cut");
	VirtualButton GUIInputBox::mSelectAllVB = VirtualButton("SelectAll");

	const String& GUIInputBox::GetGuiTypeName()
	{
		static String name = "InputBox";
		return name;
	}

	GUIInputBox::GUIInputBox(const String& styleName, const GUIDimensions& dimensions, bool multiline)
		: GUIElement(styleName, dimensions, GUIElementOption::AcceptsKeyFocus), mIsMultiline(multiline)
	{
		mImageSprite = bs_new<ImageSprite>();
		mTextSprite = bs_new<TextSprite>();
	}

	GUIInputBox::~GUIInputBox()
	{
		bs_delete(mTextSprite);
		bs_delete(mImageSprite);
	}

	GUIInputBox* GUIInputBox::Create(bool multiline, const String& styleName)
	{
		return new (bs_alloc<GUIInputBox>()) GUIInputBox(GetStyleName<GUIInputBox>(styleName), GUIDimensions::Create(), multiline);
	}

	GUIInputBox* GUIInputBox::Create(bool multiline, const GUIOptions& options, const String& styleName)
	{
		return new (bs_alloc<GUIInputBox>()) GUIInputBox(GetStyleName<GUIInputBox>(styleName), GUIDimensions::Create(options), multiline);
	}

	GUIInputBox* GUIInputBox::Create(const GUIOptions& options, const String& styleName)
	{
		return new (bs_alloc<GUIInputBox>()) GUIInputBox(GetStyleName<GUIInputBox>(styleName), GUIDimensions::Create(options), false);
	}

	void GUIInputBox::SetText(const String& text)
	{
		if (mText == text)
			return;

		bool filterOkay = true;
		if(mFilter != nullptr)
		{
			filterOkay = mFilter(text);
		}

		if(filterOkay)
		{
			Vector2I origSize = mDimensions.CalculateSizeRange(GetOptimalSizeInternal()).optimal;

			mText = text;
			mNumChars = UTF8::Count(mText);

			if (mHasFocus)
			{
				TEXT_SPRITE_DESC textDesc = GetTextDesc();

				gGUIManager().GetInputCaretTool()->UpdateText(this, textDesc);
				gGUIManager().GetInputSelectionTool()->UpdateText(this, textDesc);

				if (mNumChars > 0)
					gGUIManager().GetInputCaretTool()->MoveCaretToChar(mNumChars - 1, CARET_AFTER);
				else
					gGUIManager().GetInputCaretTool()->MoveCaretToChar(0, CARET_BEFORE);

				if (mSelectionShown)
					gGUIManager().GetInputSelectionTool()->SelectAll();

				ScrollTextToCaret();
			}

			Vector2I newSize = mDimensions.CalculateSizeRange(GetOptimalSizeInternal()).optimal;
			if (origSize != newSize)
				MarkLayoutAsDirtyInternal();
			else
				MarkContentAsDirtyInternal();
		}
	}

	void GUIInputBox::UpdateRenderElementsInternal()
	{		
		mImageDesc.width = mLayoutData.area.width;
		mImageDesc.height = mLayoutData.area.height;
		mImageDesc.borderLeft = GetStyleInternal()->border.left;
		mImageDesc.borderRight = GetStyleInternal()->border.right;
		mImageDesc.borderTop = GetStyleInternal()->border.top;
		mImageDesc.borderBottom = GetStyleInternal()->border.bottom;
		mImageDesc.color = GetTint();

		const HSpriteTexture& activeTex = GetActiveTexture();
		if(SpriteTexture::CheckIsLoaded(activeTex))
			mImageDesc.texture = activeTex;

		mImageSprite->Update(mImageDesc, (UINT64)GetParentWidgetInternal());

		TEXT_SPRITE_DESC textDesc = GetTextDesc();
		mTextSprite->Update(textDesc, (UINT64)GetParentWidgetInternal());

		ImageSprite* caretSprite = nullptr;
		if(mCaretShown && gGUIManager().GetCaretBlinkState())
		{
			gGUIManager().GetInputCaretTool()->UpdateText(this, textDesc); // TODO - These shouldn't be here. Only call this when one of these parameters changes.
			gGUIManager().GetInputCaretTool()->UpdateSprite();

			caretSprite = gGUIManager().GetInputCaretTool()->GetSprite();
		}

		if(mSelectionShown)
		{
			gGUIManager().GetInputSelectionTool()->UpdateText(this, textDesc); // TODO - These shouldn't be here. Only call this when one of these parameters changes.
			gGUIManager().GetInputSelectionTool()->UpdateSprite();
		}

		// When text bounds are reduced the scroll needs to be adjusted so that
		// input box isn't filled with mostly empty space.
		Vector2I offset(mLayoutData.area.x, mLayoutData.area.y);
		ClampScrollToBounds(mTextSprite->GetBounds(offset, Rect2I()));

		// Populate GUI render elements from the sprites
		{
			using T = impl::GUIRenderElementHelper;
			T::Populate({ T::SpriteInfo(mTextSprite, 1), T::SpriteInfo(mImageSprite, 3), T::SpriteInfo(caretSprite) }, mRenderElements);

			if (mSelectionShown)
			{
				const Vector<ImageSprite*>& sprites = gGUIManager().GetInputSelectionTool()->GetSprites();
				for (auto& entry : sprites)
				{
					for (UINT32 i = 0; i < entry->GetNumRenderElements(); i++)
					{
						mRenderElements.Add(GUIRenderElement());
						GUIRenderElement& renderElement = mRenderElements.Back();

						entry->GetRenderElementInfo(i, renderElement);

						renderElement.depth = 2;
						renderElement.type = GUIMeshType::Triangle;
					}
				}
			}
		}

		GUIElement::UpdateRenderElementsInternal();
	}

	void GUIInputBox::UpdateClippedBounds()
	{
		Vector2I offset(mLayoutData.area.x, mLayoutData.area.y);
		mClippedBounds = mImageSprite->GetBounds(offset, mLayoutData.GetLocalClipRect());
	}

	Sprite* GUIInputBox::RenderElemToSprite(UINT32 renderElemIdx, UINT32& localRenderElemIdx) const
	{
		UINT32 oldNumElements = 0;
		UINT32 newNumElements = oldNumElements + mTextSprite->GetNumRenderElements();
		if(renderElemIdx < newNumElements)
		{
			localRenderElemIdx = renderElemIdx - oldNumElements;
			return mTextSprite;
		}

		oldNumElements = newNumElements;
		newNumElements += mImageSprite->GetNumRenderElements();

		if(renderElemIdx < newNumElements)
		{
			localRenderElemIdx = renderElemIdx - oldNumElements;
			return mImageSprite;
		}

		if(mCaretShown && gGUIManager().GetCaretBlinkState())
		{
			oldNumElements = newNumElements;
			newNumElements += gGUIManager().GetInputCaretTool()->GetSprite()->GetNumRenderElements();

			if(renderElemIdx < newNumElements)
			{
				localRenderElemIdx = renderElemIdx - oldNumElements;
				return gGUIManager().GetInputCaretTool()->GetSprite();
			}
		}

		if(mSelectionShown)
		{
			const Vector<ImageSprite*>& sprites = gGUIManager().GetInputSelectionTool()->GetSprites();
			for(auto& selectionSprite : sprites)
			{
				oldNumElements = newNumElements;
				newNumElements += selectionSprite->GetNumRenderElements();

				if(renderElemIdx < newNumElements)
				{
					localRenderElemIdx = renderElemIdx - oldNumElements;
					return selectionSprite;
				}
			}
		}

		localRenderElemIdx = renderElemIdx;
		return nullptr;
	}

	Vector2I GUIInputBox::RenderElemToOffset(UINT32 renderElemIdx) const
	{
		UINT32 oldNumElements = 0;
		UINT32 newNumElements = oldNumElements + mTextSprite->GetNumRenderElements();
		if(renderElemIdx < newNumElements)
			return GetTextOffset();

		oldNumElements = newNumElements;
		newNumElements += mImageSprite->GetNumRenderElements();

		if(renderElemIdx < newNumElements)
			return Vector2I(mLayoutData.area.x, mLayoutData.area.y);;

		if(mCaretShown && gGUIManager().GetCaretBlinkState())
		{
			oldNumElements = newNumElements;
			newNumElements += gGUIManager().GetInputCaretTool()->GetSprite()->GetNumRenderElements();

			if(renderElemIdx < newNumElements)
				return gGUIManager().GetInputCaretTool()->GetSpriteOffset();
		}

		if(mSelectionShown)
		{
			UINT32 spriteIdx = 0;
			const Vector<ImageSprite*>& sprites = gGUIManager().GetInputSelectionTool()->GetSprites();
			for(auto& selectionSprite : sprites)
			{
				oldNumElements = newNumElements;
				newNumElements += selectionSprite->GetNumRenderElements();

				if(renderElemIdx < newNumElements)
					return gGUIManager().GetInputSelectionTool()->GetSelectionSpriteOffset(spriteIdx);

				spriteIdx++;
			}
		}

		return Vector2I();
	}

	Rect2I GUIInputBox::RenderElemToClipRect(UINT32 renderElemIdx) const
	{
		UINT32 oldNumElements = 0;
		UINT32 newNumElements = oldNumElements + mTextSprite->GetNumRenderElements();
		if(renderElemIdx < newNumElements)
			return GetTextClipRect();

		oldNumElements = newNumElements;
		newNumElements += mImageSprite->GetNumRenderElements();

		if(renderElemIdx < newNumElements)
			return mLayoutData.GetLocalClipRect();

		if(mCaretShown && gGUIManager().GetCaretBlinkState())
		{
			oldNumElements = newNumElements;
			newNumElements += gGUIManager().GetInputCaretTool()->GetSprite()->GetNumRenderElements();

			if(renderElemIdx < newNumElements)
			{
				return gGUIManager().GetInputCaretTool()->GetSpriteClipRect(GetTextClipRect());
			}
		}

		if(mSelectionShown)
		{
			UINT32 spriteIdx = 0;
			const Vector<ImageSprite*>& sprites = gGUIManager().GetInputSelectionTool()->GetSprites();
			for(auto& selectionSprite : sprites)
			{
				oldNumElements = newNumElements;
				newNumElements += selectionSprite->GetNumRenderElements();

				if(renderElemIdx < newNumElements)
					return gGUIManager().GetInputSelectionTool()->GetSelectionSpriteClipRect(spriteIdx, GetTextClipRect());

				spriteIdx++;
			}
		}

		return Rect2I();
	}

	Vector2I GUIInputBox::GetOptimalSizeInternal() const
	{
		UINT32 imageWidth = 0;
		UINT32 imageHeight = 0;

		const HSpriteTexture& activeTex = GetActiveTexture();
		if(SpriteTexture::CheckIsLoaded(activeTex))
		{
			imageWidth = activeTex->GetWidth();
			imageHeight = activeTex->GetHeight();
		}

		Vector2I contentSize = GUIHelper::CalcOptimalContentsSize(mText, *GetStyleInternal(), GetDimensionsInternal());
		UINT32 contentWidth = std::max(imageWidth, (UINT32)contentSize.x);
		UINT32 contentHeight = std::max(imageHeight, (UINT32)contentSize.y);

		return Vector2I(contentWidth, contentHeight);
	}

	Vector2I GUIInputBox::GetTextInputOffsetInternal() const
	{
		return mTextOffset;	
	}

	Rect2I GUIInputBox::GetTextInputRectInternal() const
	{
		Rect2I textBounds = GetCachedContentBounds();
		textBounds.x -= mLayoutData.area.x;
		textBounds.y -= mLayoutData.area.y;

		return textBounds;
	}

	UINT32 GUIInputBox::GetRenderElementDepthRangeInternal() const
	{
		return 4;
	}

	bool GUIInputBox::HasCustomCursorInternal(const Vector2I position, CursorType& type) const
	{
		if(IsInBoundsInternal(position) && !IsDisabledInternal())
		{
			type = CursorType::IBeam;
			return true;
		}

		return false;
	}

	void GUIInputBox::FillBuffer(
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

		UINT32 localRenderElementIdx;
		Sprite* sprite = RenderElemToSprite(renderElementIdx, localRenderElementIdx);
		Vector2I layoutOffset = RenderElemToOffset(renderElementIdx) + offset;
		Rect2I clipRect = RenderElemToClipRect(renderElementIdx);

		sprite->FillBuffer(vertices, uvs, indices, vertexOffset, indexOffset, maxNumVerts, maxNumIndices, vertexStride,
			indexStride, localRenderElementIdx, layoutOffset, clipRect);
	}

	bool GUIInputBox::MouseEventInternal(const GUIMouseEvent& ev)
	{
		if(ev.GetType() == GUIMouseEventType::MouseOver)
		{
			if (!IsDisabledInternal())
			{
				if (!mHasFocus)
				{
					Vector2I origSize = mDimensions.CalculateSizeRange(GetOptimalSizeInternal()).optimal;
					mState = State::Hover;
					Vector2I newSize = mDimensions.CalculateSizeRange(GetOptimalSizeInternal()).optimal;

					if (origSize != newSize)
						MarkLayoutAsDirtyInternal();
					else
						MarkContentAsDirtyInternal();
				}

				mIsMouseOver = true;
			}

			return true;
		}
		else if(ev.GetType() == GUIMouseEventType::MouseOut)
		{
			if (!IsDisabledInternal())
			{
				if (!mHasFocus)
				{
					Vector2I origSize = mDimensions.CalculateSizeRange(GetOptimalSizeInternal()).optimal;
					mState = State::Normal;
					Vector2I newSize = mDimensions.CalculateSizeRange(GetOptimalSizeInternal()).optimal;

					if (origSize != newSize)
						MarkLayoutAsDirtyInternal();
					else
						MarkContentAsDirtyInternal();
				}

				mIsMouseOver = false;
			}

			return true;
		}
		else if(ev.GetType() == GUIMouseEventType::MouseDoubleClick && ev.GetButton() == GUIMouseButton::Left)
		{
			if (!IsDisabledInternal())
			{
				ShowSelection(0);
				gGUIManager().GetInputSelectionTool()->SelectAll();

				MarkContentAsDirtyInternal();
			}

			return true;
		}
		else if(ev.GetType() == GUIMouseEventType::MouseDown && ev.GetButton() == GUIMouseButton::Left)
		{
			if (!IsDisabledInternal())
			{
				if (ev.IsShiftDown())
				{
					if (!mSelectionShown)
						ShowSelection(gGUIManager().GetInputCaretTool()->GetCaretPos());
				}
				else
				{
					bool focusGainedThisFrame = mHasFocus && mFocusGainedFrame == gTime().GetFrameIdx();

					// We want to select all on focus gain, so don't override that
					if(!focusGainedThisFrame)
						ClearSelection();

					ShowCaret();
				}

				if (mNumChars > 0)
					gGUIManager().GetInputCaretTool()->MoveCaretToPos(ev.GetPosition());
				else
					gGUIManager().GetInputCaretTool()->MoveCaretToStart();

				if (ev.IsShiftDown())
					gGUIManager().GetInputSelectionTool()->MoveSelectionToCaret(gGUIManager().GetInputCaretTool()->GetCaretPos());

				ScrollTextToCaret();
				MarkContentAsDirtyInternal();
			}

			return true;
		}
		else if(ev.GetType() == GUIMouseEventType::MouseDragStart)
		{
			if (!IsDisabledInternal())
			{
				if (!ev.IsShiftDown())
				{
					mDragInProgress = true;

					UINT32 caretPos = gGUIManager().GetInputCaretTool()->GetCaretPos();
					ShowSelection(caretPos);
					gGUIManager().GetInputSelectionTool()->SelectionDragStart(caretPos);
					MarkContentAsDirtyInternal();

					return true;
				}
			}
		}
		else if(ev.GetType() == GUIMouseEventType::MouseDragEnd)
		{
			if (!IsDisabledInternal())
			{
				if (!ev.IsShiftDown())
				{
					mDragInProgress = false;

					gGUIManager().GetInputSelectionTool()->SelectionDragEnd();
					MarkContentAsDirtyInternal();
					return true;
				}
			}
		}
		else if(ev.GetType() == GUIMouseEventType::MouseDrag)
		{
			if (!IsDisabledInternal())
			{
				if (!ev.IsShiftDown())
				{
					if (mNumChars > 0)
						gGUIManager().GetInputCaretTool()->MoveCaretToPos(ev.GetPosition());
					else
						gGUIManager().GetInputCaretTool()->MoveCaretToStart();

					gGUIManager().GetInputSelectionTool()->SelectionDragUpdate(gGUIManager().GetInputCaretTool()->GetCaretPos());

					ScrollTextToCaret();
					MarkContentAsDirtyInternal();
					return true;
				}
			}
		}

		return false;
	}

	bool GUIInputBox::TextInputEventInternal(const GUITextInputEvent& ev)
	{
		if (IsDisabledInternal())
			return false;

		Vector2I origSize = mDimensions.CalculateSizeRange(GetOptimalSizeInternal()).optimal;

		if(mSelectionShown)
			DeleteSelectedText(true);

		UINT32 charIdx = gGUIManager().GetInputCaretTool()->GetCharIdxAtCaretPos();

		bool filterOkay = true;
		if(mFilter != nullptr)
		{
			String newText = mText;
			UINT32 byteIdx = UTF8::CharToByteIndex(mText, charIdx);
			String utf8chars = UTF8::FromUtF32(U32String(1, ev.GetInputChar()));
			newText.insert(newText.begin() + byteIdx, utf8chars.begin(), utf8chars.end());

			filterOkay = mFilter(newText);
		}

		if(filterOkay)
		{
			InsertChar(charIdx, ev.GetInputChar());

			gGUIManager().GetInputCaretTool()->MoveCaretToChar(charIdx, CARET_AFTER);
			ScrollTextToCaret();

			if(!onValueChanged.Empty())
				onValueChanged(mText);
		}

		Vector2I newSize = mDimensions.CalculateSizeRange(GetOptimalSizeInternal()).optimal;
		if (origSize != newSize)
			MarkLayoutAsDirtyInternal();
		else
			MarkContentAsDirtyInternal();

		return true;
	}

	bool GUIInputBox::CommandEventInternal(const GUICommandEvent& ev)
	{
		if (IsDisabledInternal())
			return false;

		bool baseReturn = GUIElement::CommandEventInternal(ev);

		if(ev.GetType() == GUICommandEventType::Redraw)
		{
			MarkContentAsDirtyInternal();
			return true;
		}

		if(ev.GetType() == GUICommandEventType::FocusGained)
		{
			Vector2I origSize = mDimensions.CalculateSizeRange(GetOptimalSizeInternal()).optimal;
			mState = State::Focused;

			ShowSelection(0);
			gGUIManager().GetInputSelectionTool()->SelectAll();

			mHasFocus = true;
			mFocusGainedFrame = gTime().GetFrameIdx();

			Vector2I newSize = mDimensions.CalculateSizeRange(GetOptimalSizeInternal()).optimal;
			if (origSize != newSize)
				MarkLayoutAsDirtyInternal();
			else
				MarkContentAsDirtyInternal();

			return true;
		}
		
		if(ev.GetType() == GUICommandEventType::FocusLost)
		{
			Vector2I origSize = mDimensions.CalculateSizeRange(GetOptimalSizeInternal()).optimal;
			mState = State::Normal;

			HideCaret();
			ClearSelection();

			mHasFocus = false;

			Vector2I newSize = mDimensions.CalculateSizeRange(GetOptimalSizeInternal()).optimal;
			if (origSize != newSize)
				MarkLayoutAsDirtyInternal();
			else
				MarkContentAsDirtyInternal();

			return true;
		}
		
		if(ev.GetType() == GUICommandEventType::Backspace)
		{
			if(mNumChars > 0)
			{
				Vector2I origSize = mDimensions.CalculateSizeRange(GetOptimalSizeInternal()).optimal;
				if(mSelectionShown)
				{
					DeleteSelectedText();
				}
				else
				{
					UINT32 charIdx = gGUIManager().GetInputCaretTool()->GetCharIdxAtCaretPos() - 1;

					if(charIdx < mNumChars)
					{
						bool filterOkay = true;
						if(mFilter != nullptr)
						{
							String newText = mText;
							UINT32 byteIdx = UTF8::CharToByteIndex(mText, charIdx);
							UINT32 byteCount = UTF8::CharByteCount(mText, charIdx);
							newText.erase(byteIdx, byteCount);

							filterOkay = mFilter(newText);
						}

						if(filterOkay)
						{
							EraseChar(charIdx);

							if (charIdx > 0)
							{
								charIdx--;

								gGUIManager().GetInputCaretTool()->MoveCaretToChar(charIdx, CARET_AFTER);
							}
							else
								gGUIManager().GetInputCaretTool()->MoveCaretToChar(charIdx, CARET_BEFORE);

							ScrollTextToCaret();

							if(!onValueChanged.Empty())
								onValueChanged(mText);
						}
					}
				}

				Vector2I newSize = mDimensions.CalculateSizeRange(GetOptimalSizeInternal()).optimal;
				if (origSize != newSize)
					MarkLayoutAsDirtyInternal();
				else
					MarkContentAsDirtyInternal();
			}

			return true;
		}

		if(ev.GetType() == GUICommandEventType::Delete)
		{
			if(mNumChars > 0)
			{
				Vector2I origSize = mDimensions.CalculateSizeRange(GetOptimalSizeInternal()).optimal;
				if(mSelectionShown)
				{
					DeleteSelectedText();
				}
				else
				{
					UINT32 charIdx = gGUIManager().GetInputCaretTool()->GetCharIdxAtCaretPos();
					if(charIdx < mNumChars)
					{
						bool filterOkay = true;
						if(mFilter != nullptr)
						{
							String newText = mText;
							UINT32 byteIdx = UTF8::CharToByteIndex(mText, charIdx);
							UINT32 byteCount = UTF8::CharByteCount(mText, charIdx);
							newText.erase(byteIdx, byteCount);

							filterOkay = mFilter(newText);
						}

						if(filterOkay)
						{
							EraseChar(charIdx);

							if(charIdx > 0)
								charIdx--;

							gGUIManager().GetInputCaretTool()->MoveCaretToChar(charIdx, CARET_AFTER);

							ScrollTextToCaret();

							if(!onValueChanged.Empty())
								onValueChanged(mText);
						}
					}
				}

				Vector2I newSize = mDimensions.CalculateSizeRange(GetOptimalSizeInternal()).optimal;
				if (origSize != newSize)
					MarkLayoutAsDirtyInternal();
				else
					MarkContentAsDirtyInternal();
			}

			return true;
		}

		if(ev.GetType() == GUICommandEventType::MoveLeft)
		{
			if(mSelectionShown)
			{
				UINT32 selStart = gGUIManager().GetInputSelectionTool()->GetSelectionStart();
				ClearSelection();

				if (!mCaretShown)
					ShowCaret();

				if(selStart > 0)
					gGUIManager().GetInputCaretTool()->MoveCaretToChar(selStart - 1, CARET_AFTER);
				else
					gGUIManager().GetInputCaretTool()->MoveCaretToChar(0, CARET_BEFORE);
			}
			else
				gGUIManager().GetInputCaretTool()->MoveCaretLeft();

			ScrollTextToCaret();
			MarkContentAsDirtyInternal();
			return true;
		}

		if(ev.GetType() == GUICommandEventType::SelectLeft)
		{
			if(!mSelectionShown)
				ShowSelection(gGUIManager().GetInputCaretTool()->GetCaretPos());

			gGUIManager().GetInputCaretTool()->MoveCaretLeft();
			gGUIManager().GetInputSelectionTool()->MoveSelectionToCaret(gGUIManager().GetInputCaretTool()->GetCaretPos());

			ScrollTextToCaret();
			MarkContentAsDirtyInternal();
			return true;
		}

		if(ev.GetType() == GUICommandEventType::MoveRight)
		{
			if(mSelectionShown)
			{
				UINT32 selEnd = gGUIManager().GetInputSelectionTool()->GetSelectionEnd();
				ClearSelection();

				if (!mCaretShown)
					ShowCaret();

				if(selEnd > 0)
					gGUIManager().GetInputCaretTool()->MoveCaretToChar(selEnd - 1, CARET_AFTER);
				else
					gGUIManager().GetInputCaretTool()->MoveCaretToChar(0, CARET_BEFORE);
			}
			else
				gGUIManager().GetInputCaretTool()->MoveCaretRight();

			ScrollTextToCaret();
			MarkContentAsDirtyInternal();
			return true;
		}

		if(ev.GetType() == GUICommandEventType::SelectRight)
		{
			if(!mSelectionShown)
				ShowSelection(gGUIManager().GetInputCaretTool()->GetCaretPos());

			gGUIManager().GetInputCaretTool()->MoveCaretRight();
			gGUIManager().GetInputSelectionTool()->MoveSelectionToCaret(gGUIManager().GetInputCaretTool()->GetCaretPos());

			ScrollTextToCaret();
			MarkContentAsDirtyInternal();
			return true;
		}

		if(ev.GetType() == GUICommandEventType::MoveUp)
		{
			if (mSelectionShown)
				ClearSelection();

			if (!mCaretShown)
				ShowCaret();

			gGUIManager().GetInputCaretTool()->MoveCaretUp();

			ScrollTextToCaret();
			MarkContentAsDirtyInternal();
			return true;
		}

		if(ev.GetType() == GUICommandEventType::SelectUp)
		{
			if(!mSelectionShown)
				ShowSelection(gGUIManager().GetInputCaretTool()->GetCaretPos());;

			gGUIManager().GetInputCaretTool()->MoveCaretUp();
			gGUIManager().GetInputSelectionTool()->MoveSelectionToCaret(gGUIManager().GetInputCaretTool()->GetCaretPos());

			ScrollTextToCaret();
			MarkContentAsDirtyInternal();
			return true;
		}

		if(ev.GetType() == GUICommandEventType::MoveDown)
		{
			if (mSelectionShown)
				ClearSelection();

			if (!mCaretShown)
				ShowCaret();

			gGUIManager().GetInputCaretTool()->MoveCaretDown();

			ScrollTextToCaret();
			MarkContentAsDirtyInternal();
			return true;
		}

		if(ev.GetType() == GUICommandEventType::SelectDown)
		{
			if(!mSelectionShown)
				ShowSelection(gGUIManager().GetInputCaretTool()->GetCaretPos());

			gGUIManager().GetInputCaretTool()->MoveCaretDown();
			gGUIManager().GetInputSelectionTool()->MoveSelectionToCaret(gGUIManager().GetInputCaretTool()->GetCaretPos());

			ScrollTextToCaret();
			MarkContentAsDirtyInternal();
			return true;
		}

		if(ev.GetType() == GUICommandEventType::Return)
		{
			if (mIsMultiline)
			{
				Vector2I origSize = mDimensions.CalculateSizeRange(GetOptimalSizeInternal()).optimal;

				if (mSelectionShown)
					DeleteSelectedText();

				UINT32 charIdx = gGUIManager().GetInputCaretTool()->GetCharIdxAtCaretPos();

				bool filterOkay = true;
				if (mFilter != nullptr)
				{
					String newText = mText;
					UINT32 byteIdx = UTF8::CharToByteIndex(mText, charIdx);
					newText.insert(newText.begin() + byteIdx, '\n');

					filterOkay = mFilter(newText);
				}

				if (filterOkay)
				{
					InsertChar(charIdx, '\n');

					gGUIManager().GetInputCaretTool()->MoveCaretRight();
					ScrollTextToCaret();

					if (!onValueChanged.Empty())
						onValueChanged(mText);
				}

				Vector2I newSize = mDimensions.CalculateSizeRange(GetOptimalSizeInternal()).optimal;
				if (origSize != newSize)
					MarkLayoutAsDirtyInternal();
				else
					MarkContentAsDirtyInternal();

				return true;
			}
		}

		if (ev.GetType() == GUICommandEventType::Confirm)
		{
			onConfirm();
			return true;
		}

		return baseReturn;
	}

	bool GUIInputBox::VirtualButtonEventInternal(const GUIVirtualButtonEvent& ev)
	{
		if (IsDisabledInternal())
			return false;

		if(ev.GetButton() == mCutVB)
		{
			CutText();

			return true;
		}
		else if(ev.GetButton() == mCopyVB)
		{
			CopyText();

			return true;
		}
		else if(ev.GetButton() == mPasteVB)
		{
			PasteText();
			return true;
		}
		else if(ev.GetButton() == mSelectAllVB)
		{
			ShowSelection(0);
			gGUIManager().GetInputSelectionTool()->SelectAll();
			MarkContentAsDirtyInternal();

			return true;
		}

		return false;
	}

	void GUIInputBox::ShowCaret()
	{
		mCaretShown = true;

		TEXT_SPRITE_DESC textDesc = GetTextDesc();
		gGUIManager().GetInputCaretTool()->UpdateText(this, textDesc);
	}

	void GUIInputBox::HideCaret()
	{
		mCaretShown = false;
	}

	void GUIInputBox::ShowSelection(UINT32 anchorCaretPos)
	{
		TEXT_SPRITE_DESC textDesc = GetTextDesc();
		gGUIManager().GetInputSelectionTool()->UpdateText(this, textDesc);

		gGUIManager().GetInputSelectionTool()->ShowSelection(anchorCaretPos);
		mSelectionShown = true;
	}

	void GUIInputBox::ClearSelection()
	{
		gGUIManager().GetInputSelectionTool()->ClearSelectionVisuals();
		mSelectionShown = false;
	}

	void GUIInputBox::ScrollTextToCaret()
	{
		TEXT_SPRITE_DESC textDesc = GetTextDesc();

		Vector2I textOffset = GetTextOffset();
		Vector2I caretPos = gGUIManager().GetInputCaretTool()->GetCaretPosition(textOffset);
		UINT32 caretHeight = gGUIManager().GetInputCaretTool()->GetCaretHeight();
		UINT32 caretWidth = 1;

		INT32 left = textOffset.x - mTextOffset.x;
		// Include caret width here because we don't want to scroll if just the caret is outside the bounds
		// (Possible if the text width is exactly the maximum width)
		INT32 right = left + (INT32)textDesc.width + caretWidth;
		INT32 top = textOffset.y - mTextOffset.y;
		INT32 bottom = top + (INT32)textDesc.height;

		// If caret is too high to display we don't want the offset to keep adjusting itself
		caretHeight = std::min(caretHeight, (UINT32)(bottom - top));
		INT32 caretRight = caretPos.x + (INT32)caretWidth;
		INT32 caretBottom = caretPos.y + (INT32)caretHeight;

		Vector2I offset;
		if(caretPos.x < left)
		{
			offset.x = left - caretPos.x;
		}
		else if(caretRight > right)
		{
			offset.x = -(caretRight - right);
		}

		if(caretPos.y < top)
		{
			offset.y = top - caretPos.y;
		}
		else if(caretBottom > bottom)
		{
			offset.y = -(caretBottom - bottom);
		}

		mTextOffset += offset;

		gGUIManager().GetInputCaretTool()->UpdateText(this, textDesc);
		gGUIManager().GetInputSelectionTool()->UpdateText(this, textDesc);
	}

	void GUIInputBox::ClampScrollToBounds(Rect2I unclippedTextBounds)
	{
		TEXT_SPRITE_DESC textDesc = GetTextDesc();

		Vector2I newTextOffset;
		INT32 maxScrollableWidth = std::max(0, (INT32)unclippedTextBounds.width - (INT32)textDesc.width);
		INT32 maxScrollableHeight = std::max(0, (INT32)unclippedTextBounds.height - (INT32)textDesc.height);
		newTextOffset.x = Math::Clamp(mTextOffset.x, -maxScrollableWidth, 0);
		newTextOffset.y = Math::Clamp(mTextOffset.y, -maxScrollableHeight, 0);

		if(newTextOffset != mTextOffset)
		{
			mTextOffset = newTextOffset;

			gGUIManager().GetInputCaretTool()->UpdateText(this, textDesc);
			gGUIManager().GetInputSelectionTool()->UpdateText(this, textDesc);
		}
	}

	void GUIInputBox::InsertString(UINT32 charIdx, const String& string)
	{
		UINT32 byteIdx = UTF8::CharToByteIndex(mText, charIdx);

		mText.insert(mText.begin() + byteIdx, string.begin(), string.end());
		mNumChars = UTF8::Count(mText);

		TEXT_SPRITE_DESC textDesc = GetTextDesc();

		gGUIManager().GetInputCaretTool()->UpdateText(this, textDesc);
		gGUIManager().GetInputSelectionTool()->UpdateText(this, textDesc);
	}

	void GUIInputBox::InsertChar(UINT32 charIdx, UINT32 charCode)
	{
		UINT32 byteIdx = UTF8::CharToByteIndex(mText, charIdx);
		String utf8chars = UTF8::FromUtF32(U32String(1, (char32_t)charCode));

		mText.insert(mText.begin() + byteIdx, utf8chars.begin(), utf8chars.end());
		mNumChars = UTF8::Count(mText);

		TEXT_SPRITE_DESC textDesc = GetTextDesc();

		gGUIManager().GetInputCaretTool()->UpdateText(this, textDesc);
		gGUIManager().GetInputSelectionTool()->UpdateText(this, textDesc);
	}

	void GUIInputBox::EraseChar(UINT32 charIdx)
	{
		UINT32 byteIdx = UTF8::CharToByteIndex(mText, charIdx);
		UINT32 byteCount = UTF8::CharByteCount(mText, charIdx);

		mText.erase(byteIdx, byteCount);
		mNumChars = UTF8::Count(mText);

		TEXT_SPRITE_DESC textDesc = GetTextDesc();

		gGUIManager().GetInputCaretTool()->UpdateText(this, textDesc);
		gGUIManager().GetInputSelectionTool()->UpdateText(this, textDesc);
	}

	void GUIInputBox::DeleteSelectedText(bool internal)
	{
		UINT32 selStart = gGUIManager().GetInputSelectionTool()->GetSelectionStart();
		UINT32 selEnd = gGUIManager().GetInputSelectionTool()->GetSelectionEnd();

		UINT32 byteStart = UTF8::CharToByteIndex(mText, selStart);
		UINT32 byteEnd = UTF8::CharToByteIndex(mText, selEnd);

		bool filterOkay = true;
		if (!internal && mFilter != nullptr)
		{
			String newText = mText;
			newText.erase(newText.begin() + byteStart, newText.begin() + byteEnd);

			filterOkay = mFilter(newText);
		}

		if (!mCaretShown)
			ShowCaret();

		if(filterOkay)
		{
			mText.erase(mText.begin() + byteStart, mText.begin() + byteEnd);
			mNumChars = UTF8::Count(mText);

			TEXT_SPRITE_DESC textDesc = GetTextDesc();
			gGUIManager().GetInputCaretTool()->UpdateText(this, textDesc);
			gGUIManager().GetInputSelectionTool()->UpdateText(this, textDesc);

			if(selStart > 0)
			{
				UINT32 newCaretPos = selStart - 1;
				gGUIManager().GetInputCaretTool()->MoveCaretToChar(newCaretPos, CARET_AFTER);
			}
			else
			{
				gGUIManager().GetInputCaretTool()->MoveCaretToChar(0, CARET_BEFORE);
			}

			ScrollTextToCaret();

			if (!internal)
				onValueChanged(mText);
		}

		ClearSelection();
	}

	String GUIInputBox::GetSelectedText()
	{
		UINT32 selStart = gGUIManager().GetInputSelectionTool()->GetSelectionStart();
		UINT32 selEnd = gGUIManager().GetInputSelectionTool()->GetSelectionEnd();

		UINT32 byteStart = UTF8::CharToByteIndex(mText, selStart);
		UINT32 byteEnd = UTF8::CharToByteIndex(mText, selEnd);

		return mText.substr(byteStart, byteEnd - byteStart);
	}

	Vector2I GUIInputBox::GetTextOffset() const
	{
		Rect2I textBounds = GetCachedContentBounds();
		return Vector2I(textBounds.x, textBounds.y) + mTextOffset;
	}

	Rect2I GUIInputBox::GetTextClipRect() const
	{
		Rect2I contentClipRect = GetCachedContentClipRect();
		return Rect2I(contentClipRect.x - mTextOffset.x, contentClipRect.y - mTextOffset.y, contentClipRect.width, contentClipRect.height);
	}

	TEXT_SPRITE_DESC GUIInputBox::GetTextDesc() const
	{
		TEXT_SPRITE_DESC textDesc;
		textDesc.text = mText;
		textDesc.font = GetStyleInternal()->font;
		textDesc.fontSize = GetStyleInternal()->fontSize;
		textDesc.color = GetTint() * GetActiveTextColor();

		Rect2I textBounds = GetCachedContentBounds();
		textDesc.width = textBounds.width;
		textDesc.height = textBounds.height;
		textDesc.horzAlign = GetStyleInternal()->textHorzAlign;
		textDesc.vertAlign = GetStyleInternal()->textVertAlign;
		textDesc.wordWrap = mIsMultiline;

		return textDesc;
	}

	const HSpriteTexture& GUIInputBox::GetActiveTexture() const
	{
		switch(mState)
		{
		case State::Focused:
			return GetStyleInternal()->focused.texture;
		case State::Hover:
			return GetStyleInternal()->hover.texture;
		case State::Normal:
			return GetStyleInternal()->normal.texture;
		}

		return GetStyleInternal()->normal.texture;
	}

	Color GUIInputBox::GetActiveTextColor() const
	{
		switch (mState)
		{
		case State::Focused:
			return GetStyleInternal()->focused.textColor;
		case State::Hover:
			return GetStyleInternal()->hover.textColor;
		case State::Normal:
			return GetStyleInternal()->normal.textColor;
		}

		return GetStyleInternal()->normal.textColor;
	}

	SPtr<GUIContextMenu> GUIInputBox::GetContextMenuInternal() const
	{
		static SPtr<GUIContextMenu> contextMenu;

		if (contextMenu == nullptr)
		{
			contextMenu = bs_shared_ptr_new<GUIContextMenu>();

			contextMenu->AddMenuItem("Cut", std::bind(&GUIInputBox::mText, const_cast<GUIInputBox*>(this)), 0);
			contextMenu->AddMenuItem("Copy", std::bind(&GUIInputBox::CopyText, const_cast<GUIInputBox*>(this)), 0);
			contextMenu->AddMenuItem("Paste", std::bind(&GUIInputBox::PasteText, const_cast<GUIInputBox*>(this)), 0);

			contextMenu->SetLocalizedName("Cut", HString("Cut"));
			contextMenu->SetLocalizedName("Copy", HString("Copy"));
			contextMenu->SetLocalizedName("Paste", HString("Paste"));
		}

		if (!IsDisabledInternal())
			return contextMenu;

		return nullptr;
	}

	void GUIInputBox::CutText()
	{
		Vector2I origSize = mDimensions.CalculateSizeRange(GetOptimalSizeInternal()).optimal;

		CopyText();
		DeleteSelectedText();

		Vector2I newSize = mDimensions.CalculateSizeRange(GetOptimalSizeInternal()).optimal;
		if (origSize != newSize)
			MarkLayoutAsDirtyInternal();
		else
			MarkContentAsDirtyInternal();
	}

	void GUIInputBox::CopyText()
	{
		Platform::CopyToClipboard(GetSelectedText());
	}

	void GUIInputBox::PasteText()
	{
		DeleteSelectedText(true);

		String textInClipboard = Platform::CopyFromClipboard();
		UINT32 charIdx = gGUIManager().GetInputCaretTool()->GetCharIdxAtCaretPos();

		bool filterOkay = true;
		if(mFilter != nullptr)
		{
			String newText = mText;

			UINT32 byteIdx = UTF8::CharToByteIndex(newText, charIdx);
			newText.insert(newText.begin() + byteIdx, textInClipboard.begin(), textInClipboard.end());

			filterOkay = mFilter(newText);
		}

		if(filterOkay)
		{
			Vector2I origSize = mDimensions.CalculateSizeRange(GetOptimalSizeInternal()).optimal;
			InsertString(charIdx, textInClipboard);

			UINT32 numChars = UTF8::Count(textInClipboard);
			if(numChars > 0)
				gGUIManager().GetInputCaretTool()->MoveCaretToChar(charIdx + (numChars - 1), CARET_AFTER);

			ScrollTextToCaret();

			Vector2I newSize = mDimensions.CalculateSizeRange(GetOptimalSizeInternal()).optimal;
			if (origSize != newSize)
				MarkLayoutAsDirtyInternal();
			else
				MarkContentAsDirtyInternal();

			if(!onValueChanged.Empty())
				onValueChanged(mText);
		}
	}
}
