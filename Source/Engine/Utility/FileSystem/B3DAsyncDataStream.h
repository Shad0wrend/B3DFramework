//************************************* B3D Framework - Copyright 2026 Marko Pintera *************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DUtilityPrerequisites.h"
#include "Threading/B3DAsyncOp.h"
#include "Threading/B3DThread.h"
#include "Threading/B3DThreading.h"
#include "Utility/B3DUtil.h"

namespace b3d
{
	/** @addtogroup Filesystem
	 *  @{
	 */

	/**
	 * Interface for a read-only data source that supports asynchronous reads. Returned by FileSystem::OpenFileAsync().
	 * Platforms may provide native implementations (e.g. overlapped IO on Windows).
	 */
	class B3D_EXPORT IAsyncDataStream
	{
	public:
		virtual ~IAsyncDataStream() = default;

		/** Returns the total size of the file in bytes. */
		virtual u64 Size() const = 0;

		/**
		 * Asynchronously reads @p byteCount bytes starting at @p offset.
		 *
		 * @param	offset				Byte offset from the start of the file to begin reading at.
		 * @param	byteCount			Number of bytes to read.
		 * @param	userSuppliedMemory	(optional) If provided, data is read into this memory (which must have capacity
		 *								for at least @p byteCount bytes) and the returned stream wraps it without taking
		 *								ownership. The caller must ensure the memory outlives the returned stream. If not
		 *								provided, a new memory block is allocated and owned by the returned stream.
		 * @return						Operation that completes with a memory stream containing the read data. The
		 *								stream's size may be smaller than @p byteCount if the end of file was reached. The
		 *								operation completes with null if the read failed, or with an empty stream if
		 *								@p byteCount is zero or @p offset is at/after the end of file.
		 */
		virtual TAsyncOp<TShared<MemoryDataStream>> ReadAsync(u64 offset, size_t byteCount, TOptional<DataRange> userSuppliedMemory = TOptional<DataRange>()) = 0;

		/** Closes the stream, releasing the underlying file. Blocks until any in-flight reads have completed. */
		virtual void Close() = 0;
	};

	/**
	 * Default implementation of IAsyncDataStream. Wraps a regular synchronous file stream (opened via
	 * FileSystem::OpenFile) and services read requests on a dedicated worker thread. Reads are serialized through a
	 * single thread, making this suitable as a portable fallback rather than a high-throughput async I/O path.
	 */
	class B3D_EXPORT AsyncFileDataStream final : public IAsyncDataStream
	{
	public:
		/**
		 * Opens the file at the provided path for read-only access and returns a stream wrapping it.
		 *
		 * @param	fullPath	Full path to a file.
		 * @return				The async stream, or null if the file couldn't be opened.
		 */
		static TShared<AsyncFileDataStream> Create(const Path& fullPath);

		/** Constructs the stream around an already-opened synchronous stream. Use Create() instead. */
		explicit AsyncFileDataStream(const TShared<DataStream>& stream);
		~AsyncFileDataStream() override;

		u64 Size() const override { return mSize; }
		TAsyncOp<TShared<MemoryDataStream>> ReadAsync(u64 offset, size_t byteCount, TOptional<DataRange> userSuppliedMemory = TOptional<DataRange>()) override;
		void Close() override;

	private:
		/** A single queued read request. */
		struct ReadRequest
		{
			u64 Offset = 0;
			size_t ByteCount = 0;
			TOptional<DataRange> UserMemory;
			TAsyncOp<TShared<MemoryDataStream>> Op;
		};

		/** Starts the worker thread. Called once right after construction. */
		void StartWorker();

		/** Worker thread entry point. Pops and services read requests until shut down. */
		void WorkerLoop();

		/** Performs a single read (on the worker thread) and completes its operation. */
		void ProcessRequest(ReadRequest& request);

		TShared<DataStream> mStream;
		u64 mSize = 0;

		Thread mWorker;
		mutable Mutex mMutex;
		ConditionVariable mWorkAvailable;
		Queue<ReadRequest> mRequests;
		bool mShutdown = false;
		bool mClosed = false;
	};

	/** @} */
} // namespace b3d
