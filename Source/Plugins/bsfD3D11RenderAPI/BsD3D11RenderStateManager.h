//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsD3D11Prerequisites.h"
#include "Managers/BsRenderStateManager.h"

namespace b3d
{
	namespace ct
	{
		/** @addtogroup D3D11
		 *  @{
		 */

		/**	Handles creation of DirectX 11 pipeline states. */
		class D3D11RenderStateManager : public RenderStateManager
		{
		protected:
			SPtr<SamplerState> CreateSamplerStateInternalInternal(const SamplerStateInformation& desc, GpuDeviceFlags deviceMask) const override;
			SPtr<BlendState> CreateBlendStateInternalInternal(const BlendStateInformation& desc, u32 id) const override;
			SPtr<RasterizerState> CreateRasterizerStateInternalInternal(const RASTERIZER_STATE_DESC& desc, u32 id) const override;
			SPtr<DepthStencilState> CreateDepthStencilStateInternalInternal(const DEPTH_STENCIL_STATE_DESC& desc, u32 id) const override;
		};

		/** @} */
	} // namespace ct
} // namespace b3d
