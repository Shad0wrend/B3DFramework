//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsUtilityPrerequisites.h"
#include "Math/BsVector3.h"
#include "Math/BsAABox.h"
#include "Math/BsSphere.h"
#include "Math/BsMatrix4.h"

namespace b3d
{
	/** @addtogroup Math
	 *  @{
	 */

	/** Bounds represented by an axis aligned box and a sphere. */
	template<typename T>
	struct B3D_UTILITY_EXPORT TBounds
	{
		TBounds() = default;
		TBounds(const TAABox<T>& box, const TSphere<T>& sphere);
		~TBounds() = default;

		/** Returns the axis aligned box representing the bounds. */
		const TAABox<T>& GetBox() const { return mBox; }

		/** Returns the sphere representing the bounds. */
		const TSphere<T>& GetSphere() const { return mSphere; }

		/** Updates the bounds by setting the new bounding box and sphere. */
		void SetBounds(const TAABox<T>& box, const TSphere<T>& sphere);

		/** Merges the two bounds, creating a new bounds that encapsulates them both. */
		void Merge(const TBounds& rhs);

		/** Expands the bounds so it includes the provided point. */
		void Merge(const TVector3<T>& point);

		/**
		 * Transforms the bounds by the given matrix.
		 *
		 * @note
		 * As the resulting box will no longer be axis aligned, an axis align box
		 * is instead created by encompassing the transformed oriented bounding box.
		 * Retrieving the value as an actual OBB would provide a tighter fit.
		 */
		void Transform(const TMatrix4<T>& matrix);

		/**
		 * Transforms the bounds by the given matrix.
		 *
		 * @note
		 * As the resulting box will no longer be axis aligned, an axis align box
		 * is instead created by encompassing the transformed oriented bounding box.
		 * Retrieving the value as an actual OBB would provide a tighter fit.
		 *
		 * @note
		 * Provided matrix must be affine.
		 */
		void TransformAffine(const TMatrix4<T>& matrix);

		static const TBounds<T> kEmpty;
		static const TBounds<T> kUnit;
		static const TBounds<T> kInfinite;
	protected:
		TAABox<T> mBox;
		TSphere<T> mSphere;
	};

	template<> const TBounds<float> TBounds<float>::kEmpty = TBounds(TAABox<float>::kEmpty, TSphere<float>::kEmpty);
	template<> const TBounds<double> TBounds<double>::kEmpty = TBounds(TAABox<double>::kEmpty, TSphere<double>::kEmpty);

	template<> const TBounds<float> TBounds<float>::kUnit = TBounds(TAABox<float>::kUnit, TSphere<float>::kUnit);
	template<> const TBounds<double> TBounds<double>::kUnit = TBounds(TAABox<double>::kUnit, TSphere<double>::kUnit);

	template<> const TBounds<float> TBounds<float>::kInfinite = TBounds(TAABox<float>::kEmpty, TSphere<float>::kInfinite);
	template<> const TBounds<double> TBounds<double>::kInfinite = TBounds(TAABox<double>::kEmpty, TSphere<double>::kInfinite);

	/** @} */
} // namespace b3d
