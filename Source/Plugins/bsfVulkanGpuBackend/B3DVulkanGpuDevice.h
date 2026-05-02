//************************************* B3D Framework - Copyright 2026 Marko Pintera *************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DVulkanBuiltinResources.h"
#include "B3DVulkanPrerequisites.h"
#include "B3DVulkanHeapBackend.h"
#include "Managers/B3DVulkanDescriptorManager.h"
#include "GpuBackend/B3DGpuCommandBuffer.h"
#include "GpuBackend/B3DGpuDevice.h"
#include "GpuBackend/B3DGpuDeviceCapabilities.h"
#include "GpuBackend/B3DGpuBackend.h"
#include "GpuBackend/Allocators/B3DGpuAllocator.h"
#include "GpuBackend/Allocators/B3DGpuTlsfAllocator.h"

namespace b3d
{
	class VulkanGpuBackend;
	class VulkanGpuTimelineFence;

	namespace render
	{
		/** @addtogroup Vulkan
		 *  @{
		 */

		/** Contains format describing a Vulkan surface. */
		struct SurfaceFormat
		{
			VkFormat ColorFormat;
			VkFormat DepthFormat;
			VkColorSpaceKHR ColorSpace;
		};

		/**
		 * Result from the allocation functions. Wraps a GPU location (offset within a VkDeviceMemory heap)
		 * plus an optional persistent map. MappedMemory is non-null when the allocation lives in a
		 * persistently-mapped, host-visible heap and points to the start of the allocation's memory range.
		 */
		struct VulkanAllocationResult
		{
			TGpuResourceLocation<VulkanHeapBackend> Location; /**< Allocator slot — heap, offset, size, allocator-private bookkeeping. */
			void* MappedMemory = nullptr; /**< Heap.Mapped + Location.Offset for host-visible heaps; null otherwise. */

			/** Returns true once the allocator has populated this result with a live slot. */
			bool IsValid() const { return Location.IsValid(); }
		};

		/** Represents a single GPU device usable by Vulkan. */
		class VulkanGpuDevice : public GpuDevice
		{
		public:
#if B3D_PLATFORM_MACOS
			static constexpr const char* kGpuProgramLanguageName = kGpuProgramLanguageMvksl;
#else
			static constexpr const char* kGpuProgramLanguageName = kGpuProgramLanguageVksl;
#endif


			VulkanGpuDevice(VkPhysicalDevice device);
			~VulkanGpuDevice();

			/**
			 * @name GpuDevice Interface
			 * @{
			 */

			bool IsInitialized() const override { return true; }
			bool Initialize() override { return true; } // Initialized on construction

			const GpuDeviceCapabilities& GetCapabilities() const override { return mCapabilities; }
			const VideoModeInfo& GetVideoModeInfo() const override { return *mVideoModeInfo; }

			bool IsGpuProgramLanguageSupported(const StringView& language) const override { return language == kGpuProgramLanguageName; }
			SPtr<GpuProgramBytecode> CompileGpuProgramBytecode(const GpuProgramCreateInformation& createInformation) const override;

			u32 GetQueueCount(GpuQueueType type) const override { return (u32)mQueueInfos[(u32)type].Queues.size(); }
			SPtr<GpuQueue> GetQueue(GpuQueueType type, u32 index) const override;
			void SubmitTransferCommandBuffers(bool wait = false) override;
			void PresentRenderWindow(const SPtr<RenderWindow>& renderWindow, GpuQueueMask syncMask = GpuQueueMask::kAll) override;
			void WaitUntilIdle() override;
			void BeginFrame() override;
			void EndFrame() override;

			SPtr<GpuCommandBufferPool> CreateGpuCommandBufferPool(const GpuCommandBufferPoolCreateInformation& createInformation) override;
			SPtr<Texture> CreateTexture(const TextureCreateInformation& createInformation, GpuObjectCreateFlags flags = GpuObjectCreateFlag::None) override;
			SPtr<GpuBuffer> CreateGpuBuffer(const GpuBufferCreateInformation& createInformation, GpuObjectCreateFlags flags = GpuObjectCreateFlag::None) override;
			SPtr<GpuQueryPool> CreateQueryPool(const GpuQueryPoolCreateInformation& createInformation) override;
			SPtr<EventQuery> CreateEventQuery() override;
			SPtr<GpuProgram> CreateGpuProgram(const GpuProgramCreateInformation& createInformation, GpuObjectCreateFlags flags = GpuObjectCreateFlag::None) override;
			SPtr<GpuGraphicsPipelineState> CreateGpuGraphicsPipelineState(const GpuGraphicsPipelineStateCreateInformation& createInformation, GpuObjectCreateFlags flags = GpuObjectCreateFlag::None) override;
			SPtr<GpuComputePipelineState> CreateGpuComputePipelineState(const GpuComputePipelineStateCreateInformation& createInformation, GpuObjectCreateFlags flags = GpuObjectCreateFlag::None) override;
			SPtr<GpuPipelineParameterLayout> CreateGpuPipelineParameterLayout(const GpuPipelineParameterLayoutCreateInformation& createInformation) override;
			SPtr<GpuPipelineParameterSetLayout> CreateGpuPipelineParameterSetLayout(const GpuProgramParameterDescription& parameterDescription) override;
			UPtr<GpuParameterSetPool> CreateParameterSetPool(const GpuParameterSetPoolCreateInformation& createInformation) override;
			SPtr<GpuTimelineFence> CreateTimelineFence() override;

			/** Checks if submission with the specified index has finished executing on the GPU. This index is returned by SubmitCommandBuffer. */
			bool IsSubmissionComplete(u64 index) const override;

			void ConvertProjectionMatrix(const Matrix4& input, Matrix4& output) override;
			GpuUniformBufferInformation GenerateUniformBufferInformation(const String& name, TArray<GpuUniformBufferMemberInformation>& inOutUniforms) override;
			float ConvertTimestampToMilliseconds(u64 timestamp) override;

			/** @} */

			/** Returns an object describing the physical properties of the device. */
			VkPhysicalDevice GetPhysical() const { return mPhysicalDevice; }

			/** Returns an object describing the logical properties of the device. */
			VkDevice GetLogical() const { return mLogicalDevice; }

			/** Returns true if the device is one of the primary GPU's. */
			bool IsPrimary() const { return mIsPrimary; }

			/** Returns a set of properties describing the physical device. */
			const VkPhysicalDeviceProperties& GetDeviceProperties() const { return mDeviceProperties; }

			/** Returns a set of features that the application can use to check if a specific feature is supported. */
			const VkPhysicalDeviceFeatures& GetDeviceFeatures() const { return mDeviceFeatures; }

			/** Returns a set of properties describing the memory of the physical device. */
			const VkPhysicalDeviceMemoryProperties& GetMemoryProperties() const { return mMemoryProperties; }

			/**
			 * Returns index of the queue family for the specified queue type. Returns -1 if no queues for the specified type
			 * exist. There will always be a queue family for the graphics type.
			 */
			u32 GetQueueFamily(GpuQueueType type) const { return mQueueInfos[(int)type].FamilyIndex; }

			/** Perform an operation for each queue on the device. */
			void DoForEachQueue(const std::function<void(VulkanGpuQueue&)>&& callback) const;

			/** Returns the best matching surface format according to the provided parameters. */
			SurfaceFormat GetSurfaceFormat(const VkSurfaceKHR& surface, bool useHardwareSRGB) const;

			/** Returns a manager that can be used for allocating descriptor layouts and sets. */
			VulkanDescriptorManager& GetDescriptorManager() const { return *mDescriptorManager; }

			/** Returns a manager that can be used for allocating Vulkan objects wrapped as managed resources. */
			VulkanResourceManager& GetResourceManager() const { return *mResourceManager; }

			/** Returns a set of resources that are always available. */
			const VulkanBuiltinResources& GetBuiltinResources() const { return mBuiltinResources;  }

			/**
			 * Returns a set of command buffer semaphores depending on the provided sync mask.
			 *
			 * @param	syncMask		Mask that has a bit enabled for each queue to retrieve the semaphore for.
			 *							If a command buffer on a queue is not currently executing, semaphore won't be returned.
			 * @param	outSemaphores	Array into which all required semaphores will be appended to. 
			 *
			 * @note	Submit thread only.
			 */
			void GetSyncSemaphores(GpuQueueMask syncMask, TInlineArray<VulkanSemaphore*, 8> outSemaphores) const;

			/**
			 * @name Memory Allocation
			 * @{
			 */

			/**
			 * Allocates memory for @p image and binds it. Picks the best memory type satisfying @p requiredFlags
			 * and (where possible) the @p preferredFlags hint, then suballocates from the per-memory-type
			 * allocator. @p kind controls buffer-image granularity placement: Non-linear for optimally-tiled 
			 * images, linear for linearly-tiled images and buffers.
			 *
			 * TODO: TLSF allocator not currently thread safe, VMA used to be
			 *
			 * Thread safe.
			 */
			VulkanAllocationResult AllocateMemory(VkImage image, VkMemoryPropertyFlags requiredFlags, VkMemoryPropertyFlags preferredFlags, GpuResourceKind kind);

			/**
			 * Allocates memory for @p buffer and binds it. Picks the best memory type satisfying @p requiredFlags
			 * and (where possible) the @p preferredFlags hint, then suballocates from the per-memory-type allocator.
			 *
			 * Thread safe.
			 */
			VulkanAllocationResult AllocateMemory(VkBuffer buffer, VkMemoryPropertyFlags requiredFlags, VkMemoryPropertyFlags preferredFlags);

			/**
			 * Returns @p allocation to its allocator's free pool synchronously. The slot becomes
			 * immediately available for reuse on the very next AllocateMemory call.
			 *
			 * Caller must guarantee the GPU is no longer using the underlying memory range.
			 *
			 * Thread safe.
			 */
			void FreeMemory(VulkanAllocationResult& allocation);

			/**
			 * Returns the persistent CPU pointer for @p allocation, offset by @p offset bytes. The allocation must
			 * live in a host-visible memory type (heaps for those types are persistently mapped on creation).
			 *
			 * Thread safe.
			 */
			u8* MapMemory(const VulkanAllocationResult& allocation, VkDeviceSize offset = 0) const;

			/**
			 * No-op for persistently-mapped heaps; retained for symmetry with @c MapMemory.
			 *
			 * Thread safe.
			 */
			void UnmapMemory(const VulkanAllocationResult& allocation) const;

			/**
			 * Invalidates @p [offset, offset+size) within @p allocation so subsequent CPU reads observe GPU writes.
			 * Only relevant for non-coherent memory.
			 *
			 * Thread safe.
			 */
			void InvalidateMemory(const VulkanAllocationResult& allocation, VkDeviceSize offset = 0, VkDeviceSize size = VK_WHOLE_SIZE) const;

			/**
			 * Flushes @p [offset, offset+size) within @p allocation so subsequent GPU reads observe CPU writes.
			 * Only relevant for non-coherent memory.
			 *
			 * Thread safe.
			 */
			void FlushMemory(const VulkanAllocationResult& allocation, VkDeviceSize offset = 0, VkDeviceSize size = VK_WHOLE_SIZE) const;

			/** @} */

			/** Returns @c true if timeline semaphores are available on this device. */
			bool SupportsTimelineSemaphores() const { return mSupportsTimelineSemaphore; }

			/** Returns the device heap backend. */
			VulkanHeapBackend& GetHeapBackend() const { return *mHeapBackend; }

		private:
			friend class b3d::VulkanGpuBackend;

			static constexpr u32 kQueueUsageCombinationCount = 8; // 3^2, as there are three usage types in CommandBufferUsageFlag

			SPtr<SamplerState> CreateSamplerState(const SamplerStateCreateInformation& createInformation, GpuObjectCreateFlags flags = GpuObjectCreateFlag::None) override;

			/** Initializes the capabilities of the device. */
			void InitializeCapabilities();

			/**
			 * Picks a memory-type index satisfying @p typeBits and the @p required flags, with a preference
			 * scoring against @p preferred. Returns VK_MAX_MEMORY_TYPES on failure.
			 */
			u32 PickMemoryTypeIndex(u32 typeBits, VkMemoryPropertyFlags required, VkMemoryPropertyFlags preferred) const;

			/**
			 * Returns the GPU memory allocator backing memory type @p memoryTypeIndex, lazily creating it on
			 * first use. Each allocator owns its own VkDeviceMemory heaps via the shared VulkanHeapBackend
			 * and is locked to a single memory-type index.
			 */
			TGpuTlsfAllocator<VulkanHeapBackend>& GetOrCreateGpuMemoryAllocator(u32 memoryTypeIndex);

			/** Marks the device as a primary device. */
			void SetIsPrimary() { mIsPrimary = true; }

			VkPhysicalDevice mPhysicalDevice;
			VkDevice mLogicalDevice = nullptr;
			bool mIsPrimary = false;

			VulkanDescriptorManager* mDescriptorManager;
			VulkanResourceManager* mResourceManager;
			VulkanBuiltinResources mBuiltinResources;

			VkPhysicalDeviceProperties mDeviceProperties;
			VkPhysicalDeviceFeatures mDeviceFeatures;
			VkPhysicalDeviceMemoryProperties mMemoryProperties;

			/** Contains data about a set of queues of a specific type. */
			struct QueueInfo
			{
				u32 FamilyIndex = ~0u;
				Vector<SPtr<VulkanGpuQueue>> Queues;
			};

			QueueInfo mQueueInfos[GQT_COUNT];
			GpuDeviceCapabilities mCapabilities;
			SPtr<VideoModeInfo> mVideoModeInfo;

			bool mSupportsTimelineSemaphore = false;
			UPtr<VulkanHeapBackend> mHeapBackend;

			/** Per-memory-type TLSF allocator pool. Slots are lazily populated on first allocation. */
			UPtr<TGpuTlsfAllocator<VulkanHeapBackend>> mGpuMemoryAllocators[VK_MAX_MEMORY_TYPES];

			/** Guards lazy creation of @c mTlsfAllocators entries. */
			mutable Mutex mGpuMemoryAllocatorMutex;
		};

		/** @} */
	} // namespace render
} // namespace b3d
