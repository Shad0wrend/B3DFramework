//************************************* B3D Framework - Copyright 2026 Marko Pintera *************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Private/Win32/B3DWin32FileDataStream.h"

#include "FileSystem/B3DDataStream.h"
#include "Debug/B3DDebug.h"
#include "String/B3DUnicode.h"
#include <windows.h>

using namespace b3d;

namespace
{
	// Maximum number of bytes to transfer in a single ReadFile/WriteFile call (DWORD-sized count).
	constexpr u64 kMaxTransferPerCall = 0xFFFF0000ull;
}

// ************************************************************************************************************************
// Win32FileDataStream
// ************************************************************************************************************************

Win32FileDataStream::Win32FileDataStream(const Path& path, AccessMode accessMode)
	: DataStream(accessMode), mPath(path)
{ }

Win32FileDataStream::~Win32FileDataStream()
{
	if(mHandle != nullptr)
		Close();
}

bool Win32FileDataStream::Open()
{
	const bool wantRead = (mAccess & READ) != 0;
	const bool wantWrite = (mAccess & WRITE) != 0;

	DWORD access = 0;
	if(wantRead)
		access |= GENERIC_READ;

	if(wantWrite)
		access |= GENERIC_WRITE;

	// Write-only mirrors CreateAndOpenFile: create the file (truncating any existing one). Otherwise open an existing file.
	const DWORD disposition = (wantWrite && !wantRead) ? CREATE_ALWAYS : OPEN_EXISTING;

	// Match std::fstream's sharing mode (MSVC opens with _SH_DENYNO == FILE_SHARE_READ | FILE_SHARE_WRITE). Sharing
	// writes is required because the engine legitimately holds the same file open for writing while opening it again
	// for reading - e.g. PersistentCache::WriteDirtyMetaData opens a package read+write and then loads (reads) it back.
	// Without FILE_SHARE_WRITE the second open fails with ERROR_SHARING_VIOLATION even though the file exists.
	const WString widePath = UTF8::ToWide(mPath.ToString());
	HANDLE handle = CreateFileW(widePath.c_str(), access, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, disposition, FILE_ATTRIBUTE_NORMAL, nullptr);
	if(handle == INVALID_HANDLE_VALUE)
	{
		B3D_LOG(Error, LogFileSystem, "Failed to open file '{0}' (error {1}).", mPath, (u32)GetLastError());
		return false;
	}

	mHandle = handle;

	LARGE_INTEGER size{};
	if(GetFileSizeEx(handle, &size))
		mSize = (size_t)size.QuadPart;
	else
		mSize = 0;

	mCursor = 0;
	mEof = false;

	return true;
}

size_t Win32FileDataStream::Read(void* outData, size_t byteCount) const
{
	if(!B3D_ENSURE(mHandle != nullptr))
		return 0;

	u8* out = static_cast<u8*>(outData);
	size_t totalBytesRead = 0;
	while(totalBytesRead < byteCount)
	{
		const u64 remaining = byteCount - totalBytesRead;
		const DWORD bytesToRead = remaining > kMaxTransferPerCall ? (DWORD)kMaxTransferPerCall : (DWORD)remaining;

		DWORD bytesRead = 0;
		if(!ReadFile((HANDLE)mHandle, out + totalBytesRead, bytesToRead, &bytesRead, nullptr))
		{
			B3D_LOG(Error, LogFileSystem, "Error while reading from file '{0}' (error {1}).", mPath, (u32)GetLastError());
			break;
		}

		if(bytesRead == 0) // End of file reached.
			break;

		totalBytesRead += bytesRead;
	}

	mCursor += totalBytesRead;
	if(totalBytesRead < byteCount)
		mEof = true;

	return totalBytesRead;
}

size_t Win32FileDataStream::Write(const void* data, size_t byteCount)
{
	if(!B3D_ENSURE(mHandle != nullptr))
		return 0;

	if(!B3D_ENSURE(IsWriteable()))
		return 0;

	const u8* in = static_cast<const u8*>(data);
	size_t totalBytesWritten = 0;
	while(totalBytesWritten < byteCount)
	{
		const u64 remaining = byteCount - totalBytesWritten;
		const DWORD bytesToWrite = remaining > kMaxTransferPerCall ? (DWORD)kMaxTransferPerCall : (DWORD)remaining;

		DWORD bytesWritten = 0;
		if(!WriteFile((HANDLE)mHandle, in + totalBytesWritten, bytesToWrite, &bytesWritten, nullptr))
		{
			B3D_LOG(Error, LogFileSystem, "Error while writing to file '{0}' (error {1}).", mPath, (u32)GetLastError());
			break;
		}

		totalBytesWritten += bytesWritten;
	}

	mCursor += totalBytesWritten;
	if(mCursor > mSize)
		mSize = (size_t)mCursor;

	return totalBytesWritten;
}

size_t Win32FileDataStream::Skip(size_t count)
{
	if(!B3D_ENSURE(mHandle != nullptr))
		return 0;

	LARGE_INTEGER distance;
	distance.QuadPart = (LONGLONG)count;

	LARGE_INTEGER newPointer{};
	if(!SetFilePointerEx((HANDLE)mHandle, distance, &newPointer, FILE_CURRENT))
	{
		B3D_LOG(Error, LogFileSystem, "Error while seeking in file '{0}' (error {1}).", mPath, (u32)GetLastError());
		return 0;
	}

	const u64 previous = mCursor;
	mCursor = (u64)newPointer.QuadPart;
	mEof = false;

	return (size_t)(mCursor - previous);
}

size_t Win32FileDataStream::Seek(size_t pos)
{
	if(!B3D_ENSURE(mHandle != nullptr))
		return (size_t)mCursor;

	LARGE_INTEGER distance;
	distance.QuadPart = (LONGLONG)pos;

	LARGE_INTEGER newPointer{};
	if(!SetFilePointerEx((HANDLE)mHandle, distance, &newPointer, FILE_BEGIN))
	{
		B3D_LOG(Error, LogFileSystem, "Error while seeking in file '{0}' (error {1}).", mPath, (u32)GetLastError());
		return (size_t)mCursor;
	}

	mCursor = (u64)newPointer.QuadPart;
	mEof = false;

	return (size_t)mCursor;
}

size_t Win32FileDataStream::Tell() const
{
	return (size_t)mCursor;
}

bool Win32FileDataStream::Eof() const
{
	return mEof;
}

TShared<DataStream> Win32FileDataStream::Clone(bool copyData) const
{
	return B3DMakeShared<Win32FileDataStream>(mPath, (AccessMode)GetAccessMode());
}

bool Win32FileDataStream::Flush()
{
	if(!B3D_ENSURE(mHandle != nullptr))
		return false;

	if((mAccess & WRITE) != 0)
		return FlushFileBuffers((HANDLE)mHandle) != 0;

	return true;
}

bool Win32FileDataStream::Close()
{
	bool flushResult = true;
	if(mHandle != nullptr)
	{
		if((mAccess & WRITE) != 0)
			flushResult = FlushFileBuffers((HANDLE)mHandle) != 0;

		CloseHandle((HANDLE)mHandle);
		mHandle = nullptr;
	}

	return flushResult;
}

// ************************************************************************************************************************
// Win32AsyncFileDataStream
// ************************************************************************************************************************

struct Win32AsyncFileDataStream::ReadContext
{
	OVERLAPPED Overlapped; /**< Must be the first member so the OVERLAPPED can be recovered via CONTAINING_RECORD. */
	Win32AsyncFileDataStream* Stream = nullptr;
	void* Buffer = nullptr;
	bool OwnsBuffer = false;
	u64 BaseOffset = 0; /**< File offset where the read began. */
	u64 TotalByteCount = 0; /**< Total number of bytes to read (clamped to the end of file). */
	u64 BytesRead = 0; /**< Bytes read so far across all chunks. */
	TAsyncOp<TShared<MemoryDataStream>> Op;
};

TShared<Win32AsyncFileDataStream> Win32AsyncFileDataStream::Create(const Path& fullPath)
{
	// Share both reads and writes to match the synchronous stream (and the std::fstream-backed default path this
	// replaces), so opening a file for async reading doesn't fail when it's concurrently held open for writing.
	const WString widePath = UTF8::ToWide(fullPath.ToString());
	HANDLE handle = CreateFileW(widePath.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);
	if(handle == INVALID_HANDLE_VALUE)
	{
		B3D_LOG(Warning, LogFileSystem, "Failed to open file at path '{0}' for async reading.", fullPath);
		return nullptr;
	}

	if(!BindIoCompletionCallback(handle, reinterpret_cast<LPOVERLAPPED_COMPLETION_ROUTINE>(&CompletionRoutine), 0))
	{
		B3D_LOG(Warning, LogFileSystem, "Failed to bind IO completion callback for file '{0}' (error {1}).", fullPath, (u32)GetLastError());
		CloseHandle(handle);

		return nullptr;
	}

	u64 size = 0;
	LARGE_INTEGER li{};
	if(GetFileSizeEx(handle, &li))
		size = (u64)li.QuadPart;

	return B3DMakeShared<Win32AsyncFileDataStream>((void*)handle, size);
}

Win32AsyncFileDataStream::Win32AsyncFileDataStream(void* handle, u64 size)
	: mHandle(handle), mSize(size)
{ }

Win32AsyncFileDataStream::~Win32AsyncFileDataStream()
{
	Close();
}

TAsyncOp<TShared<MemoryDataStream>> Win32AsyncFileDataStream::ReadAsync(u64 offset, size_t byteCount, TOptional<DataRange> userSuppliedMemory)
{
	TAsyncOp<TShared<MemoryDataStream>> op;

	if(byteCount == 0)
	{
		op.CompleteOperation(B3DMakeShared<MemoryDataStream>());
		return op;
	}

	{
		Lock lock(mMutex);
		if(mClosed)
		{
			op.CompleteOperation(nullptr);
			return op;
		}

		mOutstandingReads++;
	}

	ReadContext* context = B3DNew<ReadContext>();
	context->Stream = this;
	context->BaseOffset = offset;
	context->Op = op;

	if(userSuppliedMemory.has_value())
	{
		context->Buffer = userSuppliedMemory->Data;
		context->OwnsBuffer = false;
	}
	else
	{
		context->Buffer = B3DAllocate(byteCount);
		context->OwnsBuffer = true;

		if(context->Buffer == nullptr)
		{
			B3D_LOG(Error, LogFileSystem, "Failed to allocate {0} bytes for an asynchronous read.", (u64)byteCount);
			CompleteContext(context, 0, false);
			return op;
		}
	}

	// Clamp the read to the bytes actually available so ReadFile never crosses the end of file. This keeps the
	// completion behaviour unambiguous: an in-bounds overlapped read always delivers exactly one completion callback,
	// whereas a read that hits EOF can fail synchronously while also queuing a completion packet (which would complete
	// the operation twice). Reads entirely at/after EOF are completed inline without issuing any IO.
	const u64 available = offset < mSize ? (mSize - offset) : 0;
	context->TotalByteCount = byteCount > available ? available : byteCount;

	if(context->TotalByteCount == 0)
	{
		CompleteContext(context, 0, true);
		return op;
	}

	IssueRead(context);
	return op;
}

bool Win32AsyncFileDataStream::IssueRead(ReadContext* context)
{
	// A single ReadFile transfers at most a DWORD's worth of bytes, so reads larger than that are split into chunks
	// and chained from the completion callback. This keeps the per-call count valid while supporting reads beyond 4 GiB.
	const u64 fileOffset = context->BaseOffset + context->BytesRead;
	const u64 remaining = context->TotalByteCount - context->BytesRead;
	const DWORD bytesToRead = remaining > kMaxTransferPerCall ? (DWORD)kMaxTransferPerCall : (DWORD)remaining;

	ZeroMemory(&context->Overlapped, sizeof(OVERLAPPED));
	context->Overlapped.Offset = (DWORD)(fileOffset & 0xFFFFFFFFull);
	context->Overlapped.OffsetHigh = (DWORD)(fileOffset >> 32);

	u8* const out = static_cast<u8*>(context->Buffer) + context->BytesRead;
	const BOOL ok = ReadFile((HANDLE)mHandle, out, bytesToRead, nullptr, &context->Overlapped);
	if(!ok)
	{
		const DWORD error = GetLastError();
		if(error != ERROR_IO_PENDING)
		{
			B3D_LOG(Warning, LogFileSystem, "Asynchronous read failed (error {0}).", (u32)error);
			CompleteContext(context, (size_t)context->BytesRead, false);

			return false;
		}
	}

	// On both ERROR_IO_PENDING and synchronous success a completion packet is queued (FILE_SKIP_COMPLETION_PORT_ON_SUCCESS
	// is not set), so the completion callback advances or finishes the operation.
	return true;
}

void Win32AsyncFileDataStream::CompleteContext(ReadContext* context, size_t bytesRead, bool succeeded)
{
	TShared<MemoryDataStream> result;
	if(succeeded)
	{
		if(context->OwnsBuffer)
			result = B3DMakeShared<MemoryDataStream>(context->Buffer, bytesRead, true);
		else
			result = B3DMakeShared<MemoryDataStream>(context->Buffer, bytesRead);
	}
	else if(context->OwnsBuffer && context->Buffer != nullptr)
	{
		B3DFree(context->Buffer);
	}

	context->Op.CompleteOperation(result);
	B3DDelete(context);

	Lock lock(mMutex);
	B3D_ASSERT(mOutstandingReads > 0);
	mOutstandingReads--;

	if(mOutstandingReads == 0)
		mAllReadsComplete.notify_all();
}

void __stdcall Win32AsyncFileDataStream::CompletionRoutine(unsigned long errorCode, unsigned long bytesTransferred, void* overlapped)
{
	ReadContext* context = CONTAINING_RECORD(reinterpret_cast<OVERLAPPED*>(overlapped), ReadContext, Overlapped);
	context->Stream->DoOnChunkComplete(context, errorCode, (size_t)bytesTransferred);
}

void Win32AsyncFileDataStream::DoOnChunkComplete(ReadContext* context, unsigned long errorCode, size_t bytesTransferred)
{
	// Reads are clamped to the file size, so ERROR_HANDLE_EOF isn't expected, but treat it (and a zero-byte transfer)
	// as a clean end of data. Any other error - including ERROR_OPERATION_ABORTED from Close() cancelling in-flight
	// reads - finishes the operation as a failure, discarding whatever was read so far.
	if(errorCode != ERROR_SUCCESS && errorCode != ERROR_HANDLE_EOF)
	{
		CompleteContext(context, (size_t)context->BytesRead, false);
		return;
	}

	context->BytesRead += bytesTransferred;

	const bool reachedEof = (errorCode == ERROR_HANDLE_EOF) || (bytesTransferred == 0);
	const bool moreToRead = context->BytesRead < context->TotalByteCount;

	if(moreToRead && !reachedEof)
	{
		bool closing;
		{
			Lock lock(mMutex);
			closing = mClosed;
		}

		if(!closing)
		{
			IssueRead(context); // Chains the next chunk; finalizes inline on synchronous failure.
			return;
		}

		// Close() was requested between chunks: stop chaining so it doesn't have to wait out a huge read, and finish
		// the operation as a failure (matching an in-flight chunk that gets cancelled via CancelIoEx).
		CompleteContext(context, (size_t)context->BytesRead, false);
		return;
	}

	CompleteContext(context, (size_t)context->BytesRead, true);
}

void Win32AsyncFileDataStream::Close()
{
	{
		Lock lock(mMutex);
		if(mClosed)
			return;

		mClosed = true;
	}

	// Cancel any in-flight reads so their completion callbacks fire promptly (with ERROR_OPERATION_ABORTED).
	if(mHandle != nullptr)
		CancelIoEx((HANDLE)mHandle, nullptr);

	{
		Lock lock(mMutex);
		mAllReadsComplete.wait(lock, [this]() { return mOutstandingReads == 0; });
	}

	if(mHandle != nullptr)
	{
		CloseHandle((HANDLE)mHandle);
		mHandle = nullptr;
	}
}
