//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "RenderAPI/BsGenericGpuBuffer.h"
#include "Error/BsException.h"
#include "RenderAPI/BsRenderAPI.h"
#include "Managers/BsHardwareBufferManager.h"
#include "Profiling/BsRenderStats.h"

using namespace bs;

u32 GetBufferSize(const GenericGpuBufferCreateInformation& desc)
{
	u32 elementSize;

	if(desc.Type == GBT_STANDARD)
		elementSize = GpuBuffer::GetFormatSize(desc.Format);
	else
		elementSize = desc.ElementSize;

	return elementSize * desc.ElementCount;
}

static GpuBufferCreateInformation GetCreateInformation(const GenericGpuBufferCreateInformation& createInformation)
{
	GpuBufferCreateInformation output;
	output.Type = createInformation.Type == GBT_STANDARD ? GpuBufferType::SimpleStorage : GpuBufferType::StructuredStorage;
	output.Flags = createInformation.Flags;

	if(output.Type == GpuBufferType::SimpleStorage)
	{
		output.SimpleStorage.Format = createInformation.Format;
		output.SimpleStorage.Count = createInformation.ElementCount;
	}
	else
	{
		output.StructuredStorage.ElementSize = createInformation.ElementSize;
		output.StructuredStorage.Count = createInformation.ElementCount;
	}

	return output;
}

GenericGpuBufferProperties::GenericGpuBufferProperties(const GenericGpuBufferCreateInformation& desc)
	: mDesc(desc)
{
	if(mDesc.Type == GBT_STANDARD)
		mDesc.ElementSize = GpuBuffer::GetFormatSize(mDesc.Format);
}

GenericGpuBuffer::GenericGpuBuffer(const GenericGpuBufferCreateInformation& desc)
	: mProperties(desc)
{
}

SPtr<ct::GenericGpuBuffer> GenericGpuBuffer::GetCore() const
{
	return std::static_pointer_cast<ct::GenericGpuBuffer>(mCoreSpecific);
}

SPtr<ct::CoreObject> GenericGpuBuffer::CreateCore() const
{
	return ct::HardwareBufferManager::Instance().CreateGpuBufferInternal(mProperties.mDesc);
}

SPtr<GenericGpuBuffer> GenericGpuBuffer::Create(const GenericGpuBufferCreateInformation& desc)
{
	return HardwareBufferManager::Instance().CreateGpuBuffer(desc);
}

namespace bs { namespace ct
{
GenericGpuBuffer::GenericGpuBuffer(const GenericGpuBufferCreateInformation& desc, GpuDeviceFlags deviceMask)
	: GpuBuffer(GetCreateInformation(desc)), mProperties(desc)
{
	if(desc.Type != GBT_STANDARD)
		B3D_ASSERT(desc.Format == BF_UNKNOWN && "Format must be set to BF_UNKNOWN when using non-standard buffers");
	else
		B3D_ASSERT(desc.ElementSize == 0 && "No element size can be provided for standard buffer. Size is determined from format.");
}

GenericGpuBuffer::GenericGpuBuffer(const GenericGpuBufferCreateInformation& desc, SPtr<GpuBuffer> underlyingBuffer)
	: GpuBuffer(GetCreateInformation(desc)), mProperties(desc), mBuffer(underlyingBuffer.get()), mSharedBuffer(std::move(underlyingBuffer)), mIsExternalBuffer(true)
{
	const auto& props = GetProperties();
	B3D_ASSERT(mSharedBuffer->GetSize() == (props.GetElementCount() * props.GetElementSize()));

	if(desc.Type != GBT_STANDARD)
		B3D_ASSERT(desc.Format == BF_UNKNOWN && "Format must be set to BF_UNKNOWN when using non-standard buffers");
	else
		B3D_ASSERT(desc.ElementSize == 0 && "No element size can be provided for standard buffer. Size is determined from format.");
}

GenericGpuBuffer::~GenericGpuBuffer()
{
	B3D_INCREMENT_RENDER_STATISTIC_CATEGORY(ResDestroyed, RenderStatObject_GpuBuffer);

	if(mBuffer && !mSharedBuffer)
		mBufferDeleter(mBuffer);
}

void GenericGpuBuffer::Initialize()
{
	B3D_INCREMENT_RENDER_STATISTIC_CATEGORY(ResCreated, RenderStatObject_GpuBuffer);
	CoreObject::Initialize();
}

void* GenericGpuBuffer::Map(u32 offset, u32 length, GpuLockOptions options, u32 deviceIdx, u32 queueIdx)
{
#if B3D_PROFILING_ENABLED
	if(options == GBL_READ_ONLY || options == GBL_READ_WRITE)
	{
		B3D_INCREMENT_RENDER_STATISTIC_CATEGORY(ResRead, RenderStatObject_GpuBuffer);
	}

	if(options == GBL_READ_WRITE || options == GBL_WRITE_ONLY || options == GBL_WRITE_ONLY_DISCARD || options == GBL_WRITE_ONLY_NO_OVERWRITE)
	{
		B3D_INCREMENT_RENDER_STATISTIC_CATEGORY(ResWrite, RenderStatObject_GpuBuffer);
	}
#endif

	return mBuffer->Lock(offset, length, options, deviceIdx, queueIdx);
}

void GenericGpuBuffer::Unmap()
{
	mBuffer->Unlock();
}

void GenericGpuBuffer::ReadData(u32 offset, u32 length, void* dest, u32 deviceIdx, u32 queueIdx)
{
	B3D_INCREMENT_RENDER_STATISTIC_CATEGORY(ResRead, RenderStatObject_GpuBuffer);

	mBuffer->ReadData(offset, length, dest, deviceIdx, queueIdx);
}

void GenericGpuBuffer::WriteData(u32 offset, u32 length, const void* source, BufferWriteType writeFlags, u32 queueIdx)
{
	B3D_INCREMENT_RENDER_STATISTIC_CATEGORY(ResWrite, RenderStatObject_GpuBuffer);

	mBuffer->WriteData(offset, length, source, writeFlags, queueIdx);
}

void GenericGpuBuffer::CopyData(GpuBuffer& srcBuffer, u32 srcOffset, u32 dstOffset, u32 length, bool discardWholeBuffer, const SPtr<CommandBuffer>& commandBuffer)
{
	auto& srcGpuBuffer = static_cast<GenericGpuBuffer&>(srcBuffer);
	mBuffer->CopyData(*srcGpuBuffer.mBuffer, srcOffset, dstOffset, length, discardWholeBuffer, commandBuffer);
}

SPtr<GenericGpuBuffer> GenericGpuBuffer::GetView(GenericGpuBufferType type, GpuBufferFormat format, u32 elementSize)
{
	const u32 elemSize = type == GBT_STANDARD ? bs::GpuBuffer::GetFormatSize(format) : elementSize;
	if((mBuffer->GetSize() % elemSize) != 0)
	{
		B3D_LOG(Error, RenderBackend, "Size of the buffer isn't divisible by individual element size provided for the buffer view.");
		return nullptr;
	}

	GenericGpuBufferCreateInformation desc;
	desc.Type = type;
	desc.Format = format;
	desc.Flags = mInformation.Flags;
	desc.ElementSize = elementSize;
	desc.ElementCount = mBuffer->GetSize() / elemSize;

	if(!mSharedBuffer)
	{
		mSharedBuffer = B3DMakeSharedFromExisting(mBuffer, mBufferDeleter);
		mIsExternalBuffer = false;
	}

	SPtr<GenericGpuBuffer> newView = Create(desc, mSharedBuffer);
	return newView;
}

SPtr<GenericGpuBuffer> GenericGpuBuffer::Create(const GenericGpuBufferCreateInformation& desc, GpuDeviceFlags deviceMask)
{
	return HardwareBufferManager::Instance().CreateGpuBuffer(desc, deviceMask);
}

SPtr<GenericGpuBuffer> GenericGpuBuffer::Create(const GenericGpuBufferCreateInformation& desc, SPtr<GpuBuffer> underlyingBuffer)
{
	return HardwareBufferManager::Instance().CreateGpuBuffer(desc, std::move(underlyingBuffer));
}
}}
