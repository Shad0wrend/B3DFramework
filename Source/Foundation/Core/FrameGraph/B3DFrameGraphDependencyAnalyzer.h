//************************************ B3D Framework - Copyright 2025 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DPrerequisites.h"
#include "B3DFrameGraphTypes.h"
#include "B3DFrameGraphDependency.h"

namespace b3d::render
{
	class FrameGraph;
	class FrameGraphPass;

	/** @addtogroup RenderAPI
	 *  @{
	 */

	/**
	 * Analyzes frame graph passes to build dependency graph.
	 * Determines execution order based on resource usage patterns.
	 *
	 * **Algorithm:**
	 * 1. Analyze all passes to determine resource access patterns
	 * 2. Build dependency edges based on:
	 *    - Read-after-Write (RAW): Consumer reads what producer wrote
	 *    - Write-after-Read (WAR): Consumer writes what producer read
	 *    - Write-after-Write (WAW): Consumer writes what producer wrote
	 * 3. Track resource lifetimes (first use, last use)
	 * 4. Prepare for topological sort
	 */
	class B3D_EXPORT FrameGraphDependencyAnalyzer
	{
	public:
		/** Constructor */
		explicit FrameGraphDependencyAnalyzer(FrameGraph& frameGraph);

		/**
		 * Analyze all passes and build dependency graph.
		 * @return True if analysis succeeded, false if errors detected
		 */
		bool Analyze();

		/** Get all pass nodes (one per pass) */
		const Vector<UPtr<FrameGraphPassNode>>& GetPassNodes() const { return mPassNodes; }

		/** Get resource lifetime information */
		const UnorderedMap<FrameGraphResourceId, FrameGraphResourceLifetime>& GetResourceLifetimes() const
		{
			return mResourceLifetimes;
		}

		/** Reset for next frame */
		void Reset();

	private:
		/** Analyze resource access patterns for all passes */
		void AnalyzeResourceAccess();

		/** Build dependency edges between passes */
		void BuildDependencies();

		/** Track resource lifetimes (first/last use) */
		void TrackResourceLifetimes();

		/** Find the node for a given pass */
		FrameGraphPassNode* FindNode(FrameGraphPass* pass);

		/** Check if a resource is written to by a pass */
		bool IsResourceWritten(FrameGraphPass* pass, FrameGraphResourceId resource);

		/** Check if a resource is read from by a pass */
		bool IsResourceRead(FrameGraphPass* pass, FrameGraphResourceId resource);

	private:
		/** Reference to the frame graph being analyzed */
		FrameGraph& mFrameGraph;

		/** Pass nodes (one per pass) */
		Vector<UPtr<FrameGraphPassNode>> mPassNodes;

		/** Resource lifetime tracking */
		UnorderedMap<FrameGraphResourceId, FrameGraphResourceLifetime> mResourceLifetimes;

		/** Map from resource to passes that write to it (for RAW dependencies) */
		UnorderedMap<FrameGraphResourceId, Vector<FrameGraphPass*>> mResourceWriters;

		/** Map from resource to passes that read from it (for WAR dependencies) */
		UnorderedMap<FrameGraphResourceId, Vector<FrameGraphPass*>> mResourceReaders;
	};

	/** @} */
}
