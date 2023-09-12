//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

namespace bs
{
	/** @addtogroup General
	 *  @{
	 */

	class DefaultAllocator
	{
	public:
		template<class ElementType>
		class ForElementType
		{
		public:

			~ForElementType()
			{
				if(mElements)
					B3DFree(mElements);
			}

			ElementType* GetElements() const { return mElements; }
			bool HasDynamicAllocations() const { return mElements != nullptr; }

			void Move(u64 mySize, u64 otherSize, ForElementType&& other)
			{
				B3D_ASSERT(this != &other);

				if(mElements)
				{
					for(u64 index = 0; index < mySize; ++index)
						mElements[index].~ElementType();

					B3DFree(mElements);
				}

				mElements = std::exchange(other.mElements, nullptr);
			}

			void Resize(u64 elementCount, u64 newCapacity)
			{
				ElementType* buffer = newCapacity > 0 ? B3DAllocateMultiple<ElementType>(newCapacity) : nullptr;

				if(buffer)
				{
					std::uninitialized_move(
						mElements,
						mElements + std::min(elementCount, newCapacity),
						buffer);
				}

				// Destoy existing elements in old memory
				if(mElements)
				{
					for(u64 index = 0; index < elementCount; ++index)
						mElements[index].~ElementType();

					B3DFree(mElements);
				}

				mElements = buffer;
			}

			ElementType* mElements = nullptr;
		};
	};

	template<u32 StackElementCount, class SecondaryAllocator = DefaultAllocator>
	class TInlineAllocator
	{
	public:
		template<class ElementType>
		class ForElementType
		{
		public:
			ElementType* GetElements() const { return mElements; }
			bool HasDynamicAllocations() const { return mElements != (ElementType*)mStackStorage; }

			void Move(u64 mySize, u64 otherSize, ForElementType&& other)
			{
				B3D_ASSERT(this != &other);

				if(!other.HasDynamicAllocations())
				{
					// Use assignment copy if we have more elements than the other array, and destroy any excess elements
					if(mySize > otherSize)
					{
						ElementType* newEnd = otherSize > 0 ? std::move(other.GetElements(), other.GetElements() + otherSize, GetElements()) : GetElements();

						for(; newEnd != GetElements() + mySize; ++newEnd)
							(*newEnd).~ElementType();
					}
					else
					{
						if(mySize > 0)
							std::move(other.GetElements(), other.GetElements() + mySize, GetElements());

						std::uninitialized_move(
							other.GetElements() + mySize,
							other.GetElements() + otherSize,
							GetElements() + mySize);
					}

					mElements = (ElementType*)mStackStorage;
				}
				else
				{
					mSecondaryAllocator.Move(mySize, otherSize, std::move(other.mSecondaryAllocator));
					mElements = std::exchange(other.mElements, nullptr);
				}
			}

			void Resize(u64 currentSize, u64 newCapacity)
			{
				// New capacity fits on stack
				if(newCapacity <= StackElementCount)
				{
					// If current allocations are dynamic, move them to stack and free dynamic allocation
					if(HasDynamicAllocations())
					{
						std::uninitialized_move(
							mSecondaryAllocator.GetElements(),
							mSecondaryAllocator.GetElements() + currentSize,
							(ElementType*)mStackStorage);

						mSecondaryAllocator.Resize(currentSize, 0);
						mElements = (ElementType*)mStackStorage;
					}
				}
				// New capacity requires a dynamic allocation
				else
				{
					// Already have a dynamic allocation, just resize
					if(HasDynamicAllocations())
					{
						mSecondaryAllocator.Resize(currentSize, newCapacity);
						mElements = mSecondaryAllocator.GetElements();
					}
					// Allocate dynamic and move from stack
					else
					{
						mSecondaryAllocator.Resize(0, newCapacity);

						std::uninitialized_move(
							(ElementType*)mStackStorage,
							((ElementType*)mStackStorage) + currentSize,
							mSecondaryAllocator.GetElements());

						mElements = mSecondaryAllocator.GetElements();
					}
				}
			}

			std::aligned_storage_t<sizeof(ElementType), alignof(ElementType)> mStackStorage[StackElementCount];
			ElementType* mElements = (ElementType*)mStackStorage;
			typename SecondaryAllocator::template ForElementType<ElementType> mSecondaryAllocator;
		};
	};


	/**
	 * Dynamically sized container that statically allocates enough room for @p N elements of type @p Type. If the element
	 * count exceeds the statically allocated buffer size the vector falls back to general purpose dynamic allocator.
	 */
	template <class Type, u64 N>
	class SmallVector final
	{
	public:
		typedef Type ValueType;
		typedef Type* Iterator;
		typedef const Type* ConstIterator;
		typedef std::reverse_iterator<Type*> ReverseIterator;
		typedef std::reverse_iterator<const Type*> ConstReverseIterator;

		// For std compatibility
		typedef Type value_type;
		typedef Type* iterator;
		typedef const Type* const_iterator;
		typedef std::reverse_iterator<Type*> reverse_iterator;
		typedef std::reverse_iterator<const Type*> const_reverse_iterator;

		SmallVector() = default;

		SmallVector(const SmallVector<ValueType, N>& other)
		{
			if(!other.Empty())
				*this = other;
		}

		SmallVector(SmallVector<ValueType, N>&& other)
		{
			if(!other.Empty())
				*this = std::move(other);
		}

		SmallVector(u64 size, const Type& value = Type())
		{
			Append(size, value);
		}

		SmallVector(std::initializer_list<Type> list)
		{
			Append(list);
		}

		template <
			typename IteratorType,
			typename = std::enable_if_t<std::is_convertible_v<typename std::iterator_traits<IteratorType>::iterator_category, std::input_iterator_tag>>>
		SmallVector(IteratorType start, IteratorType end)
		{
			this->Append(start, end);
		}

		~SmallVector()
		{
			for(auto& entry : *this)
				entry.~Type();
		}

		SmallVector<ValueType, N>& operator=(const SmallVector<ValueType, N>& other)
		{
			if(this == &other)
				return *this;

			u64 mySize = Size();
			const u64 otherSize = other.Size();

			// Use assignment copy if we have more elements than the other array, and destroy any excess elements
			if(mySize > otherSize)
			{
				Iterator newEnd;
				if(otherSize > 0)
					newEnd = std::copy(other.Begin(), other.End(), Begin());
				else
					newEnd = Begin();

				for(; newEnd != End(); ++newEnd)
					(*newEnd).~Type();
			}
			// Otherwise we need to partially copy (up to our size), and do uninitialized copy for rest. And an optional
			// grow if our capacity isn't enough (in which case we do uninitialized copy for all).
			else
			{
				if(otherSize > mCapacity)
				{
					Clear();
					mySize = 0;

					Grow(otherSize);
				}
				else if(mySize > 0)
					std::copy(other.Begin(), other.Begin() + mySize, Begin());

				std::uninitialized_copy(other.Begin() + mySize, other.End(), Begin() + mySize);
			}

			mSize = otherSize;
			return *this;
		}

		SmallVector<ValueType, N>& operator=(SmallVector<ValueType, N>&& other)
		{
			if(this == &other)
				return *this;

			u64 mySize = Size();
			const u64 otherSize = other.Size();

			mAllocator.Move(mySize, otherSize, std::move(other.mAllocator));
			mCapacity = std::exchange(other.mCapacity, N);
			mSize = std::exchange(other.mSize, 0);

			return *this;
		}

		SmallVector<ValueType, N>& operator=(std::initializer_list<Type> list)
		{
			u64 mySize = Size();
			const u64 otherSize = (u64)list.size();

			// Use assignment copy if we have more elements than the list, and destroy any excess elements
			if(mySize > otherSize)
			{
				Iterator newEnd;
				if(otherSize > 0)
					newEnd = std::copy(list.begin(), list.end(), Begin());
				else
					newEnd = Begin();

				for(; newEnd != End(); ++newEnd)
					(*newEnd).~Type();
			}
			// Otherwise we need to partially copy (up to our size), and do uninitialized copy for rest. And an optional
			// grow if our capacity isn't enough (in which case we do uninitialized copy for all).
			else
			{
				if(otherSize > mCapacity)
				{
					Clear();
					mySize = 0;

					Grow(otherSize);
				}
				else if(mySize > 0)
					std::copy(list.begin(), list.begin() + mySize, Begin());

				std::uninitialized_copy(list.begin() + mySize, list.end(), Begin() + mySize);
			}

			mSize = otherSize;
			return *this;
		}

		bool operator==(const SmallVector<ValueType, N>& other)
		{
			if(this->Size() != other.Size()) return false;
			return std::equal(this->Begin(), this->End(), other.Begin());
		}

		bool operator!=(const SmallVector<ValueType, N>& other)
		{
			return !(*this == other);
		}

		bool operator<(const SmallVector<ValueType, N>& other) const
		{
			return std::lexicographical_compare(Begin(), End(), other.Begin(), other.End());
		}

		bool operator>(const SmallVector<ValueType, N>& other) const
		{
			return other < *this;
		}

		bool operator<=(const SmallVector<ValueType, N>& other) const
		{
			return !(other < *this);
		}

		bool operator>=(const SmallVector<ValueType, N>& other) const
		{
			return !(*this < other);
		}

		Type& operator[](u64 index)
		{
			B3D_ASSERT(index < mSize && "Array index out-of-range.");

			return mAllocator.GetElements()[index];
		}

		const Type& operator[](u64 index) const
		{
			B3D_ASSERT(index < mSize && "Array index out-of-range.");

			return mAllocator.GetElements()[index];
		}

		bool Empty() const { return mSize == 0; }

		Iterator Begin() { return mAllocator.GetElements(); }
		Iterator End() { return mAllocator.GetElements() + mSize; }

		ConstIterator Begin() const { return mAllocator.GetElements(); }
		ConstIterator End() const { return mAllocator.GetElements() + mSize; }

		ConstIterator Cbegin() const { return mAllocator.GetElements(); }
		ConstIterator Cend() const { return mAllocator.GetElements() + mSize; }

		ReverseIterator Rbegin() { return ReverseIterator(End()); }
		ReverseIterator Rend() { return ReverseIterator(Begin()); }

		ConstReverseIterator Rbegin() const { return ConstReverseIterator(End()); }
		ConstReverseIterator Rend() const { return ConstReverseIterator(Begin()); }

		ConstReverseIterator Crbegin() const { return ConstReverseIterator(End()); }
		ConstReverseIterator Crend() const { return ConstReverseIterator(Begin()); }

		u64 Size() const { return mSize; }
		u64 Capacity() const { return mCapacity; }

		Type* Data() { return mAllocator.GetElements(); }
		const Type* Data() const { return mAllocator.GetElements(); }

		Type& Front()
		{
			B3D_ASSERT(!Empty());
			return mAllocator.GetElements()[0];
		}

		Type& Back()
		{
			B3D_ASSERT(!Empty());
			return mAllocator.GetElements()[mSize - 1];
		}

		const Type& Front() const
		{
			B3D_ASSERT(!Empty());
			return mAllocator.GetElements()[0];
		}

		const Type& Back() const
		{
			B3D_ASSERT(!Empty());
			return mAllocator.GetElements()[mSize - 1];
		}

		void Add(const Type& element)
		{
			if(mSize == mCapacity)
				Grow(mCapacity << 1);

			new(&mAllocator.GetElements()[mSize++]) Type(element);
		}

		void Add(Type&& element)
		{
			if(mSize == mCapacity)
				Grow(mCapacity << 1);

			new(&mAllocator.GetElements()[mSize++]) Type(std::move(element));
		}

		template <
			typename IteratorType,
			typename = std::enable_if_t<std::is_convertible_v<typename std::iterator_traits<IteratorType>::iterator_category, std::input_iterator_tag>>>
		void Append(IteratorType start, IteratorType end)
		{
			const u64 count = (u64)std::distance(start, end);

			if((Size() + count) > Capacity())
				this->Grow(Size() + count);

			std::uninitialized_copy(start, end, this->End());
			mSize += count;
		}

		void Append(u64 count, const Type& element)
		{
			if((Size() + count) > Capacity())
				this->Grow(Size() + count);

			std::uninitialized_fill_n(End(), count, element);
			mSize += count;
		}

		void Append(std::initializer_list<Type> list)
		{
			Append(list.begin(), list.end());
		}

		void Pop()
		{
			B3D_ASSERT(mSize > 0 && "Popping an empty array.");
			mSize--;
			mAllocator.GetElements()[mSize].~Type();
		}

		Iterator Erase(ConstIterator iter)
		{
			B3D_ASSERT(iter >= Begin() && "Iterator to erase is out of bounds.");
			B3D_ASSERT(iter < End() && "Erasing at past-the-end iterator.");

			Iterator toErase = const_cast<Iterator>(iter);
			std::move(toErase + 1, End(), toErase);
			Pop();

			return toErase;
		}

		Iterator Erase(ConstIterator start, ConstIterator end)
		{
			B3D_ASSERT(start >= Begin() && "Iterator to erase is out of bounds.");
			B3D_ASSERT(start <= end && "Trying to erase invalid range.");
			B3D_ASSERT(end <= End() && "Erasing at past-the-end iterator.");

			Iterator mutableStart = const_cast<Iterator>(start);
			Iterator mutableEnd = const_cast<Iterator>(end);

			Iterator current = mutableStart;
			Iterator iter = std::move(mutableEnd, End(), mutableStart);

			while(mutableStart != mutableEnd)
			{
				--mutableEnd;
				mutableEnd->~Type();
			}

			mSize = iter - Begin();
			return current;
		}

		void Remove(u64 index)
		{
			Erase(Begin() + index);
		}

		bool Contains(const Type& element)
		{
			for(u64 i = 0; i < mSize; i++)
			{
				if(mAllocator.GetElements()[i] == element)
					return true;
			}

			return false;
		}

		void RemoveValue(const Type& element)
		{
			for(u64 i = 0; i < mSize; i++)
			{
				if(mAllocator.GetElements()[i] == element)
				{
					Remove(i);
					break;
				}
			}
		}

		void Clear()
		{
			for(u64 i = 0; i < mSize; ++i)
				mAllocator.GetElements()[i].~Type();

			mSize = 0;
		}

		void Reserve(u64 capacity)
		{
			if(capacity > mCapacity)
				Grow(capacity);
		}

		void Resize(u64 size, const Type& value = Type())
		{
			if(size > mCapacity)
				Grow(size);

			if(size > mSize)
			{
				for(u64 i = mSize; i < size; i++)
					new(&mAllocator.GetElements()[i]) Type(value);
			}
			else
			{
				for(u64 i = size; i < mSize; i++)
					mAllocator.GetElements()[i].~Type();
			}

			mSize = size;
		}

		// STD compatible API
		Iterator begin() { return Begin(); } // NOLINT
		Iterator end() { return End(); } // NOLINT

		ConstIterator begin() const { return Begin(); } // NOLINT
		ConstIterator end() const { return End(); } // NOLINT

		ConstIterator cbegin() const { return Cbegin(); } // NOLINT
		ConstIterator cend() const { return Cend(); } // NOLINT

		ReverseIterator rbegin() { return Rbegin(); } // NOLINT
		ReverseIterator rend() { return Rend(); } // NOLINT

		ConstReverseIterator rbegin() const { return Rbegin(); } // NOLINT
		ConstReverseIterator rend() const { return Rend(); } // NOLINT

		ConstReverseIterator crbegin() const { return Crbegin(); } // NOLINT
		ConstReverseIterator crend() const { return Crend(); } // NOLINT

		u64 size() const { return Size(); } // NOLINT
		u64 capacity() const { return Capacity(); } // NOLINT

		Type* data() { return Data(); } // NOLINT
		const Type* data() const { return Data(); } // NOLINT

		Type& front() { return Front(); } // NOLINT
		const Type& front() const { return Front(); } // NOLINT

		Type& back() { return Back(); } // NOLINT
		const Type& back() const { return Back(); } // NOLINT

		Iterator erase(ConstIterator iter) { return Erase(iter); } // NOLINT
		Iterator erase(ConstIterator start, ConstIterator end) { return Erase(start, end); } // NOLINT

		void clear() { Clear(); } // NOLINT
		void reserve(u64 capacity) { Reserve(capacity); } // NOLINT
		void resize(u64 size, const Type& value = Type()) { Resize(size, value); } // NOLINT

	private:
		void Grow(u64 capacity)
		{
			mAllocator.Resize(mSize, capacity);
			mCapacity = std::max(capacity, N);
		}

		typename TInlineAllocator<N>::template ForElementType<Type> mAllocator;
		u64 mSize = 0;
		u64 mCapacity = N;
	};

	/** @} */
} // namespace bs
