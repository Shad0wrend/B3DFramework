//************************************ B3D Framework - Copyright 2025 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DPrerequisites.h"
#include "B3DFrameGraphTypes.h"
#include "RenderAPI/B3DGpuDevice.h"

namespace b3d::render
{
	class FrameGraphPass;

	/** @addtogroup RenderAPI
	 *  @{
	 */

	/**
	 * Represents a dependency edge between two passes in the frame graph.
	 * Dependencies are determined by resource usage (read-after-write, write-after-read, write-after-write).
	 */
	struct FrameGraphPassDependency
	{
		/** The pass that must execute before this dependency is satisfied */
		FrameGraphPass* ProducerPass = nullptr;

		/** The pass that depends on the producer pass */
		FrameGraphPass* ConsumerPass = nullptr;

		/** The resource that creates this dependency */
		FrameGraphResourceId Resource;

		/** Type of dependency (read-after-write, write-after-read, write-after-write) */
		enum class Type
		{
			/** Consumer reads after producer writes (true dependency) */
			ReadAfterWrite,

			/** Consumer writes after producer reads (anti-dependency) */
			WriteAfterRead,

			/** Consumer writes after producer writes (output dependency) */
			WriteAfterWrite
		};

		Type DependencyType = Type::ReadAfterWrite;
	};

	/**
	 * Represents a pass node in the dependency graph.
	 * Tracks incoming and outgoing dependencies for topological sorting.
	 */
	class B3D_EXPORT FrameGraphPassNode
	{
	public:
		/** Constructor */
		explicit FrameGraphPassNode(FrameGraphPass* pass);

		/** Get the pass associated with this node */
		FrameGraphPass* GetPass() const { return mPass; }

		/** Add an incoming dependency (a pass that must execute before this one) */
		void AddIncomingDependency(const FrameGraphPassDependency& dependency);

		/** Add an outgoing dependency (a pass that depends on this one) */
		void AddOutgoingDependency(const FrameGraphPassDependency& dependency);

		/** Get all incoming dependencies */
		const Vector<FrameGraphPassDependency>& GetIncomingDependencies() const { return mIncomingDependencies; }

		/** Get all outgoing dependencies */
		const Vector<FrameGraphPassDependency>& GetOutgoingDependencies() const { return mOutgoingDependencies; }

		/** Get the reference count (number of unresolved incoming dependencies) */
		u32 GetReferenceCount() const { return mReferenceCount; }

		/** Set the reference count */
		void SetReferenceCount(u32 count) { mReferenceCount = count; }

		/** Decrement the reference count */
		void DecrementReferenceCount() { mReferenceCount--; }

		/** Check if this node is ready to execute (all dependencies satisfied) */
		bool IsReady() const { return mReferenceCount == 0; }

		/** Mark this node as culled (will not execute) */
		void SetCulled(bool culled) { mCulled = culled; }

		/** Check if this node is culled */
		bool IsCulled() const { return mCulled; }

		/** Reset for next frame */
		void Reset();

	private:
		/** The pass this node represents */
		FrameGraphPass* mPass = nullptr;

		/** Dependencies where this pass is the consumer (must wait for these) */
		Vector<FrameGraphPassDependency> mIncomingDependencies;

		/** Dependencies where this pass is the producer (others wait for this) */
		Vector<FrameGraphPassDependency> mOutgoingDependencies;

		/** Number of unresolved incoming dependencies (for topological sort) */
		u32 mReferenceCount = 0;

		/** Whether this pass has been culled (not needed for final output) */
		bool mCulled = false;
	};

	/** @} */
}
