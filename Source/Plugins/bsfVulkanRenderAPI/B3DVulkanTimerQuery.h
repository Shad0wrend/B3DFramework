//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DVulkanPrerequisites.h"
#include "RenderAPI/B3DTimerQuery.h"

namespace b3d
{
	namespace render
	{
		/** @addtogroup Vulkan
		 *  @{
		 */

		/** @copydoc TimerQuery */
		class VulkanTimerQuery : public TimerQuery
		{
		public:
			VulkanTimerQuery(VulkanGpuDevice& device);
			~VulkanTimerQuery() override;

			void Begin(GpuCommandBuffer& commandBuffer) override;
			void End(GpuCommandBuffer& commandBuffer) override;
			bool IsReady() const override;
			float GetTimeMs() override;

			/** Returns true if the query Begin() was called, but not End(). */
			bool IsInProgress() const;

		private:
			VulkanGpuDevice& mDevice;

			VulkanQuery* mBeginInternalQuery = nullptr;
			VulkanQuery* mEndInternalQuery = nullptr;

			float mTimeDelta = 0.0f;
			bool mQueryFinalized : 1;
		};

		/** @} */
	} // namespace render
} // namespace b3d
