//************************************ B3D Framework - Copyright 2025 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "B3DRenderableUniformBufferManager.h"
#include "B3DRendererObject.h"
#include "B3DRendererRenderable.h"
#include "B3DRendererDecal.h"
#include "RenderAPI/B3DGpuBackend.h"
#include "RenderAPI/B3DGpuDevice.h"

using namespace b3d;
using namespace b3d::render;

void RenderableUniformBufferManager::Initialize(
	GpuDevice& device,
	const SPtr<GpuPipelineParameterSetLayout>& renderableParameterSetLayout,
	const SPtr<GpuPipelineParameterSetLayout>& decalParameterSetLayout)
{
	mDevice = &device;
	mRenderableParameterSetLayout = renderableParameterSetLayout;
	mDecalParameterSetLayout = decalParameterSetLayout;

	const u32 perObjectSize = gPerObjectUniformDefinition.GetSize();
	GpuBufferCreateInformation perObjectCreateInfo = GpuBufferCreateInformation::CreateUniform(perObjectSize, GpuBufferFlag::StoreOnGPU);

	mRenderablePool.Initialize(device, perObjectCreateInfo, kRenderableEntriesPerBuffer, 1);

	const u32 decalSize = gDecalParamDef.GetSize();
	GpuBufferCreateInformation decalCreateInfo = GpuBufferCreateInformation::CreateUniform(decalSize, GpuBufferFlag::StoreOnGPU);

	mDecalPool.Initialize(device, decalCreateInfo, kDecalEntriesPerBuffer, 1);

	GpuBufferCreateInformation stagingCreateInfo;
	stagingCreateInfo.Type = GpuBufferType::StagingWrite;
	stagingCreateInfo.Staging.Size = perObjectSize;
	stagingCreateInfo.Flags = GpuBufferFlag::AllowWriteCachingOnCPU; // TODO - Only while GpuUniformBuffer doesn't support non-cached writes

	mStagingPool.Initialize(device, stagingCreateInfo, kStagingEntriesPerBuffer, 1);
}

RenderableUniformBufferManager::AllocationResult RenderableUniformBufferManager::Allocate(AllocationType type)
{
	AllocationResult result;

	// Allocate or reuse an entry index
	u32 entryIndex;
	if(mFreeListHead != ~0u)
	{
		entryIndex = mFreeListHead;
		mFreeListHead = mAllocationEntries[entryIndex].NextFreeIndex;
	}
	else
	{
		entryIndex = (u32)mAllocationEntries.size();
		mAllocationEntries.push_back({});
	}

	AllocationEntry& entry = mAllocationEntries[entryIndex];
	entry.Type = type;
	entry.NextFreeIndex = ~0u;

	// Allocate per-object buffer (always)
	entry.PerObjectSuballocation = mRenderablePool.Allocate();
	result.PerObjectSuballocation = entry.PerObjectSuballocation;

	// Allocate decal buffer if requested
	const bool isDecal = type == AllocationType::Decal;
	if(isDecal)
	{
		entry.DecalSuballocation = mDecalPool.Allocate();
		result.Extras = DecalExtraSuballocation{ entry.DecalSuballocation };
	}
	else
	{
		entry.DecalSuballocation = GpuBufferSuballocation();
		result.Extras = std::monostate{};
	}

	// Get or create shared parameter set
	SPtr<GpuBuffer> decalBuffer = isDecal ? entry.DecalSuballocation.GetBuffer() : nullptr;
	result.ParameterSet = GetOrCreateParameterSet(entry.PerObjectSuballocation.GetBuffer(), decalBuffer);

	// Set up handle
	result.Handle.Index = entryIndex;
	result.Handle.Type = type;

	return result;
}

void RenderableUniformBufferManager::Release(AllocationHandle handle)
{
	if(!handle.IsValid())
		return;

	B3D_ASSERT(handle.Index < mAllocationEntries.size());

	AllocationEntry& entry = mAllocationEntries[handle.Index];

	if(!entry.PerObjectSuballocation.IsValid())
		return;

	// Release parameter set
	SPtr<GpuBuffer> decalBuffer = entry.DecalSuballocation.IsValid() ? entry.DecalSuballocation.GetBuffer() : nullptr;
	ReleaseParameterSet(entry.PerObjectSuballocation.GetBuffer(), decalBuffer);

	// Release pool allocations
	mRenderablePool.Release(entry.PerObjectSuballocation);

	if(entry.DecalSuballocation.IsValid())
		mDecalPool.Release(entry.DecalSuballocation);

	// Clear entry and add to free-list
	entry.PerObjectSuballocation = GpuBufferSuballocation();
	entry.DecalSuballocation = GpuBufferSuballocation();
	entry.Type = AllocationType::Normal;
	entry.NextFreeIndex = mFreeListHead;
	mFreeListHead = handle.Index;
}

SPtr<render::GpuParameterSet> RenderableUniformBufferManager::GetOrCreateParameterSet(const SPtr<GpuBuffer>& perObjectBuffer, const SPtr<GpuBuffer>& decalBuffer)
{
	BufferKey key = { perObjectBuffer.get(), decalBuffer ? decalBuffer.get() : nullptr };

	auto iter = mParameterSetsByBuffer.find(key);
	if(iter != mParameterSetsByBuffer.end())
	{
		iter->second.RefCount++;
		return iter->second.ParameterSet;
	}

	const bool isDecal = decalBuffer != nullptr;
	const SPtr<GpuPipelineParameterSetLayout>& layout = isDecal ? mDecalParameterSetLayout : mRenderableParameterSetLayout;

	SPtr<GpuParameterSet> parameterSet = mDevice->CreateGpuParameterSet(layout, GpuPipelineSet::kPerObject);

	parameterSet->SetUniformBuffer("PerObject", perObjectBuffer, 0);

	if(isDecal)
		parameterSet->SetUniformBuffer("DecalParams", decalBuffer, 0);

	BufferParameterSetEntry entry;
	entry.ParameterSet = parameterSet;
	entry.RefCount = 1;

	mParameterSetsByBuffer[key] = entry;

	return parameterSet;
}

void RenderableUniformBufferManager::ReleaseParameterSet(const SPtr<GpuBuffer>& perObjectBuffer, const SPtr<GpuBuffer>& decalBuffer)
{
	BufferKey key = { perObjectBuffer.get(), decalBuffer ? decalBuffer.get() : nullptr };

	auto iter = mParameterSetsByBuffer.find(key);
	if(iter == mParameterSetsByBuffer.end())
		return;

	iter->second.RefCount--;
	if(iter->second.RefCount == 0)
		mParameterSetsByBuffer.erase(iter);
}

void RenderableUniformBufferManager::UpdatePerObjectBuffer(const RendererObject& object, const SPtr<GpuCommandBuffer>& commandBuffer)
{
	if(!object.PerObjectSuballocation.IsValid())
		return;

	GpuBufferSuballocation staging = mStagingPool.Allocate();

	gPerObjectUniformDefinition.gMatWorld.Set(staging, object.WorldTransform);
	gPerObjectUniformDefinition.gMatInvWorld.Set(staging, object.WorldTransform.InverseAffine());
	gPerObjectUniformDefinition.gMatWorldNoScale.Set(staging, object.WorldNoScale);
	gPerObjectUniformDefinition.gMatInvWorldNoScale.Set(staging, object.WorldNoScale.InverseAffine());
	gPerObjectUniformDefinition.gMatPrevWorld.Set(staging, object.PrevWorldTransform);
	gPerObjectUniformDefinition.gWorldDeterminantSign.Set(staging, object.WorldTransform.Determinant3x3() >= 0.0f ? 1.0f : -1.0f);
	gPerObjectUniformDefinition.gLayer.Set(staging, (i32)object.Layer);

	staging.GetBuffer()->FlushCache(staging.GetSuballocationIndex());

	const SPtr<GpuCommandBuffer>& actualCommandBuffer = commandBuffer ? commandBuffer : mDevice->GetQueue(GQT_GRAPHICS, 0)->GetOrCreateTransferCommandBuffer();

	const GpuBufferSuballocation& destination = object.PerObjectSuballocation;
	actualCommandBuffer->CopyBufferToBuffer(staging.GetBuffer(), destination.GetBuffer(), staging.GetSuballocationOffset(), destination.GetSuballocationOffset(), staging.GetSize());
}

void RenderableUniformBufferManager::AdvanceFrame()
{
	mStagingPool.AdvanceFrame();
}
