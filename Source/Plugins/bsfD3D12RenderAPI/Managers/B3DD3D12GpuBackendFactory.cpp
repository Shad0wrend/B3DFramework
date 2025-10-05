//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "B3DD3D12GpuBackendFactory.h"
#include "B3DD3D12GpuBackend.h"

namespace b3d
{
	/** Implementation of GpuBackendFactory for DirectX 12. */
	class D3D12GpuBackendFactoryImpl : public GpuBackendFactory
	{
	public:
		void Create() override
		{
			// Start up the D3D12 GPU backend
			D3D12GpuBackend::StartUp();
		}

		const char* Name() const override
		{
			return "DirectX 12";
		}
	};

	void D3D12GpuBackendFactory::Create()
	{
		// Create the factory implementation and register it with the GpuBackendManager
		SPtr<GpuBackendFactory> factory = B3DMakeShared<D3D12GpuBackendFactoryImpl>();
		GpuBackendManager::Instance().RegisterFactory(factory);

		// Immediately create the backend
		factory->Create();
	}
} // namespace b3d
