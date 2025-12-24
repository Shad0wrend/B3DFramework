//************************************ B3D Framework - Copyright 2025 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DNullPrerequisites.h"
#include "RenderAPI/B3DGpuParameterSetPool.h"

namespace b3d
{
	namespace render
	{
		class NullGpuDevice;

		/** @addtogroup Null
		 *  @{
		 */

		/** Null implementation of GpuParameterSetPool. */
		class NullGpuParameterSetPool final : public GpuParameterSetPool
		{
		public:
			NullGpuParameterSetPool(NullGpuDevice& device, const GpuParameterSetPoolCreateInformation& createInformation);
			~NullGpuParameterSetPool() override = default;

			SPtr<GpuParameterSet> Create(const SPtr<GpuPipelineParameterSetLayout>& layout, u32 setIndex, bool deferredInitialize = false) override;
			void Free(const SPtr<GpuParameterSet>& parameterSet) override;
			void Reset() override;

		private:
			NullGpuDevice& mDevice;
		};

		/** @} */
	} // namespace render
} // namespace b3d
