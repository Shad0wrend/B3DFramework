//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "B3DFrameGraphBarrier.h"
#include "Debug/B3DDebug.h"

using namespace b3d;
using namespace b3d::render;

ImageLayout FrameGraphLayoutHelper::GetLayoutForUsage(GpuResourceUseFlags usage, GpuAccessFlags access)
{
	// Priority order: attachment usage > shader usage > transfer usage

	// Color attachment - write or read/write
	if (usage.IsSet(GpuResourceUseFlag::ColorAttachment))
	{
		return ImageLayout::ColorAttachment;
	}

	// Depth/stencil attachment - check if read-only
	if (usage.IsSet(GpuResourceUseFlag::DepthStencilAttachment))
	{
		// Read-only depth (can be sampled while bound)
		if (access == GpuAccessFlag::Read)
			return ImageLayout::DepthStencilReadOnly;

		// Write or read/write depth
		return ImageLayout::DepthStencilAttachment;
	}

	// Shader access (sampling, storage, etc.)
	if (usage.IsSet(GpuResourceUseFlag::ShaderAccess))
	{
		// If write access is involved, use General layout
		if (access.IsSet(GpuAccessFlag::Write))
			return ImageLayout::General;

		// Read-only shader access
		return ImageLayout::ShaderReadOnly;
	}

	// Transfer operations
	if (usage.IsSet(GpuResourceUseFlag::Transfer))
	{
		if (access.IsSet(GpuAccessFlag::Write))
			return ImageLayout::TransferDestination;

		return ImageLayout::TransferSource;
	}

	// Default to general layout for undefined or complex cases
	if (usage != GpuResourceUseFlag::Undefined)
		return ImageLayout::General;

	return ImageLayout::Undefined;
}

bool FrameGraphLayoutHelper::RequiresTransition(ImageLayout sourceLayout, ImageLayout destinationLayout)
{
	// No transition needed if layouts match
	if (sourceLayout == destinationLayout)
		return false;

	// Undefined source means we don't care about current contents
	// But we still need to transition to the destination layout
	if (sourceLayout == ImageLayout::Undefined)
		return true;

	// Undefined destination means we're discarding the image
	// This is unusual but technically requires a transition
	if (destinationLayout == ImageLayout::Undefined)
		return true;

	// All other cases require transition
	return true;
}
