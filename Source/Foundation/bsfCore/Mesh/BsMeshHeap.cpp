//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Mesh/BsMeshHeap.h"
#include "CoreThread/BsCoreThread.h"
#include "Mesh/BsTransientMesh.h"
#include "Managers/BsHardwareBufferManager.h"
#include "RenderAPI/BsVertexDataDesc.h"
#include "RenderAPI/BsVertexData.h"
#include "Mesh/BsMeshData.h"
#include "Math/BsMath.h"
#include "RenderAPI/BsEventQuery.h"
#include "RenderAPI/BsRenderAPI.h"

namespace bs
{
	MeshHeap::MeshHeap(UINT32 numVertices, UINT32 numIndices,
		const SPtr<VertexDataDesc>& vertexDesc, IndexType indexType)
		:mNumVertices(numVertices), mNumIndices(numIndices), mVertexDesc(vertexDesc), mIndexType(indexType), mNextFreeId(0)
	{
	}

	SPtr<MeshHeap> MeshHeap::Create(UINT32 numVertices, UINT32 numIndices, const SPtr<VertexDataDesc>& vertexDesc,
		IndexType indexType)
	{
		MeshHeap* meshHeap = new (bs_alloc<MeshHeap>()) MeshHeap(numVertices, numIndices, vertexDesc, indexType);
		SPtr<MeshHeap> meshHeapPtr = bs_core_ptr<MeshHeap>(meshHeap);

		meshHeapPtr->SetThisPtrInternal(meshHeapPtr);
		meshHeapPtr->Initialize();

		return meshHeapPtr;
	}

	SPtr<TransientMesh> MeshHeap::Alloc(const SPtr<MeshData>& meshData, DrawOperationType drawOp)
	{
		UINT32 meshIdx = mNextFreeId++;

		SPtr<MeshHeap> thisPtr = std::static_pointer_cast<MeshHeap>(GetThisPtr());
		TransientMesh* transientMesh = new (bs_alloc<TransientMesh>()) TransientMesh(thisPtr, meshIdx,
			meshData->GetNumVertices(), meshData->GetNumIndices(), drawOp);
		SPtr<TransientMesh> transientMeshPtr = bs_core_ptr<TransientMesh>(transientMesh);

		transientMeshPtr->SetThisPtrInternal(transientMeshPtr);
		transientMeshPtr->Initialize();

		mMeshes[meshIdx] = transientMeshPtr;

		QueueGpuCommand(GetCore(), std::bind(&ct::MeshHeap::Alloc, GetCore().get(), transientMeshPtr->GetCore(), meshData));

		return transientMeshPtr;
	}

	void MeshHeap::Dealloc(const SPtr<TransientMesh>& mesh)
	{
		auto iterFind = mMeshes.find(mesh->mId);
		if(iterFind == mMeshes.end())
			return;

		mesh->MarkAsDestroyed();
		mMeshes.erase(iterFind);

		QueueGpuCommand(GetCore(), std::bind(&ct::MeshHeap::Dealloc, GetCore().get(), mesh->GetCore()));
	}

	SPtr<ct::MeshHeap> MeshHeap::GetCore() const
	{
		return std::static_pointer_cast<ct::MeshHeap>(mCoreSpecific);
	}

	SPtr<ct::CoreObject> MeshHeap::CreateCore() const
	{
		ct::MeshHeap* obj = new (bs_alloc<ct::MeshHeap>()) ct::MeshHeap(mNumVertices, mNumIndices,
			mVertexDesc, mIndexType, GDF_DEFAULT);

		SPtr<ct::MeshHeap> corePtr = bs_shared_ptr<ct::MeshHeap>(obj);
		obj->SetThisPtrInternal(corePtr);

		return corePtr;
	}

	namespace ct
	{
	const float MeshHeap::GrowPercent = 1.5f;

	MeshHeap::MeshHeap(UINT32 numVertices, UINT32 numIndices,
		const SPtr<VertexDataDesc>& vertexDesc, IndexType indexType, GpuDeviceFlags deviceMask)
		: mNumVertices(numVertices), mNumIndices(numIndices), mCPUIndexData(nullptr), mVertexDesc(vertexDesc)
		, mIndexType(indexType), mDeviceMask(deviceMask), mNextQueryId(0)
	{
		for (UINT32 i = 0; i <= mVertexDesc->GetMaxStreamIdx(); i++)
		{
			mCPUVertexData.push_back(nullptr);
		}
	}

	MeshHeap::~MeshHeap()
	{
		THROW_IF_NOT_CORE_THREAD;

		for (auto& cpuVertBuffer : mCPUVertexData)
			bs_free(cpuVertBuffer);

		if (mCPUIndexData != nullptr)
			bs_free(mCPUIndexData);

		mVertexData = nullptr;
		mIndexBuffer = nullptr;
		mVertexDesc = nullptr;
	}

	void MeshHeap::Initialize()
	{
		THROW_IF_NOT_CORE_THREAD;

		GrowVertexBuffer(mNumVertices);
		GrowIndexBuffer(mNumIndices);

		CoreObject::Initialize();
	}

	void MeshHeap::Alloc(SPtr<TransientMesh> mesh, const SPtr<MeshData>& meshData)
	{
		// Find free vertex chunk and grow if needed
		UINT32 smallestVertFit = 0;
		UINT32 smallestVertFitIdx = 0;

		while (smallestVertFit == 0)
		{
			UINT32 curIdx = 0;
			for (auto& chunkIdx : mFreeVertChunks)
			{
				ChunkData& chunk = mVertChunks[chunkIdx];

				if (chunk.Size >= meshData->GetNumVertices() && (chunk.Size < smallestVertFit || smallestVertFit == 0))
				{
					smallestVertFit = chunk.Size;
					smallestVertFitIdx = curIdx;
				}

				curIdx++;
			}

			if (smallestVertFit > 0)
				break;

			UINT32 newNumVertices = mNumVertices;
			while (newNumVertices < (mNumVertices + meshData->GetNumVertices()))
			{
				newNumVertices = Math::RoundToInt(newNumVertices * GrowPercent);
			}

			GrowVertexBuffer(newNumVertices);
		}

		// Find free index chunk and grow if needed
		UINT32 smallestIdxFit = 0;
		UINT32 smallestIdxFitIdx = 0;

		while (smallestIdxFit == 0)
		{
			UINT32 curIdx = 0;
			for (auto& chunkIdx : mFreeIdxChunks)
			{
				ChunkData& chunk = mIdxChunks[chunkIdx];

				if (chunk.Size >= meshData->GetNumIndices() && (chunk.Size < smallestIdxFit || smallestIdxFit == 0))
				{
					smallestIdxFit = chunk.Size;
					smallestIdxFitIdx = curIdx;
				}

				curIdx++;
			}

			if (smallestIdxFit > 0)
				break;

			UINT32 newNumIndices = mNumIndices;
			while (newNumIndices < (mNumIndices + meshData->GetNumIndices()))
			{
				newNumIndices = Math::RoundToInt(newNumIndices * GrowPercent);
			}

			GrowIndexBuffer(newNumIndices);
		}

		UINT32 freeVertChunkIdx = 0;
		UINT32 freeIdxChunkIdx = 0;

		auto freeVertIter = mFreeVertChunks.begin();
		freeVertChunkIdx = (*freeVertIter);
		for (UINT32 i = 0; i < smallestVertFitIdx; i++)
		{
			freeVertIter++;
			freeVertChunkIdx = (*freeVertIter);
		}

		mFreeVertChunks.erase(freeVertIter);

		auto freeIdxIter = mFreeIdxChunks.begin();
		freeIdxChunkIdx = (*freeIdxIter);
		for (UINT32 i = 0; i < smallestIdxFitIdx; i++)
		{
			freeIdxIter++;
			freeIdxChunkIdx = (*freeIdxIter);
		}

		mFreeIdxChunks.erase(freeIdxIter);

		ChunkData& vertChunk = mVertChunks[freeVertChunkIdx];
		ChunkData& idxChunk = mIdxChunks[freeIdxChunkIdx];

		UINT32 vertChunkStart = vertChunk.Start;
		UINT32 idxChunkStart = idxChunk.Start;

		UINT32 remainingNumVerts = vertChunk.Size - meshData->GetNumVertices();
		UINT32 remainingNumIdx = idxChunk.Size - meshData->GetNumIndices();

		vertChunk.Size = meshData->GetNumVertices();
		idxChunk.Size = meshData->GetNumIndices();

		if (remainingNumVerts > 0)
		{
			if (!mEmptyVertChunks.empty())
			{
				UINT32 emptyChunkIdx = mEmptyVertChunks.top();
				ChunkData& emptyChunk = mVertChunks[emptyChunkIdx];
				mEmptyVertChunks.pop();

				emptyChunk.Start = vertChunkStart + meshData->GetNumVertices();
				emptyChunk.Size = remainingNumVerts;
			}
			else
			{
				ChunkData newChunk;
				newChunk.Size = remainingNumVerts;
				newChunk.Start = vertChunkStart + meshData->GetNumVertices();

				mVertChunks.push_back(newChunk);
				mFreeVertChunks.push_back((UINT32)(mVertChunks.size() - 1));
			}
		}

		if (remainingNumIdx > 0)
		{
			if (!mEmptyIdxChunks.empty())
			{
				UINT32 emptyChunkIdx = mEmptyIdxChunks.top();
				ChunkData& emptyChunk = mIdxChunks[emptyChunkIdx];
				mEmptyIdxChunks.pop();

				emptyChunk.Start = idxChunkStart + meshData->GetNumIndices();
				emptyChunk.Size = remainingNumIdx;
			}
			else
			{
				ChunkData newChunk;
				newChunk.Size = remainingNumIdx;
				newChunk.Start = idxChunkStart + meshData->GetNumIndices();

				mIdxChunks.push_back(newChunk);
				mFreeIdxChunks.push_back((UINT32)(mIdxChunks.size() - 1));
			}
		}

		AllocatedData newAllocData;
		newAllocData.VertChunkIdx = freeVertChunkIdx;
		newAllocData.IdxChunkIdx = freeIdxChunkIdx;
		newAllocData.UseFlags = UseFlags::GPUFree;
		newAllocData.EventQueryIdx = CreateEventQuery();
		newAllocData.Mesh = mesh;

		mMeshAllocData[mesh->GetMeshHeapId()] = newAllocData;

		// Actually copy data
		for (UINT32 i = 0; i <= mVertexDesc->GetMaxStreamIdx(); i++)
		{
			if (!mVertexDesc->HasStream(i))
				continue;

			if (!meshData->GetVertexDesc()->HasStream(i))
				continue;

			// Ensure vertex sizes match
			UINT32 vertSize = mVertexData->VertexDeclaration->GetProperties().GetVertexSize(i);
			UINT32 otherVertSize = meshData->GetVertexDesc()->GetVertexStride(i);
			if (otherVertSize != vertSize)
			{
				BS_EXCEPT(InvalidParametersException, "Provided vertex size for stream " + toString(i) + " doesn't match meshes vertex size. Needed: " +
					toString(vertSize) + ". Got: " + toString(otherVertSize));
			}

			SPtr<VertexBuffer> vertexBuffer = mVertexData->GetBuffer(i);

			UINT8* vertDest = mCPUVertexData[i] + vertChunkStart * vertSize;
			memcpy(vertDest, meshData->GetStreamData(i), meshData->GetNumVertices() * vertSize);

			vertexBuffer->WriteData(vertChunkStart * vertSize, meshData->GetNumVertices() * vertSize, vertDest, BTW_NO_OVERWRITE);
		}

		const IndexBufferProperties& ibProps = mIndexBuffer->GetProperties();

		UINT32 idxSize = ibProps.GetIndexSize();

		// Ensure index sizes match
		if (meshData->GetIndexElementSize() != idxSize)
		{
			BS_EXCEPT(InvalidParametersException, "Provided index size doesn't match meshes index size. Needed: " +
				toString(idxSize) + ". Got: " + toString(meshData->GetIndexElementSize()));
		}

		UINT8* idxDest = mCPUIndexData + idxChunkStart * idxSize;
		memcpy(idxDest, meshData->GetIndexData(), meshData->GetNumIndices() * idxSize);
		mIndexBuffer->WriteData(idxChunkStart * idxSize, meshData->GetNumIndices() * idxSize, idxDest, BTW_NO_OVERWRITE);
	}

	void MeshHeap::Dealloc(SPtr<TransientMesh> mesh)
	{
		auto findIter = mMeshAllocData.find(mesh->GetMeshHeapId());
		assert(findIter != mMeshAllocData.end());

		AllocatedData& allocData = findIter->second;
		if (allocData.UseFlags == UseFlags::GPUFree)
		{
			allocData.UseFlags = UseFlags::Free;
			FreeEventQuery(allocData.EventQueryIdx);

			mFreeVertChunks.push_back(allocData.VertChunkIdx);
			mFreeIdxChunks.push_back(allocData.IdxChunkIdx);

			MergeWithNearbyChunks(allocData.VertChunkIdx, allocData.IdxChunkIdx);

			mMeshAllocData.erase(findIter);
		}
		else if (allocData.UseFlags == UseFlags::Used)
			allocData.UseFlags = UseFlags::CPUFree;
	}

	void MeshHeap::GrowVertexBuffer(UINT32 numVertices)
	{
		mNumVertices = numVertices;
		mVertexData = SPtr<VertexData>(bs_new<VertexData>());

		mVertexData->VertexCount = mNumVertices;
		mVertexData->VertexDeclaration = VertexDeclaration::Create(mVertexDesc, mDeviceMask);

		// Create buffers and copy data
		for (UINT32 i = 0; i <= mVertexDesc->GetMaxStreamIdx(); i++)
		{
			if (!mVertexDesc->HasStream(i))
				continue;

			UINT32 vertSize = mVertexData->VertexDeclaration->GetProperties().GetVertexSize(i);

			VERTEX_BUFFER_DESC desc;
			desc.VertexSize = vertSize;
			desc.NumVerts = mVertexData->VertexCount;
			desc.Usage = GBU_DYNAMIC;

			SPtr<VertexBuffer> vertexBuffer = VertexBuffer::Create(desc, mDeviceMask);
			mVertexData->SetBuffer(i, vertexBuffer);

			// Copy all data to the new buffer
			UINT8* oldBuffer = mCPUVertexData[i];
			UINT8* buffer = (UINT8*)bs_alloc(vertSize * numVertices);

			UINT32 destOffset = 0;
			if (oldBuffer != nullptr)
			{
				for (auto& allocData : mMeshAllocData)
				{
					ChunkData& oldChunk = mVertChunks[allocData.second.VertChunkIdx];

					UINT8* oldData = oldBuffer + oldChunk.Start * vertSize;
					memcpy(buffer + destOffset * vertSize, oldData, oldChunk.Size * vertSize);

					destOffset += oldChunk.Size;
				}

				bs_free(oldBuffer);
			}

			if (destOffset > 0)
				vertexBuffer->WriteData(0, destOffset * vertSize, buffer, BTW_NO_OVERWRITE);

			mCPUVertexData[i] = buffer;
		}

		// Reorder chunks
		UINT32 destOffset = 0;
		Vector<ChunkData> newVertChunks;
		List<UINT32> freeVertChunks;

		for (auto& allocData : mMeshAllocData)
		{
			ChunkData& oldChunk = mVertChunks[allocData.second.VertChunkIdx];

			ChunkData newChunk;
			newChunk.Start = destOffset;
			newChunk.Size = oldChunk.Size;

			allocData.second.VertChunkIdx = (UINT32)newVertChunks.size();
			newVertChunks.push_back(newChunk);

			destOffset += oldChunk.Size;
		}

		// Add free chunk
		if (destOffset != mNumVertices)
		{
			ChunkData newChunk;
			newChunk.Start = destOffset;
			newChunk.Size = mNumVertices - destOffset;

			newVertChunks.push_back(newChunk);
			freeVertChunks.push_back((UINT32)(newVertChunks.size() - 1));
		}

		mVertChunks = newVertChunks;
		mFreeVertChunks = freeVertChunks;

		while (!mEmptyVertChunks.empty())
			mEmptyVertChunks.pop();
	}

	void MeshHeap::GrowIndexBuffer(UINT32 numIndices)
	{
		mNumIndices = numIndices;

		INDEX_BUFFER_DESC ibDesc;
		ibDesc.IndexType = mIndexType;
		ibDesc.NumIndices = mNumIndices;
		ibDesc.Usage = GBU_DYNAMIC;

		mIndexBuffer = IndexBuffer::Create(ibDesc, mDeviceMask);

		const IndexBufferProperties& ibProps = mIndexBuffer->GetProperties();

		// Copy all data to the new buffer
		UINT32 idxSize = ibProps.GetIndexSize();

		UINT8* oldBuffer = mCPUIndexData;
		UINT8* buffer = (UINT8*)bs_alloc(idxSize * numIndices);

		UINT32 destOffset = 0;
		if (oldBuffer != nullptr)
		{
			for (auto& allocData : mMeshAllocData)
			{
				ChunkData& oldChunk = mIdxChunks[allocData.second.IdxChunkIdx];

				UINT8* oldData = oldBuffer + oldChunk.Start * idxSize;
				memcpy(buffer + destOffset * idxSize, oldData, oldChunk.Size * idxSize);

				destOffset += oldChunk.Size;
			}

			bs_free(oldBuffer);
		}

		if (destOffset > 0)
			mIndexBuffer->WriteData(0, destOffset * idxSize, buffer, BTW_NO_OVERWRITE);

		mCPUIndexData = buffer;

		// Reorder chunks
		destOffset = 0;
		Vector<ChunkData> newIdxChunks;
		List<UINT32> freeIdxChunks;

		for (auto& allocData : mMeshAllocData)
		{
			ChunkData& oldChunk = mIdxChunks[allocData.second.IdxChunkIdx];

			ChunkData newChunk;
			newChunk.Start = destOffset;
			newChunk.Size = oldChunk.Size;

			allocData.second.IdxChunkIdx = (UINT32)newIdxChunks.size();
			newIdxChunks.push_back(newChunk);

			destOffset += oldChunk.Size;
		}

		// Add free chunk
		if (destOffset != mNumIndices)
		{
			ChunkData newChunk;
			newChunk.Start = destOffset;
			newChunk.Size = mNumIndices - destOffset;

			newIdxChunks.push_back(newChunk);
			freeIdxChunks.push_back((UINT32)(newIdxChunks.size() - 1));
		}

		mIdxChunks = newIdxChunks;
		mFreeIdxChunks = freeIdxChunks;

		while (!mEmptyIdxChunks.empty())
			mEmptyIdxChunks.pop();
	}

	UINT32 MeshHeap::CreateEventQuery()
	{
		UINT32 idx = 0;
		if (mFreeEventQueries.size() > 0)
		{
			idx = mFreeEventQueries.top();
			mFreeEventQueries.pop();
		}
		else
		{
			QueryData newQuery;
			newQuery.Query = EventQuery::Create();
			newQuery.QueryId = 0;

			mEventQueries.push_back(newQuery);
			idx = (UINT32)(mEventQueries.size() - 1);
		}

		return idx;
	}

	void MeshHeap::FreeEventQuery(UINT32 idx)
	{
		mEventQueries[idx].Query->OnTriggered.Clear();
		mEventQueries[idx].QueryId = 0;
		mFreeEventQueries.push(idx);
	}

	SPtr<VertexData> MeshHeap::GetVertexData() const
	{
		return mVertexData;
	}

	SPtr<IndexBuffer> MeshHeap::GetIndexBuffer() const
	{
		return mIndexBuffer;
	}

	SPtr<VertexDataDesc> MeshHeap::GetVertexDesc() const
	{
		return mVertexDesc;
	}

	UINT32 MeshHeap::GetVertexOffset(UINT32 meshId) const
	{
		auto findIter = mMeshAllocData.find(meshId);
		assert(findIter != mMeshAllocData.end());

		UINT32 chunkIdx = findIter->second.VertChunkIdx;
		return mVertChunks[chunkIdx].Start;
	}

	UINT32 MeshHeap::GetIndexOffset(UINT32 meshId) const
	{
		auto findIter = mMeshAllocData.find(meshId);
		assert(findIter != mMeshAllocData.end());

		UINT32 chunkIdx = findIter->second.IdxChunkIdx;
		return mIdxChunks[chunkIdx].Start;
	}

	void MeshHeap::NotifyUsedOnGpu(UINT32 meshId)
	{
		auto findIter = mMeshAllocData.find(meshId);
		assert(findIter != mMeshAllocData.end());

		AllocatedData& allocData = findIter->second;
		assert(allocData.UseFlags != UseFlags::Free);

		if (allocData.UseFlags == UseFlags::GPUFree)
			allocData.UseFlags = UseFlags::Used;

		SPtr<MeshHeap> thisPtr = std::static_pointer_cast<MeshHeap>(GetThisPtr());

		QueryData& queryData = mEventQueries[allocData.EventQueryIdx];
		queryData.QueryId = mNextQueryId++;
		queryData.Query->OnTriggered.Clear();
		queryData.Query->OnTriggered.Connect(std::bind(&MeshHeap::QueryTriggered, thisPtr, meshId, queryData.QueryId));
		queryData.Query->Begin();
	}

	// Note: Need to use a shared ptr here to ensure MeshHeap doesn't get deallocated sometime during this callback
	void MeshHeap::QueryTriggered(SPtr<MeshHeap> thisPtr, UINT32 meshId, UINT32 queryId)
	{
		auto findIter = thisPtr->mMeshAllocData.find(meshId);
		assert(findIter != thisPtr->mMeshAllocData.end());

		AllocatedData& allocData = findIter->second;

		// If query ids don't match then it means there either a more recent query or
		// the buffer was discarded and we are not interested in query result
		QueryData& queryData = thisPtr->mEventQueries[allocData.EventQueryIdx];
		if (queryId == queryData.QueryId)
		{
			assert(allocData.UseFlags != UseFlags::Free && allocData.UseFlags != UseFlags::GPUFree);

			if (allocData.UseFlags == UseFlags::CPUFree)
			{
				allocData.UseFlags = UseFlags::Free;
				thisPtr->FreeEventQuery(allocData.EventQueryIdx);

				thisPtr->mFreeVertChunks.push_back(allocData.VertChunkIdx);
				thisPtr->mFreeIdxChunks.push_back(allocData.IdxChunkIdx);

				thisPtr->MergeWithNearbyChunks(allocData.VertChunkIdx, allocData.IdxChunkIdx);

				thisPtr->mMeshAllocData.erase(findIter);
			}
			else
				allocData.UseFlags = UseFlags::GPUFree;
		}

		queryData.Query->OnTriggered.Clear();
	}

	void MeshHeap::MergeWithNearbyChunks(UINT32 chunkVertIdx, UINT32 chunkIdxIdx)
	{
		// Merge vertex chunks
		ChunkData& vertChunk = mVertChunks[chunkVertIdx];
		for (auto& freeChunkIdx : mFreeVertChunks)
		{
			if (chunkVertIdx == freeChunkIdx)
				continue;

			ChunkData& curChunk = mVertChunks[freeChunkIdx];
			if (curChunk.Size == 0) // Already merged
				continue;

			bool merged = false;
			if (curChunk.Start == (vertChunk.Start + vertChunk.Size))
			{
				vertChunk.Size += curChunk.Size;

				merged = true;
			}
			else if ((curChunk.Start + curChunk.Size) == vertChunk.Start)
			{
				vertChunk.Start = curChunk.Start;
				vertChunk.Size += curChunk.Size;

				merged = true;
			}

			if (merged)
			{
				// We can't remove the chunk since that would break the indexing scheme, so
				// mark it as empty and set size to 0. It will be reused when needed.
				curChunk.Start = 0;
				curChunk.Size = 0;
				mEmptyVertChunks.push(freeChunkIdx);
			}
		}

		// Merge index chunks
		ChunkData& idxChunk = mIdxChunks[chunkIdxIdx];
		for (auto& freeChunkIdx : mFreeIdxChunks)
		{
			if (chunkIdxIdx == freeChunkIdx)
				continue;

			ChunkData& curChunk = mIdxChunks[freeChunkIdx];
			if (curChunk.Size == 0) // Already merged
				continue;

			bool merged = false;
			if (curChunk.Start == (idxChunk.Start + idxChunk.Size))
			{
				idxChunk.Size += curChunk.Size;

				merged = true;
			}
			else if ((curChunk.Start + curChunk.Size) == idxChunk.Start)
			{
				idxChunk.Start = curChunk.Start;
				idxChunk.Size += curChunk.Size;

				merged = true;
			}

			if (merged)
			{
				// We can't remove the chunk since that would break the indexing scheme, so
				// mark it as empty and set size to 0. It will be reused when needed.
				curChunk.Start = 0;
				curChunk.Size = 0;
				mEmptyIdxChunks.push(freeChunkIdx);
			}
		}
	}
	}
}
