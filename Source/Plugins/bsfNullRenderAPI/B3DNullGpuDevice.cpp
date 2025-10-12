//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "B3DNullGpuDevice.h"
#include "B3DNullGpuQueue.h"
#include "B3DNullGpuCommandBuffer.h"
#include "B3DNullGpuCommandBufferPool.h"
#include "B3DNullGpuBuffer.h"
#include "B3DNullTexture.h"
#include "B3DNullGpuProgram.h"
#include "B3DNullGpuPipelineState.h"
#include "B3DNullGpuParameters.h"
#include "B3DNullGpuPipelineParameterLayout.h"
#include "B3DNullSamplerState.h"
#include "B3DNullEventQuery.h"
#include "B3DNullGpuQueryPool.h"
#include "RenderAPI/B3DVideoModeInfo.h"
#include "Math/B3DMatrix4.h"

namespace b3d
{
	namespace render
	{
		NullGpuDevice::NullGpuDevice()
		{
			mVideoModeInfo = B3DMakeShared<VideoModeInfo>();
		}

		NullGpuDevice::~NullGpuDevice() = default;

		bool NullGpuDevice::Initialize()
		{
			if (mIsInitialized)
				return true;

			// Create a single queue for each type
			for (u32 i = 0; i < GQT_COUNT; i++)
			{
				mQueueInfos[i].FamilyIndex = i;
				mQueueInfos[i].Queues.Add(B3DMakeShared<NullGpuQueue>(*this, (GpuQueueUsage)i, 0));
			}

			mIsInitialized = true;
			return true;
		}

		SPtr<GpuProgramBytecode> NullGpuDevice::CompileGpuProgramBytecode(const GpuProgramCreateInformation& createInformation) const
		{
			return B3DMakeShared<GpuProgramBytecode>();
		}

		u32 NullGpuDevice::GetQueueCount(GpuQueueUsage usage) const
		{
			return (u32)mQueueInfos[(u32)usage].Queues.size();
		}

		SPtr<GpuQueue> NullGpuDevice::GetQueue(GpuQueueUsage usage, u32 index) const
		{
			if (index < mQueueInfos[(u32)usage].Queues.size())
				return mQueueInfos[(u32)usage].Queues[index];

			return nullptr;
		}

		SPtr<render::GpuCommandBufferPool> NullGpuDevice::CreateGpuCommandBufferPool(const render::GpuCommandBufferPoolCreateInformation& createInformation)
		{
			return B3DMakeSharedFromExisting(new(B3DAllocate<NullGpuCommandBufferPool>()) NullGpuCommandBufferPool(*this, createInformation));
		}

		SPtr<Texture> NullGpuDevice::CreateTexture(const TextureCreateInformation& createInformation, bool deferredInitialize)
		{
			SPtr<NullTexture> texture = B3DMakeShared<NullTexture>(*this, createInformation);

			if (!deferredInitialize)
				texture->Initialize();

			return texture;
		}

		SPtr<GpuBuffer> NullGpuDevice::CreateGpuBuffer(const GpuBufferCreateInformation& createInformation, bool deferredInitialize)
		{
			SPtr<NullGpuBuffer> buffer = B3DMakeShared<NullGpuBuffer>(*this, createInformation);

			if (!deferredInitialize)
				buffer->Initialize();

			return buffer;
		}

		SPtr<GpuQueryPool> NullGpuDevice::CreateQueryPool(const GpuQueryPoolCreateInformation& createInformation)
		{
			return B3DMakeShared<NullGpuQueryPool>(*this, createInformation);
		}

		SPtr<EventQuery> NullGpuDevice::CreateEventQuery()
		{
			return B3DMakeShared<NullEventQuery>(*this);
		}

		SPtr<GpuProgram> NullGpuDevice::CreateGpuProgram(const GpuProgramCreateInformation& createInformation, bool deferredInitialize)
		{
			SPtr<NullGpuProgram> program = B3DMakeShared<NullGpuProgram>(*this, createInformation);

			if (!deferredInitialize)
				program->Initialize();

			return program;
		}

		SPtr<GpuParameters> NullGpuDevice::CreateGpuParameters(const SPtr<GpuPipelineParameterLayout>& parameterLayout, bool deferredInitialize)
		{
			SPtr<NullGpuParameters> parameters = B3DMakeShared<NullGpuParameters>(*this, parameterLayout);

			if (!deferredInitialize)
				parameters->Initialize();

			return parameters;
		}

		SPtr<GpuGraphicsPipelineState> NullGpuDevice::CreateGpuGraphicsPipelineState(const GpuGraphicsPipelineStateCreateInformation& createInformation, bool deferredInitialize)
		{
			SPtr<NullGpuGraphicsPipelineState> pipelineState = B3DMakeShared<NullGpuGraphicsPipelineState>(*this, createInformation);

			if (!deferredInitialize)
				pipelineState->Initialize();

			return pipelineState;
		}

		SPtr<GpuComputePipelineState> NullGpuDevice::CreateGpuComputePipelineState(const GpuComputePipelineStateCreateInformation& createInformation, bool deferredInitialize)
		{
			SPtr<NullGpuComputePipelineState> pipelineState = B3DMakeShared<NullGpuComputePipelineState>(*this, createInformation);

			if (!deferredInitialize)
				pipelineState->Initialize();

			return pipelineState;
		}

		SPtr<GpuPipelineParameterLayout> NullGpuDevice::CreateGpuPipelineParameterLayout(const GpuPipelineParameterLayoutCreateInformation& createInformation, bool deferredInitialize)
		{
			SPtr<NullGpuPipelineParameterLayout> layout = B3DMakeShared<NullGpuPipelineParameterLayout>(*this, createInformation);

			if (!deferredInitialize)
				layout->Initialize();

			return layout;
		}

		void NullGpuDevice::ConvertProjectionMatrix(const Matrix4& input, Matrix4& output)
		{
			output = input;
		}

		GpuDataParameterBlockInformation NullGpuDevice::GenerateUniformBlockInformation(const String& name, Vector<GpuDataParameterInformation>& inOutUniforms)
		{
			GpuDataParameterBlockInformation blockInfo;
			blockInfo.Name = name;
			blockInfo.BlockSize = 0;
			blockInfo.IsShareable = true;

			for (auto& uniform : inOutUniforms)
			{
				uniform.GpuOffset = blockInfo.BlockSize;
				uniform.CpuOffset = blockInfo.BlockSize;
				blockInfo.BlockSize += uniform.ElementSize;
			}

			return blockInfo;
		}

		SPtr<SamplerState> NullGpuDevice::CreateSamplerState(const SamplerStateCreateInformation& createInformation, bool deferredInitialize)
		{
			SPtr<NullSamplerState> samplerState = B3DMakeShared<NullSamplerState>(*this, createInformation);

			if (!deferredInitialize)
				samplerState->Initialize();

			return samplerState;
		}
	} // namespace render
} // namespace b3d
