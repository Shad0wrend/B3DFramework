//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DVulkanPrerequisites.h"
#include "RenderAPI/B3DOcclusionQuery.h"

namespace b3d
{
	namespace render
	{
		/** @addtogroup Vulkan
		 *  @{
		 */

		/** @copydoc OcclusionQuery */
		class VulkanOcclusionQuery : public OcclusionQuery
		{
		public:
			VulkanOcclusionQuery(VulkanGpuDevice& device, bool binary);
			~VulkanOcclusionQuery() override;

			void Begin(GpuCommandBuffer& commandBuffer) override;
			void End(GpuCommandBuffer& commandBuffer) override;
			bool IsReady() const override;
			u32 GetSampleCount() override;

			/** Returns true if the query begin() was called, but not end(). */
			bool IsInProgress() const;

		private:
			VulkanGpuDevice& mDevice;
			VulkanQuery* mInternalQuery = nullptr;

			u64 mSampleCount = 0;
			bool mQueryEndCalled : 1;
			bool mQueryFinalized : 1;
		};

		/** @} */
	} // namespace render
} // namespace b3d
