//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "GUI/BsDropDownAreaPlacement.h"
#include "Debug/BsDebug.h"

namespace bs
{
	DropDownAreaPlacement DropDownAreaPlacement::AroundPosition(const Vector2I& position)
	{
		DropDownAreaPlacement instance;
		instance.mType = Type::Position;
		instance.mPosition = position;

		return instance;
	}

	DropDownAreaPlacement DropDownAreaPlacement::AroundBoundsVert(const Rect2I& bounds)
	{
		DropDownAreaPlacement instance;
		instance.mType = Type::BoundsVert;
		instance.mBounds = bounds;

		return instance;
	}

	DropDownAreaPlacement DropDownAreaPlacement::AroundBoundsHorz(const Rect2I& bounds)
	{
		DropDownAreaPlacement instance;
		instance.mType = Type::BoundsHorz;
		instance.mBounds = bounds;

		return instance;
	}

	DropDownAreaPlacement DropDownAreaPlacement::AroundBounds(const Rect2I& bounds)
	{
		DropDownAreaPlacement instance;
		instance.mType = Type::BoundsAll;
		instance.mBounds = bounds;

		return instance;
	}

	Rect2I DropDownAreaPlacement::GetOptimalBounds(UINT32 width, UINT32 height, const Rect2I& availableArea, HorzDir& horzDir, VertDir& vertDir) const
	{
		Rect2I output;

		int potentialLeftStart = 0;
		int potentialRightStart = 0;
		int potentialTopStart = 0;
		int potentialBottomStart = 0;

		switch (GetType())
		{
		case DropDownAreaPlacement::Type::Position:
			potentialLeftStart = potentialRightStart = GetPosition().X;
			potentialTopStart = potentialBottomStart = GetPosition().Y;
			break;
		case DropDownAreaPlacement::Type::BoundsHorz:
			potentialRightStart = GetBounds().X;
			potentialLeftStart = GetBounds().X + GetBounds().Width;
			potentialBottomStart = GetBounds().Y + GetBounds().Height;
			potentialTopStart = GetBounds().Y;
			break;
		case DropDownAreaPlacement::Type::BoundsVert:
			potentialRightStart = GetBounds().X + GetBounds().Width;
			potentialLeftStart = GetBounds().X;
			potentialBottomStart = GetBounds().Y;
			potentialTopStart = GetBounds().Y + GetBounds().Height;
			break;
		case DropDownAreaPlacement::Type::BoundsAll:
			potentialRightStart = GetBounds().X + GetBounds().Width;
			potentialLeftStart = GetBounds().X;
			potentialBottomStart = GetBounds().Y + GetBounds().Height;
			potentialTopStart = GetBounds().Y;
			break;
		}

		// Determine x position and whether to align to left or right side of the drop down list
		UINT32 availableRightwardWidth = (UINT32)std::max(0, (availableArea.X + (INT32)availableArea.Width) - potentialRightStart);
		UINT32 availableLeftwardWidth = (UINT32)std::max(0, potentialLeftStart - availableArea.X);

		//// Prefer right if possible
		if (width <= availableRightwardWidth)
		{
			output.X = potentialRightStart;
			output.Width = width;
			horzDir = HorzDir::Right;
		}
		else
		{
			if (availableRightwardWidth >= availableLeftwardWidth)
			{
				output.X = potentialRightStart;
				output.Width = std::min(width, availableRightwardWidth);
				horzDir = HorzDir::Right;
			}
			else
			{
				output.X = potentialLeftStart - std::min(width, availableLeftwardWidth);
				output.Width = std::min(width, availableLeftwardWidth);
				horzDir = HorzDir::Left;
			}
		}

		// Determine y position and whether to open upward or downward
		UINT32 availableDownwardHeight = (UINT32)std::max(0, (availableArea.Y + (INT32)availableArea.Height) - potentialBottomStart);
		UINT32 availableUpwardHeight = (UINT32)std::max(0, potentialTopStart - availableArea.Y);

		//// Prefer down if possible
		if (height <= availableDownwardHeight)
		{
			output.Y = potentialBottomStart;
			output.Height = height;
			vertDir = VertDir::Down;
		}
		else
		{
			if (availableDownwardHeight >= availableUpwardHeight)
			{
				output.Y = potentialBottomStart;
				output.Height = std::min(height, availableDownwardHeight);;
				vertDir = VertDir::Down;
			}
			else
			{
				output.Y = potentialTopStart - (INT32)std::min(height, availableUpwardHeight);
				output.Height = std::min(height, availableUpwardHeight);
				vertDir = VertDir::Up;
			}
		}

		return output;
	}
}
