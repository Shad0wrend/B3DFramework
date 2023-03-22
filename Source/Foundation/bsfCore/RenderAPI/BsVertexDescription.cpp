//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "RenderAPI/BsVertexDescription.h"
#include "Managers/BsHardwareBufferManager.h"
#include "Private/RTTI/BsVertexDescriptionRTTI.h"

using namespace bs;

VertexDescription::VertexDescription(const SmallVector<VertexElement, 8>& elements)
	:mVertexElements(elements)
{
	// Sort by stream, but preserve remaining ordering
	std::stable_sort(mVertexElements.begin(), mVertexElements.end(), [](const VertexElement& lhs, const VertexElement& rhs)
	{
		return lhs.GetStreamIndex() < rhs.GetStreamIndex();
	});

	CalculateOffsets();
}

void VertexDescription::CalculateOffsets()
{
	const u32 largestStreamIndex = GetLargestStreamIndex();
	const u32 streamCount = largestStreamIndex + 1;

	u32* streamOffsets = B3DStackAllocate<u32>(streamCount);
	B3DZeroOut(streamOffsets, streamCount);

	for(auto& element : mVertexElements)
	{
		const u32 streamIndex = element.GetStreamIndex();

		element.mOffset = streamOffsets[streamIndex];
		streamOffsets[streamIndex] += element.GetSize();
	}

	B3DStackFree(streamOffsets);
}

u32 VertexDescription::GetLargestStreamIndex() const
{
	u32 maxStreamIdx = 0;
	u32 numElems = (u32)mVertexElements.size();
	for(u32 i = 0; i < numElems; i++)
	{
		for(auto& vertElem : mVertexElements)
		{
			maxStreamIdx = std::max((u32)maxStreamIdx, (u32)vertElem.GetStreamIndex());
		}
	}

	return maxStreamIdx;
}

bool VertexDescription::HasStream(u32 streamIndex) const
{
	for(auto& vertElem : mVertexElements)
	{
		if(vertElem.GetStreamIndex() == streamIndex)
			return true;
	}

	return false;
}

bool VertexDescription::HasElement(VertexElementSemantic semantic, u32 semanticIndex, u32 streamIndex) const
{
	auto findIter = std::find_if(mVertexElements.begin(), mVertexElements.end(), [semantic, semanticIndex, streamIndex](const VertexElement& x)
								 { return x.GetSemantic() == semantic && x.GetSemanticIndex() == semanticIndex && x.GetStreamIndex() == streamIndex; });

	if(findIter != mVertexElements.end())
	{
		return true;
	}

	return false;
}

u32 VertexDescription::GetElementSize(VertexElementSemantic semantic, u32 semanticIndex, u32 streamIndex) const
{
	for(auto& element : mVertexElements)
	{
		if(element.GetSemantic() == semantic && element.GetSemanticIndex() == semanticIndex && element.GetStreamIndex() == streamIndex)
			return element.GetSize();
	}

	return -1;
}

u32 VertexDescription::GetElementOffsetFromStream(VertexElementSemantic semantic, u32 semanticIndex, u32 streamIndex) const
{
	u32 vertexOffset = 0;
	for(auto& element : mVertexElements)
	{
		if(element.GetStreamIndex() != streamIndex)
			continue;

		if(element.GetSemantic() == semantic && element.GetSemanticIndex() == semanticIndex)
			break;

		vertexOffset += element.GetSize();
	}

	return vertexOffset;
}

u32 VertexDescription::GetVertexStride(u32 streamIndex) const
{
	u32 vertexStride = 0;
	for(auto& element : mVertexElements)
	{
		if(element.GetStreamIndex() == streamIndex)
			vertexStride += element.GetSize();
	}

	return vertexStride;
}

u32 VertexDescription::GetVertexStride() const
{
	u32 vertexStride = 0;
	for(auto& element : mVertexElements)
	{
		vertexStride += element.GetSize();
	}

	return vertexStride;
}

u32 VertexDescription::GetStreamOffset(u32 streamIndex) const
{
	u32 streamOffset = 0;
	for(auto& element : mVertexElements)
	{
		if(element.GetStreamIndex() == streamIndex)
			break;

		streamOffset += element.GetSize();
	}

	return streamOffset;
}

const VertexElement* VertexDescription::GetElement(VertexElementSemantic semantic, u32 semanticIndex, u32 streamIndex) const
{
	auto findIter = std::find_if(mVertexElements.begin(), mVertexElements.end(), [semantic, semanticIndex, streamIndex](const VertexElement& x)
								 { return x.GetSemantic() == semantic && x.GetSemanticIndex() == semanticIndex && x.GetStreamIndex() == streamIndex; });

	if(findIter != mVertexElements.end())
		return &(*findIter);

	return nullptr;
}

/************************************************************************/
/* 								SERIALIZATION                      		*/
/************************************************************************/

RTTITypeBase* VertexDescription::GetRttiStatic()
{
	return VertexDescriptionRTTI::Instance();
}

RTTITypeBase* VertexDescription::GetRtti() const
{
	return VertexDescription::GetRttiStatic();
}
