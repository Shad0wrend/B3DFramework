//************************************ bs::framework - Copyright 2024 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Debug/BsDebug.h"
#include "Prerequisites/BsPrerequisitesUtil.h"

namespace bs
{
	/** @addtogroup Internal-Utility
	 *  @{
	 */

	/** @addtogroup RTTI-Internal
	 *  @{
	 */

	/** Interface for a RTTI iterator. */
	class IRTTIIterator
	{
	public:
		virtual ~IRTTIIterator() = default;

		/** Returns true if the iterator points to a valid value. */
		virtual bool IsValid() const = 0;

		/** Resets the iterator to the beginning of the container. */
		virtual void ResetToBeginning() = 0;

		/** Resets the iterator to the end of the container. */
		virtual void ResetToEnd() = 0;

		/** Returns the number of elements in the container. */
		virtual u64 GetElementCount() const = 0;

		/** Assigns the value at the current iterator location. */
		virtual void SetValue(const void* value) = 0;

		/** Returns the current value of the iterator. */
		virtual const void* GetValue() const = 0;

		/** Increment operator. */
		virtual void Increment() = 0;
	};

	/**
	 * Provides an adapter that allows TRTTIIterator<T> to iterate over some type T that might not provide the default iterator interface.
	 * The default implementation is the implementation for a faux iterator that only performs a single (non-container) field access.
	 * This allows RTTIIterator to be used for containers and single values alike.
	 */
	template<class T, typename = void>
	struct TRTTIIteratorAdapter
	{
		using IteratorType = T*;
		using ElementType = T;

		static IteratorType Begin(T& container) { return &container; }
		static IteratorType End(T& container) { return nullptr; }
		static bool IsValid(T& container, IteratorType iterator) { return iterator != nullptr; }
		static IteratorType Insert(T& container, IteratorType location, const ElementType& value) { *location = value; return location; }
		static IteratorType Insert(T& container, IteratorType location, ElementType&& value) { *location = std::move(value); return location; }
		static IteratorType Increment(IteratorType iterator) { iterator = nullptr; return iterator; }
		static ElementType& GetValue(IteratorType iterator) { return *iterator; }
		static u64 Size(T& container) { return 1; }
		
	};

	template<class T>
	struct TRTTIIteratorAdapter<T, std::enable_if_t<B3DHasIterator<T>::value>>
	{
		using IteratorType = typename T::iterator;
		using ElementType = typename T::value_type;

		static IteratorType Begin(T& container) { return container.begin(); }
		static IteratorType End(T& container) { return container.end(); }
		static bool IsValid(T& container, IteratorType iterator) { return iterator != container.end(); }
		static IteratorType Insert(T& container, IteratorType location, const ElementType& value) { return container.insert(location, value); }
		static IteratorType Insert(T& container, IteratorType location, ElementType&& value) { return container.insert(location, std::move(value)); }
		static IteratorType Increment(IteratorType iterator) { ++iterator; return iterator; }
		static ElementType& GetValue(IteratorType iterator) { return *iterator; }
		static u64 Size(T& container) { return (u64)container.size(); }
	};

	/** Wraps a container that can be used for sequentially reading container contents, inserting new elements in the container, and retrieving container element count. */
	template <class ContainerType>
	class TRTTIIterator : public IRTTIIterator
	{
	public:
		using IteratorType = typename TRTTIIteratorAdapter<ContainerType>::IteratorType;
		using ElementType = typename TRTTIIteratorAdapter<ContainerType>::ElementType;

		TRTTIIterator(ContainerType& container)
			: mContainer(&container), mIterator(TRTTIIteratorAdapter<ContainerType>::Begin(container))
		{}

		bool IsValid() const override { return TRTTIIteratorAdapter<ContainerType>::IsValid(*mContainer, mIterator); }
		void ResetToBeginning() override { mIterator = TRTTIIteratorAdapter<ContainerType>::Begin(*mContainer); }
		void ResetToEnd() override { mIterator = TRTTIIteratorAdapter<ContainerType>::End(*mContainer); }
		u64 GetElementCount() const override { return TRTTIIteratorAdapter<ContainerType>::Size(*mContainer); }
		void SetValue(const void* value) override { operator=(*static_cast<const ElementType*>(value)); }
		const void* GetValue() const override { return &(*mIterator); }
		void Increment() override { operator++(); }

		/** Assigns (copies) the value at the current iterator location. */
		TRTTIIterator& operator=(const ElementType& value)
		{
			mIterator = TRTTIIteratorAdapter<ContainerType>::Insert(*mContainer, mIterator, value);
			mIterator = TRTTIIteratorAdapter<ContainerType>::Increment(mIterator);

			return *this;
		}

		/** Assigns (moves) the value at the current iterator location. */
		TRTTIIterator& operator=(ElementType&& value)
		{
			mIterator = TRTTIIteratorAdapter<ContainerType>::Insert(*mContainer, mIterator, std::move(value));
			mIterator = TRTTIIteratorAdapter<ContainerType>::Increment(mIterator);

			return *this;
		}

		/** Returns the current value of the iterator. */
		ElementType& operator*()
		{
			return TRTTIIteratorAdapter<ContainerType>::GetValue(mIterator);
		}

		/** Pre-increment operator. */
		TRTTIIterator& operator++()
		{
			mIterator = TRTTIIteratorAdapter<ContainerType>::Increment(mIterator);
			return *this;
		}

	protected:
		ContainerType* mContainer = nullptr;
		IteratorType mIterator;
	};

	/** Deleter that can be passed to unique pointer referencing TRTTIIterator<ContainerType>. */
	template<typename ContainerType>
	struct TRTTIIteratorDeleter
	{
		TRTTIIteratorDeleter(FrameAllocator* allocator = nullptr)
			: mAllocator(allocator)
		{ }

		void operator()(TRTTIIterator<ContainerType>* iterator)
		{
			if(B3D_ENSURE(mAllocator != nullptr))
				mAllocator->Destruct(iterator);
		}

	private:
		FrameAllocator* mAllocator;
	};

	/** @} */
	/** @} */
} // namespace bs
