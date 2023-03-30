//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "RenderAPI/BsGpuPipelineState.h"

#include "BsCoreApplication.h"
#include "BsGpuBackend.h"
#include "BsGpuDevice.h"
#include "RenderAPI/BsRasterizerState.h"
#include "RenderAPI/BsBlendState.h"
#include "RenderAPI/BsDepthStencilState.h"
#include "RenderAPI/BsGpuProgram.h"
#include "RenderAPI/BsGpuProgramParameterDescription.h"
#include "RenderAPI/BsGpuPipelineParameterLayout.h"

using namespace bs;

/** Converts a sim thread pipeline state descriptor to a core thread one. */
void ConvertPassDesc(const PIPELINE_STATE_DESC& input, ct::GpuGraphicsPipelineStateInformation& output)
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

GpuGraphicsPipelineState::GpuGraphicsPipelineState(const PIPELINE_STATE_DESC& desc)
	: TGraphicsPipelineState(desc)
{
	GpuPipelineParameterLayoutInformation paramsDesc;
	if(desc.VertexProgram != nullptr)
	{
		desc.VertexProgram->BlockUntilCoreInitialized();
		paramsDesc.Vertex = desc.VertexProgram->GetParameterDescription();
	}

	if(desc.FragmentProgram != nullptr)
	{
		desc.FragmentProgram->BlockUntilCoreInitialized();
		paramsDesc.Fragment = desc.FragmentProgram->GetParameterDescription();
	}

	if(desc.GeometryProgram != nullptr)
	{
		desc.GeometryProgram->BlockUntilCoreInitialized();
		paramsDesc.Geometry = desc.GeometryProgram->GetParameterDescription();
	}

	if(desc.HullProgram != nullptr)
	{
		desc.HullProgram->BlockUntilCoreInitialized();
		paramsDesc.Hull = desc.HullProgram->GetParameterDescription();
	}

	if(desc.DomainProgram != nullptr)
	{
		desc.DomainProgram->BlockUntilCoreInitialized();
		paramsDesc.Domain = desc.DomainProgram->GetParameterDescription();
	}

	mParameterLayout = GpuPipelineParameterLayout::Create(paramsDesc);
}

SPtr<ct::GpuGraphicsPipelineState> GpuGraphicsPipelineState::GetCore() const
{
	return std::static_pointer_cast<ct::GpuGraphicsPipelineState>(mCoreSpecific);
}

SPtr<ct::CoreObject> GpuGraphicsPipelineState::CreateCore() const
{
	ct::GpuGraphicsPipelineStateCreateInformation createInformation;
	ConvertPassDesc(mData, createInformation);

	const SPtr<GpuDevice>& gpuDevice = GetCoreApplication().GetPrimaryGpuDevice();
	if(!gpuDevice)
		return nullptr;

	return gpuDevice->CreateGpuGraphicsPipelineState(createInformation, true);
}

SPtr<GpuGraphicsPipelineState> GpuGraphicsPipelineState::Create(const PIPELINE_STATE_DESC& desc)
{
	SPtr<GpuGraphicsPipelineState> pipelineState =
		B3DMakeCoreFromExisting<GpuGraphicsPipelineState>(new(B3DAllocate<GpuGraphicsPipelineState>()) GpuGraphicsPipelineState(desc));
	pipelineState->SetShared(pipelineState);
	pipelineState->Initialize();

	return pipelineState;
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

GpuComputePipelineState::GpuComputePipelineState(const SPtr<GpuProgram>& program)
	: TComputePipelineState(program)
{
	GpuPipelineParameterLayoutInformation paramsDesc;
	program->BlockUntilCoreInitialized();
	paramsDesc.Compute = program->GetParameterDescription();

	mParameterLayout = GpuPipelineParameterLayout::Create(paramsDesc);
}

SPtr<ct::GpuComputePipelineState> GpuComputePipelineState::GetCore() const
{
	return std::static_pointer_cast<ct::GpuComputePipelineState>(mCoreSpecific);
}

SPtr<ct::CoreObject> GpuComputePipelineState::CreateCore() const
{
	const SPtr<GpuDevice>& gpuDevice = GetCoreApplication().GetPrimaryGpuDevice();
	if(!gpuDevice)
		return nullptr;

	ct::GpuComputePipelineStateCreateInformation createInformation;
	createInformation.Program = mProgram->GetCore();

	return gpuDevice->CreateGpuComputePipelineState(createInformation, true);
}

SPtr<GpuComputePipelineState> GpuComputePipelineState::Create(const SPtr<GpuProgram>& program)
{
	SPtr<GpuComputePipelineState> pipelineState =
		B3DMakeCoreFromExisting<GpuComputePipelineState>(new(B3DAllocate<GpuComputePipelineState>()) GpuComputePipelineState(program));
	pipelineState->SetShared(pipelineState);
	pipelineState->Initialize();

	return pipelineState;
}

namespace bs { namespace ct
{
GpuGraphicsPipelineState::GpuGraphicsPipelineState(GpuDevice& gpuDevice, const GpuGraphicsPipelineStateCreateInformation& createInformation)
	: TGraphicsPipelineState(createInformation), mGpuDevice(gpuDevice)
{}

void GpuGraphicsPipelineState::Initialize()
{
	GpuPipelineParameterLayoutInformation parameterLayoutCreateInformation;
	if(mData.VertexProgram != nullptr)
		parameterLayoutCreateInformation.Vertex = mData.VertexProgram->GetParameterDescription();

	if(mData.FragmentProgram != nullptr)
		parameterLayoutCreateInformation.Fragment = mData.FragmentProgram->GetParameterDescription();

	if(mData.GeometryProgram != nullptr)
		parameterLayoutCreateInformation.Geometry = mData.GeometryProgram->GetParameterDescription();

	if(mData.HullProgram != nullptr)
		parameterLayoutCreateInformation.Hull = mData.HullProgram->GetParameterDescription();

	if(mData.DomainProgram != nullptr)
		parameterLayoutCreateInformation.Domain = mData.DomainProgram->GetParameterDescription();

	mParameterLayout = mGpuDevice.CreateGpuPipelineParameterLayout(parameterLayoutCreateInformation);

	CoreObject::Initialize();
}

GpuComputePipelineState::GpuComputePipelineState(GpuDevice& gpuDevice, const GpuComputePipelineStateCreateInformation& createInformation)
	: TComputePipelineState(createInformation.Program), mGpuDevice(gpuDevice)
{}

void GpuComputePipelineState::Initialize()
{
	GpuPipelineParameterLayoutInformation parameterLayoutCreateInformation;
	parameterLayoutCreateInformation.Compute = mProgram->GetParameterDescription();

	mParameterLayout = mGpuDevice.CreateGpuPipelineParameterLayout(parameterLayoutCreateInformation);

	CoreObject::Initialize();
}
}}
