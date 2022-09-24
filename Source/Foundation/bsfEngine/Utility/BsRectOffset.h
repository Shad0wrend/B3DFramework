//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsPrerequisites.h"

namespace bs
{
	/** @addtogroup Utility-Engine
	 *  @{
	 */

	/** Rectangle represented in the form of offsets from some parent rectangle. */
	struct BS_SCRIPT_EXPORT(pl:true,m:Math) RectOffset
	{
		RectOffset() = default;

		RectOffset(INT32 left, INT32 right, INT32 top, INT32 bottom)
			:Left(left), Right(right), Top(top), Bottom(bottom)
		{ }

		bool operator==(const RectOffset& rhs) const
		{
			return Left == rhs.Left && Right == rhs.Right && Top == rhs.Top && Bottom == rhs.Bottom;
		}

		bool operator!=(const RectOffset& rhs) const
		{
			return !operator==(rhs);
		}

		INT32 Left = 0;
		INT32 Right = 0;
		INT32 Top = 0;
		INT32 Bottom = 0;
	};

	/** @} */
}
