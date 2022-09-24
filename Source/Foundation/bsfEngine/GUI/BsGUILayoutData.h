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
	struct BS_EXPORT GUILayoutData
	{
		GUILayoutData()
		{
			SetPanelDepth(0);
		}

		/**	Set widget part of element depth (Most significant part). */
		void SetWidgetDepth(UINT8 widgetDepth)
		{
			UINT32 shiftedDepth = widgetDepth << 24;

			Depth = shiftedDepth | (Depth & 0x00FFFFFF);
		}

		/** Set panel part of element depth. Less significant than widget depth but more than custom element depth. */
		void SetPanelDepth(INT16 panelDepth)
		{
			UINT32 signedDepth = ((INT32)panelDepth + 32768) << 8;

			Depth = signedDepth | (Depth & 0xFF0000FF);;
		}

		/**	Retrieve widget part of element depth (Most significant part). */
		UINT8 GetWidgetDepth() const
		{
			return (Depth >> 24) & 0xFF;
		}

		/** Retrieve panel part of element depth. Less significant than widget depth but more than custom element depth. */
		INT16 GetPanelDepth() const
		{
			return (((INT32)Depth >> 8) & 0xFFFF) - 32768;
		}

		/**	Returns a clip rectangle that is relative to the current bounds. */
		Rect2I GetLocalClipRect() const
		{
			Rect2I localClipRect = ClipRect;
			localClipRect.X -= Area.X;
			localClipRect.Y -= Area.Y;

			return localClipRect;
		}

		Rect2I Area;
		Rect2I ClipRect;
		UINT32 Depth = 0;
		UINT16 DepthRangeMin = -1;
		UINT16 DepthRangeMax = -1;
	};

	/** @} */
}
