//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "B3DNullGpuBackend.h"
#include "B3DNullGpuDevice.h"

namespace b3d
{
	void NullGpuBackend::OnStartUp()
	{
		Super::OnStartUp();

		// Create a single null device
		mDevices.Add(B3DMakeShared<render::NullGpuDevice>());
	}

	void NullGpuBackend::OnShutDown()
	{
		mDevices.clear();

		Super::OnShutDown();
	}

	NullGpuBackend& GetNullGpuBackend()
	{
		return static_cast<NullGpuBackend&>(GpuBackend::Instance());
	}
} // namespace b3d
