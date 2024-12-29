//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsPrerequisites.h"
#include "Math/BsRect2I.h"

namespace bs
{
	/** @addtogroup GUI-Internal
	 *  @{
	 */

	/**
	 * Contains all attributes that are output by GUI layouts and assigned to GUI elements. This includes element position,
	 * size and depth.
	 */
	struct B3D_EXPORT GUILayoutData
	{
		GUILayoutData()
		{
			SetPanelDepth(0);
		}

		/**	Set widget part of element depth (Most significant part). */
		void SetWidgetDepth(u8 widgetDepth)
		{
			u32 shiftedDepth = widgetDepth << 24;

			Depth = shiftedDepth | (Depth & 0x00FFFFFF);
		}

		/** Set panel part of element depth. Less significant than widget depth but more than custom element depth. */
		void SetPanelDepth(i16 panelDepth)
		{
			u32 signedDepth = ((i32)panelDepth + 32768) << 8;

			Depth = signedDepth | (Depth & 0xFF0000FF);
		}

		/**	Retrieve widget part of element depth (Most significant part). */
		u8 GetWidgetDepth() const
		{
			return (Depth >> 24) & 0xFF;
		}

		/** Retrieve panel part of element depth. Less significant than widget depth but more than custom element depth. */
		i16 GetPanelDepth() const
		{
			return (((i32)Depth >> 8) & 0xFFFF) - 32768;
		}

		/**	Returns a clip rectangle that is relative to the current bounds. */
		Rect2I GetLocalClipRect() const
		{
			Rect2I localClipRect = AbsoluteClippedArea;
			localClipRect.X -= AbsoluteArea.X;
			localClipRect.Y -= AbsoluteArea.Y;

			return localClipRect;
		}

		Vector2I RelativePosition; /**< Coordinates relative to the parent GUI element. Set during UpdateLayout pass. */
		Vector2I AbsolutePosition; /**< Coordinates relative to the parent GUI widget. Set during UpdateVisibleBounds pass. */
		Size2UI Size; /**< Size of the GUI element in pixels. */
		Rect2I AbsoluteArea; /**< Absolute coordinates, plus size of the GUI element. */ // TODO - To be removed
		Rect2I AbsoluteClippedArea; /**< AbsoluteArea clipped by the parent's visible area. */
		u32 Depth = 0;
		u16 DepthRangeMin = -1;
		u16 DepthRangeMax = -1;
	};

	/** @} */
} // namespace bs
