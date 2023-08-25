//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Image/BsTextureAtlasLayout.h"
#include "Debug/BsDebug.h"
#include "Utility/BsBitwise.h"

using namespace bs;

bool TextureAtlasLayout::AddElement(u32 width, u32 height, u32& x, u32& y)
{
	if(width == 0 || height == 0)
	{
		x = 0;
		y = 0;
		return true;
	}

	// Try adding without expanding, if that fails try to expand
	if(!AddToNode(0, width, height, x, y, false))
	{
		if(!AddToNode(0, width, height, x, y, true))
			return false;
	}

	// Update size to cover all nodes
	if(mPow2)
	{
		mWidth = std::max(mWidth, Bitwise::NextPow2(x + width));
		mHeight = std::max(mHeight, Bitwise::NextPow2(y + height));
	}
	else
	{
		mWidth = std::max(mWidth, x + width);
		mHeight = std::max(mHeight, y + height);
	}

	return true;
}

void TextureAtlasLayout::Clear()
{
	mNodes.clear();
	mNodes.push_back(TexAtlasNode(0, 0, mWidth, mHeight));

	mWidth = mInitialWidth;
	mHeight = mInitialHeight;
}

bool TextureAtlasLayout::AddToNode(u32 nodeIdx, u32 width, u32 height, u32& x, u32& y, bool allowGrowth)
{
	TexAtlasNode* node = &mNodes[nodeIdx];
	float aspect = node->Width / (float)node->Height;

	if(node->Children[0] != (u32)-1)
	{
		if(AddToNode(node->Children[0], width, height, x, y, allowGrowth))
			return true;

		return AddToNode(node->Children[1], width, height, x, y, allowGrowth);
	}
	else
	{
		if(node->NodeFull)
			return false;

		if(width > node->Width || height > node->Height)
			return false;

		if(!allowGrowth)
		{
			if(node->X + width > mWidth || node->Y + height > mHeight)
				return false;
		}

		if(width == node->Width && height == node->Height)
		{
			x = node->X;
			y = node->Y;
			node->NodeFull = true;

			return true;
		}

		float dw = (float)(node->Width - width);
		float dh = (node->Height - height) * aspect;

		u32 nextChildIdx = (u32)mNodes.size();
		node->Children[0] = nextChildIdx;
		node->Children[1] = nextChildIdx + 1;

		TexAtlasNode nodeCopy = *node;
		node = nullptr; // Undefined past this point
		if(dw > dh)
		{
			mNodes.emplace_back(nodeCopy.X, nodeCopy.Y, width, nodeCopy.Height);
			mNodes.emplace_back(nodeCopy.X + width, nodeCopy.Y, nodeCopy.Width - width, nodeCopy.Height);
		}
		else
		{
			mNodes.emplace_back(nodeCopy.X, nodeCopy.Y, nodeCopy.Width, height);
			mNodes.emplace_back(nodeCopy.X, nodeCopy.Y + height, nodeCopy.Width, nodeCopy.Height - height);
		}

		return AddToNode(nodeCopy.Children[0], width, height, x, y, allowGrowth);
	}
}

Vector<TextureAtlasUtility::Page> TextureAtlasUtility::CreateAtlasLayout(Vector<Element>& elements, u32 width, u32 height, u32 maxWidth, u32 maxHeight, bool pow2)
{
	for(size_t i = 0; i < elements.size(); i++)
	{
		elements[i].Output.Idx = (u32)i; // Preserve original index before sorting
		elements[i].Output.Page = -1;
	}

	std::sort(elements.begin(), elements.end(), [](const Element& a, const Element& b)
			  { return a.Input.Width * a.Input.Height > b.Input.Width * b.Input.Height; });

	Vector<TextureAtlasLayout> layouts;
	u32 remainingCount = (u32)elements.size();
	while(remainingCount > 0)
	{
		layouts.push_back(TextureAtlasLayout(width, height, maxWidth, maxHeight, pow2));
		TextureAtlasLayout& curLayout = layouts.back();

		// Find largest unassigned element that fits
		u32 sizeLimit = std::numeric_limits<u32>::max();
		while(true)
		{
			u32 largestId = -1;

			// Assumes elements are sorted from largest to smallest
			for(u32 i = 0; i < (u32)elements.size(); i++)
			{
				if(elements[i].Output.Page == -1)
				{
					u32 size = elements[i].Input.Width * elements[i].Input.Height;
					if(size < sizeLimit)
					{
						largestId = i;
						break;
					}
				}
			}

			if(largestId == (u32)-1)
				break; // Nothing fits, start a new page

			Element& element = elements[largestId];

			// Check if an element is too large to ever fit
			if(element.Input.Width > maxWidth || element.Input.Height > maxHeight)
			{
				B3D_LOG(Warning, Generic, "Some of the provided elements don't fit in an atlas of provided size. "
										 "Returning empty array of pages.");
				return Vector<Page>();
			}

			if(curLayout.AddElement(element.Input.Width, element.Input.Height, element.Output.X, element.Output.Y))
			{
				element.Output.Page = (u32)layouts.size() - 1;
				remainingCount--;
			}
			else
				sizeLimit = element.Input.Width * element.Input.Height;
		}
	}

	Vector<Page> pages;
	for(auto& layout : layouts)
		pages.push_back({ layout.GetWidth(), layout.GetHeight() });

	return pages;
}

// Based on: https://github.com/nical/guillotiere
DynamicTextureAtlasLayout::DynamicTextureAtlasLayout(const DynamicTextureAtlasLayoutSettings& settings)
	: mSettings(settings)
{
	B3D_ENSURE(settings.SmallSizeLimit < settings.LargeSizeLimit);
	B3D_ENSURE(settings.Size.Width > 0 && settings.Size.Height > 0);
	B3D_ENSURE(settings.Alignment.Width > 0 && settings.Alignment.Height > 0);
	B3D_ENSURE(settings.Alignment.Width <= settings.Size.Width);
	B3D_ENSURE(settings.Alignment.Height <= settings.Size.Height);

	Node rootNode;
	rootNode.Area = Rect2I(0, 0, mSettings.Size.Width, mSettings.Size.Height);
	rootNode.State = NodeState::Free;

	mFreeNodeBuckets[0].Size = mSettings.SmallSizeLimit;
	mFreeNodeBuckets[1].Size = mSettings.LargeSizeLimit;
	mFreeNodeBuckets[2].Size = ~0u;

	mNodes.push_back(rootNode);
	mRootNodeIndex = 0;

	FreeNodeBucket& rootNodeBucket = mFreeNodeBuckets[GetFreeNodeBucketForSize(mSettings.Size)];
	rootNodeBucket.FreeNodes.push_back(0);
}

Optional<DynamicTextureAtlasLayout::Allocation> DynamicTextureAtlasLayout::AddElement(const Size2UI& size)
{
	if(size.Width == 0 || size.Height == 0)
		return {};

	const Size2UI alignedSize = AlignSize(size);

	u32 bestFreeNodeId = FindBestFreeNode(alignedSize);
	if(bestFreeNodeId == ~0u)
		return {};

	Node& bestFreeNode = mNodes[bestFreeNodeId]; 

	const Rect2I allocatedArea(bestFreeNode.Area.X, bestFreeNode.Area.Y, alignedSize.Width, alignedSize.Height);
	const NodeSplitResult splitResult = Split(bestFreeNode, alignedSize);
	const NodeOrientation bestFreeNodeFlippedOrientation = bestFreeNode.Orientation == NodeOrientation::Horizontal ? NodeOrientation::Vertical : NodeOrientation::Horizontal;

	u32 allocatedNodeId = ~0u;
	u32 smallerLeftoverNodeId = ~0u;
	u32 largerLeftoverNodeId = ~0u;

	if(splitResult.LargerAreaOrientation == bestFreeNode.Orientation)
	{
		// Add larger node as sibling to the best fit node
		if(splitResult.LargerLeftoverArea != Rect2I::kEmpty)
		{
			const u32 nextSiblingId = bestFreeNode.NextSiblingId;
			largerLeftoverNodeId = AllocateNode();

			Node& largerLeftoverNode = mNodes[largerLeftoverNodeId];
			largerLeftoverNode.ParentNodeId = bestFreeNode.ParentNodeId;
			largerLeftoverNode.NextSiblingId = nextSiblingId;
			largerLeftoverNode.PreviousSiblingId = bestFreeNodeId;
			largerLeftoverNode.Area = splitResult.LargerLeftoverArea;
			largerLeftoverNode.State = NodeState::Free;
			largerLeftoverNode.Orientation = bestFreeNode.Orientation;

			bestFreeNode.NextSiblingId = largerLeftoverNodeId;
			if(nextSiblingId != ~0u)
				mNodes[nextSiblingId].PreviousSiblingId = largerLeftoverNodeId;
		}

		// Split best fit node
		if(splitResult.SmallerLeftoverArea != Rect2I::kEmpty)
		{
			bestFreeNode.State = NodeState::Container;

			allocatedNodeId = AllocateNode();
			smallerLeftoverNodeId = AllocateNode();

			Node& allocatedNode = mNodes[allocatedNodeId];
			allocatedNode.ParentNodeId = bestFreeNodeId;
			allocatedNode.NextSiblingId = smallerLeftoverNodeId;
			allocatedNode.Area = allocatedArea;
			allocatedNode.State = NodeState::Allocated;
			allocatedNode.Orientation = bestFreeNodeFlippedOrientation;

			Node& smallerLeftoverNode = mNodes[smallerLeftoverNodeId];
			smallerLeftoverNode.ParentNodeId = bestFreeNodeId;
			smallerLeftoverNode.PreviousSiblingId = allocatedNodeId;
			smallerLeftoverNode.Area = splitResult.SmallerLeftoverArea;
			smallerLeftoverNode.State = NodeState::Free;
			smallerLeftoverNode.Orientation = bestFreeNodeFlippedOrientation;
			
		}
		// Allocated area fits in the best fit node with no leftover
		else
		{
			allocatedNodeId = bestFreeNodeId;
			bestFreeNode.State = NodeState::Allocated;
			bestFreeNode.Area = allocatedArea;
		}
	}
	else
	{
		bestFreeNode.State = NodeState::Container;

		if(splitResult.LargerLeftoverArea != Rect2I::kEmpty)
		{
			largerLeftoverNodeId = AllocateNode();

			Node& largerLeftoverNode = mNodes[largerLeftoverNodeId];
			largerLeftoverNode.ParentNodeId = bestFreeNodeId;
			largerLeftoverNode.Area = splitResult.LargerLeftoverArea;
			largerLeftoverNode.State = NodeState::Free;
			largerLeftoverNode.Orientation = bestFreeNodeFlippedOrientation;
		}

		if(splitResult.SmallerLeftoverArea != Rect2I::kEmpty)
		{
			const u32 containerNodeId = AllocateNode();
			allocatedNodeId = AllocateNode();
			smallerLeftoverNodeId = AllocateNode();

			Node& containerNode = mNodes[containerNodeId];
			containerNode.ParentNodeId = bestFreeNodeId;
			containerNode.PreviousSiblingId = largerLeftoverNodeId;
			containerNode.State = NodeState::Container;
			containerNode.Orientation = bestFreeNodeFlippedOrientation;

			if(largerLeftoverNodeId != ~0u)
				mNodes[largerLeftoverNodeId].NextSiblingId = containerNodeId;

			Node& allocatedNode = mNodes[allocatedNodeId];
			allocatedNode.ParentNodeId = containerNodeId;
			allocatedNode.NextSiblingId = smallerLeftoverNodeId;
			allocatedNode.Area = allocatedArea;
			allocatedNode.State = NodeState::Allocated;
			allocatedNode.Orientation = bestFreeNode.Orientation;

			Node& smallerLeftoverNode = mNodes[smallerLeftoverNodeId];
			smallerLeftoverNode.ParentNodeId = containerNodeId;
			smallerLeftoverNode.PreviousSiblingId = allocatedNodeId;
			smallerLeftoverNode.Area = splitResult.SmallerLeftoverArea;
			smallerLeftoverNode.State = NodeState::Free;
			smallerLeftoverNode.Orientation = bestFreeNode.Orientation;
		}
		else
		{
			allocatedNodeId = AllocateNode();
			
			Node& allocatedNode = mNodes[allocatedNodeId];
			allocatedNode.ParentNodeId = bestFreeNodeId;
			allocatedNode.NextSiblingId = largerLeftoverNodeId;
			allocatedNode.Area = allocatedArea;
			allocatedNode.State = NodeState::Allocated;
			allocatedNode.Orientation = bestFreeNodeFlippedOrientation;

			if(largerLeftoverNodeId != ~0u)
				mNodes[largerLeftoverNodeId].PreviousSiblingId = allocatedNodeId;
		}
	}

	if(smallerLeftoverNodeId != ~0u)
		RegisterFreeNode(smallerLeftoverNodeId, Size2UI(splitResult.SmallerLeftoverArea.Width, splitResult.SmallerLeftoverArea.Height));

	if(largerLeftoverNodeId != ~0u)
		RegisterFreeNode(largerLeftoverNodeId, Size2UI(splitResult.LargerLeftoverArea.Width, splitResult.LargerLeftoverArea.Height));

	Allocation output;
	output.NodeId = allocatedNodeId;
	output.Position = Vector2I(bestFreeNode.Area.X, bestFreeNode.Area.Y);

	return output;
}

void DynamicTextureAtlasLayout::RemoveElement(u32 nodeId)
{
	if(!B3D_ENSURE(nodeId < (u32)mNodes.size()))
		return;

	Node& nodeToFree = mNodes[nodeId];
	B3D_ENSURE(nodeToFree.State == NodeState::Allocated);

	nodeToFree.State = NodeState::Free;

	u32 currentNodeId = nodeId;
	while(true)
	{
		{
			Node& currentNode = mNodes[currentNodeId];

			if(currentNode.NextSiblingId != ~0u)
				MergeWithNextSibling(currentNodeId);

			const u32 previousSiblingId = currentNode.PreviousSiblingId;
			if(previousSiblingId != ~0u)
			{
				MergeWithNextSibling(previousSiblingId);
				currentNodeId = previousSiblingId;
			}
		}

		{
			Node& currentNode = mNodes[currentNodeId];
			const u32 parentNodeId = currentNode.ParentNodeId;
			if(currentNode.PreviousSiblingId == ~0u && currentNode.NextSiblingId == ~0u && parentNodeId != ~0u)
			{
				Node& parentNode = mNodes[parentNodeId];
				B3D_ENSURE(parentNode.State == NodeState::Container);

				parentNode.Area = currentNode.Area;
				parentNode.State = NodeState::Free;

				FreeNode(currentNodeId);
				currentNodeId = parentNodeId;
			}
			else
			{
				const Size2UI freedArea(currentNode.Area.Width, currentNode.Area.Height);
				RegisterFreeNode(currentNodeId, freedArea);
				break;
			}
		}
	}
}

u32 DynamicTextureAtlasLayout::FindBestFreeNode(const Size2UI& size)
{
	const u32 bestBucketIndex = GetFreeNodeBucketForSize(size);
	const bool useWorstFit = bestBucketIndex == (u32)(mFreeNodeBuckets.size() - 1); // Worst fit for bucket containing large objects

	for(u32 bucketIndex = bestBucketIndex; bucketIndex < (u32)(mFreeNodeBuckets.size() - 1); ++bucketIndex)
	{
		u32 bestScore = useWorstFit ? 0 : std::numeric_limits<u32>::max();
		u32 bestNodeIndex = ~0u;
		u32 bestBucketEntryIndex = ~0u;

		FreeNodeBucket& bucket = mFreeNodeBuckets[bucketIndex];
		for(u32 bucketEntryIndex = 0; bucketEntryIndex < (u32)bucket.FreeNodes.size(); ++bucketEntryIndex)
		{
			const u32 nodeIndex = bucket.FreeNodes[bucketEntryIndex];
			Node& node = mNodes[nodeIndex];

			// Merged nodes aren't removed from the free list, so we do it here
			if(node.State != NodeState::Free)
			{
				B3DSwapAndErase(bucket.FreeNodes, bucketEntryIndex);
				continue;
			}

			const i32 deltaX = (i32)size.Width - (i32)node.Area.Width;
			const i32 deltaY = (i32)size.Height - (i32)node.Area.Height;

			if(deltaX < 0 || deltaY < 0)
				continue;

			if(deltaX == 0 && deltaY == 0)
			{
				bestNodeIndex = nodeIndex;
				bestBucketEntryIndex = bucketEntryIndex;
				break;
			}

			const u32 score = Math::Min(deltaX, deltaY);
			if(useWorstFit && score > bestScore || !useWorstFit && score < bestScore)
			{
				bestScore = score;
				bestNodeIndex = nodeIndex;
				bestBucketEntryIndex = bucketEntryIndex;
			}
		}

		if(bestNodeIndex != ~0u)
		{
			B3DSwapAndErase(bucket.FreeNodes, bestBucketEntryIndex);
			return bestNodeIndex;
		}
	}

	return ~0u;
}

void DynamicTextureAtlasLayout::RegisterFreeNode(u32 nodeId, const Size2UI& size)
{
	B3D_ENSURE(mNodes[nodeId].State == NodeState::Free);

	const u32 bestBucketIndex = GetFreeNodeBucketForSize(size);
	mFreeNodeBuckets[bestBucketIndex].FreeNodes.push_back(nodeId);
}

Size2UI DynamicTextureAtlasLayout::AlignSize(const Size2UI& size) const
{
	Size2UI output;
	output.Width = Math::CeilToMultiple(size.Width, mSettings.Alignment.Width);
	output.Height = Math::CeilToMultiple(size.Height, mSettings.Alignment.Height);

	return output;
}

u32 DynamicTextureAtlasLayout::GetFreeNodeBucketForSize(const Size2UI& size) const
{
	const u32 largestDimension = Math::Max(size.Width, size.Height);
	for(u32 bucketIndex = 0; bucketIndex < (u32)mFreeNodeBuckets.size(); bucketIndex++)
	{
		if(largestDimension <= mFreeNodeBuckets[bucketIndex].Size)
			return bucketIndex;
	}

	return (u32)mFreeNodeBuckets.size() - 1;
}

DynamicTextureAtlasLayout::NodeSplitResult DynamicTextureAtlasLayout::Split(const Node& nodeToSplit, const Size2UI& requiredSize) const
{
	NodeSplitResult result;

	if(requiredSize.Width > nodeToSplit.Area.Width || requiredSize.Height > nodeToSplit.Area.Height)
	{
		B3D_ENSURE(false);
		return result;
	}

	if(requiredSize.Width == nodeToSplit.Area.Width && requiredSize.Height == nodeToSplit.Area.Height)
	{
		result.LargerAreaOrientation = nodeToSplit.Orientation;
		return result;
	}

	const Rect2I rightLeftoverArea(
		nodeToSplit.Area.X + (i32)requiredSize.Width,
		nodeToSplit.Area.Y,
		nodeToSplit.Area.Width - requiredSize.Width,
		requiredSize.Height);

	const Rect2I bottomLeftoverArea(
		nodeToSplit.Area.X,
		nodeToSplit.Area.Y + (i32)requiredSize.Height,
		requiredSize.Width,
		nodeToSplit.Area.Height - requiredSize.Height);

	if(rightLeftoverArea.Width * rightLeftoverArea.Height < bottomLeftoverArea.Width * bottomLeftoverArea.Height)
	{
		result.SmallerLeftoverArea = rightLeftoverArea;
		result.LargerLeftoverArea = bottomLeftoverArea;
		result.LargerLeftoverArea.Width = nodeToSplit.Area.Width;
		result.LargerAreaOrientation = NodeOrientation::Vertical;
	}
	else
	{
		result.SmallerLeftoverArea = bottomLeftoverArea;
		result.LargerLeftoverArea = rightLeftoverArea;
		result.LargerLeftoverArea.Height = nodeToSplit.Area.Height;
		result.LargerAreaOrientation = NodeOrientation::Horizontal;
	}

	return result;
}

u32 DynamicTextureAtlasLayout::AllocateNode()
{
	if(mUnusedNodeListHead < (u32)mNodes.size())
	{
		const u32 freeNodeId = mUnusedNodeListHead;
		mUnusedNodeListHead = mNodes[mUnusedNodeListHead].NextSiblingId;

		B3D_ENSURE(mNodes[freeNodeId].State == NodeState::Unused);
		return freeNodeId;
	}

	mNodes.push_back(Node());
	return (u32)mNodes.size() - 1;
}

void DynamicTextureAtlasLayout::FreeNode(u32 nodeId)
{
	mNodes[nodeId].State = NodeState::Unused;
	mNodes[nodeId].NextSiblingId = mUnusedNodeListHead;
	mUnusedNodeListHead = nodeId;
}

void DynamicTextureAtlasLayout::MergeWithNextSibling(u32 nodeId)
{
	Node& nodeToMerge = mNodes[nodeId];
	B3D_ENSURE(nodeToMerge.State == NodeState::Free);

	if(nodeToMerge.NextSiblingId == ~0u)
		return;

	const u32 nextSiblingId = nodeToMerge.NextSiblingId;
	Node& nextNode = mNodes[nextSiblingId];
	if(nextNode.State != NodeState::Free)
		return;

	if(nodeToMerge.Orientation == NodeOrientation::Horizontal)
	{
		B3D_ENSURE(nodeToMerge.Area.Y == nextNode.Area.Y);
		B3D_ENSURE(nodeToMerge.Area.Height == nextNode.Area.Height);

		nodeToMerge.Area.Width += nextNode.Area.Width;
	}
	else
	{
		B3D_ENSURE(nodeToMerge.Area.X == nextNode.Area.X);
		B3D_ENSURE(nodeToMerge.Area.Width == nextNode.Area.Width);

		nodeToMerge.Area.Height += nextNode.Area.Height;
	}

	const u32 nextNextSiblingId = nextNode.NextSiblingId;
	nodeToMerge.NextSiblingId = nextNextSiblingId;
	if(nextNextSiblingId != ~0u)
		mNodes[nextNextSiblingId].PreviousSiblingId = nodeId;
	
	FreeNode(nextSiblingId);
}

void DynamicTextureAtlasLayout::Clear()
{
	mNodes.clear();
	mUnusedNodeListHead = ~0u;

	for(auto& bucket : mFreeNodeBuckets)
		bucket.FreeNodes.clear();

	Node rootNode;
	rootNode.Area = Rect2I(0, 0, mSettings.Size.Width, mSettings.Size.Height);
	rootNode.State = NodeState::Free;

	mNodes.push_back(rootNode);
	mRootNodeIndex = 0;

	FreeNodeBucket& rootNodeBucket = mFreeNodeBuckets[GetFreeNodeBucketForSize(mSettings.Size)];
	rootNodeBucket.FreeNodes.push_back(0);
}

