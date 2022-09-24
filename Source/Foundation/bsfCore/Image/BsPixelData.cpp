//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Image/BsPixelData.h"
#include "Image/BsPixelUtil.h"
#include "Private/RTTI/BsPixelDataRTTI.h"
#include "Image/BsColor.h"
#include "Math/BsVector2.h"
#include "Math/BsMath.h"
#include "Debug/BsDebug.h"

namespace bs
{
	PixelData::PixelData(const PixelVolume& extents, PixelFormat pixelFormat)
		:mExtents(extents), mFormat(pixelFormat)
	{
		PixelUtil::GetPitch(extents.GetWidth(), extents.GetHeight(), extents.GetDepth(), pixelFormat, mRowPitch,
			mSlicePitch);
	}

	PixelData::PixelData(UINT32 width, UINT32 height, UINT32 depth, PixelFormat pixelFormat)
		: mExtents(0, 0, 0, width, height, depth), mFormat(pixelFormat)
	{
		PixelUtil::GetPitch(width, height, depth, pixelFormat, mRowPitch, mSlicePitch);
	}

	PixelData::PixelData(const PixelData& copy)
		:GpuResourceData(copy)
	{
		mFormat = copy.mFormat;
		mRowPitch = copy.mRowPitch;
		mSlicePitch = copy.mSlicePitch;
		mExtents = copy.mExtents;
	}

	PixelData& PixelData::operator=(const PixelData& rhs)
	{
		GpuResourceData::operator= (rhs);

		mFormat = rhs.mFormat;
		mRowPitch = rhs.mRowPitch;
		mSlicePitch = rhs.mSlicePitch;
		mExtents = rhs.mExtents;

		return *this;
	}

	UINT32 PixelData::GetRowSkip() const
	{
		UINT32 optimalRowPitch, optimalSlicePitch;
		PixelUtil::GetPitch(GetWidth(), GetHeight(), GetDepth(), mFormat, optimalRowPitch,
			optimalSlicePitch);

		return mRowPitch - optimalRowPitch;
	}

	UINT32 PixelData::GetSliceSkip() const
	{
		UINT32 optimalRowPitch, optimalSlicePitch;
		PixelUtil::GetPitch(GetWidth(), GetHeight(), GetDepth(), mFormat, optimalRowPitch,
			optimalSlicePitch);

		return mSlicePitch - optimalSlicePitch;
	}

	UINT32 PixelData::GetConsecutiveSize() const
	{
		return PixelUtil::GetMemorySize(GetWidth(), GetHeight(), GetDepth(), mFormat);
	}

	UINT32 PixelData::GetSize() const
	{
		return mSlicePitch * GetDepth();
	}

	PixelData PixelData::GetSubVolume(const PixelVolume& volume) const
	{
		if (PixelUtil::IsCompressed(mFormat))
		{
			if (volume.Left == GetLeft() && volume.Top == GetTop() && volume.Front == GetFront() &&
				volume.Right == GetRight() && volume.Bottom == GetBottom() && volume.Back == GetBack())
			{
				// Entire buffer is being queried
				return *this;
			}

			BS_EXCEPT(InvalidParametersException, "Cannot return subvolume of compressed PixelBuffer");
		}

		if (!mExtents.Contains(volume))
		{
			BS_EXCEPT(InvalidParametersException, "Bounds out of range");
		}

		const size_t elemSize = PixelUtil::GetNumElemBytes(mFormat);
		PixelData rval(volume.GetWidth(), volume.GetHeight(), volume.GetDepth(), mFormat);

		rval.SetExternalBuffer(((UINT8*)GetData()) + ((volume.Left - GetLeft())*elemSize)
			+ ((volume.Top - GetTop())*mRowPitch)
			+ ((volume.Front - GetFront())*mSlicePitch));

		rval.mFormat = mFormat;
		PixelUtil::GetPitch(volume.GetWidth(), volume.GetHeight(), volume.GetDepth(), mFormat, rval.mRowPitch,
							rval.mSlicePitch);

		return rval;
	}

	Color PixelData::SampleColorAt(const Vector2& coords, TextureFilter filter) const
	{
		Vector2 pixelCoords = coords * Vector2((float)mExtents.GetWidth(), (float)mExtents.GetHeight());

		INT32 maxExtentX = std::max(0, (INT32)mExtents.GetWidth() - 1);
		INT32 maxExtentY = std::max(0, (INT32)mExtents.GetHeight() - 1);

		if(filter == TF_BILINEAR)
		{
			pixelCoords -= Vector2(0.5f, 0.5f);

			UINT32 x = (UINT32)Math::Clamp(Math::FloorToInt(pixelCoords.X), 0, maxExtentX);
			UINT32 y = (UINT32)Math::Clamp(Math::FloorToInt(pixelCoords.Y), 0, maxExtentY);

			float fracX = pixelCoords.X - x;
			float fracY = pixelCoords.Y - y;

			x = Math::Clamp(x, 0U, (UINT32)maxExtentX);
			y = Math::Clamp(y, 0U, (UINT32)maxExtentY);

			INT32 x1 = Math::Clamp(x + 1, 0U, (UINT32)maxExtentX);
			INT32 y1 = Math::Clamp(y + 1, 0U, (UINT32)maxExtentY);

			Color color = Color::ZERO;
			color += (1.0f - fracX) * (1.0f - fracY) * GetColorAt(x, y);
			color += fracX * (1.0f - fracY) * GetColorAt(x1, y);
			color += (1.0f - fracX) * fracY * GetColorAt(x, y1);
			color += fracX * fracY * GetColorAt(x1, y1);

			return color;
		}
		else
		{
			UINT32 x = (UINT32)Math::Clamp(Math::FloorToInt(pixelCoords.X), 0, maxExtentX);
			UINT32 y = (UINT32)Math::Clamp(Math::FloorToInt(pixelCoords.Y), 0, maxExtentY);

			return GetColorAt(x, y);
		}
	}

	Color PixelData::GetColorAt(UINT32 x, UINT32 y, UINT32 z) const
	{
		Color cv;

		UINT32 pixelSize = PixelUtil::GetNumElemBytes(mFormat);
		UINT32 pixelOffset = z * mSlicePitch + y * mRowPitch + x * pixelSize;
		PixelUtil::UnpackColor(&cv, mFormat, (unsigned char *)GetData() + pixelOffset);

		return cv;
	}

	void PixelData::SetColorAt(const Color& color, UINT32 x, UINT32 y, UINT32 z)
	{
		UINT32 pixelSize = PixelUtil::GetNumElemBytes(mFormat);
		UINT32 pixelOffset = z * mSlicePitch + y * mRowPitch + x * pixelSize;
		PixelUtil::PackColor(color, mFormat, (unsigned char *)GetData() + pixelOffset);
	}

	Vector<Color> PixelData::GetColors() const
	{
		UINT32 depth = mExtents.GetDepth();
		UINT32 height = mExtents.GetHeight();
		UINT32 width = mExtents.GetWidth();

		UINT32 pixelSize = PixelUtil::GetNumElemBytes(mFormat);
		UINT8* data = GetData();

		Vector<Color> colors(width * height * depth);
		for (UINT32 z = 0; z < depth; z++)
		{
			UINT32 zArrayIdx = z * width * height;
			UINT32 zDataIdx = z * mSlicePitch;

			for (UINT32 y = 0; y < height; y++)
			{
				UINT32 yArrayIdx = y * width;
				UINT32 yDataIdx = y * mRowPitch;

				for (UINT32 x = 0; x < width; x++)
				{
					UINT32 arrayIdx = x + yArrayIdx + zArrayIdx;
					UINT32 dataIdx = x * pixelSize + yDataIdx + zDataIdx;

					UINT8* dest = data + dataIdx;
					PixelUtil::UnpackColor(&colors[arrayIdx], mFormat, dest);
				}
			}
		}

		return colors;
	}

	template<class T>
	void PixelData::SetColorsInternal(const T& colors, UINT32 numElements)
	{
		UINT32 depth = mExtents.GetDepth();
		UINT32 height = mExtents.GetHeight();
		UINT32 width = mExtents.GetWidth();

		UINT32 totalNumElements = width * height * depth;
		if (numElements != totalNumElements)
		{
			BS_LOG(Error, PixelUtility, "Unable to set colors, invalid array size.");
			return;
		}

		UINT32 pixelSize = PixelUtil::GetNumElemBytes(mFormat);
		UINT8* data = GetData();

		for (UINT32 z = 0; z < depth; z++)
		{
			UINT32 zArrayIdx = z * width * height;
			UINT32 zDataIdx = z * mSlicePitch;

			for (UINT32 y = 0; y < height; y++)
			{
				UINT32 yArrayIdx = y * width;
				UINT32 yDataIdx = y * mRowPitch;

				for (UINT32 x = 0; x < width; x++)
				{
					UINT32 arrayIdx = x + yArrayIdx + zArrayIdx;
					UINT32 dataIdx = x * pixelSize + yDataIdx + zDataIdx;

					UINT8* dest = data + dataIdx;
					PixelUtil::PackColor(colors[arrayIdx], mFormat, dest);
				}
			}
		}
	}

	template BS_CORE_EXPORT void PixelData::SetColorsInternal(Color* const &, UINT32);
	template BS_CORE_EXPORT void PixelData::SetColorsInternal(const Vector<Color>&, UINT32);

	void PixelData::SetColors(const Vector<Color>& colors)
	{
		SetColorsInternal(colors, (UINT32)colors.size());
	}

	void PixelData::SetColors(Color* colors, UINT32 numElements)
	{
		SetColorsInternal(colors, numElements);
	}

	void PixelData::SetColors(const Color& color)
	{
		UINT32 depth = mExtents.GetDepth();
		UINT32 height = mExtents.GetHeight();
		UINT32 width = mExtents.GetWidth();

		UINT32 pixelSize = PixelUtil::GetNumElemBytes(mFormat);
		UINT32 packedColor[4];
		assert(pixelSize <= sizeof(packedColor));

		PixelUtil::PackColor(color, mFormat, packedColor);

		UINT8* data = GetData();
		for (UINT32 z = 0; z < depth; z++)
		{
			UINT32 zDataIdx = z * mSlicePitch;

			for (UINT32 y = 0; y < height; y++)
			{
				UINT32 yDataIdx = y * mRowPitch;

				for (UINT32 x = 0; x < width; x++)
				{
					UINT32 dataIdx = x * pixelSize + yDataIdx + zDataIdx;

					UINT8* dest = data + dataIdx;
					memcpy(dest, packedColor, pixelSize);
				}
			}
		}
	}

	float PixelData::GetDepthAt(UINT32 x, UINT32 y, UINT32 z) const
	{
		UINT32 pixelSize = PixelUtil::GetNumElemBytes(mFormat);
		UINT32 pixelOffset = z * mSlicePitch + y * mRowPitch + x * pixelSize;
		return PixelUtil::UnpackDepth(mFormat, (unsigned char *)GetData() + pixelOffset);;
	}

	Vector<float> PixelData::GetDepths() const
	{
		UINT32 depth = mExtents.GetDepth();
		UINT32 height = mExtents.GetHeight();
		UINT32 width = mExtents.GetWidth();

		UINT32 pixelSize = PixelUtil::GetNumElemBytes(mFormat);
		UINT8* data = GetData();

		Vector<float> depths(width * height * depth);
		for (UINT32 z = 0; z < depth; z++)
		{
			UINT32 zArrayIdx = z * width * height;
			UINT32 zDataIdx = z * mSlicePitch;

			for (UINT32 y = 0; y < height; y++)
			{
				UINT32 yArrayIdx = y * width;
				UINT32 yDataIdx = y * mRowPitch;

				for (UINT32 x = 0; x < width; x++)
				{
					UINT32 arrayIdx = x + yArrayIdx + zArrayIdx;
					UINT32 dataIdx = x * pixelSize + yDataIdx + zDataIdx;

					UINT8* dest = data + dataIdx;
					depths[arrayIdx] = PixelUtil::UnpackDepth(mFormat, dest);
				}
			}
		}

		return depths;
	}

	SPtr<PixelData> PixelData::Create(const PixelVolume &extents, PixelFormat pixelFormat)
	{
		SPtr<PixelData> pixelData = bs_shared_ptr_new<PixelData>(extents, pixelFormat);
		pixelData->AllocateInternalBuffer();

		return pixelData;
	}

	SPtr<PixelData> PixelData::Create(UINT32 width, UINT32 height, UINT32 depth, PixelFormat pixelFormat)
	{
		SPtr<PixelData> pixelData = bs_shared_ptr_new<PixelData>(width, height, depth, pixelFormat);
		pixelData->AllocateInternalBuffer();

		return pixelData;
	}

	UINT32 PixelData::GetInternalBufferSize() const
	{
		return GetSize();
	}

	/************************************************************************/
	/* 								SERIALIZATION                      		*/
	/************************************************************************/

	RTTITypeBase* PixelData::GetRttiStatic()
	{
		return PixelDataRTTI::Instance();
	}

	RTTITypeBase* PixelData::GetRtti() const
	{
		return PixelData::GetRttiStatic();
	}
}
