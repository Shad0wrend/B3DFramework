//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DNullPrerequisites.h"
#include "RenderAPI/B3DRenderWindow.h"

namespace b3d::render
{
	/** @addtogroup Null
	 *  @{
	 */

	/** Null implementation of IRenderWindowSurface. */
	class NullRenderWindowSurface : public IRenderWindowSurface
	{
	public:
		NullRenderWindowSurface(const RenderWindowSurfaceCreateInformation& createInformation);
		~NullRenderWindowSurface();

		void RebuildSwapChain(u32 width, u32 height, bool vsync) override;
		void MarkSwapChainAsInvalid() override;
		void Destroy() override;
	};

	/** @} */
} // namespace b3d::render
