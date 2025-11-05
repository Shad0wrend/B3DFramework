//************************************ B3D Framework - Copyright 2023 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DUtilityPrerequisites.h"
#include "B3DSignal.h"
#include "Debug/B3DDebug.h"

namespace b3d
{
	/** Provides an easy way to wait for N operations to complete executing. */
	class WaitGroup : INonCopyable
	{
	public:
		inline WaitGroup(u32 initialOperationCount = 0);

		/** Notifies the group that an operation completed executing. If anything is waiting for the wait group, it will be unblocked if the operation count reached 0. Returns true if operation count reached 0. */
		inline bool NotifyDone();

		/** Increments the operation count. */
		inline void Increment(u32 count = 1);

		/** Blocks the caller until the operation count reaches 0. */
		inline void Wait();

	private:
		struct Data
		{
			Mutex Mutex;
			Signal Signal;
			std::atomic<u32> OperationCount;
		};

		// Important to keep data as shared, so worker threads calling NotifyDone() maintain ownership until they exit.
		// Otherwise the thread waiting on the wait group could destroy the wait group (and the mutex) before the worker fully exits NotifyDone (i.e. while mutex is still locked in NotifyDone).
		const SPtr<Data> mData; 
	};

	WaitGroup::WaitGroup(u32 initialOperationCount)
		: mData(B3DMakeShared<Data>())
	{
		mData->OperationCount = initialOperationCount;
	}

	bool WaitGroup::NotifyDone()
	{
		const u32 newCount = --mData->OperationCount;
		B3D_ENSURE(newCount != std::numeric_limits<u32>::max()); // Overflow

		if(newCount == 0)
		{
			Lock lock(mData->Mutex);
			mData->Signal.NotifyAll();

			return true;
		}

		return false;
	}

	void WaitGroup::Increment(u32 count)
	{
		mData->OperationCount += count;
	}

	void WaitGroup::Wait()
	{
		Lock lock(mData->Mutex);

		mData->Signal.Wait(lock, [this] { return mData->OperationCount == 0; });
	}
}  // namespace b3d
