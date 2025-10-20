//************************************ B3D Framework - Copyright 2025 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DPrerequisites.h"
#include "B3DFrameGraphDependency.h"

namespace b3d::render
{
	class FrameGraph;

	/** @addtogroup RenderAPI
	 *  @{
	 */

	/**
	 * Performs topological sort on frame graph passes to determine execution order.
	 * Uses Kahn's algorithm for cycle detection and ordering.
	 *
	 * **Algorithm (Kahn's):**
	 * 1. Find all nodes with no incoming dependencies (reference count = 0)
	 * 2. Add them to a queue
	 * 3. While queue is not empty:
	 *    a. Dequeue a node and add to sorted list
	 *    b. For each outgoing dependency:
	 *       - Decrement consumer's reference count
	 *       - If consumer's reference count reaches 0, enqueue it
	 * 4. If sorted list size != total nodes, there's a cycle
	 */
	class B3D_EXPORT FrameGraphTopologicalSort
	{
	public:
		/** Constructor */
		FrameGraphTopologicalSort() = default;

		/**
		 * Perform topological sort on pass nodes.
		 * @param nodes The pass nodes to sort
		 * @param outSortedNodes The sorted nodes (output)
		 * @return True if sort succeeded, false if cycle detected
		 */
		bool Sort(const Vector<UPtr<FrameGraphPassNode>>& nodes, Vector<FrameGraphPassNode*>& outSortedNodes);

		/**
		 * Get the passes involved in the cycle (if any).
		 * Only valid if Sort() returned false.
		 */
		const Vector<FrameGraphPass*>& GetCyclePasses() const { return mCyclePasses; }

	private:
		/** Passes involved in a detected cycle */
		Vector<FrameGraphPass*> mCyclePasses;
	};

	/** @} */
}
