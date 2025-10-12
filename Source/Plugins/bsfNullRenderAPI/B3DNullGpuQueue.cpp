//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "B3DNullGpuQueue.h"
#include "B3DNullGpuDevice.h"

namespace b3d
{
	namespace render
	{
		NullGpuQueue::NullGpuQueue(NullGpuDevice& device, GpuQueueUsage usage, u32 index)
			: GpuQueue(device, usage, index)
		{ }
	} // namespace render
} // namespace b3d
