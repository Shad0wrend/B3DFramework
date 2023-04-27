//************************************ bs::framework - Copyright 2023 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsGpuDevice.h"
#include "BsGpuCommandBuffer.h"
#include "BsEventQuery.h"
#include "BsTimerQuery.h"
#include "BsOcclusionQuery.h"

using namespace bs;

GpuQueue::GpuQueue(GpuDevice& gpuDevice, GpuQueueUsage usage, u32 index)
	:mGpuDevice(gpuDevice), mUsage(usage), mIndex(index)
{
	
}

SPtr<ct::GpuCommandBuffer> GpuQueue::GetOrCreateTransferCommandBuffer()
{
	Lock lock(mMutex);

	PerThreadTransferCommandBufferInformation& transferCommandBufferInformation = mTransferCommandBuffers[B3D_CURRENT_THREAD_ID];
	if(transferCommandBufferInformation.CommandBufferPool == nullptr)
	{
		ct::GpuCommandBufferPoolCreateInformation poolCreateInformation;
		poolCreateInformation.Thread = B3D_CURRENT_THREAD_ID;
		poolCreateInformation.Usage = mUsage;

		transferCommandBufferInformation.CommandBufferPool = mGpuDevice.CreateGpuCommandBufferPool(poolCreateInformation);
	}

	if(transferCommandBufferInformation.CurrentTransferCommandBuffer == nullptr)
	{
		ct::GpuCommandBufferCreateInformation commandBufferCreateInformation;
		commandBufferCreateInformation.Name = "Transfer";

		transferCommandBufferInformation.CurrentTransferCommandBuffer = transferCommandBufferInformation.CommandBufferPool->Create(commandBufferCreateInformation);
	}

	return transferCommandBufferInformation.CurrentTransferCommandBuffer;
}

void GpuDevice::SubmitCommandBuffer(const SPtr<ct::GpuCommandBuffer>& commandBuffer, u32 syncMask, u32 queueIndex)
{
	if (!B3D_ENSURE(commandBuffer))
		return;

	const u32 queueCount = GetQueueCount(commandBuffer->GetUsage());
	if (!B3D_ENSURE(queueIndex < queueCount))
		return;

	const SPtr<GpuQueue>& queue = GetQueue(commandBuffer->GetUsage(), queueIndex);
	if (!B3D_ENSURE(queue))
		return;

	queue->SubmitCommandBuffer(commandBuffer, syncMask);
}

void GpuQueue::SubmitCommandBuffer(const SPtr<ct::GpuCommandBuffer>& commandBuffer, u32 syncMask)
{
	SubmitCommandBuffer(commandBuffer, syncMask, true);
}

void GpuQueue::SubmitTransferCommandBuffer(bool wait)
{
	SPtr<ct::GpuCommandBuffer> commandBufferToSubmit;

	{
		Lock lock(mMutex);

		if(auto found = mTransferCommandBuffers.find(B3D_CURRENT_THREAD_ID); found != mTransferCommandBuffers.end())
		{
			commandBufferToSubmit = found->second.CurrentTransferCommandBuffer;
			found->second.CurrentTransferCommandBuffer = nullptr;
		}
	}

	if (commandBufferToSubmit != nullptr)
	{
		commandBufferToSubmit->End();
		SubmitCommandBuffer(commandBufferToSubmit, 0xFFFFFFFF, false);
	}

	if (wait)
		WaitUntilIdle();
}

