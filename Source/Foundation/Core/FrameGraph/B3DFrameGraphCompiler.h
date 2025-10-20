//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DPrerequisites.h"
#include "B3DFrameGraph.h"
#include "B3DFrameGraphPass.h"
#include "B3DFrameGraphBarrier.h"

namespace b3d::render
{
	/** @addtogroup RenderAPI
	 *  @{
	 */

	/**
	 * Compiled frame graph output.
	 *
	 * Contains the result of frame graph compilation, including the ordered list of passes to execute;
	 */
	class B3D_EXPORT CompiledFrameGraph
	{
	public:
		/** Passes in topologically sorted execution order (culled passes removed) */
		Vector<FrameGraphPass*> SortedPasses;

		/** Barrier batches to issue before passes */
		Vector<FrameGraphBarrierBatch> BarrierBatches;

		/** Complete usage history for all resources. */
		UnorderedMap<FrameGraphResourceId, ResourceUsageHistory> UsageHistories;

		/** Render targets for render passes, created during compilation. */
		UnorderedMap<FrameGraphPass*, SPtr<RenderTarget>> RenderTargets; // TODO - Store RenderTargets in passes directly?
	};

	/**
	 * Compiles a frame graph into an executable form.
	 *
	 * The compiler is responsible for:
	 * - Executing pass setup callbacks to collect resource dependencies
	 * - Validating resource usage and access patterns
	 * - Analyzing dependencies and computing execution order
	 * - Culling unused passes
	 * - Calculating synchronization barriers and layout transitions
	 * - Creating render targets for render passes
	 * - Performing resource lifetime analysis
	 *
	 * All compilation logic is contained within this single class for simplicity.
	 */
	class B3D_EXPORT FrameGraphCompiler
	{
	public:
		explicit FrameGraphCompiler(FrameGraph& frameGraph);

		/**
		 * Compiles the frame graph.
		 *
		 * Executes all setup callbacks, validates the graph, and produces a compiled graph
		 * ready for execution.
		 *
		 * @return  Compiled frame graph, or nullptr if validation failed
		 */
		UPtr<CompiledFrameGraph> Compile();

	private:
		//////////////////////////////////////////////////////////////////////////
		// Phase 1: Validation
		//////////////////////////////////////////////////////////////////////////

		/** Executes setup functions for all passes */
		void ExecuteSetupFunctions();

		/** Validates the graph (checks resources exist) */
		bool Validate();

		/** Validates a single pass */
		bool ValidatePass(FrameGraphPass* pass);

		/** Validates a resource access */
		bool ValidateResourceAccess(FrameGraphPass* pass, const FrameGraphResourceAccess& access);

		/** Validates dependencies (Phase 2: checks for isolated passes, etc.) */
		bool ValidateDependencies();

		/** Validates render pass setup (Phase 3) */
		bool ValidateRenderPasses();

		/** Validates layouts (Phase 3) */
		bool ValidateLayouts(const CompiledFrameGraph& compiledGraph);

		//////////////////////////////////////////////////////////////////////////
		// Phase 2: Dependency Analysis & Scheduling
		//////////////////////////////////////////////////////////////////////////

		/** Analyzes resource dependencies and builds the dependency graph */
		bool AnalyzeDependencies();

		/** Analyzes which passes read/write which resources */
		void AnalyzeResourceAccess();

		/** Builds dependency edges (RAW, WAR, WAW) */
		void BuildDependencyGraph();

		/** Tracks resource lifetimes (first use, last use, etc.) */
		void TrackResourceLifetimes();

		/** Culls unused passes (reverse DFS from outputs) */
		void CullUnusedPasses();

		/** Marks a pass and all its dependencies as used */
		void MarkPassAsUsed(FrameGraphPass* pass);

		/** Topologically sorts passes using Kahn's algorithm */
		bool TopologicalSort(Vector<FrameGraphPass*>& outSortedPasses);

		//////////////////////////////////////////////////////////////////////////
		// Phase 3: Barrier Generation & Synchronization
		//////////////////////////////////////////////////////////////////////////

		/** Builds usage history for all resources */
		void BuildUsageHistory(const Vector<FrameGraphPass*>& passes);

		/** Records a single resource usage */
		void RecordUsage(
			FrameGraphResourceId resource,
			FrameGraphPass* pass,
			GpuResourceUseFlags usage,
			GpuAccessFlags access);

		/** Determines the appropriate layout for a resource usage */
		ImageLayout DetermineLayout(GpuResourceUseFlags usage, GpuAccessFlags access) const;

		/** Builds transitions from usage history */
		void BuildTransitions(Vector<ResourceTransition>& outTransitions);

		/** Builds transitions for a single resource */
		void BuildTransitionsForResource(
			const ResourceUsageHistory& history,
			Vector<ResourceTransition>& outTransitions);

		/** Checks if a barrier is needed between two consecutive uses */
		bool NeedsBarrier(const ResourceUsage& prevUsage, const ResourceUsage& currUsage) const;

		/** Builds GPU barriers from transitions */
		void BuildBarriers(
			const Vector<ResourceTransition>& transitions,
			Vector<FrameGraphBarrierBatch>& outBarriers);

		/** Creates a buffer barrier from a transition */
		TOptional<GpuBufferBarrier> CreateBufferBarrier(const ResourceTransition& transition);

		/** Creates a texture barrier from a transition */
		TOptional<GpuTextureBarrier> CreateTextureBarrier(const ResourceTransition& transition);

		/** Creates render targets for render passes */
		void CreateRenderTargets(
			const Vector<FrameGraphPass*>& passes,
			UnorderedMap<FrameGraphPass*, SPtr<RenderTarget>>& outRenderTargets);

		/** Builds a render target for a single render pass */
		SPtr<RenderTarget> BuildRenderTarget(FrameGraphPass* pass);

		//////////////////////////////////////////////////////////////////////////
		// Member Variables
		//////////////////////////////////////////////////////////////////////////

		FrameGraph& mFrameGraph;

		/** Map of resources to passes that write them (Phase 2) */
		UnorderedMap<FrameGraphResourceId, Vector<FrameGraphPass*>> mResourceWriters;

		/** Map of resources to passes that read them (Phase 2) */
		UnorderedMap<FrameGraphResourceId, Vector<FrameGraphPass*>> mResourceReaders;

		/** Resource lifetime tracking (Phase 2) */
		UnorderedMap<FrameGraphResourceId, FrameGraphResourceLifetime> mResourceLifetimes;

		/** Sorted pass execution order (Phase 2) */
		Vector<FrameGraphPass*> mSortedPasses;

		/** Number of culled passes (Phase 2) */
		u32 mCulledPassCount = 0;

		/** Passes involved in dependency cycles (Phase 2) */
		Vector<FrameGraphPass*> mCyclePasses;

		/** Resource usage histories (Phase 3) */
		UnorderedMap<FrameGraphResourceId, ResourceUsageHistory> mUsageHistories;
	};

	/** @} */
}
