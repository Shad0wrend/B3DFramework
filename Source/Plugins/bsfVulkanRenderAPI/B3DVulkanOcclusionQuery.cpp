//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "B3DVulkanOcclusionQuery.h"
#include "B3DVulkanGpuDevice.h"
#include "Managers/B3DVulkanQueryManager.h"
#include "B3DVulkanGpuCommandBuffer.h"
#include "Profiling/B3DRenderStats.h"

using namespace b3d;
using namespace b3d::render;

VulkanOcclusionQuery::VulkanOcclusionQuery(VulkanGpuDevice& device, bool binary)
	: OcclusionQuery(binary), mDevice(device), mQueryEndCalled(false), mQueryFinalized(false)
{
	B3D_INCREMENT_RENDER_STATISTIC_CATEGORY(ResCreated, RenderStatObject_Query);
}

VulkanOcclusionQuery::~VulkanOcclusionQuery()
{
	if(mInternalQuery != nullptr)
		mDevice.GetQueryPool().ReleaseQuery(*mInternalQuery);

	mInternalQuery = nullptr;

	B3D_INCREMENT_RENDER_STATISTIC_CATEGORY(ResDestroyed, RenderStatObject_Query);
}

void VulkanOcclusionQuery::Begin(GpuCommandBuffer& commandBuffer)
{
	VulkanQueryPool& queryPool = mDevice.GetQueryPool();

	// Clear any existing queries
	if(mInternalQuery != nullptr)
		mDevice.GetQueryPool().ReleaseQuery(*mInternalQuery);

	mInternalQuery = nullptr;
	mQueryEndCalled = false;
	mSampleCount = 0;

	// Retrieve and queue new query
	VulkanGpuCommandBuffer& vulkanCommandBuffer = static_cast<VulkanGpuCommandBuffer&>(commandBuffer);
	mInternalQuery = queryPool.BeginOcclusionQuery(vulkanCommandBuffer, !mBinary);
}

void VulkanOcclusionQuery::End(GpuCommandBuffer& commandBuffer)
{
	if(mInternalQuery == nullptr)
	{
		B3D_LOG(Error, RenderBackend, "End() called but query was never started.");
		return;
	}

	// Could have been interrupted
	if(mQueryEndCalled)
		return;

	mQueryEndCalled = true;
	mQueryFinalized = false;

	VulkanGpuCommandBuffer& vulkanCommandBuffer = static_cast<VulkanGpuCommandBuffer&>(commandBuffer);

	VulkanQueryPool& queryPool = mDevice.GetQueryPool();
	queryPool.EndOcclusionQuery(vulkanCommandBuffer, *mInternalQuery);
}

bool VulkanOcclusionQuery::IsInProgress() const
{
	return mInternalQuery != nullptr && !mQueryEndCalled;
}

bool VulkanOcclusionQuery::IsReady() const
{
	if(!mQueryEndCalled)
		return false;

	if(mQueryFinalized)
		return true;

	u64 sampleCount;
	bool ready = true;

	if(mInternalQuery != nullptr)
		ready &= !mInternalQuery->IsBound() && mInternalQuery->GetResult(sampleCount);

	return ready;
}

u32 VulkanOcclusionQuery::GetSampleCount()
{
	if(!mQueryFinalized)
	{
		u64 totalSampleCount = 0;
		bool ready = true;

		if(mInternalQuery != nullptr)
		{
			u64 sampleCount = 0;
			ready &= !mInternalQuery->IsBound() && mInternalQuery->GetResult(sampleCount);

			totalSampleCount += sampleCount;
		}

		if(ready)
		{
			mQueryFinalized = true;
			mSampleCount = totalSampleCount;

			VulkanQueryPool& queryPool = mDevice.GetQueryPool();
			if(mInternalQuery != nullptr)
				queryPool.ReleaseQuery(*mInternalQuery);

			mInternalQuery = nullptr;
		}
	}

	if(mBinary)
		return mSampleCount == 0 ? 0 : 1;

	return (u32)mSampleCount;
}
