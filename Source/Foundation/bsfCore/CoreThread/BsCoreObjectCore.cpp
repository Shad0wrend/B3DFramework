//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "CoreThread/BsCoreObjectCore.h"
#include "CoreThread/BsCoreThread.h"

using namespace bs;

namespace bs { namespace ct
{
Signal CoreObject::mCoreGpuObjectLoadedCondition;
Mutex CoreObject::mCoreGpuObjectLoadedMutex;

CoreObject::CoreObject()
	: mFlags(0)
{}

CoreObject::~CoreObject()
{ }

void CoreObject::Initialize()
{
	{
		Lock lock(mCoreGpuObjectLoadedMutex);
		SetIsInitialized(true);
	}

	SetScheduledToBeInitialized(false);

	mCoreGpuObjectLoadedCondition.NotifyAll();
}

void CoreObject::Synchronize()
{
	if(!IsInitialized())
	{
#if B3D_DEBUG
		if(B3D_CURRENT_THREAD_ID == CoreThread::Instance().GetCoreThreadId())
			B3D_EXCEPT(InternalErrorException, "You cannot call this method on the core thread. It will cause a deadlock!");
#endif

		GetCoreThread().PostCommand([] {}, true);

		Lock lock(mCoreGpuObjectLoadedMutex);
		if(!IsInitialized() && !IsScheduledToBeInitialized())
			B3D_EXCEPT(InternalErrorException, "Attempting to wait until initialization finishes but object is not scheduled to be initialized.");

		mCoreGpuObjectLoadedCondition.Wait(lock, [this] { return IsInitialized(); });
	}
}

void CoreObject::SetShared(SPtr<CoreObject> sharedToThis)
{
	mThis = sharedToThis;
}
}}
