//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DD3D12Prerequisites.h"
#include "Managers/B3DGpuBackendManager.h"

namespace b3d
{
	/** @addtogroup D3D12RenderAPI
	 *  @{
	 */

	/** Factory class for creating the DirectX 12 GPU backend. */
	class D3D12GpuBackendFactory
	{
	public:
		/**
		 * Creates the D3D12 GPU backend factory and registers it with the GpuBackendManager.
		 * This should be called when the D3D12 render API plugin is loaded.
		 */
		static void Create();

		/** Returns the name of the render API. */
		static const char* GetAPIName() { return "DirectX 12"; }
	};

	/** @} */
} // namespace b3d
