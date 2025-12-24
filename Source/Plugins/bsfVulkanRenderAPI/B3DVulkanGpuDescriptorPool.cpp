//************************************ B3D Framework - Copyright 2025 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "B3DVulkanGpuDescriptorPool.h"
#include "B3DVulkanGpuDevice.h"
#include "B3DVulkanGpuParameterSet.h"
#include "B3DVulkanGpuPipelineParameterLayout.h"

namespace b3d::render
{
	VulkanGpuDescriptorPool::VulkanGpuDescriptorPool(VulkanGpuDevice& device, const GpuDescriptorPoolCreateInformation& createInformation)
		: GpuDescriptorPool(createInformation)
		, mDevice(device)
	{
		VkDescriptorPoolSize poolSizes[10];
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		poolSizes[0].descriptorCount = createInformation.MaxSampledImages;

		poolSizes[1].type = VK_DESCRIPTOR_TYPE_SAMPLER;
		poolSizes[1].descriptorCount = createInformation.MaxSamplers;

		poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[2].descriptorCount = createInformation.MaxCombinedImageSamplers;

		poolSizes[3].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[3].descriptorCount = createInformation.MaxUniformBuffers;

		poolSizes[4].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		poolSizes[4].descriptorCount = createInformation.MaxUniformBuffersDynamic;

		poolSizes[5].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		poolSizes[5].descriptorCount = createInformation.MaxStorageImages;

		poolSizes[6].type = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
		poolSizes[6].descriptorCount = createInformation.MaxSampledBuffers;

		poolSizes[7].type = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
		poolSizes[7].descriptorCount = createInformation.MaxStorageBuffers;

		poolSizes[8].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		poolSizes[8].descriptorCount = createInformation.MaxStorageBuffers;

		poolSizes[9].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
		poolSizes[9].descriptorCount = createInformation.MaxStorageBuffersDynamic;

		VkDescriptorPoolCreateInfo poolCreateInformation;
		poolCreateInformation.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolCreateInformation.pNext = nullptr;
		poolCreateInformation.maxSets = createInformation.MaxSets;
		poolCreateInformation.poolSizeCount = sizeof(poolSizes) / sizeof(poolSizes[0]);
		poolCreateInformation.pPoolSizes = poolSizes;

		// Transient mode: no individual free, enables O(1) reset
		// Persistent mode: individual free allowed, no bulk reset
		if (createInformation.Mode == GpuDescriptorPoolMode::Persistent)
			poolCreateInformation.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		else
			poolCreateInformation.flags = 0;

		VkResult result = vkCreateDescriptorPool(mDevice.GetLogical(), &poolCreateInformation, gVulkanAllocator, &mPool);
		B3D_ASSERT(result == VK_SUCCESS);
	}

	VulkanGpuDescriptorPool::~VulkanGpuDescriptorPool()
	{
		if (mPool != VK_NULL_HANDLE)
			vkDestroyDescriptorPool(mDevice.GetLogical(), mPool, gVulkanAllocator);
	}

	SPtr<GpuParameterSet> VulkanGpuDescriptorPool::Allocate(
		const SPtr<GpuPipelineParameterSetLayout>& layout, u32 setIndex)
	{
		if (mAllocatedSetCount >= mInformation.MaxSets)
		{
			B3D_LOG(Error, RenderBackend, "Descriptor pool exhausted. Cannot allocate more parameter sets.");
			return nullptr;
		}

		// Create the parameter set using the standard constructor
		// The parameter set will allocate from the VulkanDescriptorManager's pool for now.
		// TODO: In a future iteration, VulkanGpuParameterSet should accept a pool parameter
		// and allocate directly from this pool instead of the manager's pool.
		auto paramSet = B3DMakeShared<VulkanGpuParameterSet>(mDevice, layout, setIndex);
		paramSet->Initialize();

		mAllocatedSetCount++;
		return paramSet;
	}

	void VulkanGpuDescriptorPool::Reset()
	{
		if(mInformation.Mode == GpuDescriptorPoolMode::Persistent)
		{
			B3D_LOG(Error, RenderBackend, "Cannot perform Reset on a Persistent mode descriptor pool.");
			return;
		}

		VkResult result = vkResetDescriptorPool(mDevice.GetLogical(), mPool, 0);
		B3D_ASSERT(result == VK_SUCCESS);

		mAllocatedSetCount = 0;
	}

	void VulkanGpuDescriptorPool::Free(const SPtr<GpuParameterSet>& parameterSet)
	{
		if(mInformation.Mode == GpuDescriptorPoolMode::Transient)
		{
			B3D_LOG(Error, RenderBackend, "Cannot free individual descriptors in Transient mode descriptor pool.");
			return;
		}

		B3D_ASSERT(parameterSet != nullptr);

		// TODO: The parameter set needs to track which pool it came from and free its
		// descriptor set accordingly. For now, this is a placeholder.
		// In a full implementation, VulkanGpuParameterSet would have a reference to the
		// owning pool and call FreeRawSet() with its VkDescriptorSet.

		if (mAllocatedSetCount > 0)
			mAllocatedSetCount--;
	}

	VkDescriptorSet VulkanGpuDescriptorPool::AllocateVkSet(VkDescriptorSetLayout layout)
	{
		if (mAllocatedSetCount >= mInformation.MaxSets)
			return VK_NULL_HANDLE;

		VkDescriptorSetAllocateInfo allocInfo;
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.descriptorPool = mPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &layout;

		VkDescriptorSet set = VK_NULL_HANDLE;
		VkResult result = vkAllocateDescriptorSets(mDevice.GetLogical(), &allocInfo, &set);

		if (result != VK_SUCCESS)
		{
			B3D_LOG(Error, RenderBackend, "Failed to allocate descriptor set from pool.");
			return VK_NULL_HANDLE;
		}

		mAllocatedSetCount++;
		return set;
	}

	void VulkanGpuDescriptorPool::FreeVkSet(VkDescriptorSet set)
	{
		B3D_ASSERT(mInformation.Mode == GpuDescriptorPoolMode::Persistent);

		if (set == VK_NULL_HANDLE)
			return;

		VkResult result = vkFreeDescriptorSets(mDevice.GetLogical(), mPool, 1, &set);
		B3D_ASSERT(result == VK_SUCCESS);

		if (mAllocatedSetCount > 0)
			mAllocatedSetCount--;
	}
} // namespace b3d::render
