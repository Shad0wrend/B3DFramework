//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Mesh/BsMeshData.h"
#include "Math/BsVector2.h"
#include "Math/BsVector3.h"
#include "Math/BsSphere.h"
#include "Math/BsAABox.h"
#include "Managers/BsHardwareBufferManager.h"
#include "Private/RTTI/BsMeshDataRTTI.h"
#include "RenderAPI/BsVertexDeclaration.h"
#include "RenderAPI/BsVertexDataDesc.h"
#include "Error/BsException.h"
#include "Debug/BsDebug.h"

namespace bs
{
	MeshData::MeshData(UINT32 numVertices, UINT32 numIndexes, const SPtr<VertexDataDesc>& vertexData, IndexType indexType)
	   :mNumVertices(numVertices), mNumIndices(numIndexes), mIndexType(indexType), mVertexData(vertexData)
	{
		AllocateInternalBuffer();
	}

	MeshData::MeshData()
		:mNumVertices(0), mNumIndices(0), mIndexType(IT_32BIT)
	{ }

	MeshData::~MeshData()
	{ }

	UINT32 MeshData::GetNumIndices() const
	{
		return mNumIndices;
	}

	UINT16* MeshData::GetIndices16() const
	{
		if(mIndexType != IT_16BIT)
			BS_EXCEPT(InternalErrorException, "Attempting to get 16bit index buffer, but internally allocated buffer is 32 bit.");

		UINT32 indexBufferOffset = GetIndexBufferOffset();

		return (UINT16*)(GetData() + indexBufferOffset);
	}

	UINT32* MeshData::GetIndices32() const
	{
		if(mIndexType != IT_32BIT)
			BS_EXCEPT(InternalErrorException, "Attempting to get 32bit index buffer, but internally allocated buffer is 16 bit.");

		UINT32 indexBufferOffset = GetIndexBufferOffset();

		return (UINT32*)(GetData() + indexBufferOffset);
	}

	UINT32 MeshData::GetInternalBufferSize() const
	{
		return GetIndexBufferSize() + GetStreamSize();
	}

	// TODO - This doesn't handle the case where multiple elements in same slot have different data types
	SPtr<MeshData> MeshData::Combine(const Vector<SPtr<MeshData>>& meshes, const Vector<Vector<SubMesh>>& allSubMeshes,
		Vector<SubMesh>& subMeshes)
	{
		UINT32 totalVertexCount = 0;
		UINT32 totalIndexCount = 0;
		for(auto& meshData : meshes)
		{
			totalVertexCount += meshData->GetNumVertices();
			totalIndexCount += meshData->GetNumIndices();
		}

		SPtr<VertexDataDesc> vertexData = bs_shared_ptr_new<VertexDataDesc>();
		
		Vector<VertexElement> combinedVertexElements;
		for(auto& meshData : meshes)
		{
			for(UINT32 i = 0; i < meshData->GetVertexDesc()->GetNumElements(); i++)
			{
				const VertexElement& newElement = meshData->GetVertexDesc()->GetElement(i);

				INT32 alreadyExistsIdx = -1;
				UINT32 idx = 0;

				for(auto& existingElement : combinedVertexElements)
				{
					if(newElement.GetSemantic() == existingElement.GetSemantic() && newElement.GetSemanticIdx() == existingElement.GetSemanticIdx()
						&& newElement.GetStreamIdx() == existingElement.GetStreamIdx())
					{
						if(newElement.GetType() != existingElement.GetType())
						{
							BS_EXCEPT(NotImplementedException, "Two elements have same semantics but different types. This is not supported.");
						}

						alreadyExistsIdx = idx;
						break;
					}

					idx++;
				}

				if(alreadyExistsIdx == -1)
				{
					combinedVertexElements.push_back(newElement);
					vertexData->AddVertElem(newElement.GetType(), newElement.GetSemantic(), newElement.GetSemanticIdx(), newElement.GetStreamIdx());
				}
			}
		}

		SPtr<MeshData> combinedMeshData = bs_shared_ptr_new<MeshData>(totalVertexCount, totalIndexCount, vertexData);

		// Copy indices
		UINT32 vertexOffset = 0;
		UINT32 indexOffset = 0;
		UINT32* idxPtr = combinedMeshData->GetIndices32();
		for(auto& meshData : meshes)
		{
			UINT32 numIndices = meshData->GetNumIndices();
			UINT32* srcData = meshData->GetIndices32();

			for(UINT32 j = 0; j < numIndices; j++)
				idxPtr[j] = srcData[j] + vertexOffset;

			indexOffset += numIndices;
			idxPtr += numIndices;
			vertexOffset += meshData->GetNumVertices();
		}

		// Copy sub-meshes
		UINT32 meshIdx = 0;
		indexOffset = 0;
		for(auto& meshData : meshes)
		{
			UINT32 numIndices = meshData->GetNumIndices();
			const Vector<SubMesh> curSubMeshes = allSubMeshes[meshIdx];

			for(auto& subMesh : curSubMeshes)
			{
				subMeshes.push_back(SubMesh(subMesh.indexOffset + indexOffset, subMesh.indexCount, subMesh.drawOp));
			}

			indexOffset += numIndices;
			meshIdx++;
		}

		// Copy vertices
		vertexOffset = 0;
		for(auto& meshData : meshes)
		{
			for(auto& element : combinedVertexElements)
			{
				UINT32 dstVertexStride = vertexData->GetVertexStride(element.GetStreamIdx());
				UINT8* dstData = combinedMeshData->GetElementData(element.GetSemantic(), element.GetSemanticIdx(), element.GetStreamIdx());
				dstData += vertexOffset * dstVertexStride;

				UINT32 numSrcVertices = meshData->GetNumVertices();
				UINT32 vertexSize = vertexData->GetElementSize(element.GetSemantic(), element.GetSemanticIdx(), element.GetStreamIdx());

				if(meshData->GetVertexDesc()->HasElement(element.GetSemantic(), element.GetSemanticIdx(), element.GetStreamIdx()))
				{
					UINT32 srcVertexStride = meshData->GetVertexDesc()->GetVertexStride(element.GetStreamIdx());
					UINT8* srcData = meshData->GetElementData(element.GetSemantic(), element.GetSemanticIdx(), element.GetStreamIdx());

					for(UINT32 i = 0; i < numSrcVertices; i++)
					{
						memcpy(dstData, srcData, vertexSize);
						dstData += dstVertexStride;
						srcData += srcVertexStride;
					}
				}
				else
				{
					for(UINT32 i = 0; i < numSrcVertices; i++)
					{
						memset(dstData, 0, vertexSize);
						dstData += dstVertexStride;
					}
				}
			}

			vertexOffset += meshData->GetNumVertices();
		}

		return combinedMeshData;
	}

	void MeshData::SetVertexData(VertexElementSemantic semantic, void* data, UINT32 size, UINT32 semanticIdx, UINT32 streamIdx)
	{
		assert(data != nullptr);

		if(!mVertexData->HasElement(semantic, semanticIdx, streamIdx))
		{
			BS_LOG(Warning, Mesh, "MeshData doesn't contain an element of specified type: Semantic: {0}, "
				"Semantic index: {1}, Stream index: {2}", semantic, semanticIdx, streamIdx);
			return;
		}

		UINT32 elementSize = mVertexData->GetElementSize(semantic, semanticIdx, streamIdx);
		UINT32 totalSize = elementSize * mNumVertices;

		if(totalSize != size)
		{
			BS_EXCEPT(InvalidParametersException, "Buffer sizes don't match. Expected: " + toString(totalSize) + ". Got: " + toString(size));
		}

		UINT32 indexBufferOffset = GetIndexBufferSize();

		UINT32 elementOffset = GetElementOffset(semantic, semanticIdx, streamIdx);
		UINT32 vertexStride = mVertexData->GetVertexStride(streamIdx);

		UINT8* dst = GetData() + indexBufferOffset + elementOffset;
		UINT8* src = (UINT8*)data;
		for(UINT32 i = 0; i < mNumVertices; i++)
		{
			memcpy(dst, src, elementSize);
			dst += vertexStride;
			src += elementSize;
		}
	}

	void MeshData::GetVertexData(VertexElementSemantic semantic, void* data, UINT32 size, UINT32 semanticIdx, UINT32 streamIdx)
	{
		assert(data != nullptr);

		if (!mVertexData->HasElement(semantic, semanticIdx, streamIdx))
		{
			BS_LOG(Warning, Mesh, "MeshData doesn't contain an element of specified type: Semantic: {0}, "
				"Semantic index: {1}, Stream index: {2}", semantic, semanticIdx, streamIdx);
			return;
		}

		UINT32 elementSize = mVertexData->GetElementSize(semantic, semanticIdx, streamIdx);
		UINT32 totalSize = elementSize * mNumVertices;

		if (totalSize != size)
		{
			BS_EXCEPT(InvalidParametersException, "Buffer sizes don't match. Expected: " + toString(totalSize) + ". Got: " + toString(size));
		}

		UINT32 indexBufferOffset = GetIndexBufferSize();

		UINT32 elementOffset = GetElementOffset(semantic, semanticIdx, streamIdx);
		UINT32 vertexStride = mVertexData->GetVertexStride(streamIdx);

		UINT8* src = GetData() + indexBufferOffset + elementOffset;
		UINT8* dst = (UINT8*)data;
		for (UINT32 i = 0; i < mNumVertices; i++)
		{
			memcpy(dst, src, elementSize);
			dst += elementSize;
			src += vertexStride;
		}
	}

	VertexElemIter<Vector2> MeshData::GetVec2DataIter(VertexElementSemantic semantic, UINT32 semanticIdx, UINT32 streamIdx)
	{
		UINT8* data;
		UINT32 vertexStride;
		GetDataForIterator(semantic, semanticIdx, streamIdx, data, vertexStride);

		return VertexElemIter<Vector2>(data, vertexStride, mNumVertices);
	}

	VertexElemIter<Vector3> MeshData::GetVec3DataIter(VertexElementSemantic semantic, UINT32 semanticIdx, UINT32 streamIdx)
	{
		UINT8* data;
		UINT32 vertexStride;
		GetDataForIterator(semantic, semanticIdx, streamIdx, data, vertexStride);

		return VertexElemIter<Vector3>(data, vertexStride, mNumVertices);
	}

	VertexElemIter<Vector4> MeshData::GetVec4DataIter(VertexElementSemantic semantic, UINT32 semanticIdx, UINT32 streamIdx)
	{
		UINT8* data;
		UINT32 vertexStride;
		GetDataForIterator(semantic, semanticIdx, streamIdx, data, vertexStride);

		return VertexElemIter<Vector4>(data, vertexStride, mNumVertices);
	}

	VertexElemIter<UINT32> MeshData::GetDwordDataIter(VertexElementSemantic semantic, UINT32 semanticIdx, UINT32 streamIdx)
	{
		UINT8* data;
		UINT32 vertexStride;
		GetDataForIterator(semantic, semanticIdx, streamIdx, data, vertexStride);

		return VertexElemIter<UINT32>(data, vertexStride, mNumVertices);
	}

	void MeshData::GetDataForIterator(VertexElementSemantic semantic, UINT32 semanticIdx, UINT32 streamIdx, UINT8*& data, UINT32& stride) const
	{
		if(!mVertexData->HasElement(semantic, semanticIdx, streamIdx))
		{
			BS_EXCEPT(InvalidParametersException, "MeshData doesn't contain an element of specified type: Semantic: " + toString(semantic) + ", Semantic index: "
				+ toString(semanticIdx) + ", Stream index: " + toString(streamIdx));
		}

		UINT32 indexBufferOffset = GetIndexBufferSize();

		UINT32 elementOffset = GetElementOffset(semantic, semanticIdx, streamIdx);

		data = GetData() + indexBufferOffset + elementOffset;
		stride = mVertexData->GetVertexStride(streamIdx);
	}

	UINT32 MeshData::GetIndexBufferOffset() const
	{
		return 0;
	}

	UINT32 MeshData::GetStreamOffset(UINT32 streamIdx) const
	{
		UINT32 streamOffset = mVertexData->GetStreamOffset(streamIdx);

		return streamOffset * mNumVertices;
	}

	UINT8* MeshData::GetElementData(VertexElementSemantic semantic, UINT32 semanticIdx, UINT32 streamIdx) const
	{
		return GetData() + GetIndexBufferSize() + GetElementOffset(semantic, semanticIdx, streamIdx);
	}

	UINT8* MeshData::GetStreamData(UINT32 streamIdx) const
	{
		return GetData() + GetIndexBufferSize() + GetStreamOffset(streamIdx);
	}

	UINT32 MeshData::GetIndexElementSize() const
	{
		return mIndexType == IT_32BIT ? sizeof(UINT32) : sizeof(UINT16);
	}

	UINT32 MeshData::GetElementOffset(VertexElementSemantic semantic, UINT32 semanticIdx, UINT32 streamIdx) const
	{
		return GetStreamOffset(streamIdx) + mVertexData->GetElementOffsetFromStream(semantic, semanticIdx, streamIdx);
	}

	UINT32 MeshData::GetIndexBufferSize() const
	{
		return mNumIndices * GetIndexElementSize();
	}

	UINT32 MeshData::GetStreamSize(UINT32 streamIdx) const
	{
		return mVertexData->GetVertexStride(streamIdx) * mNumVertices;
	}

	UINT32 MeshData::GetStreamSize() const
	{
		return mVertexData->GetVertexStride() * mNumVertices;
	}

	Bounds MeshData::CalculateBounds() const
	{
		Bounds bounds;

		SPtr<VertexDataDesc> vertexDesc = GetVertexDesc();
		for (UINT32 i = 0; i < vertexDesc->GetNumElements(); i++)
		{
			const VertexElement& curElement = vertexDesc->GetElement(i);

			if (curElement.GetSemantic() != VES_POSITION || (curElement.GetType() != VET_FLOAT3 && curElement.GetType() != VET_FLOAT4))
				continue;

			UINT8* data = GetElementData(curElement.GetSemantic(), curElement.GetSemanticIdx(), curElement.GetStreamIdx());
			UINT32 stride = vertexDesc->GetVertexStride(curElement.GetStreamIdx());

			if (GetNumVertices() > 0)
			{
				Vector3 curPosition = *(Vector3*)data;
				Vector3 accum = curPosition;
				Vector3 min = curPosition;
				Vector3 max = curPosition;

				for (UINT32 i = 1; i < GetNumVertices(); i++)
				{
					curPosition = *(Vector3*)(data + stride * i);
					accum += curPosition;
					min = Vector3::Min(min, curPosition);
					max = Vector3::Max(max, curPosition);
				}

				Vector3 center = accum / (float)GetNumVertices();
				float radiusSqrd = 0.0f;

				for (UINT32 i = 0; i < GetNumVertices(); i++)
				{
					curPosition = *(Vector3*)(data + stride * i);
					float dist = center.SquaredDistance(curPosition);

					if (dist > radiusSqrd)
						radiusSqrd = dist;
				}

				float radius = Math::Sqrt(radiusSqrd);

				bounds = Bounds(AABox(min, max), Sphere(center, radius));
				break;
			}
		}

		return bounds;
	}

	/************************************************************************/
	/* 								SERIALIZATION                      		*/
	/************************************************************************/

	RTTITypeBase* MeshData::GetRttiStatic()
	{
		return MeshDataRTTI::Instance();
	}

	RTTITypeBase* MeshData::GetRtti() const
	{
		return MeshData::GetRttiStatic();
	}
}
