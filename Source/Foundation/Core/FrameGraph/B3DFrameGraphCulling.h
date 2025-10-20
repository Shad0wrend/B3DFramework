//************************************ B3D Framework - Copyright 2025 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DPrerequisites.h"
#include "B3DFrameGraphTypes.h"
#include "B3DFrameGraphDependency.h"

namespace b3d::render
{
	class FrameGraph;

	/** @addtogroup RenderAPI
	 *  @{
	 */

	/**
	 * Culls unused passes from the frame graph.
	 * A pass is considered used if:
	 * 1. It writes to a resource marked as output
	 * 2. It's part of a dependency chain leading to an output
	 *
	 * **Algorithm (Reverse DFS):**
	 * 1. Mark all passes as culled initially
	 * 2. Find all passes that write to output resources
	 * 3. Perform reverse DFS from output passes:
	 *    - Mark pass as not culled
	 *    - Recursively mark all incoming dependencies as not culled
	 * 4. Any pass still marked as culled is removed
	 */
	class B3D_EXPORT FrameGraphCulling
	{
	public:
		/** Constructor */
		explicit FrameGraphCulling(FrameGraph& frameGraph);

		/**
		 * Cull unused passes.
		 * @param nodes The pass nodes to analyze
		 * @param resourceLifetimes Resource lifetime information
		 */
		void Cull(Vector<UPtr<FrameGraphPassNode>>& nodes,
			const UnorderedMap<FrameGraphResourceId, FrameGraphResourceLifetime>& resourceLifetimes);

		/** Get the number of passes culled */
		u32 GetCulledPassCount() const { return mCulledPassCount; }

	private:
		/** Mark a pass and its dependencies as used (recursive) */
		void MarkPassAsUsed(FrameGraphPassNode* node,
			const Vector<UPtr<FrameGraphPassNode>>& allNodes);

		/** Find the node for a given pass */
		FrameGraphPassNode* FindNode(FrameGraphPass* pass,
			const Vector<UPtr<FrameGraphPassNode>>& nodes);

	private:
		/** Reference to the frame graph */
		FrameGraph& mFrameGraph;

		/** Number of passes culled */
		u32 mCulledPassCount = 0;
	};

	/** @} */
}
