//************************************* B3D Framework - Copyright 2026 Marko Pintera *************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "FileSystem/B3DAsyncDataStream.h"

#include "FileSystem/B3DFileSystem.h"
#include "FileSystem/B3DDataStream.h"
#include "Debug/B3DDebug.h"

using namespace b3d;

TShared<AsyncFileDataStream> AsyncFileDataStream::Create(const Path& fullPath)
{
	TShared<DataStream> stream = FileSystem::OpenFile(fullPath, true);
	if(stream == nullptr)
		return nullptr;

	TShared<AsyncFileDataStream> asyncStream = B3DMakeShared<AsyncFileDataStream>(stream);
	asyncStream->StartWorker();

	return asyncStream;
}

AsyncFileDataStream::AsyncFileDataStream(const TShared<DataStream>& stream)
	: mStream(stream), mSize(stream != nullptr ? stream->Size() : 0)
{ }

AsyncFileDataStream::~AsyncFileDataStream()
{
	Close();
}

void AsyncFileDataStream::StartWorker()
{
	// Note: Launching a new thread for each worker is not ideal, but we'll letting it slide as this is a fallback path. Each platform
	// should override this path. If that changes, we'll need a manager to pool the threads a likely limit number of work at a time.
	mWorker = Thread([this]() { WorkerLoop(); });
}

TAsyncOp<TShared<MemoryDataStream>> AsyncFileDataStream::ReadAsync(u64 offset, size_t byteCount, TOptional<DataRange> userSuppliedMemory)
{
	TAsyncOp<TShared<MemoryDataStream>> op;

	{
		Lock lock(mMutex);
		if(!mShutdown)
		{
			ReadRequest request;
			request.Offset = offset;
			request.ByteCount = byteCount;
			request.UserMemory = userSuppliedMemory;
			request.Op = op;

			mRequests.push(std::move(request));
			mWorkAvailable.notify_one();

			return op;
		}
	}

	// Stream is shutting down; complete immediately with no data.
	op.CompleteOperation(nullptr);
	return op;
}

void AsyncFileDataStream::WorkerLoop()
{
	while(true)
	{
		ReadRequest request;
		{
			Lock lock(mMutex);
			mWorkAvailable.wait(lock, [this]() { return mShutdown || !mRequests.empty(); });

			// On shutdown, keep draining any remaining requests so their waiters don't block forever.
			if(mRequests.empty())
				return;

			request = std::move(mRequests.front());
			mRequests.pop();
		}

		ProcessRequest(request);
	}
}

void AsyncFileDataStream::ProcessRequest(ReadRequest& request)
{
	TShared<MemoryDataStream> result;

	if(mStream != nullptr && request.ByteCount > 0)
	{
		mStream->Seek((size_t)request.Offset);

		if(request.UserMemory.has_value())
		{
			const DataRange& range = *request.UserMemory;
			const size_t bytesRead = mStream->Read(range.Data, request.ByteCount);
			result = B3DMakeShared<MemoryDataStream>(range.Data, bytesRead);
		}
		else
		{
			void* buffer = B3DAllocate(request.ByteCount);
			if(buffer != nullptr)
			{
				const size_t bytesRead = mStream->Read(buffer, request.ByteCount);
				result = B3DMakeShared<MemoryDataStream>(buffer, bytesRead, true);
			}
			else
			{
				B3D_LOG(Error, LogFileSystem, "Failed to allocate {0} bytes for an asynchronous read.", (u64)request.ByteCount);
			}
		}
	}
	else if(request.ByteCount == 0)
	{
		result = B3DMakeShared<MemoryDataStream>();
	}

	request.Op.CompleteOperation(result);
}

void AsyncFileDataStream::Close()
{
	{
		Lock lock(mMutex);
		if(mClosed)
			return;

		// Setting mClosed here (rather than after the join) ensures only one caller ever proceeds, so a concurrent
		// Close() and the destructor can't both join the worker or close the stream.
		mClosed = true;
		mShutdown = true;
	}
	mWorkAvailable.notify_all();

	// Joins the worker if it was started; no-op otherwise. Must happen before the Thread is destroyed.
	mWorker.WaitUntilComplete();

	// The worker has exited and mClosed gates out any other caller, so nothing else can touch mStream; close it
	// without holding the lock across the blocking flush.
	if(mStream != nullptr)
	{
		mStream->Close();
		mStream = nullptr;
	}
}
