//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "RenderAPI/BsGpuPipelineState.h"

#include "BsCoreApplication.h"
#include "BsGpuBackend.h"
#include "BsGpuDevice.h"
#include "RenderAPI/BsDepthStencilState.h"
#include "RenderAPI/BsGpuProgram.h"
#include "RenderAPI/BsGpuProgramParameterDescription.h"
#include "RenderAPI/BsGpuPipelineParameterLayout.h"

using namespace bs;

bool RenderTargetBlendStateInformation::operator==(const RenderTargetBlendStateInformation& rhs) const
{
	return BlendEnable == rhs.BlendEnable &&
		ColorSourceFactor == rhs.ColorSourceFactor &&
		ColorDestinationFactor == rhs.ColorDestinationFactor &&
		ColorBlendOperation == rhs.ColorBlendOperation &&
		AlphaSourceFactor == rhs.AlphaSourceFactor &&
		AlphaDestinationFactor == rhs.AlphaDestinationFactor &&
		AlphaBlendOperation == rhs.AlphaBlendOperation &&
		RenderTargetWriteMask == rhs.RenderTargetWriteMask;
}

bool BlendStateInformation::operator==(const BlendStateInformation& rhs) const
{
	bool equals = EnableAlphaToCoverage == rhs.EnableAlphaToCoverage &&
		EnableIndependantBlend == rhs.EnableIndependantBlend;

	if(equals)
	{
		for(u32 i = 0; i < B3D_MAXIMUM_RENDER_TARGET_COUNT; i++)
		{
			equals &= RenderTargets[i] == rhs.RenderTargets[i];
		}
	}

	return equals;
}

u64 BlendStateInformation::GenerateHash(const BlendStateInformation& value)
{
	size_t hash = 0;
	B3DCombineHash(hash, value.EnableAlphaToCoverage);
	B3DCombineHash(hash, value.EnableIndependantBlend);

	for(u32 i = 0; i < B3D_MAXIMUM_RENDER_TARGET_COUNT; i++)
	{
		B3DCombineHash(hash, value.RenderTargets[i].BlendEnable);
		B3DCombineHash(hash, (u32)value.RenderTargets[i].ColorSourceFactor);
		B3DCombineHash(hash, (u32)value.RenderTargets[i].ColorDestinationFactor);
		B3DCombineHash(hash, (u32)value.RenderTargets[i].ColorBlendOperation);
		B3DCombineHash(hash, (u32)value.RenderTargets[i].AlphaSourceFactor);
		B3DCombineHash(hash, (u32)value.RenderTargets[i].AlphaDestinationFactor);
		B3DCombineHash(hash, (u32)value.RenderTargets[i].AlphaBlendOperation);
		B3DCombineHash(hash, value.RenderTargets[i].RenderTargetWriteMask);
	}

	return (u64)hash;
}

bool RasterizerStateInformation::operator==(const RasterizerStateInformation& rhs) const
{
	return PolygonMode == rhs.PolygonMode &&
		CullMode == rhs.CullMode &&
		DepthBias == rhs.DepthBias &&
		DepthBiasClamp == rhs.DepthBiasClamp &&
		SlopeScaledDepthBias == rhs.SlopeScaledDepthBias &&
		DepthClipEnable == rhs.DepthClipEnable &&
		ScissorEnable == rhs.ScissorEnable &&
		MultisampleEnable == rhs.MultisampleEnable &&
		AntialiasedLineEnable == rhs.AntialiasedLineEnable;
}

u64 RasterizerStateInformation::GenerateHash(const RasterizerStateInformation& value)
{
	size_t hash = 0;
	B3DCombineHash(hash, (u32)value.PolygonMode);
	B3DCombineHash(hash, (u32)value.CullMode);
	B3DCombineHash(hash, value.DepthBias);
	B3DCombineHash(hash, value.DepthBiasClamp);
	B3DCombineHash(hash, value.SlopeScaledDepthBias);
	B3DCombineHash(hash, value.DepthClipEnable);
	B3DCombineHash(hash, value.ScissorEnable);
	B3DCombineHash(hash, value.MultisampleEnable);
	B3DCombineHash(hash, value.AntialiasedLineEnable);

	return (u64)hash;
}

/** Converts a sim thread pipeline state descriptor to a core thread one. */
void ConvertPassDesc(const PIPELINE_STATE_DESC& input, ct::GpuGraphicsPipelineStateInformation& output)
{
	output.BlendState = input.BlendState;
	output.RasterizerState = input.RasterizerState;
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
