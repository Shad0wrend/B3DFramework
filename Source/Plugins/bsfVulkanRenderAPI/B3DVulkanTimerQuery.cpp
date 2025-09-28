//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "B3DVulkanTimerQuery.h"
#include "B3DVulkanGpuDevice.h"
#include "Managers/B3DVulkanQueryManager.h"
#include "B3DVulkanGpuCommandBuffer.h"
#include "Profiling/B3DRenderStats.h"

using namespace b3d;
using namespace b3d::render;

VulkanTimerQuery::VulkanTimerQuery(VulkanGpuDevice& device)
	: mDevice(device), mQueryFinalized(false)
{
	B3D_INCREMENT_RENDER_STATISTIC_CATEGORY(ResCreated, RenderStatObject_Query);
}

VulkanTimerQuery::~VulkanTimerQuery()
{
	if(mBeginInternalQuery != nullptr)
		mDevice.GetQueryPool().ReleaseQuery(*mBeginInternalQuery);

	if(mEndInternalQuery != nullptr)
		mDevice.GetQueryPool().ReleaseQuery(*mEndInternalQuery);

	mBeginInternalQuery = nullptr;
	mEndInternalQuery = nullptr;

	B3D_INCREMENT_RENDER_STATISTIC_CATEGORY(ResDestroyed, RenderStatObject_Query);
}

void VulkanTimerQuery::Begin(GpuCommandBuffer& commandBuffer)
{
	VulkanQueryPool& queryPool = mDevice.GetQueryPool();

	// Clear any existing queries
	if(mBeginInternalQuery != nullptr)
		queryPool.ReleaseQuery(*mBeginInternalQuery);

	if(mEndInternalQuery != nullptr)
		queryPool.ReleaseQuery(*mEndInternalQuery);

	mBeginInternalQuery = nullptr;
	mEndInternalQuery = nullptr;

	mTimeDelta = 0.0f;

	// Retrieve and queue new query
	VulkanGpuCommandBuffer& vulkanCommandBuffer = static_cast<VulkanGpuCommandBuffer&>(commandBuffer);
	mBeginInternalQuery = queryPool.BeginTimerQuery(vulkanCommandBuffer);
}

void VulkanTimerQuery::End(GpuCommandBuffer& commandBuffer)
{
	if(!B3D_ENSURE(mBeginInternalQuery != nullptr))
		return;

	mQueryFinalized = false;

	VulkanGpuCommandBuffer& vulkanCommandBuffer = static_cast<VulkanGpuCommandBuffer&>(commandBuffer);

	VulkanQueryPool& queryPool = mDevice.GetQueryPool();
	mEndInternalQuery = queryPool.BeginTimerQuery(vulkanCommandBuffer);
}

bool VulkanTimerQuery::IsInProgress() const
{
	return mBeginInternalQuery != nullptr && mEndInternalQuery != nullptr;
}

bool VulkanTimerQuery::IsReady() const
{
	if(mBeginInternalQuery == nullptr || mEndInternalQuery == nullptr)
		return false;

	if(mQueryFinalized)
		return true;

	u64 timeBegin, timeEnd;
	bool ready = true;
	ready &= !mBeginInternalQuery->IsBound() && mBeginInternalQuery->GetResult(timeBegin);
	ready &= !mEndInternalQuery->IsBound() && mEndInternalQuery->GetResult(timeEnd);

	return ready;
}

float VulkanTimerQuery::GetTimeMs()
{
	if(!mQueryFinalized)
	{
		u64 totalTimeDiff = 0;
		bool ready = true;

		if(mBeginInternalQuery != nullptr && mEndInternalQuery != nullptr)
		{
			u64 timeBegin = 0;
			u64 timeEnd = 0;
			ready &= !mBeginInternalQuery->IsBound() && mBeginInternalQuery->GetResult(timeBegin);
			ready &= !mEndInternalQuery->IsBound() && mEndInternalQuery->GetResult(timeEnd);

			totalTimeDiff += (timeEnd - timeBegin);
		}

		if(ready)
		{
			mQueryFinalized = true;

			double timestampToMs = (double)mDevice.GetDeviceProperties().limits.timestampPeriod / 1e6; // Nano to milli
			mTimeDelta = (float)((double)totalTimeDiff * timestampToMs);

			VulkanQueryPool& queryPool = mDevice.GetQueryPool();
			if(mBeginInternalQuery != nullptr)
				queryPool.ReleaseQuery(*mBeginInternalQuery);

			if(mEndInternalQuery != nullptr)
				queryPool.ReleaseQuery(*mEndInternalQuery);

			mBeginInternalQuery = nullptr;
			mEndInternalQuery = nullptr;
		}
	}

	return mTimeDelta;
}
