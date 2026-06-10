//************************************* B3D Framework - Copyright 2026 Marko Pintera *************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "B3DGpuWorkContext.h"
#include "B3DGpuDevice.h"
#include "B3DGpuCommandBuffer.h"
#include "B3DGpuCommandBufferPoolRing.h"
#include "GpuBackend/Allocators/B3DGpuResource.h"

using namespace b3d;

namespace
{
	constexpr GpuQueueType kTransferQueueType = GQT_GRAPHICS;
	constexpr u32 kTransferQueueIndex = 0;
}

GpuWorkContext::GpuWorkContext(PrivatelyConstruct, GpuDevice& device)
	: mDevice(device), mTracker(nullptr), mOwnedTracker(B3DMakeUnique<GpuFenceCompletionTracker>(device.CreateTimelineFence()))
{
	mTracker = mOwnedTracker.get();
}

GpuWorkContext::GpuWorkContext(PrivatelyConstruct, GpuDevice& device, IGpuCompletionTracker& tracker)
	: mDevice(device), mTracker(&tracker)
{
}

TShared<GpuWorkContext> GpuWorkContext::Create(GpuDevice& device)
{
	return B3DMakeShared<GpuWorkContext>(PrivatelyConstruct(), device);
}

TShared<GpuWorkContext> GpuWorkContext::Create(GpuDevice& device, IGpuCompletionTracker& tracker)
{
	return B3DMakeShared<GpuWorkContext>(PrivatelyConstruct(), device, tracker);
}

IGpuAllocator& GpuWorkContext::GetOrCreateTransientAllocator(u32 memoryType)
{
	TUnique<IGpuAllocator>& slot = mTransientAllocators[memoryType];
	if (slot == nullptr)
		slot = mDevice.CreateTransientAllocator(memoryType, *mTracker);

	B3D_ASSERT(slot != nullptr && "Backend does not support context transient allocation.");
	return *slot;
}


GpuWorkContext::~GpuWorkContext()
{
	if (!mTransferPoolRing)
		return;

	if (mTransferCommandBuffer != nullptr)
	{
		mTransferCommandBuffer->End();
		mTransferCommandBuffer = nullptr;
	}

	mTransferPoolRing->Destroy();
	mTransferPoolRing = nullptr;
}

const TShared<render::GpuCommandBuffer>& GpuWorkContext::GetTransferCommandBuffer()
{
	if (!mTransferPoolRing)
	{
		render::GpuCommandBufferPoolCreateInformation poolCreateInformation;
		poolCreateInformation.Thread = B3D_CURRENT_THREAD_ID;
		poolCreateInformation.Type = kTransferQueueType;
		poolCreateInformation.UsePoolReset = true;

		mTransferPoolRing = B3DMakeUnique<render::GpuCommandBufferPoolRing>(mDevice, poolCreateInformation);
	}

	if (mTransferCommandBuffer == nullptr)
	{
		render::GpuCommandBufferCreateInformation commandBufferCreateInformation;
		commandBufferCreateInformation.Name = "Transfer";

		mTransferCommandBuffer = mTransferPoolRing->GetCurrentPool().FindOrCreate(commandBufferCreateInformation);
	}

	return mTransferCommandBuffer;
}

void GpuWorkContext::SubmitCommandBuffer(const GpuSubmissionInformation& information, u32 queueIndex)
{
	// NOTE: Transfer and work are submitted to the same queue (graphics 0), so same-queue submission
	//		 order preserves the transfer-before-work dependency with no explicit semaphore. If the work
	//		 command buffer ever targets a different queue, the caller must add an explicit sync mask for
	//		 the transfer queue - same-queue ordering no longer guarantees transfer visibility.

	// Flush this context's pending transfers first (non-blocking) so they are visible to the work below.
	SubmitTransferCommandBuffers(false);

	if (!B3D_ENSURE(information.CommandBuffer))
		return;

	const GpuQueueType queueType = information.CommandBuffer->GetQueueType();
	const u32 queueCount = mDevice.GetQueueCount(queueType);
	if (!B3D_ENSURE(queueIndex < queueCount))
		return;

	const TShared<GpuQueue> queue = mDevice.GetQueue(queueType, queueIndex);
	if (!B3D_ENSURE(queue))
		return;

	queue->SubmitCommandBuffer(information);
}

void GpuWorkContext::SubmitCommandBuffer(const TShared<render::GpuCommandBuffer>& commandBuffer, GpuQueueMask syncMask, u32 queueIndex)
{
	GpuSubmissionInformation information;
	information.CommandBuffer = commandBuffer;
	information.SyncMask = syncMask;

	SubmitCommandBuffer(information, queueIndex);
}

void GpuWorkContext::SubmitTransferCommandBuffers(bool wait)
{
	TShared<render::GpuCommandBuffer> commandBufferToSubmit = mTransferCommandBuffer;
	mTransferCommandBuffer = nullptr;

	if (commandBufferToSubmit == nullptr && !wait)
		return;

	TShared<GpuQueue> queue = mDevice.GetQueue(kTransferQueueType, kTransferQueueIndex);
	if (queue == nullptr)
		return;

	if (commandBufferToSubmit != nullptr)
	{
		commandBufferToSubmit->End();

		GpuSubmissionInformation submissionInfo;
		submissionInfo.CommandBuffer = commandBufferToSubmit;
		submissionInfo.SyncMask = GpuQueueMask::kAll;
		queue->SubmitCommandBuffer(submissionInfo);
	}

	if (wait)
		queue->WaitUntilIdle();
}

void GpuWorkContext::AdvanceFrame()
{
	if (mTransferPoolRing)
		mTransferPoolRing->AdvanceFrame();

	mTransferCommandBuffer = nullptr;

	// Reclaim transient memory at the frame boundary: retire each linear allocator's open page, then
	// drain any pages whose completion marker has signaled. No-op for backends with no transient
	// allocators (the map stays empty).
	for (auto& entry : mTransientAllocators)
	{
		if (entry.second == nullptr)
			continue;

		entry.second->FreeAll();
		entry.second->ReclaimUnused(false);
	}
}
