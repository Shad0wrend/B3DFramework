//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Prerequisites/BsPrerequisitesUtil.h"
#include "BsRect2.h"

namespace bs
{
	/** @addtogroup Math
	 *  @{
	 */

	/** Represents a 2D rectangle. Rectangle is represented with an origin in top left and width/height. */
	template<typename PositionType, typename SizeType = PositionType>
	struct TRect2
	{
		constexpr TRect2() = default;

		constexpr TRect2(PositionType x, PositionType y, SizeType width, SizeType height)
			: X(x), Y(y), Width(width), Height(height)
		{}

		TRect2(const TVector2<PositionType>& position, const TSize2<SizeType>& size);

		PositionType X = (PositionType)0;
		PositionType Y = (PositionType)0;
		SizeType Width = (SizeType)0;
		SizeType Height = (SizeType)0;

		/** Converts a unit with one underlying type to another. */
		template<typename PositionType2, typename SizeType2 = PositionType2>
		TRect2<PositionType2, SizeType2> To() const { return TRect2<PositionType2, SizeType2>((PositionType2)X, (PositionType2)Y, (SizeType2)Width, (SizeType2)Height); }

		/** Returns true if the area covered is 0. */
		bool IsEmpty() const { return Width == 0 || Height == 0; }

		/** Returns the size of the rectangle. */
		TSize2<SizeType> GetSize() const { return TSize2<SizeType>(Width, Height); }

		/** Returns true if the rectangle contains the provided point. */
		bool Contains(const TVector2<PositionType>& point) const;

		/** Returns true if the rectangle fully contains the provided rectangle. */
		bool Contains(const TRect2& other) const;

		/**
		 * Returns true if the rectangle overlaps the provided rectangle. Also returns true if the rectangles are contained
		 * within each other completely (no intersecting edges).
		 */
		bool Overlaps(const TRect2& other) const;

		/** Extends this rectangle so that the provided rectangle is completely contained within it. */
		void Encapsulate(const TRect2& other);

		/** Sets the X/Y coordinates of the rectangle. */
		void SetPosition(const TVector2<PositionType>& position);

		/** Sets the width/height of the rectangle. */
		void SetSize(const TSize2<SizeType>& size);

		/** Clips current rectangle so that it does not overlap the provided rectangle. */
		void Clip(const TRect2& clipRect);

		/**
		 * Cuts the current rectangle with the provided rectangle and outputs the pieces. The pieces will contain all area
		 * of the current rectangle without including the cut rectangle area.
		 */
		u32 Cut(const TRect2& rectangleToCutWith, Array<TRect2, 4>& outPieces) const;

		/**
		 * Cuts the current rectangle with the provided rectangles and outputs the pieces. The pieces will contain all area
		 * of the current rectangle without including the cut rectangles area.
		 */
		void Cut(const Vector<TRect2>& rectanglesToCutWith, Vector<TRect2>& outPieces) const;

		/**
		 * Transforms the bounds by the given matrix. Resulting value is an axis aligned rectangle encompassing the
		 * transformed points.
		 *
		 * @note
		 * Since the resulting value is an AA rectangle of the original transformed rectangle, the bounds will be larger
		 * than needed. Oriented rectangle would provide a much tighter fit.
		 */
		void Transform(const Matrix4& matrix);

		/**
		 * Adds a unique area to the provided @p outAreas list. The list will be modified by adding, removing or cutting areas in the list to ensure
		 * only non-overlapping areas are present in the list.
		 *
		 * @param	area		Area to try to add to the list.
		 * @param	inOutAreas	List of unique areas to append the result to. Must be Vector<T> or FrameVector<T>.
		 */
		template<class T>
		static void AddUnique(const TRect2& area, T& inOutAreas);

		bool operator==(const TRect2& rhs) const
		{
			return X == rhs.X && Y == rhs.Y && Width == rhs.Width && Height == rhs.Height;
		}

		bool operator!=(const TRect2& rhs) const
		{
			return !(*this == rhs);
		}

		static const TRect2 kEmpty;
	};

	template<> const TRect2<i32> TRect2<i32>::kEmpty;
	template<> const TRect2<i32, u32> TRect2<i32, u32>::kEmpty;
	template<> const TRect2<float> TRect2<float>::kEmpty;

	extern template struct B3D_SCRIPT_EXPORT(DocumentationGroup(Math), ExportAsStruct(true)) TRect2<i32>;
	extern template struct B3D_SCRIPT_EXPORT(DocumentationGroup(Math), ExportAsStruct(true)) TRect2<i32, u32>;
	extern template struct B3D_SCRIPT_EXPORT(DocumentationGroup(Math), ExportAsStruct(true)) TRect2<double>;

	/** @} */
}

/** @cond STDLIB */

namespace std
{
	/** Hash value generator for TRect2<T>. */
	template<typename PositionType, typename SizeType>
	struct hash<bs::TRect2<PositionType, SizeType>>
	{
		size_t operator()(const bs::TRect2<PositionType, SizeType>& value) const
		{
			size_t hash = 0;
			bs::B3DCombineHash(hash, value.X);
			bs::B3DCombineHash(hash, value.Y);
			bs::B3DCombineHash(hash, value.Width);
			bs::B3DCombineHash(hash, value.Height);

			return hash;
		}
	};
} // namespace std

/** @endcond */

namespace bs
{
	/** @addtogroup Math
	 *  @{
	 */

	/** Represents a 2D rectangle using integer values. Rectangle is represented with an origin in top left and width/height. */
	class B3D_UTILITY_EXPORT Rect2I
	{
	public:
		constexpr Rect2I() = default;

		constexpr Rect2I(i32 x, i32 y, u32 width, u32 height)
			: X(x), Y(y), Width(width), Height(height)
		{}

		Rect2I(const Vector2I& position, const Size2UI& size);

		i32 X = 0;
		i32 Y = 0;
		u32 Width = 0;
		u32 Height = 0;

		/** Returns true if the area covered is 0. */
		bool IsEmpty() const { return Width == 0 || Height == 0; }

		/** Returns the size of the rectangle. */
		Size2UI GetSize() const { return Size2UI(Width, Height); }

		/** Returns true if the rectangle contains the provided point. */
		bool Contains(const Vector2I& point) const;

		/** Returns true if the rectangle fully contains the provided rectangle. */
		bool Contains(const Rect2I& other) const;

		/**
		 * Returns true if the rectangle overlaps the provided rectangle. Also returns true if the rectangles are contained
		 * within each other completely (no intersecting edges).
		 */
		bool Overlaps(const Rect2I& other) const;

		/** Extends this rectangle so that the provided rectangle is completely contained within it. */
		void Encapsulate(const Rect2I& other);

		/** Sets the X/Y coordinates of the rectangle. */
		void SetPosition(const Vector2I& position);

		/** Sets the width/height of the rectangle. */
		void SetSize(const Size2UI& size);

		/** Clips current rectangle so that it does not overlap the provided rectangle. */
		void Clip(const Rect2I& clipRect);

		/**
		 * Cuts the current rectangle with the provided rectangle and outputs the pieces. The pieces will contain all area
		 * of the current rectangle without including the cut rectangle area.
		 */
		u32 Cut(const Rect2I& rectangleToCutWith, Array<Rect2I, 4>& outPieces) const;

		/**
		 * Cuts the current rectangle with the provided rectangles and outputs the pieces. The pieces will contain all area
		 * of the current rectangle without including the cut rectangles area.
		 */
		void Cut(const Vector<Rect2I>& rectanglesToCutWith, Vector<Rect2I>& outPieces) const;

		/**
		 * Transforms the bounds by the given matrix. Resulting value is an axis aligned rectangle encompassing the
		 * transformed points.
		 *
		 * @note
		 * Since the resulting value is an AA rectangle of the original transformed rectangle, the bounds will be larger
		 * than needed. Oriented rectangle would provide a much tighter fit.
		 */
		void Transform(const Matrix4& matrix);

		/**
		 * Adds a unique area to the provided @p outAreas list. The list will be modified by adding, removing or cutting areas in the list to ensure
		 * only non-overlapping areas are present in the list.
		 *
		 * @param	area		Area to try to add to the list.
		 * @param	inOutAreas	List of unique areas to append the result to. Must be Vector<T> or FrameVector<T>.
		 */
		template<class T>
		static void AddUnique(const Rect2I& area, T& inOutAreas);

		bool operator==(const Rect2I& rhs) const
		{
			return X == rhs.X && Y == rhs.Y && Width == rhs.Width && Height == rhs.Height;
		}

		bool operator!=(const Rect2I& rhs) const
		{
			return !(*this == rhs);
		}

		explicit operator Rect2() const
		{
			return Rect2((float)X, (float)Y, (float)Width, (float)Height);
		}

		static const Rect2I kEmpty;
	};

	/** @} */
} // namespace bs

/** @cond STDLIB */

namespace std
{
/** Hash value generator for bs::Rect2I. */
template<>
struct hash<bs::Rect2I>
{
	size_t operator()(const bs::Rect2I& value) const
	{
		size_t hash = 0;
		bs::B3DCombineHash(hash, value.X);
		bs::B3DCombineHash(hash, value.Y);
		bs::B3DCombineHash(hash, value.Width);
		bs::B3DCombineHash(hash, value.Height);

		return hash;
	}
};
} // namespace std

/** @endcond */
