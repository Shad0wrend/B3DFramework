//************************************ B3D Framework - Copyright 2025 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DRenderBeastPrerequisites.h"
#include "RenderAPI/B3DGpuBufferPool.h"
#include "RenderAPI/B3DGpuParameterSet.h"
#include <variant>

namespace b3d::render
{
	struct RendererObject;

	/** @addtogroup RenderBeast
	 *  @{
	 */

	/**
	 * Manages renderable uniform buffer allocations. Objects whose suballocations come from the same underlying GpuBuffer
	 * share a GpuParameterSet, to be bound at set index 1.
	 */
	class RenderableUniformBufferManager
	{
	public:
		/** Type of allocation, determining which buffers are allocated. */
		enum class AllocationType
		{
			Normal,   /**< Just per-object buffer. */
			Decal     /**< Per-object buffer + decal parameter buffer. */
		};

		/** Opaque handle for a uniform buffer allocation. Used for releasing. */
		struct AllocationHandle
		{
			/** Checks if this handle is valid. */
			bool IsValid() const { return Index != kInvalidIndex; }

			/** Invalidates this handle. */
			void Invalidate() { Index = kInvalidIndex; }

			static constexpr u32 kInvalidIndex = ~0u;

		private:
			friend class RenderableUniformBufferManager;

			u32 Index = kInvalidIndex;
			AllocationType Type = AllocationType::Normal;
		};

		/** Extra suballocation for decal rendering. */
		struct DecalExtraSuballocation
		{
			GpuBufferSuballocation DecalSuballocation;
		};

		/** Variant holding optional extra suballocations. std::monostate represents no extras. */
		using ExtraSuballocations = std::variant<std::monostate, DecalExtraSuballocation>;

		/** Result of a uniform buffer allocation. */
		struct AllocationResult
		{
			AllocationHandle Handle;
			SPtr<GpuParameterSet> ParameterSet;
			GpuBufferSuballocation PerObjectSuballocation;
			ExtraSuballocations Extras;

			/** Checks if this result is valid. */
			bool IsValid() const { return Handle.IsValid(); }
		};

		RenderableUniformBufferManager() = default;
		~RenderableUniformBufferManager() = default;

		/**
		 * Initializes the manager. Must be called before use.
		 *
		 * @param device					GPU device for buffer creation.
		 * @param renderableParameterSetLayout	Layout for normal renderable per-object parameter sets.
		 * @param decalParameterSetLayout		Layout for decal per-object parameter sets.
		 */
		void Initialize(
			GpuDevice& device,
			const SPtr<GpuPipelineParameterSetLayout>& renderableParameterSetLayout,
			const SPtr<GpuPipelineParameterSetLayout>& decalParameterSetLayout);

		/**
		 * Allocates uniform buffer(s) for a renderable.
		 *
		 * @param type		Type indicating which buffers to allocate.
		 * @return			Result containing handle, parameter set, and suballocations.
		 */
		AllocationResult Allocate(AllocationType type = AllocationType::Normal);

		/**
		 * Releases a uniform buffer allocation.
		 *
		 * @param handle	Handle from a previous Allocate() call.
		 */
		void Release(AllocationHandle handle);

		/**
		 * Updates per-object buffer using transform data stored in a renderer object.
		 *
		 * @param object		Renderer object whose per-object buffer should be updated.
		 * @param commandBuffer	Command buffer to queue the copy on. If null, uses the transfer command buffer.
		 */
		void UpdatePerObjectBuffer(const RendererObject& object, const SPtr<GpuCommandBuffer>& commandBuffer = nullptr);

		/**
		 * Advances the staging pool frame counter. Call at end of each render frame.
		 */
		void AdvanceFrame();

	private:
		static constexpr u32 kRenderableEntriesPerBuffer = 1024;
		static constexpr u32 kDecalEntriesPerBuffer = 256;
		static constexpr u32 kStagingEntriesPerBuffer = 256;

		/** Composite key for parameter set lookup: (PerObjectBuffer, DecalBuffer or nullptr). */
		using BufferKey = std::pair<GpuBuffer*, GpuBuffer*>;

		/** Hash functor for BufferKey. */
		struct BufferKeyHash
		{
			size_t operator()(const BufferKey& key) const
			{
				size_t hash = 0;
				B3DCombineHash(hash, key.first);
				B3DCombineHash(hash, key.second);

				return hash;
			}
		};

		/** Tracks GpuParameterSets shared by objects using the same underlying buffer combination. */
		struct BufferParameterSetEntry
		{
			SPtr<GpuParameterSet> ParameterSet;
			u32 RefCount = 0;
		};

		/** Internal entry tracking a single allocation. */
		struct AllocationEntry
		{
			GpuBufferSuballocation PerObjectSuballocation;
			GpuBufferSuballocation DecalSuballocation;
			AllocationType Type = AllocationType::Normal;
			u32 NextFreeIndex = ~0u;
		};

		/**
		 * Gets or creates a shared GpuParameterSet for the given buffer combination.
		 *
		 * @param perObjectBuffer	The per-object uniform buffer.
		 * @param decalBuffer		The decal parameter buffer, or nullptr for non-decals.
		 * @return					Shared GpuParameterSet for set #1.
		 */
		SPtr<GpuParameterSet> GetOrCreateParameterSet(const SPtr<GpuBuffer>& perObjectBuffer, const SPtr<GpuBuffer>& decalBuffer);

		/**
		 * Decrements ref count for a buffer combination's shared parameter set, removing if zero.
		 *
		 * @param perObjectBuffer	The per-object uniform buffer.
		 * @param decalBuffer		The decal parameter buffer, or nullptr for non-decals.
		 */
		void ReleaseParameterSet(const SPtr<GpuBuffer>& perObjectBuffer, const SPtr<GpuBuffer>& decalBuffer);

		GpuBufferPool mRenderablePool;
		GpuBufferPool mDecalPool;
		TransientGpuBufferPool mStagingPool;
		UnorderedMap<BufferKey, BufferParameterSetEntry, BufferKeyHash> mParameterSetsByBuffer;
		SPtr<GpuPipelineParameterSetLayout> mRenderableParameterSetLayout;
		SPtr<GpuPipelineParameterSetLayout> mDecalParameterSetLayout;
		GpuDevice* mDevice = nullptr;

		/** Pool of allocation entries indexed by handle. */
		Vector<AllocationEntry> mAllocationEntries;

		/** Head of free-list for reusing released entries. */
		u32 mFreeListHead = ~0u;
	};

	/** @} */
}
