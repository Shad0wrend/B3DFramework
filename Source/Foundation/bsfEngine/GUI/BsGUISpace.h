//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsPrerequisites.h"
#include "GUI/BsGUIElement.h"

namespace bs
{
	/** @addtogroup GUI
	 *  @{
	 */

	/** GUI element that may be inserted into layouts in order to make a space of a fixed size. */
	class B3D_EXPORT GUIFixedSpace : public GUIElement
	{
	public:
		GUIFixedSpace(GUILogicalUnit size);
		~GUIFixedSpace() override = default;

		/**	Returns the size of the space in pixels. */
		GUILogicalUnit GetSize() const { return mSize; }

		/**	Changes the size of the space to the specified value, in pixels. */
		void SetSize(GUILogicalUnit size)
		{
			if(mSize != size)
			{
				mSize = size;
				MarkLayoutAsDirty();
			}
		}

		/**	Creates a new fixed space GUI element. */
		static GUIFixedSpace* Create(GUILogicalUnit size);

	public: // ***** INTERNAL ******
		/** @name Internal
		 *  @{
		 */

		GUILogicalSize CalculateUnconstrainedOptimalSize() const override { return GUILogicalSize(GetSize(), GetSize()); }
		GUIConstrainedSize CalculateConstrainedSize() const override;

		/** @} */
	protected:
		GUILogicalUnit mSize;

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/
	public:
		friend class GUIFixedSpaceRTTI;
		static RTTIType* GetRttiStatic();
		RTTIType* GetRtti() const override;
	};

	/**
	 * GUI element that may be inserted into layouts to make a space of a flexible size. The space will expand only if
	 * there is room and other elements are not squished because of it. If multiple flexible spaces are in a layout, their
	 * sizes will be shared equally.
	 *
	 * @note
	 * For example if you had a horizontal layout with a button, and you wanted to align that button to the right of the
	 * layout, you would insert a flexible space before the button in the layout.
	 */
	class B3D_EXPORT GUIFlexibleSpace : public GUIElement
	{
	public:
		GUIFlexibleSpace();
		~GUIFlexibleSpace() override = default;

		/**	Creates a new flexible space GUI element. */
		static GUIFlexibleSpace* Create();

	public: // ***** INTERNAL ******
		/** @name Internal
		 *  @{
		 */

		GUILogicalSize CalculateUnconstrainedOptimalSize() const override { return GUILogicalSize(0, 0); }
		GUIConstrainedSize CalculateConstrainedSize() const override;

		/** @} */

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/
	public:
		friend class GUIFlexibleSpaceRTTI;
		static RTTIType* GetRttiStatic();
		RTTIType* GetRtti() const override;
	};

	/** @} */
} // namespace bs
