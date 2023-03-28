//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "RenderAPI/BsGpuPipelineState.h"
#include "RenderAPI/BsRasterizerState.h"
#include "RenderAPI/BsBlendState.h"
#include "RenderAPI/BsDepthStencilState.h"
#include "RenderAPI/BsGpuProgram.h"
#include "RenderAPI/BsGpuProgramParameterDescription.h"
#include "RenderAPI/BsGpuPipelineParameterLayout.h"
#include "Managers/BsRenderStateManager.h"

using namespace bs;

/** Converts a sim thread pipeline state descriptor to a core thread one. */
void ConvertPassDesc(const PIPELINE_STATE_DESC& input, ct::PIPELINE_STATE_DESC& output)
{
	output.BlendState = input.BlendState != nullptr ? input.BlendState->GetCore() : nullptr;
	output.RasterizerState = input.RasterizerState != nullptr ? input.RasterizerState->GetCore() : nullptr;
	output.DepthStencilState = input.DepthStencilState != nullptr ? input.DepthStencilState->GetCore() : nullptr;
	output.VertexProgram = input.VertexProgram != nullptr ? input.VertexProgram->GetCore() : nullptr;
	output.FragmentProgram = input.FragmentProgram != nullptr ? input.FragmentProgram->GetCore() : nullptr;
	output.GeometryProgram = input.GeometryProgram != nullptr ? input.GeometryProgram->GetCore() : nullptr;
	output.HullProgram = input.HullProgram != nullptr ? input.HullProgram->GetCore() : nullptr;
	output.DomainProgram = input.DomainProgram != nullptr ? input.DomainProgram->GetCore() : nullptr;
}

template <bool Core>
TGraphicsPipelineState<Core>::TGraphicsPipelineState(const StateDescType& data)
	: mData(data)
{}

template class TGraphicsPipelineState<false>;
template class TGraphicsPipelineState<true>;

GraphicsPipelineState::GraphicsPipelineState(const PIPELINE_STATE_DESC& desc)
	: TGraphicsPipelineState(desc)
{
	GpuPipelineParameterDescription paramsDesc;
	if(desc.VertexProgram != nullptr)
	{
		desc.VertexProgram->BlockUntilCoreInitialized();
		paramsDesc.Vertex = desc.VertexProgram->GetParamDesc();
	}

	if(desc.FragmentProgram != nullptr)
	{
		desc.FragmentProgram->BlockUntilCoreInitialized();
		paramsDesc.Fragment = desc.FragmentProgram->GetParamDesc();
	}

	if(desc.GeometryProgram != nullptr)
	{
		desc.GeometryProgram->BlockUntilCoreInitialized();
		paramsDesc.Geometry = desc.GeometryProgram->GetParamDesc();
	}

	if(desc.HullProgram != nullptr)
	{
		desc.HullProgram->BlockUntilCoreInitialized();
		paramsDesc.Hull = desc.HullProgram->GetParamDesc();
	}

	if(desc.DomainProgram != nullptr)
	{
		desc.DomainProgram->BlockUntilCoreInitialized();
		paramsDesc.Domain = desc.DomainProgram->GetParamDesc();
	}

	mParamInfo = GpuPipelineParameterLayout::Create(paramsDesc);
}

SPtr<ct::GraphicsPipelineState> GraphicsPipelineState::GetCore() const
{
	return std::static_pointer_cast<ct::GraphicsPipelineState>(mCoreSpecific);
}

SPtr<ct::CoreObject> GraphicsPipelineState::CreateCore() const
{
	ct::PIPELINE_STATE_DESC desc;
	ConvertPassDesc(mData, desc);

	return ct::RenderStateManager::Instance().CreateGraphicsPipelineStateInternal(desc);
}

SPtr<GraphicsPipelineState> GraphicsPipelineState::Create(const PIPELINE_STATE_DESC& desc)
{
	return RenderStateManager::Instance().CreateGraphicsPipelineState(desc);
}

template <bool Core>
TComputePipelineState<Core>::TComputePipelineState()
{}

template <bool Core>
TComputePipelineState<Core>::TComputePipelineState(const GpuProgramType& program)
	: mProgram(program)
{}

template class TComputePipelineState<false>;
template class TComputePipelineState<true>;

ComputePipelineState::ComputePipelineState(const SPtr<GpuProgram>& program)
	: TComputePipelineState(program)
{
	GpuPipelineParameterDescription paramsDesc;
	program->BlockUntilCoreInitialized();
	paramsDesc.Compute = program->GetParamDesc();

	mParamInfo = GpuPipelineParameterLayout::Create(paramsDesc);
}

SPtr<ct::ComputePipelineState> ComputePipelineState::GetCore() const
{
	return std::static_pointer_cast<ct::ComputePipelineState>(mCoreSpecific);
}

SPtr<ct::CoreObject> ComputePipelineState::CreateCore() const
{
	return ct::RenderStateManager::Instance().CreateComputePipelineStateInternal(mProgram->GetCore());
}

SPtr<ComputePipelineState> ComputePipelineState::Create(const SPtr<GpuProgram>& program)
{
	return RenderStateManager::Instance().CreateComputePipelineState(program);
}

namespace bs { namespace ct
{
GraphicsPipelineState::GraphicsPipelineState(const PIPELINE_STATE_DESC& desc, GpuDeviceFlags deviceMask)
	: TGraphicsPipelineState(desc), mDeviceMask(deviceMask)
{}

void GraphicsPipelineState::Initialize()
{
	GpuPipelineParameterDescription paramsDesc;
	if(mData.VertexProgram != nullptr)
		paramsDesc.Vertex = mData.VertexProgram->GetParamDesc();

	if(mData.FragmentProgram != nullptr)
		paramsDesc.Fragment = mData.FragmentProgram->GetParamDesc();

	if(mData.GeometryProgram != nullptr)
		paramsDesc.Geometry = mData.GeometryProgram->GetParamDesc();

	if(mData.HullProgram != nullptr)
		paramsDesc.Hull = mData.HullProgram->GetParamDesc();

	if(mData.DomainProgram != nullptr)
		paramsDesc.Domain = mData.DomainProgram->GetParamDesc();

	mParamInfo = GpuPipelineParameterLayout::Create(paramsDesc, mDeviceMask);

	CoreObject::Initialize();
}

SPtr<GraphicsPipelineState> GraphicsPipelineState::Create(const PIPELINE_STATE_DESC& desc, GpuDeviceFlags deviceMask)
{
	return RenderStateManager::Instance().CreateGraphicsPipelineState(desc, deviceMask);
}

ComputePipelineState::ComputePipelineState(const SPtr<GpuProgram>& program, GpuDeviceFlags deviceMask)
	: TComputePipelineState(program), mDeviceMask(deviceMask)
{}

void ComputePipelineState::Initialize()
{
	GpuPipelineParameterDescription paramsDesc;
	paramsDesc.Compute = mProgram->GetParamDesc();

	mParamInfo = GpuPipelineParameterLayout::Create(paramsDesc, mDeviceMask);

	CoreObject::Initialize();
}

SPtr<ComputePipelineState> ComputePipelineState::Create(const SPtr<GpuProgram>& program, GpuDeviceFlags deviceMask)
{
	return RenderStateManager::Instance().CreateComputePipelineState(program, deviceMask);
}
}}
