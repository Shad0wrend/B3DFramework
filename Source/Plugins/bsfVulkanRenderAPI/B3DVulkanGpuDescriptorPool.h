//************************************ B3D Framework - Copyright 2025 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DVulkanPrerequisites.h"
#include "RenderAPI/B3DGpuDescriptorPool.h"

namespace b3d
{
	namespace render
	{
		/** @addtogroup Vulkan
		 *  @{
		 */

		/** Vulkan implementation of GpuDescriptorPool. */
		class VulkanGpuDescriptorPool final : public GpuDescriptorPool
		{
		public:
			VulkanGpuDescriptorPool(VulkanGpuDevice& device, const GpuDescriptorPoolCreateInformation& createInformation);
			~VulkanGpuDescriptorPool() override;

			SPtr<GpuParameterSet> Allocate(const SPtr<GpuPipelineParameterSetLayout>& layout, u32 setIndex) override;
			void Free(const SPtr<GpuParameterSet>& parameterSet) override;
			void Reset() override;

			/** Returns the underlying Vulkan descriptor pool handle. */
			VkDescriptorPool GetVulkanHandle() const { return mPool; }

		private:
			/**
			 * Allocates a native Vulkan descriptor set from the pool.
			 *
			 * @param layout	The descriptor set layout.
			 * @return			The allocated VkDescriptorSet, or VK_NULL_HANDLE on failure.
			 */
			VkDescriptorSet AllocateVkSet(VkDescriptorSetLayout layout);

			/**
			 * Frees a native Vulkan descriptor set.
			 *
			 * @param set	The descriptor set to free.
			 */
			void FreeVkSet(VkDescriptorSet set);

			VulkanGpuDevice& mDevice;
			VkDescriptorPool mPool = VK_NULL_HANDLE;
		};

		/** @} */
	} // namespace render
} // namespace b3d
