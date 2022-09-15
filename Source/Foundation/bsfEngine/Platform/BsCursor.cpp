//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Platform/BsCursor.h"
#include "Platform/BsPlatform.h"
#include "Error/BsException.h"
#include "Resources/BsBuiltinResources.h"
#include "Debug/BsDebug.h"

namespace bs
{
	Cursor::Cursor()
	{
		for(UINT32 i = 0; i < (UINT32)CursorType::Count; i++)
			RestoreCursorIcon((CursorType)i);
	}

	void Cursor::SetScreenPosition(const Vector2I& screenPos)
	{
		Platform::SetCursorPosition(screenPos);
	}

	Vector2I Cursor::GetScreenPosition()
	{
		return Platform::GetCursorPosition();
	}

	void Cursor::Hide()
	{
		Platform::HideCursor();
	}

	void Cursor::Show()
	{
		Platform::ShowCursor();
	}

	void Cursor::ClipToWindow(const RenderWindow& window)
	{
		Platform::ClipCursorToWindow(window);
	}

	void Cursor::ClipToRect(const Rect2I& screenRect)
	{
		Platform::ClipCursorToRect(screenRect);
	}

	void Cursor::ClipDisable()
	{
		Platform::ClipCursorDisable();
	}

	void Cursor::SetCursor(CursorType type)
	{
		UINT32 id = (UINT32)type;
		if((UINT32)mActiveCursorId != id)
		{
			mActiveCursorId = id;
			UpdateCursorImage();
		}
	}

	void Cursor::SetCursor(const String& name)
	{
		auto iterFind = mCustomIconNameToId.find(name);
		if(iterFind == mCustomIconNameToId.end())
		{
			BS_LOG(Warning, Platform, "Cannot find cursor icon with name: " + name);
			return;
		}

		UINT32 id = iterFind->second;
		if((UINT32)mActiveCursorId != id)
		{
			mActiveCursorId = id;
			UpdateCursorImage();
		}
	}

	void Cursor::SetCursorIcon(const String& name, const PixelData& pixelData, const Vector2I& hotSpot)
	{
		auto iterFind = mCustomIconNameToId.find(name);
		if(iterFind != mCustomIconNameToId.end())
		{
			UINT32 id = iterFind->second;
			mCustomIcons[id] = CustomIcon(pixelData, hotSpot);

			if((UINT32)mActiveCursorId == id)
				UpdateCursorImage(); // Refresh active
		}
		else
		{
			UINT32 id = mNextUniqueId++;
			mCustomIconNameToId[name] = id;
			mCustomIcons[id] = CustomIcon(pixelData, hotSpot);
		}
	}

	void Cursor::SetCursorIcon(CursorType type, const PixelData& pixelData, const Vector2I& hotSpot)
	{
		UINT32 id = (UINT32)type;

		mCustomIcons[id].pixelData = pixelData;
		mCustomIcons[id].hotSpot = hotSpot;

		if((UINT32)mActiveCursorId == id)
			UpdateCursorImage(); // Refresh active
	}

	void Cursor::ClearCursorIcon(const String& name)
	{
		auto iterFind = mCustomIconNameToId.find(name);
		if(iterFind == mCustomIconNameToId.end())
			return;

		mCustomIcons.erase(iterFind->second);
		mCustomIconNameToId.erase(iterFind);
	}

	void Cursor::ClearCursorIcon(CursorType type)
	{
		RestoreCursorIcon(type);

		if(mActiveCursorId == (INT32)type)
			UpdateCursorImage(); // Refresh active
	}

	void Cursor::RestoreCursorIcon(CursorType type)
	{
		UINT32 id = (UINT32)type;
		mCustomIcons[id] = CustomIcon();

		switch (type)
		{
		case CursorType::Arrow:
			mCustomIcons[id].pixelData = BuiltinResources::Instance().GetCursorArrow(mCustomIcons[id].hotSpot);
			return;
		case CursorType::Wait:
			mCustomIcons[id].pixelData = BuiltinResources::Instance().GetCursorWait(mCustomIcons[id].hotSpot);
			return;
		case CursorType::IBeam:
			mCustomIcons[id].pixelData = BuiltinResources::Instance().GetCursorIBeam(mCustomIcons[id].hotSpot);
			return;
		case CursorType::ArrowDrag:
			mCustomIcons[id].pixelData = BuiltinResources::Instance().GetCursorArrowDrag(mCustomIcons[id].hotSpot);
			return;
		case CursorType::SizeNESW:
			mCustomIcons[id].pixelData = BuiltinResources::Instance().GetCursorSizeNesw(mCustomIcons[id].hotSpot);
			return;
		case CursorType::SizeNS:
			mCustomIcons[id].pixelData = BuiltinResources::Instance().GetCursorSizeNs(mCustomIcons[id].hotSpot);
			return;
		case CursorType::SizeNWSE:
			mCustomIcons[id].pixelData = BuiltinResources::Instance().GetCursorSizeNwse(mCustomIcons[id].hotSpot);
			return;
		case CursorType::SizeWE:
			mCustomIcons[id].pixelData = BuiltinResources::Instance().GetCursorSizeWe(mCustomIcons[id].hotSpot);
			return;
		case CursorType::Deny:
			mCustomIcons[id].pixelData = BuiltinResources::Instance().GetCursorDeny(mCustomIcons[id].hotSpot);
			return;
		case CursorType::ArrowLeftRight:
			mCustomIcons[id].pixelData = BuiltinResources::Instance().GetCursorMoveLeftRight(mCustomIcons[id].hotSpot);
			return;
		default:
			break;
		}

		BS_EXCEPT(InvalidParametersException, "Invalid cursor type: " + toString((UINT32)type));
	}

	void Cursor::UpdateCursorImage()
	{
		if (mActiveCursorId < 0)
			return;

		CustomIcon& customIcon = mCustomIcons[mActiveCursorId];
		Platform::SetCursor(customIcon.pixelData, customIcon.hotSpot);
	}

	Cursor& gCursor()
	{
		return static_cast<Cursor&>(Cursor::Instance());
	}
}
