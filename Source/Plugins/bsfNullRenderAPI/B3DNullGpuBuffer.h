//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DNullPrerequisites.h"
#include "RenderAPI/B3DGpuBuffer.h"

namespace b3d
{
	namespace render
	{
		class NullGpuDevice;

		/** @addtogroup Null
		 *  @{
		 */

		/**	Null implementation of a GPU buffer. */
		class NullGpuBuffer : public GpuBuffer
		{
		public:
			NullGpuBuffer(NullGpuDevice& device, const GpuBufferCreateInformation& createInformation);
			~NullGpuBuffer();

			void SetName(const StringView& name) override { mName = name; }
			void ReadData(u32 offset, u32 length, void* destination, const SPtr<GpuQueue>& gpuQueue = nullptr) override {}
			void WriteData(u32 offset, u32 length, const void* source, BufferWriteType writeFlags = BWT_NORMAL, const SPtr<GpuCommandBuffer>& commandBuffer = nullptr) override {}
			void CopyData(GpuBuffer& srcBuffer, u32 srcOffset, u32 dstOffset, u32 length, bool discardWholeBuffer = false, const SPtr<GpuCommandBuffer>& commandBuffer = nullptr) override {}
			GpuQueueMask GetUseMask(GpuAccessFlags accessFlags) override { return GpuQueueMask::kNone; }
			u32 GetBoundCount() const override { return 0; }
			u32 GetUseCount() const override { return 0; }

		protected:
			friend class NullGpuDevice;

			void Initialize() override {}
			void* Map(u32 offset, u32 length, GpuLockOptions options) override;
			void Unmap() override {}
			void RecreateInternalBuffer() override {}

			void* mStagingBuffer = nullptr;
		};

		/** @} */
	} // namespace render
} // namespace b3d
