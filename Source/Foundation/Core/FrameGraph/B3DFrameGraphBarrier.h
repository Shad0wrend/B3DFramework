//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DPrerequisites.h"
#include "B3DFrameGraphTypes.h"
#include "RenderAPI/B3DGpuCommandBuffer.h"

namespace b3d::render
{
	/** @addtogroup FrameGraph
	 *  @{
	 */

	class FrameGraphPass;

	// TODO - Merge this entire file and it's .cpp file into FrameGraphCompiler.h/.cpp

	/**
	 * Represents a single use of a resource within a pass.
	 * Tracks both the usage flags and the image layout at that point.
	 */
	struct ResourceUsage
	{
		FrameGraphPass* Pass = nullptr;
		GpuResourceUseFlags Usage;
		GpuAccessFlags Access;
		ImageLayout Layout = ImageLayout::Undefined;

		ResourceUsage() = default;

		ResourceUsage(FrameGraphPass* pass, GpuResourceUseFlags usage, GpuAccessFlags access, ImageLayout layout = ImageLayout::Undefined)
			: Pass(pass), Usage(usage), Access(access), Layout(layout)
		{}
	};

	/**
	 * Complete history of how a resource is used throughout the frame graph.
	 * Contains chronological list of all uses.
	 */
	struct ResourceUsageHistory
	{
		FrameGraphResourceId Resource;
		Vector<ResourceUsage> Uses;
	};

	/**
	 * Represents a required transition between two resource usages.
	 * Contains source and destination states for barrier generation.
	 */
	struct ResourceTransition
	{
		FrameGraphResourceId Resource;
		FrameGraphPass* SourcePass;
		FrameGraphPass* DestinationPass;

		GpuResourceUseFlags SourceUsage;
		GpuAccessFlags SourceAccess;
		ImageLayout SourceLayout = ImageLayout::Undefined;

		GpuResourceUseFlags DestinationUsage;
		GpuAccessFlags DestinationAccess;
		ImageLayout DestinationLayout = ImageLayout::Undefined;

		ResourceTransition() = default;

		ResourceTransition(
			FrameGraphResourceId resource,
			FrameGraphPass* sourcePass,
			FrameGraphPass* destinationPass,
			GpuResourceUseFlags sourceUsage,
			GpuAccessFlags sourceAccess,
			ImageLayout sourceLayout,
			GpuResourceUseFlags destinationUsage,
			GpuAccessFlags destinationAccess,
			ImageLayout destinationLayout)
			: Resource(resource)
			, SourcePass(sourcePass)
			, DestinationPass(destinationPass)
			, SourceUsage(sourceUsage)
			, SourceAccess(sourceAccess)
			, SourceLayout(sourceLayout)
			, DestinationUsage(destinationUsage)
			, DestinationAccess(destinationAccess)
			, DestinationLayout(destinationLayout)
		{}
	};

	/**
	 * A batch of barriers to be issued before a specific pass.
	 * Combines all required barriers for a pass into a single GpuBarriers structure.
	 */
	struct FrameGraphBarrierBatch
	{
		FrameGraphPass* DestinationPass = nullptr;
		GpuBarriers Barriers;
	};

	/**
	 * Helper functions for determining optimal image layouts.
	 */
	class B3D_EXPORT FrameGraphLayoutHelper
	{
	public:
		/**
		 * Determines the optimal image layout for a given resource usage.
		 *
		 * @param usage		Resource usage flags
		 * @param access	Access flags (read/write)
		 * @return			Optimal layout for the usage
		 */
		static ImageLayout GetLayoutForUsage(GpuResourceUseFlags usage, GpuAccessFlags access);

		/**
		 * Checks if a layout transition is required.
		 *
		 * @param sourceLayout		Current layout
		 * @param destinationLayout	Desired layout
		 * @return					True if transition is needed
		 */
		static bool RequiresTransition(ImageLayout sourceLayout, ImageLayout destinationLayout);
	};

	/** @} */
}
