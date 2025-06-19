//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsD3D11RenderStateManager.h"
#include "BsD3D11SamplerState.h"
#include "BsD3D11DepthStencilState.h"
#include "BsD3D11RasterizerState.h"
#include "BsD3D11BlendState.h"

using namespace b3d;
using namespace b3d::render;

SPtr<SamplerState> D3D11RenderStateManager::CreateSamplerStateInternalInternal(const SamplerStateInformation& desc, GpuDeviceFlags deviceMask) const
{
	SPtr<SamplerState> ret = B3DMakeSharedFromExisting<D3D11SamplerState>(new(B3DAllocate<D3D11SamplerState>()) D3D11SamplerState(desc, deviceMask));
	ret->SetShared(ret);

	return ret;
}

SPtr<render::BlendState> D3D11RenderStateManager::CreateBlendStateInternalInternal(const BlendStateInformation& desc, u32 id) const
{
	SPtr<BlendState> ret = B3DMakeSharedFromExisting<D3D11BlendState>(new(B3DAllocate<D3D11BlendState>()) D3D11BlendState(desc, id));
	ret->SetShared(ret);

	return ret;
}

SPtr<render::RasterizerState> D3D11RenderStateManager::CreateRasterizerStateInternalInternal(const RASTERIZER_STATE_DESC& desc, u32 id) const
{
	SPtr<RasterizerState> ret = B3DMakeSharedFromExisting<D3D11RasterizerState>(new(B3DAllocate<D3D11RasterizerState>()) D3D11RasterizerState(desc, id));
	ret->SetShared(ret);

	return ret;
}

SPtr<render::DepthStencilState> D3D11RenderStateManager::CreateDepthStencilStateInternalInternal(const DEPTH_STENCIL_STATE_DESC& desc, u32 id) const
{
	SPtr<DepthStencilState> ret = B3DMakeSharedFromExisting<D3D11DepthStencilState>(new(B3DAllocate<D3D11DepthStencilState>()) D3D11DepthStencilState(desc, id));
	ret->SetShared(ret);

	return ret;
}
