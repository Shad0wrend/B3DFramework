//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsVulkanIndexBuffer.h"
#include "BsVulkanHardwareBuffer.h"
#include "Profiling/BsRenderStats.h"

using namespace bs;
using namespace bs::ct;

static void DeleteHardwareBuffer(HardwareBuffer* buffer)
{
	B3DPoolDelete(static_cast<VulkanHardwareBuffer*>(buffer));
}

VulkanIndexBuffer::VulkanIndexBuffer(const IndexBufferCreateInformation& desc, GpuDeviceFlags deviceMask)
	: IndexBuffer(desc, deviceMask), mDeviceMask(deviceMask)
{}

void VulkanIndexBuffer::Initialize()
{
	mBuffer = B3DPoolNew<VulkanHardwareBuffer>(HardwareBufferType::Index, mBufferFlags, mSize, mDeviceMask);
	mBufferDeleter = &DeleteHardwareBuffer;

	IndexBuffer::Initialize();
}

VulkanBuffer* VulkanIndexBuffer::GetResource(u32 deviceIdx) const
{
	return static_cast<VulkanHardwareBuffer*>(mBuffer)->GetResource(deviceIdx);
}
