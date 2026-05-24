//************************************* B3D Framework - Copyright 2026 Marko Pintera *************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DUtilityPrerequisites.h"
#include "FileSystem/B3DDataStream.h"
#include "FileSystem/B3DAsyncDataStream.h"
#include "Threading/B3DThreading.h"

namespace b3d
{
	/** @addtogroup Filesystem
	 *  @{
	 */

	/** Data stream for handling files using native Win32 file APIs (CreateFile/ReadFile/WriteFile). */
	class Win32FileDataStream final : public DataStream
	{
	public:
		/**
		 * Constructs a file stream.
		 *
		 * @param	filePath	Path of the file to open.
		 * @param	accessMode	Determines should the file be opened in read, write or read/write mode.
		 */
		Win32FileDataStream(const Path& filePath, AccessMode accessMode = READ);
		~Win32FileDataStream() override;

		/** Opens the file stream. Must be called before any actions on the stream. Returns false if not successful. */
		bool Open();
		bool IsFile() const override { return true; }
		size_t Read(void* data, size_t byteCount) const override;
		size_t Write(const void* data, size_t byteCount) override;
		size_t Skip(size_t count) override;
		size_t Seek(size_t pos) override;
		size_t Tell() const override;
		bool Eof() const override;
		TShared<DataStream> Clone(bool copyData = true) const override;
		bool Flush() override;
		bool Close() override;

		/** Returns the path of the file opened by the stream. */
		const Path& GetPath() const { return mPath; }

	protected:
		Path mPath;
		void* mHandle = nullptr;        // Win32 HANDLE; null when the stream is closed.
		mutable u64 mCursor = 0;        // Current read/write byte offset from the start of the file.
		mutable bool mEof = false;      // Set once a read couldn't satisfy the full request (end of file reached).
	};

	/**
	 * Asynchronous read-only data stream backed by Win32 overlapped IO. Reads are issued asynchronously and completed on
	 * a system thread-pool thread via an IO completion callback.
	 */
	class Win32AsyncFileDataStream final : public IAsyncDataStream
	{
	public:
		/**
		 * Opens the file at the provided path for asynchronous read-only access.
		 *
		 * @param	fullPath	Full path to a file.
		 * @return				The async stream, or null if the file couldn't be opened.
		 */
		static TShared<Win32AsyncFileDataStream> Create(const Path& fullPath);

		/** Constructs the stream around an already-opened overlapped file handle. Use Create() instead. */
		Win32AsyncFileDataStream(void* handle, u64 size);
		~Win32AsyncFileDataStream() override;

		u64 Size() const override { return mSize; }
		TAsyncOp<TShared<MemoryDataStream>> ReadAsync(u64 offset, size_t byteCount, TOptional<DataRange> userSuppliedMemory = TOptional<DataRange>()) override;
		void Close() override;

	private:
		struct ReadContext;

		/** Win32 IO completion callback. Recovers the ReadContext from the OVERLAPPED and advances the read. */
		static void __stdcall CompletionRoutine(unsigned long errorCode, unsigned long bytesTransferred, void* overlapped);

		/**
		 * Issues an overlapped read for the next outstanding chunk of @p context. Reads larger than a single ReadFile
		 * call are split into chunks and chained across completions. Returns false if the read failed synchronously, in
		 * which case the operation has already been finalized.
		 */
		bool IssueRead(ReadContext* context);

		/**
		 * Completion handler for a single chunk. Accumulates the bytes read and either chains the next chunk or
		 * finalizes the operation.
		 */
		void DoOnChunkComplete(ReadContext* context, unsigned long errorCode, size_t bytesTransferred);

		/** Builds the result, completes the operation, releases the context and decrements the outstanding read count. */
		void CompleteContext(ReadContext* context, size_t bytesRead, bool succeeded);

		void* mHandle = nullptr; /**< Win32 HANDLE opened with FILE_FLAG_OVERLAPPED. */
		u64 mSize = 0;

		Mutex mMutex;
		ConditionVariable mAllReadsComplete;
		u32 mOutstandingReads = 0;
		bool mClosed = false;
	};

	/** @} */
} // namespace b3d
