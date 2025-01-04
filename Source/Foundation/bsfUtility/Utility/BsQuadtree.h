//************************************ bs::framework - Copyright 2014 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Prerequisites/BsPrerequisitesUtil.h"
#include "Math/BsMath.h"
#include "Math/BsVector4I.h"
#include "Math/BsSIMD.h"
#include "Allocators/BsPoolAlloc.h"

namespace bs
{
	/** @addtogroup General
	 *  @{
	 */

	/** Identifier that may be used for finding an element in the quadtree. */
	class QuadtreeElementId
	{
	public:
		QuadtreeElementId() = default;

		QuadtreeElementId(void* node, u32 indexInNode)
			: Node(node), IndexInNode(indexInNode)
		{}

	private:
		template <class, class>
		friend class TQuadtree;

		void* Node = nullptr;
		u32 IndexInNode = 0u;
	};

	/** Contains a reference to one of the four child nodes in a quadtree node. */
	struct QuadtreeChildNodeId
	{
		union
		{
			struct
			{
				u32 X : 1;
				u32 Y : 1;
				u32 Empty : 1;
			};

			struct
			{
				u32 Index : 2;
				u32 Empty2 : 1;
			};
		};

		QuadtreeChildNodeId()
			: Empty(true)
		{}

		QuadtreeChildNodeId(u32 x, u32 y)
			: X(x), Y(y), Empty(false)
		{}

		QuadtreeChildNodeId(u32 index)
			: Index(index), Empty2(false)
		{}
	};

	/** Contains a range of child nodes in a quadtree node (any or all of the possible 4 nodes). */
	struct QuadtreeChildNodeIdRange
	{
		union
		{
			struct
			{
				u32 PositiveX : 1;
				u32 PositiveY : 1;
				u32 NegativeX : 1;
				u32 NegativeY : 1;
			};

			struct
			{
				u32 PositiveBits : 2;
				u32 NegativeBits : 2;
			};

			u32 AllBits : 4;
		};

		/** Constructs a range overlapping no nodes. */
		QuadtreeChildNodeIdRange()
			: AllBits(0)
		{}

		/** Constructs a range overlapping a single node. */
		QuadtreeChildNodeIdRange(QuadtreeChildNodeId child)
			: PositiveBits(child.Index), NegativeBits(~child.Index)
		{}

		/** Checks if the range contains the provided child. */
		bool Contains(QuadtreeChildNodeId child) const
		{
			QuadtreeChildNodeIdRange childRange(child);
			return (AllBits & childRange.AllBits) == childRange.AllBits;
		}
	};

	/**
	 * Spatial partitioning tree for 2D space.
	 *
	 * @tparam	ElementType	Type of elements to be stored in the tree.
	 * @tparam	Options		Class that controls various options of the tree. It must provide the following enums:
	 *							- LoosePadding: Denominator used to determine how much padding to add to each child node.
	 *											The extra padding percent is determined as (1.0f / LoosePadding). Larger
	 *											padding ensures elements are less likely to get stuck on a higher node
	 *											due to them straddling the boundary between the nodes.
	 *							- MinimumElementsPerNode: Determines at which point should node's children be removed and moved
	 *													  back into the parent (node is collapsed). This can occur on element
	 *													  removal, when the element count drops below the specified number.
	 *							- MaximumElementsPerNode: Determines at which point should a node be split into child nodes.
	 *													  If an element counter moves past this number the elements will be
	 *													  added to child nodes, if possible. If a node is already at maximum
	 *													  depth, this is ignored.
	 *							- MaximumDepth: Maximum depth of nodes in the tree. Nodes at this depth will not be subdivided
	 *											even if they element counts go past MaximumElementsPerNode.
	 *						It must also provide the following methods:
	 *							- "static simd::Rect2 GetBounds(const ElementType&, void*)"
	 *								- Returns the bounds for the provided element
	 *							- "static void SetElementId(const TQuadtree::ElementId&, void*)"
	 *								- Gets called when element's ID is first assigned or subsequently modified
	 */
	template <class ElementType, class Options>
	class TQuadtree
	{
		/**
		 * A sequential group of elements within a node. If number of elements exceeds the limit of the block multiple
		 * blocks will be linked together in a linked list fashion.
		 */
		struct NodeElementSuballocationBlock
		{
			ElementType Elements[Options::MaximumElementsPerNode];
			NodeElementSuballocationBlock* NextBlock = nullptr;
		};

		/**
		 * A sequential group of element bounds within a node. If number of elements exceeds the limit of the block multiple
		 * blocks will be linked together in a linked list fashion.
		 */
		struct NodeElementBoundsSuballocationBlock
		{
			simd::Rect2 Bounds[Options::MaximumElementsPerNode];
			NodeElementBoundsSuballocationBlock* NextBlock = nullptr;
		};

		/** Contains data about elements within a node. */
		struct NodeElementData
		{
			NodeElementSuballocationBlock* ElementsBlock = nullptr;
			NodeElementBoundsSuballocationBlock* ElementBoundsBlock = nullptr;
			u32 ElementCount = 0; /**< Number of elements on this node only. Only relevant for leaf nodes. */
		};

	public:
		/** Represents a single quadtree node. */
		class Node
		{
		public:
			/** Constructs a new leaf node with the specified parent. */
			Node(Node* parent): mParent(parent), mElementCountWithChildren(0), mIsLeaf(true) {}

			/** Returns a child node with the specified index. May return null. */
			Node* GetChild(QuadtreeChildNodeId child) const { return mChildren[child.Index]; }

			/** Checks has the specified child node been created. */
			bool HasChild(QuadtreeChildNodeId child) const { return mChildren[child.Index] != nullptr; }

		private:
			friend class ElementIterator;
			friend class TQuadtree;

			/**
			 * Maps an element index within the node to a specific sub-allocation block in which the element is located. Returns pointers
			 * to the relevant sub-allocation blocks as output parameters, and index within those blocks as the return value.
			 */
			u32 MapElementIndexToBlock(u32 indexInNode, NodeElementSuballocationBlock** elements, NodeElementBoundsSuballocationBlock** bounds);

			NodeElementData mElementData;
			Node* mParent;
			Node* mChildren[4] = { nullptr, nullptr, nullptr, nullptr };
			u32 mElementCountWithChildren : 31; /**< Total number of elements including all children of the node. */
			u32 mIsLeaf : 1;
		};

		/**
		 * Contains bounds for a specific node. This is necessary since the nodes themselves do not store bounds
		 * information. Instead we construct it on-the-fly as we traverse the tree, using this class.
		 */
		class NodeBounds
		{
		public:
			NodeBounds() = default;

			/** Initializes a new bounds object using the provided node bounds. */
			NodeBounds(const simd::Rect2& bounds);

			/** Returns the bounds of the node this object represents. */
			const simd::Rect2& GetBounds() const { return mBounds; }

			/** Attempts to find a child node that can fully contain the provided bounds. */
			QuadtreeChildNodeId FindContainingChild(const simd::Rect2& bounds) const;

			/** Returns a range of child nodes that intersect the provided bounds. */
			QuadtreeChildNodeIdRange FindIntersectingChildren(const simd::Rect2& bounds) const;

			/** Calculates bounds for the provided child node. */
			NodeBounds GetChild(QuadtreeChildNodeId child) const;

		private:
			simd::Rect2 mBounds;
			float mChildExtent;
			float mChildOffset;
		};

		/** Contains a information about a specific quadtree node, to be used during node traversal. */
		class NodeTraversalContext
		{
		public:
			NodeTraversalContext() = default;
			NodeTraversalContext(const Node* node, const NodeBounds& bounds): Node(node), Bounds(bounds) {}

			const Node* Node = nullptr;
			NodeBounds Bounds;
		};

		/**
		 * Iterator that iterates over quadtree nodes. By default only the first inserted node will be iterated over and it
		 * is up the the user to add new ones using PushChild(). The iterator takes care of updating the node bounds
		 * accordingly.
		 */
		class NodeIterator
		{
		public:
			/** Initializes the iterator, starting with the root quadtree node. */
			NodeIterator(const TQuadtree& tree);

			/** Initializes the iterator using a specific node and its bounds. */
			NodeIterator(const Node* node, const NodeBounds& bounds);

			/**
			 * Returns information about the current node. MoveNext() must be called at least once and it must return true
			 * prior to attempting to access this data.
			 */
			const NodeTraversalContext& GetCurrent() const { return mCurrentNodeContext; }

			/**
			 * Moves to the next entry in the iterator. Iterator starts at a position before the first element, therefore
			 * this method must be called at least once before attempting to access the current node. If the method returns
			 * false it means the iterator end has been reached and attempting to access data will result in an error.
			 */
			bool MoveNext();

			/** Inserts a child of the current node to be iterated over. */
			void PushChild(const QuadtreeChildNodeId& child);

		private:
			NodeTraversalContext mCurrentNodeContext;
			TInlineArray<NodeTraversalContext, Options::MaximumDepth * 4> mNodeStack;
		};

		/** Iterator that iterates over all elements in a single node. */
		class ElementIterator
		{
		public:
			ElementIterator() = default;

			/** Constructs an iterator that iterates over the specified node's elements. */
			ElementIterator(const Node* node);

			/**
			 * Moves to the next element in the node. Iterator starts at a position before the first element, therefore
			 * this method must be called at least once before attempting to access the current element data. If the method
			 * returns false it means iterator end has been reached and attempting to access data will result in an error.
			 */
			bool MoveNext();

			/**
			 * Returns the bounds of the current element. MoveNext() must be called at least once and it must return true
			 * prior to attempting to access this data.
			 */
			const simd::Rect2& GetCurrentBounds() const { return mCurrentElementBoundsBlock->Bounds[mCurrentIndex]; }

			/**
			 * Returns the contents of the current element. MoveNext() must be called at least once and it must return true
			 * prior to attempting to access this data.
			 */
			const ElementType& GetCurrentElement() const { return mCurrentElementBlock->Elements[mCurrentIndex]; }

		private:
			i32 mCurrentIndex = -1;
			NodeElementSuballocationBlock* mCurrentElementBlock = nullptr;
			NodeElementBoundsSuballocationBlock* mCurrentElementBoundsBlock = nullptr;
			u32 mEndOfGroupElementIndex = 0;
		};

		/** Iterators that iterates over all elements intersecting the specified AABox. */
		class AreaIntersectIterator
		{
		public:
			/** Constructs an iterator that iterates over all elements in the specified tree that intersect the specified bounds. */
			AreaIntersectIterator(const TQuadtree& tree, const Rect2& bounds);

			/**
			 * Returns the contents of the current element. MoveNext() must be called at least once and it must return true
			 * prior to attempting to access this data.
			 */
			const ElementType& GetElement() const { return mElementIterator.GetCurrentElement(); }

			/**
			 * Moves to the next intersecting element. Iterator starts at a position before the first element, therefore
			 * this method must be called at least once before attempting to access the current element data. If the method
			 * returns false it means iterator end has been reached and attempting to access data will result in an error.
			 */
			bool MoveNext();

		private:
			NodeIterator mNodeIterator;
			ElementIterator mElementIterator;
			simd::Rect2 mBounds;
		};

		/**
		 * Constructs an quadtree with the specified bounds.
		 *
		 * @param	center		Origin of the root node.
		 * @param	extent		Extent (half-size) of the root node in all directions;
		 * @param	context		Optional user context that will be passed along to GetBounds() and SetElementId() methods on the provided Options class.
		 */
		TQuadtree(const Vector2& center, float extent, void* context = nullptr);

		~TQuadtree() { FreeNode(&mRoot); }

		/** Adds a new element to the quadtree. */
		void AddElement(const ElementType& element) { AddElementToNode(element, &mRoot, mRootBounds); }

		/** Removes an existing element from the quadtree. */
		void RemoveElement(const QuadtreeElementId& elementId);

	private:
		/**
		 * Adds a new element to the specified node. If the provided node is not a leaf node, traverses the hierarchy until it finds the correct node.
		 * Potentially also subdivides the leaf node if subdivision conditions have been reached.
		 */
		void AddElementToNode(const ElementType& element, Node* node, const NodeBounds& nodeBounds);

		/**
		 * Adds a new element to the node's element list. This method does no additional checks so caller must ensure the node is a leaf node
		 * and does not need to be subdivided.
		 */
		void AddElementToLeafNode(Node* node, const ElementType& element, const simd::Rect2& elementBounds);

		/** Removes the specified element from the node's element list. */
		void RemoveElementFromLeafNode(Node* node, u32 elementIndexInNode);

		/** Frees all elements from a node. */
		void FreeElementData(NodeElementData& elementData);

		/** Cleans up memory used by the provided node. Should be called instead of the node destructor. */
		void FreeNode(Node* node);

		Node mRoot{ nullptr };
		NodeBounds mRootBounds;
		float mMinimumNodeExtent;
		void* mContext;

		PoolAlloc<sizeof(Node)> mNodeAllocator;
		PoolAlloc<sizeof(NodeElementSuballocationBlock)> mElementBlockAllocator;
		PoolAlloc<sizeof(NodeElementBoundsSuballocationBlock), 512, 16> mElementBoundsBlockAllocator;
	};

	template <class ElementType, class Options>
	u32 TQuadtree<ElementType, Options>::Node::MapElementIndexToBlock(u32 indexInNode, NodeElementSuballocationBlock** elements, NodeElementBoundsSuballocationBlock** bounds)
	{
		const u32 groupCount = Math::DivideAndRoundUp(mElementData.ElementCount, (u32)Options::MaximumElementsPerNode);
		const u32 groupIndex = groupCount - indexInNode / Options::MaximumElementsPerNode - 1;

		*elements = mElementData.ElementsBlock;
		*bounds = mElementData.ElementBoundsBlock;
		for(u32 i = 0; i < groupIndex; i++)
		{
			*elements = (*elements)->NextBlock;
			*bounds = (*bounds)->NextBlock;
		}

		return indexInNode % Options::MaximumElementsPerNode;
	}

	template <class ElementType, class Options>
	TQuadtree<ElementType, Options>::NodeBounds::NodeBounds(const simd::Rect2& bounds)
		: mBounds(bounds)
	{
		static constexpr float kChildExtentScale = 0.5f * (1.0f + 1.0f / (float)Options::LoosePadding);

		mChildExtent = bounds.Extents.X * kChildExtentScale;
		mChildOffset = bounds.Extents.X - mChildExtent;
	}

	template <class ElementType, class Options>
	QuadtreeChildNodeId TQuadtree<ElementType, Options>::NodeBounds::FindContainingChild(const simd::Rect2& bounds) const
	{
		auto queryCenter = simd::load<simd::float32x4>(&bounds.Center);

		auto nodeCenter = simd::load<simd::float32x4>(&mBounds.Center);
		auto childOffset = simd::load_splat<simd::float32x4>(&mChildOffset);

		auto negativeCenter = simd::sub(nodeCenter, childOffset);
		auto negativeDiff = simd::sub(queryCenter, negativeCenter);

		auto positiveCenter = simd::add(nodeCenter, childOffset);
		auto positiveDiff = simd::sub(positiveCenter, queryCenter);

		auto diff = simd::min(negativeDiff, positiveDiff);

		auto queryExtents = simd::load<simd::float32x4>(&bounds.Extents);
		auto childExtent = simd::load_splat<simd::float32x4>(&mChildExtent);

		QuadtreeChildNodeId output;

		simd::mask_float32x4 mask = simd::cmp_gt(simd::add(queryExtents, diff), childExtent);
		if(simd::test_bits_any(simd::bit_cast<simd::uint32x4>(mask)) == false)
		{
			auto ones = simd::make_uint<simd::uint32x4>(1, 1, 1, 1);
			auto zeroes = simd::make_uint<simd::uint32x4>(0, 0, 0, 0);

			// Find node closest to the query center
			mask = simd::cmp_gt(queryCenter, nodeCenter);
			auto result = simd::blend(ones, zeroes, mask);

			Vector4I scalarResult;
			simd::store(&scalarResult, result);

			output.X = scalarResult.X;
			output.Y = scalarResult.Y;

			output.Empty = false;
		}

		return output;
	}

	template <class ElementType, class Options>
	QuadtreeChildNodeIdRange TQuadtree<ElementType, Options>::NodeBounds::FindIntersectingChildren(const simd::Rect2& bounds) const
	{
		auto queryCenter = simd::load<simd::float32x4>(&bounds.Center);
		auto queryExtents = simd::load<simd::float32x4>(&bounds.Extents);

		auto queryMax = simd::add(queryCenter, queryExtents);
		auto queryMin = simd::sub(queryCenter, queryExtents);

		auto nodeCenter = simd::load<simd::float32x4>(&mBounds.Center);
		auto childOffset = simd::load_splat<simd::float32x4>(&mChildOffset);

		auto negativeCenter = simd::sub(nodeCenter, childOffset);
		auto positiveCenter = simd::add(nodeCenter, childOffset);

		auto childExtent = simd::load_splat<simd::float32x4>(&mChildExtent);
		auto negativeMax = simd::add(negativeCenter, childExtent);
		auto positiveMin = simd::sub(positiveCenter, childExtent);

		QuadtreeChildNodeIdRange output;

		auto ones = simd::make_uint<simd::uint32x4>(1, 1, 1, 1);
		auto zeroes = simd::make_uint<simd::uint32x4>(0, 0, 0, 0);

		simd::mask_float32x4 mask = simd::cmp_gt(queryMax, positiveMin);
		simd::uint32x4 result = simd::blend(ones, zeroes, mask);

		Vector4I scalarResult;
		simd::store(&scalarResult, result);

		output.PositiveX = scalarResult.X;
		output.PositiveY = scalarResult.Y;

		mask = simd::cmp_le(queryMin, negativeMax);
		result = simd::blend(ones, zeroes, mask);

		simd::store(&scalarResult, result);

		output.NegativeX = scalarResult.X;
		output.NegativeY = scalarResult.Y;

		return output;
	}

	template <class ElementType, class Options>
	typename TQuadtree<ElementType, Options>::NodeBounds TQuadtree<ElementType, Options>::NodeBounds::GetChild(QuadtreeChildNodeId child) const
	{
		static constexpr const float map[2] = { -1.0f, 1.0f };

		return NodeBounds(
			simd::Rect2(
				Vector2(
					mBounds.Center.X + mChildOffset * map[child.X],
					mBounds.Center.Y + mChildOffset * map[child.Y]),
				mChildExtent));
	}

	template <class ElementType, class Options>
	TQuadtree<ElementType, Options>::NodeIterator::NodeIterator(const TQuadtree& tree)
		: mCurrentNodeContext(NodeTraversalContext(&tree.mRoot, tree.mRootBounds))
	{
		mNodeStack.Add(mCurrentNodeContext);
	}

	template <class ElementType, class Options>
	TQuadtree<ElementType, Options>::NodeIterator::NodeIterator(const Node* node, const NodeBounds& bounds)
		: mCurrentNodeContext(NodeTraversalContext(node, bounds))
	{
		mNodeStack.Add(mCurrentNodeContext);
	}

	template <class ElementType, class Options>
	bool TQuadtree<ElementType, Options>::NodeIterator::MoveNext()
	{
		if(mNodeStack.Empty())
		{
			mCurrentNodeContext = NodeTraversalContext();
			return false;
		}

		mCurrentNodeContext = mNodeStack.Back();
		mNodeStack.Erase(mNodeStack.End() - 1);

		return true;
	}

	template <class ElementType, class Options>
	void TQuadtree<ElementType, Options>::NodeIterator::PushChild(const QuadtreeChildNodeId& child)
	{
		Node* const childNode = mCurrentNodeContext.Node->GetChild(child);
		NodeBounds childBounds = mCurrentNodeContext.Bounds.GetChild(child);

		mNodeStack.EmplaceBack(childNode, childBounds);
	}

	template <class ElementType, class Options>
	TQuadtree<ElementType, Options>::ElementIterator::ElementIterator(const Node* node)
		: mCurrentIndex(-1)
		, mCurrentElementBlock(node->mElementData.ElementsBlock)
		, mCurrentElementBoundsBlock(node->mElementData.ElementBoundsBlock)
	{
		const u32 groupCount = Math::DivideAndRoundUp(node->mElementData.ElementCount, (u32)Options::MaximumElementsPerNode);
		mEndOfGroupElementIndex = node->mElementData.ElementCount - (groupCount - 1) * Options::MaximumElementsPerNode;
	}

	template <class ElementType, class Options>
	bool TQuadtree<ElementType, Options>::ElementIterator::MoveNext()
	{
		if(!mCurrentElementBlock)
			return false;

		mCurrentIndex++;

		if((u32)mCurrentIndex == mEndOfGroupElementIndex) // Next group
		{
			mCurrentElementBlock = mCurrentElementBlock->NextBlock;
			mCurrentElementBoundsBlock = mCurrentElementBoundsBlock->NextBlock;
			mEndOfGroupElementIndex = Options::MaximumElementsPerNode; // Following groups are always full
			mCurrentIndex = 0;

			if(!mCurrentElementBlock)
				return false;
		}

		return true;
	}

	template <class ElementType, class Options>
	TQuadtree<ElementType, Options>::AreaIntersectIterator::AreaIntersectIterator(const TQuadtree& tree, const Rect2& bounds)
		: mNodeIterator(tree), mBounds(simd::Rect2(bounds))
	{}

	template <class ElementType, class Options>
	bool TQuadtree<ElementType, Options>::AreaIntersectIterator::MoveNext()
	{
		while(true)
		{
			// First check elements of the current node (if any)
			while(mElementIterator.MoveNext())
			{
				const simd::Rect2& bounds = mElementIterator.GetCurrentBounds();
				if(bounds.Overlaps(mBounds))
					return true;
			}

			// No more elements in this node, move to the next one
			if(!mNodeIterator.MoveNext())
				return false; // No more nodes to check

			const NodeTraversalContext& nodeTraversalContext = mNodeIterator.GetCurrent();
			mElementIterator = ElementIterator(nodeTraversalContext.Node);

			// Add all intersecting child nodes to the iterator
			QuadtreeChildNodeIdRange childRange = nodeTraversalContext.Bounds.FindIntersectingChildren(mBounds);
			for(u32 i = 0; i < 4; i++)
			{
				if(childRange.Contains(i) && nodeTraversalContext.Node->HasChild(i))
					mNodeIterator.PushChild(i);
			}
		}

		return false;
	}

	template <class ElementType, class Options>
	TQuadtree<ElementType, Options>::TQuadtree(const Vector2& center, float extent, void* context)
		: mRootBounds(simd::Rect2(center, extent))
		, mMinimumNodeExtent(extent * std::pow(0.5f * (1.0f + 1.0f / (float)Options::LoosePadding), Options::MaximumDepth))
		, mContext(context)
	{ }

	template <class ElementType, class Options>
	void TQuadtree<ElementType, Options>::RemoveElement(const QuadtreeElementId& elementId)
	{
		Node* node = (Node*)elementId.Node;

		RemoveElementFromLeafNode(node, elementId.IndexInNode);

		// Reduce element counts in this and any parent nodes, check if nodes need collapsing
		Node* currentNode = node;
		Node* nodeToCollapse = nullptr;
		while(currentNode)
		{
			--currentNode->mElementCountWithChildren;

			if(currentNode->mElementCountWithChildren < Options::MinimumElementsPerNode)
				nodeToCollapse = currentNode;

			currentNode = currentNode->mParent;
		}

		if(nodeToCollapse)
		{
			// Add all the child node elements to the current node
			auto fnAddChildElementsRecursive = [this, node](Node* nodeToIterate, auto&& fnAddChilElementsRecursive) -> void
			{
				for(u32 i = 0; i < 4; i++)
				{
					if(nodeToIterate->HasChild(i))
					{
						Node* const childNode = nodeToIterate->GetChild(i);

						ElementIterator elementIterator(childNode);
						while(elementIterator.MoveNext())
							AddElementToLeafNode(node, elementIterator.GetCurrentElement(), elementIterator.GetCurrentBounds());

						fnAddChilElementsRecursive(childNode, fnAddChilElementsRecursive);
					}
				}
			};

			fnAddChildElementsRecursive(node, fnAddChildElementsRecursive);

			node->mIsLeaf = true;

			// Recursively delete all child nodes
			for(u32 i = 0; i < 4; i++)
			{
				if(node->mChildren[i])
				{
					FreeNode(node->mChildren[i]);

					mNodeAllocator.Destruct(node->mChildren[i]);
					node->mChildren[i] = nullptr;
				}
			}
		}
	}

	template <class ElementType, class Options>
	void TQuadtree<ElementType, Options>::AddElementToNode(const ElementType& element, Node* node, const NodeBounds& nodeBounds)
	{
		const simd::Rect2 elementBounds = Options::GetBounds(element, mContext);

		++node->mElementCountWithChildren;
		if(node->mIsLeaf)
		{
			const simd::Rect2& bounds = nodeBounds.GetBounds();

			// Check if the node has too many elements and should be broken up
			if((node->mElementData.ElementCount + 1) > Options::MaximumElementsPerNode && bounds.Extents.X > mMinimumNodeExtent)
			{
				NodeElementData elementData = node->mElementData;

				ElementIterator elementIterator(node);
				node->mElementData = NodeElementData();

				// Mark the node as non-leaf, allowing children to be created
				node->mIsLeaf = false;
				node->mElementCountWithChildren = 0;

				// Re-add all the elements to this node, but this time it will add them to child elements due to it not being a leaf flag
				while(elementIterator.MoveNext())
					AddElementToNode(elementIterator.GetCurrentElement(), node, nodeBounds);

				// Free the element and element bound blocks from this node
				FreeElementData(elementData);

				// Insert the current element
				AddElementToNode(element, node, nodeBounds);
			}
			else
			{
				// No need to subdivide, just add the element to this node
				AddElementToLeafNode(node, element, elementBounds);
			}
		}
		else
		{
			// Attempt to find a child the element fits into
			QuadtreeChildNodeId child = nodeBounds.FindContainingChild(elementBounds);

			if(child.Empty)
			{
				// Element doesn't fit into a child, add it to this node
				AddElementToLeafNode(node, element, elementBounds);
			}
			else
			{
				// Create the child node if needed, and add the element to it
				if(!node->mChildren[child.Index])
					node->mChildren[child.Index] = mNodeAllocator.template Construct<Node>(node);

				AddElementToNode(element, node->mChildren[child.Index], nodeBounds.GetChild(child));
			}
		}
	}

	template <class ElementType, class Options>
	void TQuadtree<ElementType, Options>::AddElementToLeafNode(Node* node, const ElementType& element, const simd::Rect2& elementBounds)
	{
		NodeElementData& elementData = node->mElementData;

		const u32 freeElementIndex = elementData.ElementCount % Options::MaximumElementsPerNode;
		if(freeElementIndex == 0) // New sub-allocation block needed
		{
			NodeElementSuballocationBlock* elementsBlock = (NodeElementSuballocationBlock*)mElementBlockAllocator.template Construct<NodeElementSuballocationBlock>();
			NodeElementBoundsSuballocationBlock* elementBoundsBlock = (NodeElementBoundsSuballocationBlock*)mElementBoundsBlockAllocator.template Construct<NodeElementBoundsSuballocationBlock>();

			elementsBlock->NextBlock = elementData.ElementsBlock;
			elementBoundsBlock->NextBlock = elementData.ElementBoundsBlock;

			elementData.ElementsBlock = elementsBlock;
			elementData.ElementBoundsBlock = elementBoundsBlock;
		}

		elementData.ElementsBlock->Elements[freeElementIndex] = element;
		elementData.ElementBoundsBlock->Bounds[freeElementIndex] = elementBounds;

		const u32 elementIndex = elementData.ElementCount;
		Options::SetElementId(element, QuadtreeElementId(node, elementIndex), mContext);

		++elementData.ElementCount;
	}

	template <class ElementType, class Options>
	void TQuadtree<ElementType, Options>::RemoveElementFromLeafNode(Node* node, u32 elementIndexInNode)
	{
		NodeElementData& elementData = node->mElementData;

		NodeElementSuballocationBlock* elementsBlock;
		NodeElementBoundsSuballocationBlock* elementBoundsBlock;
		elementIndexInNode = node->MapElementIndexToBlock(elementIndexInNode, &elementsBlock, &elementBoundsBlock);

		NodeElementSuballocationBlock* lastElementsBlock;
		NodeElementBoundsSuballocationBlock* lastElementBoundsBlock;
		u32 lastElementIdx = node->MapElementIndexToBlock(elementData.ElementCount - 1, &lastElementsBlock, &lastElementBoundsBlock);

		if(elementData.ElementCount > 1)
		{
			std::swap(elementsBlock->Elements[elementIndexInNode], lastElementsBlock->Elements[lastElementIdx]);
			std::swap(elementBoundsBlock->Bounds[elementIndexInNode], lastElementBoundsBlock->Bounds[lastElementIdx]);

			Options::SetElementId(elementsBlock->Elements[elementIndexInNode], QuadtreeElementId(node, elementIndexInNode), mContext);
		}

		if(lastElementIdx == 0) // Last element in that group, remove it completely
		{
			elementData.ElementsBlock = lastElementsBlock->NextBlock;
			elementData.ElementBoundsBlock = lastElementBoundsBlock->NextBlock;

			mElementBlockAllocator.Destruct(lastElementsBlock);
			mElementBoundsBlockAllocator.Destruct(lastElementBoundsBlock);
		}

		--elementData.ElementCount;
	}

	template <class ElementType, class Options>
	void TQuadtree<ElementType, Options>::FreeElementData(NodeElementData& elementData)
	{
		// Free the element and element bounds blocks from this node
		NodeElementSuballocationBlock* currentElementsBlock = elementData.ElementsBlock;
		while(currentElementsBlock)
		{
			NodeElementSuballocationBlock* const toDelete = currentElementsBlock;
			currentElementsBlock = currentElementsBlock->NextBlock;

			mElementBlockAllocator.Destruct(toDelete);
		}

		NodeElementBoundsSuballocationBlock* currentElementBoundsBlock = elementData.ElementBoundsBlock;
		while(currentElementBoundsBlock)
		{
			NodeElementBoundsSuballocationBlock* const toDelete = currentElementBoundsBlock;
			currentElementBoundsBlock = currentElementBoundsBlock->NextBlock;

			mElementBoundsBlockAllocator.Destruct(toDelete);
		}

		elementData.ElementsBlock = nullptr;
		elementData.ElementBoundsBlock = nullptr;
		elementData.ElementCount = 0;
	}

	template <class ElementType, class Options>
	void TQuadtree<ElementType, Options>::FreeNode(Node* node)
	{
		FreeElementData(node->mElementData);

		for(auto& entry : node->mChildren)
		{
			if(entry != nullptr)
			{
				FreeNode(entry);
				mNodeAllocator.Destruct(entry);
			}
		}
	}

	/** @} */
} // namespace bs
