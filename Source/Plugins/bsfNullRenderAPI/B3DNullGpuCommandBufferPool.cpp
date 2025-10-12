//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "B3DNullGpuCommandBufferPool.h"
#include "B3DNullGpuCommandBuffer.h"
#include "B3DNullGpuDevice.h"

namespace b3d
{
	namespace render
	{
		NullGpuCommandBufferPool::NullGpuCommandBufferPool(NullGpuDevice& device, const GpuCommandBufferPoolCreateInformation& createInformation)
			: Base(device, createInformation)
		{ }

		SPtr<GpuCommandBuffer> NullGpuCommandBufferPool::Create(const GpuCommandBufferCreateInformation& createInformation)
		{
			const u32 id = mNextCommandBufferId++;
			SPtr<NullGpuCommandBuffer> commandBuffer = B3DMakeShared<NullGpuCommandBuffer>(
				static_cast<NullGpuDevice&>(mGpuDevice), *this, id, mInformation.Thread, mInformation.Usage, createInformation);

			mCommandBuffers[id] = commandBuffer;
			return commandBuffer;
		}

		SPtr<GpuCommandBuffer> NullGpuCommandBufferPool::FindOrCreate(const GpuCommandBufferCreateInformation& createInformation)
		{
			// For simplicity, always create a new one
			return Create(createInformation);
		}
	} // namespace render
} // namespace b3d
