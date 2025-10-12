//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "B3DNullRenderWindowSurface.h"

namespace b3d::render
{
	NullRenderWindowSurface::NullRenderWindowSurface(const RenderWindowSurfaceCreateInformation& createInformation)
	{
		// No-op constructor
	}

	NullRenderWindowSurface::~NullRenderWindowSurface()
	{
		// No-op destructor
	}

	void NullRenderWindowSurface::RebuildSwapChain(u32 width, u32 height, bool vsync)
	{
		// No-op implementation
	}

	void NullRenderWindowSurface::MarkSwapChainAsInvalid()
	{
		// No-op implementation
	}

	void NullRenderWindowSurface::Destroy()
	{
		// No-op implementation
	}
} // namespace b3d::render
