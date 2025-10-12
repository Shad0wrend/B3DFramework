//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "B3DNullGpuBuffer.h"
#include "B3DNullGpuDevice.h"

namespace b3d
{
	namespace render
	{
		NullGpuBuffer::NullGpuBuffer(NullGpuDevice& device, const GpuBufferCreateInformation& createInformation)
			: GpuBuffer(device, createInformation, b3d::GpuBuffer::CalculateSuballocatedBufferSize(createInformation, device))
		{
			// Allocate a dummy staging buffer for map/unmap operations
			mStagingBuffer = B3DAllocate(mTotalSize);
		}

		NullGpuBuffer::~NullGpuBuffer()
		{
			if (mStagingBuffer)
				B3DFree(mStagingBuffer);
		}

		void* NullGpuBuffer::Map(u32 offset, u32 length, GpuLockOptions options)
		{
			if (mStagingBuffer)
				return static_cast<u8*>(mStagingBuffer) + offset;

			return nullptr;
		}
	} // namespace render
} // namespace b3d
