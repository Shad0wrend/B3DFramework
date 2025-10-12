//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "B3DNullGpuPipelineState.h"
#include "B3DNullGpuDevice.h"

namespace b3d
{
	namespace render
	{
		NullGpuGraphicsPipelineState::NullGpuGraphicsPipelineState(NullGpuDevice& gpuDevice, const GpuGraphicsPipelineStateCreateInformation& createInformation)
			: GpuGraphicsPipelineState(gpuDevice, createInformation)
			, mGpuDevice(gpuDevice)
		{ }

		NullGpuComputePipelineState::NullGpuComputePipelineState(NullGpuDevice& gpuDevice, const GpuComputePipelineStateCreateInformation& createInformation)
			: GpuComputePipelineState(gpuDevice, createInformation)
			, mGpuDevice(gpuDevice)
		{ }
	} // namespace render
} // namespace b3d
