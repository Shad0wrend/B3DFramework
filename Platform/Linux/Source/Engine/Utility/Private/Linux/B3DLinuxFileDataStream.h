//************************************* B3D Framework - Copyright 2026 Marko Pintera *************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DUtilityPrerequisites.h"
#include "Private/Unix/B3DUnixFileDataStream.h"
#include "Threading/B3DThreading.h"
#include "Utility/B3DModule.h"

#include <atomic>
#include <liburing.h>

namespace b3d
{
	/** @addtogroup Filesystem
	 *  @{
	 */

	class LinuxFileDataStream;

	/**
	 * Process-wide owner of the shared io_uring instance that backs every LinuxFileDataStream's async reads.
	 *
	 * Threading contract:
	 *   - The reaper thread is the single owner of the ring. Only the reaper touches the submission & completion queues (SQE/CQE).
	 *   - SubmitRead() is safe from any thread: it pushes the request onto an MPSC queue under mSubmitMutex and signals
	 *     the eventfd, then returns. The reaper drains the queue on its next wake and issues the SQEs itself.
	 *   - The reaper signals completion by calling LinuxFileDataStream::OnAsyncReadComplete() on the owning stream,
	 *     which the stream uses to release its outstanding-read count and wake any Close()-side drain.
	 */
	class LinuxFileIOManager final : public Module<LinuxFileIOManager>
	{
	public:
		/** State for a single in-flight async read. Manager-owned; lifetime: SubmitRead() through FinalizeRequest(). */
		struct AsyncReadRequest
		{
			LinuxFileDataStream* Stream = nullptr; /**< Owning stream; receives OnAsyncReadComplete() after finalize. */
			int Fd = -1;
			void* Buffer = nullptr;
			bool OwnsBuffer = false;
			u64 BaseOffset = 0; /**< File offset where the read began. */
			u64 TotalByteCount = 0; /**< Total bytes to read (already clamped to EOF by the caller). */
			u64 BytesRead = 0; /**< Bytes accumulated so far across all chunks. */
			TAsyncOp<TShared<MemoryDataStream>> Op;
		};

		/** True iff OnStartUp() successfully brought the ring online and the reaper is reaping. */
		bool IsRunning() const { return mRunning.load(std::memory_order_acquire); }

		/**
		 * Submits an async read request to the shared ring. Takes ownership of @p request; the manager finalizes,
		 * completes the AsyncOp, and B3DDeletes the request when the read finishes (or fails). Safe from any thread.
		 *
		 * If the manager is not running (e.g. shutting down), the request is finalized inline as a failure - the
		 * stream's OnAsyncReadComplete() still runs so the outstanding-read counter is consistent.
		 */
		void SubmitRead(AsyncReadRequest* request);

	protected:
		/** Brings up the ring + eventfd + reaper thread. Logs Warning and leaves IsRunning() == false on any failure. */
		void OnStartUp() override;

		/** Signals the reaper to stop, waits for it to exit, tears down the ring + eventfd. */
		void OnShutDown() override;

	private:
		/** Reaper thread entry point: waits on the ring, reaps CQEs, drains the pending queue, chains chunks. */
		void ReaperThreadMain();

		/** Submits one chunk for an in-flight request. Returns true on successful submit. */
		bool SubmitChunk(AsyncReadRequest* request);

		/** Builds the result, completes the AsyncOp, notifies the owning stream, deletes the request. */
		void FinalizeRequest(AsyncReadRequest* request, size_t bytesRead, bool succeeded);

		struct io_uring mRing;
		int mEventFd = -1;
		Thread mReaper;
		Mutex mSubmitMutex; /**< Guards mPending only; not held across ring ops. */
		// Double-buffered MPSC drain: submitters push_back to mPending under mSubmitMutex; on each wake the reaper
		// O(1) swaps the two vectors, drains mReadyBuf unlocked, then clear()s it (preserving capacity). Both
		// vectors reach steady-state capacity after the first level-load burst and never reallocate afterwards.
		// FIFO ordering is preserved because submitters push_back in arrival order and the reaper iterates
		// front-to-back.
		Vector<AsyncReadRequest*> mPending;
		Vector<AsyncReadRequest*> mReadyBuf; /**< Reaper-only after the swap. */
		std::atomic<bool> mRunning{false};
		std::atomic<bool> mStopping{false};
		u32 mWaitFailureCount = 0; /**< Reaper-local; log-throttle counter for io_uring_wait_cqe failures. */
	};

	/**
	 * Linux-specific file data stream. Inherits the synchronous POSIX surface from UnixFileDataStream and adds a native
	 * asynchronous ReadAsync() backed by the shared LinuxFileIOManager when FileAccessFlag::Async is set.
	 *
	 * When opened without Async, no async machinery is touched and ReadAsync() falls back to the synchronous default
	 * inherited from DataStream. When opened with Async, Open() checks that LinuxFileIOManager is running; if not (e.g.
	 * pre-5.1 kernel, sandboxed environment), the stream stays usable and ReadAsync() degrades to the synchronous
	 * fallback.
	 *
	 * The stream itself owns no ring/reaper - those live in the process-shared manager. The stream's per-instance state
	 * tracks only outstanding-read counting so Close() can drain in-flight reads before the fd is released.
	 *
	 * @note Close() is not safe to call concurrently from multiple threads (matches the Win32 stream contract). The
	 *       DataStream API is non-thread-safe by contract; the existing mAsyncMutex + CV cover the user-thread vs
	 *       reaper-thread boundary, which is the only one that actually exists.
	 */
	class LinuxFileDataStream final : public UnixFileDataStream
	{
	public:
		LinuxFileDataStream(const Path& filePath, FileAccessFlags access = FileAccessFlag::Read);
		~LinuxFileDataStream() override;

		/** Opens the file stream and (if Async was requested) checks the shared LinuxFileIOManager is running. */
		bool Open() override;
		TAsyncOp<TShared<MemoryDataStream>> ReadAsync(u64 offset, size_t byteCount, TOptional<DataRange> userSuppliedMemory = TOptional<DataRange>()) override;
		TShared<DataStream> Clone(bool copyData = true) const override;
		bool Close() override;

		/**
		 * Called by the LinuxFileIOManager from the reaper thread once an async read it accepted has finalized. Locks
		 * mAsyncMutex, decrements mOutstandingReads, and notifies the CV so a pending Close() drain can proceed.
		 */
		void OnAsyncReadComplete();

	private:
		Mutex mAsyncMutex;
		ConditionVariable mAllReadsComplete;
		u32 mOutstandingReads = 0;
		bool mAsyncReady = false;
		bool mClosed = false;
	};

	/** @} */
} // namespace b3d
