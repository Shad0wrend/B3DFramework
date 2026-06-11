//************************************* B3D Framework - Copyright 2026 Marko Pintera *************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "B3DGpuDevice.h"
#include "B3DGpuCommandBuffer.h"
#include "CoreObject/B3DRenderThread.h"
#include "Image/B3DTexture.h"
#include "GpuBackend/B3DGpuBuffer.h"
#include "GpuBackend/Allocators/B3DGpuResource.h"

using namespace b3d;

GpuDevice::GpuDevice()
	: mPrimaryContext(GpuWorkContext::Create(*this, mPrimaryTracker))
{
}

GpuWorkContext& GpuDevice::GetPrimaryContext()
{
	EnsureRenderThread();
	return *mPrimaryContext;
}

TUnique<IGpuAllocator> GpuDevice::CreateTransientAllocator(u32 /*memoryType*/, IGpuCompletionTracker& /*completionTracker*/)
{
	// Default: context-owned transient allocation is unsupported. Backends that support it override this.
	return nullptr;
}

TShared<render::GpuBuffer> GpuDevice::CreateGpuBuffer(const GpuBufferCreateInformation& /*createInformation*/, IGpuAllocator& /*allocator*/, GpuObjectCreateFlags /*flags*/)
{
	B3D_ENSURE_LOG(false, "This backend does not support allocator-driven buffer creation.");
	return nullptr;
}

void GpuDevice::ShutdownPrimaryContext()
{
	// The primary context's transfer pool is created on the render thread, so its destructor must run
	// there. Device teardown usually runs on the main thread (with the render thread still alive), so
	// marshal the release (which destroys the context) onto the render thread.
	if (B3D_CURRENT_THREAD_ID == RenderThread::Instance().GetThreadId())
		mPrimaryContext.reset();
	else
		GetRenderThread().PostCommand([this] { mPrimaryContext.reset(); }, "Shutdown primary GpuWorkContext", true);
}

bool GpuFrameCompletionTracker::IsMarkerComplete(u64 marker) const
{
	const u64 currentFrame = mFrameIndex.load(std::memory_order_acquire);
	return marker + RenderThread::kMaximumFramesInFlight <= currentFrame;
}

TShared<SamplerState> GpuDevice::FindOrCreateSamplerState(const SamplerStateCreateInformation& createInformation)
{
	Lock lock(mSamplerStateMutex);

	if (auto found = mCachedSamplerStates.find(createInformation); found != mCachedSamplerStates.end())
	{
		TShared<SamplerState> existingSamplerState = found->second;
		if (existingSamplerState != nullptr)
			return existingSamplerState;
	}

	TShared<SamplerState> newSamplerState = CreateSamplerState(createInformation);
	mCachedSamplerStates[createInformation] = newSamplerState;

	return newSamplerState;
}

