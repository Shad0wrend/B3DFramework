//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Managers/BsRenderStateManager.h"
#include "RenderAPI/BsSamplerState.h"
#include "RenderAPI/BsDepthStencilState.h"

using namespace bs;

SPtr<SamplerState> RenderStateManager::CreateSamplerState(const SamplerStateCreateInformation& desc) const
{
	SPtr<SamplerState> state = CreateSamplerStatePtrInternal(desc);
	state->Initialize();

	return state;
}

SPtr<DepthStencilState> RenderStateManager::CreateDepthStencilState(const DEPTH_STENCIL_STATE_DESC& desc) const
{
	SPtr<DepthStencilState> state = CreateDepthStencilStatePtrInternal(desc);
	state->Initialize();

	return state;
}

SPtr<SamplerState> RenderStateManager::CreateSamplerStatePtrInternal(const SamplerStateCreateInformation& desc) const
{
	SPtr<SamplerState> samplerState = B3DMakeCoreFromExisting<SamplerState>(new(B3DAllocate<SamplerState>()) SamplerState(desc));
	samplerState->SetShared(samplerState);

	return samplerState;
}

SPtr<DepthStencilState> RenderStateManager::CreateDepthStencilStatePtrInternal(const DEPTH_STENCIL_STATE_DESC& desc) const
{
	SPtr<DepthStencilState> depthStencilState = B3DMakeCoreFromExisting<DepthStencilState>(new(B3DAllocate<DepthStencilState>()) DepthStencilState(desc));
	depthStencilState->SetShared(depthStencilState);

	return depthStencilState;
}

const SPtr<SamplerState>& RenderStateManager::GetDefaultSamplerState() const
{
	if(mDefaultSamplerState == nullptr)
		mDefaultSamplerState = CreateSamplerState(SamplerStateInformation());

	return mDefaultSamplerState;
}

const SPtr<DepthStencilState>& RenderStateManager::GetDefaultDepthStencilState() const
{
	if(mDefaultDepthStencilState == nullptr)
		mDefaultDepthStencilState = CreateDepthStencilState(DEPTH_STENCIL_STATE_DESC());

	return mDefaultDepthStencilState;
}

namespace bs { namespace ct
{
SPtr<SamplerState> RenderStateManager::CreateSamplerState(const SamplerStateCreateInformation& desc, GpuDeviceFlags deviceMask) const
{
	SPtr<SamplerState> state = FindCachedState(desc);
	if(state == nullptr)
	{
		state = CreateSamplerStateInternalInternal(desc, deviceMask);
		state->Initialize();

		NotifySamplerStateCreated(desc, state);
	}

	return state;
}

SPtr<DepthStencilState> RenderStateManager::CreateDepthStencilState(const DEPTH_STENCIL_STATE_DESC& desc) const
{
	u32 id = 0;
	SPtr<DepthStencilState> state = FindCachedState(desc, id);
	if(state == nullptr)
	{
		state = CreateDepthStencilStateInternalInternal(desc, id);
		state->Initialize();

		CachedDepthStencilState cachedData(id);
		cachedData.State = state;

		NotifyDepthStencilStateCreated(desc, cachedData);
	}

	return state;
}

SPtr<SamplerState> RenderStateManager::CreateSamplerStateInternal(const SamplerStateCreateInformation& desc, GpuDeviceFlags deviceMask) const
{
	SPtr<SamplerState> state = FindCachedState(desc);
	if(state == nullptr)
	{
		state = CreateSamplerStateInternalInternal(desc, deviceMask);

		NotifySamplerStateCreated(desc, state);
	}

	return state;
}

SPtr<DepthStencilState> RenderStateManager::CreateDepthStencilStateInternal(const DEPTH_STENCIL_STATE_DESC& desc) const
{
	u32 id = 0;
	SPtr<DepthStencilState> state = FindCachedState(desc, id);
	if(state == nullptr)
	{
		state = CreateDepthStencilStateInternalInternal(desc, id);

		CachedDepthStencilState cachedData(id);
		cachedData.State = state;

		NotifyDepthStencilStateCreated(desc, cachedData);
	}

	return state;
}

void RenderStateManager::OnShutDown()
{
	mDefaultDepthStencilState = nullptr;
	mDefaultSamplerState = nullptr;
}

const SPtr<SamplerState>& RenderStateManager::GetDefaultSamplerState() const
{
	if(mDefaultSamplerState == nullptr)
		mDefaultSamplerState = CreateSamplerState(SamplerStateInformation());

	return mDefaultSamplerState;
}

const SPtr<DepthStencilState>& RenderStateManager::GetDefaultDepthStencilState() const
{
	if(mDefaultDepthStencilState == nullptr)
		mDefaultDepthStencilState = CreateDepthStencilState(DEPTH_STENCIL_STATE_DESC());

	return mDefaultDepthStencilState;
}

void RenderStateManager::NotifySamplerStateCreated(const SamplerStateCreateInformation& desc, const SPtr<SamplerState>& state) const
{
	Lock lock(mMutex);

	mCachedSamplerStates[desc] = state;
}

void RenderStateManager::NotifyDepthStencilStateCreated(const DEPTH_STENCIL_STATE_DESC& desc, const CachedDepthStencilState& state) const
{
	Lock lock(mMutex);

	mCachedDepthStencilStates[desc] = state;
}

void RenderStateManager::NotifySamplerStateDestroyed(const SamplerStateInformation& desc) const
{
	Lock lock(mMutex);

	mCachedSamplerStates.erase(desc);
}

SPtr<SamplerState> RenderStateManager::FindCachedState(const SamplerStateInformation& desc) const
{
	Lock lock(mMutex);

	auto iterFind = mCachedSamplerStates.find(desc);
	if(iterFind != mCachedSamplerStates.end())
		return iterFind->second.lock();

	return nullptr;
}

SPtr<DepthStencilState> RenderStateManager::FindCachedState(const DEPTH_STENCIL_STATE_DESC& desc, u32& id) const
{
	Lock lock(mMutex);

	auto iterFind = mCachedDepthStencilStates.find(desc);
	if(iterFind != mCachedDepthStencilStates.end())
	{
		id = iterFind->second.Id;

		if(!iterFind->second.State.expired())
			return iterFind->second.State.lock();

		return nullptr;
	}

	id = mNextDepthStencilStateId++;
	B3D_ASSERT(id <= 0x3FF); // 10 bits maximum

	return nullptr;
}

SPtr<SamplerState> RenderStateManager::CreateSamplerStateInternalInternal(const SamplerStateCreateInformation& desc, GpuDeviceFlags deviceMask) const
{
	SPtr<SamplerState> state =
		B3DMakeSharedFromExisting<SamplerState>(new(B3DAllocate<SamplerState>()) SamplerState(desc, deviceMask));
	state->SetShared(state);

	return state;
}

SPtr<DepthStencilState> RenderStateManager::CreateDepthStencilStateInternalInternal(const DEPTH_STENCIL_STATE_DESC& desc, u32 id) const
{
	SPtr<DepthStencilState> state = B3DMakeSharedFromExisting<DepthStencilState>(new(B3DAllocate<DepthStencilState>()) DepthStencilState(desc, id));
	state->SetShared(state);

	return state;
}
}}
