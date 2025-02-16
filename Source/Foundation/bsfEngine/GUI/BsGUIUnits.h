//************************************ bs::framework - Copyright 2025 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsPrerequisites.h"
#include "Math/BsUnitValue.h"
#include "Math/BsSize2.h"
#include "Math/BsVector2.h"

namespace bs
{
	/** @addtogroup GUI
	 *  @{
	 */

	/** Location in 2D space in physical pixels. Physical pixels represent actual pixels as displayed on the output monitor. */
	struct B3D_SCRIPT_EXPORT(ExportAsStruct(true)) PhysicalPixel { };

	/**
	 * Location in 2D space in logical pixels. Logical pixels are defined at 1/96th of one logical inch. Logical pixels are transformed
	 * into physical pixels by scaling it by the display's DPI scale. If your display is set to 96 DPI, then one logical pixel equals one physical pixel.
	 */
	struct B3D_SCRIPT_EXPORT(ExportAsStruct(true)) LogicalPixel { };

	extern template struct B3D_SCRIPT_EXPORT(DocumentationGroup(GUI), ExportAsStruct(true)) TUnitValue<i32, LogicalPixel>;
	extern template struct B3D_SCRIPT_EXPORT(DocumentationGroup(GUI), ExportAsStruct(true)) TUnitValue<float, LogicalPixel>;
	extern template struct B3D_SCRIPT_EXPORT(DocumentationGroup(GUI), ExportAsStruct(true)) TUnitValue<i32, PhysicalPixel>;
	extern template struct B3D_SCRIPT_EXPORT(DocumentationGroup(GUI), ExportAsStruct(true)) TUnitValue<float, PhysicalPixel>;

	template<> const TVector2<TUnitValue<i32, LogicalPixel>> TVector2<TUnitValue<i32, LogicalPixel>>::kZero{BsZero};
	template<> const TVector2<TUnitValue<float, LogicalPixel>> TVector2<TUnitValue<float, LogicalPixel>>::kZero{BsZero};
	template<> const TVector2<TUnitValue<i32, PhysicalPixel>> TVector2<TUnitValue<i32, PhysicalPixel>>::kZero{BsZero};
	template<> const TVector2<TUnitValue<float, PhysicalPixel>> TVector2<TUnitValue<float, PhysicalPixel>>::kZero{BsZero};

	template<> const TVector2<TUnitValue<i32, LogicalPixel>> TVector2<TUnitValue<i32, LogicalPixel>>::kOne{1, 1};
	template<> const TVector2<TUnitValue<float, LogicalPixel>> TVector2<TUnitValue<float, LogicalPixel>>::kOne{1.0f, 1.0f};
	template<> const TVector2<TUnitValue<i32, PhysicalPixel>> TVector2<TUnitValue<i32, PhysicalPixel>>::kOne{1, 1};
	template<> const TVector2<TUnitValue<float, PhysicalPixel>> TVector2<TUnitValue<float, PhysicalPixel>>::kOne{1.0f, 1.0f};

	template<> const TVector2<TUnitValue<i32, LogicalPixel>> TVector2<TUnitValue<i32, LogicalPixel>>::kUnitX{1, 0};
	template<> const TVector2<TUnitValue<float, LogicalPixel>> TVector2<TUnitValue<float, LogicalPixel>>::kUnitX{1.0f, 0.0f};
	template<> const TVector2<TUnitValue<i32, PhysicalPixel>> TVector2<TUnitValue<i32, PhysicalPixel>>::kUnitX{1, 0};
	template<> const TVector2<TUnitValue<float, PhysicalPixel>> TVector2<TUnitValue<float, PhysicalPixel>>::kUnitX{1.0f, 0.0f};

	template<> const TVector2<TUnitValue<i32, LogicalPixel>> TVector2<TUnitValue<i32, LogicalPixel>>::kUnitY{0, 1};
	template<> const TVector2<TUnitValue<float, LogicalPixel>> TVector2<TUnitValue<float, LogicalPixel>>::kUnitY{0.0f, 1.0f};
	template<> const TVector2<TUnitValue<i32, PhysicalPixel>> TVector2<TUnitValue<i32, PhysicalPixel>>::kUnitY{0, 1};
	template<> const TVector2<TUnitValue<float, PhysicalPixel>> TVector2<TUnitValue<float, PhysicalPixel>>::kUnitY{0.0f, 1.0f};

	extern template struct B3D_SCRIPT_EXPORT(DocumentationGroup(GUI), ExportAsStruct(true)) TVector2<TUnitValue<i32, LogicalPixel>>;
	extern template struct B3D_SCRIPT_EXPORT(DocumentationGroup(GUI), ExportAsStruct(true)) TVector2<TUnitValue<float, LogicalPixel>>;
	extern template struct B3D_SCRIPT_EXPORT(DocumentationGroup(GUI), ExportAsStruct(true)) TVector2<TUnitValue<i32, PhysicalPixel>>;
	extern template struct B3D_SCRIPT_EXPORT(DocumentationGroup(GUI), ExportAsStruct(true)) TVector2<TUnitValue<float, PhysicalPixel>>;
	
	template<> const TSize2<TUnitValue<i32, LogicalPixel>> TSize2<TUnitValue<i32, LogicalPixel>>::kZero{BsZero};
	template<> const TSize2<TUnitValue<float, LogicalPixel>> TSize2<TUnitValue<float, LogicalPixel>>::kZero{BsZero};
	template<> const TSize2<TUnitValue<i32, PhysicalPixel>> TSize2<TUnitValue<i32, PhysicalPixel>>::kZero{BsZero};
	template<> const TSize2<TUnitValue<float, PhysicalPixel>> TSize2<TUnitValue<float, PhysicalPixel>>::kZero{BsZero};

	extern template struct B3D_SCRIPT_EXPORT(DocumentationGroup(GUI), ExportAsStruct(true)) TSize2<TUnitValue<i32, LogicalPixel>>;
	extern template struct B3D_SCRIPT_EXPORT(DocumentationGroup(GUI), ExportAsStruct(true)) TSize2<TUnitValue<float, LogicalPixel>>;
	extern template struct B3D_SCRIPT_EXPORT(DocumentationGroup(GUI), ExportAsStruct(true)) TSize2<TUnitValue<i32, PhysicalPixel>>;
	extern template struct B3D_SCRIPT_EXPORT(DocumentationGroup(GUI), ExportAsStruct(true)) TSize2<TUnitValue<float, PhysicalPixel>>;

	/** @} */
} // namespace bs
