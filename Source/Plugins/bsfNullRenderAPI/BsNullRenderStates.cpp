//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsNullRenderStates.h"

using namespace bs;
using namespace bs::ct;

SPtr<ct::SamplerState> NullRenderStateManager::CreateSamplerStateInternalInternal(const SamplerStateInformation& desc, GpuDeviceFlags deviceMask) const
{
	SPtr<NullSamplerState> ret = B3DMakeShared<NullSamplerState>(desc, deviceMask);
	ret->SetShared(ret);

	return ret;
}

SPtr<ct::BlendState> NullRenderStateManager::CreateBlendStateInternalInternal(const BlendStateInformation& desc, u32 id) const
{
	SPtr<NullBlendState> ret = B3DMakeShared<NullBlendState>(desc, id);
	ret->SetShared(ret);

	return ret;
}

SPtr<ct::RasterizerState> NullRenderStateManager::CreateRasterizerStateInternalInternal(const RASTERIZER_STATE_DESC& desc, u32 id) const
{
	SPtr<NullRasterizerState> ret = B3DMakeShared<NullRasterizerState>(desc, id);
	ret->SetShared(ret);

	return ret;
}

SPtr<ct::DepthStencilState> NullRenderStateManager::CreateDepthStencilStateInternalInternal(const DEPTH_STENCIL_STATE_DESC& desc, u32 id) const
{
	SPtr<NullDepthStencilState> ret = B3DMakeShared<NullDepthStencilState>(desc, id);
	ret->SetShared(ret);

	return ret;
}
