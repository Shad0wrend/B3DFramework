//************************************ bs::framework - Copyright 2018-2019 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsUtilityPrerequisites.h"

namespace bs
{
	/** @addtogroup General
	 *  @{
	 */

	enum ThreadSafetyPolicy
	{
		ThreadSafe,
		ThreadUnsafe
	};

	template <typename Type, typename AllocatorTag = DefaultAllocatorTag>
	struct DefaultDeleter
	{
		constexpr DefaultDeleter() = default;

		template<class OtherType, std::enable_if_t<std::is_convertible_v<OtherType*, Type*>, int> = 0>
		constexpr DefaultDeleter(const Deleter<OtherType, AllocatorTag>& other) noexcept {}

		void operator()(Type* object) const
		{
			B3DDelete(object);
		}
	};

	template<ThreadSafetyPolicy ThreadSafety>
	struct TSharedControlBlock
	{
	protected:
		constexpr TSharedControlBlock() = default;

	public:
		// Non-copyable
		TSharedControlBlock(const TSharedControlBlock&) = delete;
		TSharedControlBlock& operator=(TSharedControlBlock&) = delete;

		virtual ~TSharedControlBlock() = default;

		/** Returns the number of currently held strong references. */
		u32 GetStrongReferenceCount() const
		{
			if constexpr(ThreadSafety == ThreadSafe)
				return StrongReferenceCount.load(std::memory_order_relaxed);
			else
				return StrongReferenceCount;
		}

		/** Increments the strong reference count. As long as strong reference count is non-zero the owned object will be kept alive. */
		void IncrementStrongReferenceCount()
		{
			if constexpr(ThreadSafety == ThreadSafe)
				StrongReferenceCount.fetch_add(1, std::memory_order_relaxed);
			else
				++StrongReferenceCount;
		}

		/**
		 * Decrements the strong reference count. If the strong reference count reaches zero the owned object will be destroyed. Additionally
		 * if there are no weak resource handles alive either, control block data will be destroyed.
		 */
		void DecrementStrongReferenceCount()
		{
			if constexpr(ThreadSafety == ThreadSafe)
			{
				if(StrongReferenceCount.fetch_sub(1, std::memory_order_acq_rel) == 1)
				{
					DestroyOwnedObject();
					DecrementWeakReferenceCount();
				}
			}
			else
			{
				if(--StrongReferenceCount == 0)
				{
					DestroyOwnedObject();
					DecrementWeakReferenceCount();
				}
			}
		}

		/** Increments the weak reference count. This keeps the control block data alive, but not the owned object itself. */
		void IncrementWeakReferenceCount()
		{
			if constexpr(ThreadSafety == ThreadSafe)
				WeakReferenceCount.fetch_add(1, std::memory_order_relaxed);
			else
				++WeakReferenceCount;
		}

		/**
		 * Decrements the weak reference count. If this was the last weak reference and there are no strong references either, control block data
		 * will be destroyed.
		 */
		void DecrementWeakReferenceCount()
		{
			if constexpr(ThreadSafety == ThreadSafe)
			{
				if(WeakReferenceCount.fetch_sub(1, std::memory_order_acq_rel) == 1)
					DestroySelf();
			}
			else
			{
				if(--WeakReferenceCount == 0)
					DestroySelf();
			}
		}

		/** Increments the strong reference count, but only if it is not already at zero. Returns true if incremented. */
		bool IncrementStrongReferenceCountIfNonZero()
		{
			if constexpr(ThreadSafety == ThreadSafe)
			{
				std::uint32_t referenceCount = StrongReferenceCount.load(std::memory_order_acquire);
				while(referenceCount != 0)
				{
					if(StrongReferenceCount.compare_exchange_weak(referenceCount, referenceCount + 1, std::memory_order_release, std::memory_order_relaxed))
						return true;
				}

				return false;
			}
			else
			{
				if(StrongReferenceCount == 0)
					return false;

				++StrongReferenceCount;
				return true;
			}
		}

		/**	Destroys the object the control block is pointing to. */
		virtual void DestroyOwnedObject() = 0;

		/** Destroys the control block. */
		virtual void DestroySelf() = 0;

		// TODO - Add support for shared memory between object and control block

	private:
		using CounterType = std::conditional_t<ThreadSafety == ThreadSafe, std::atomic<u32>, u32>;

		CounterType StrongReferenceCount{ 1 }; /**< References keeping the object alive (strong handles). */
		CounterType WeakReferenceCount{ 1 }; /**< References keeping the control block data alive (weak handles + 1 if any strong handle is alive). */
	};

	// Optimization for empty types (https://en.cppreference.com/w/cpp/language/ebo), i.e. if the FirstType is an empty type, only takes up size for SecondType
	template <class FirstType, class SecondType, bool = std::is_empty_v<FirstType> && !std::is_final_v<FirstType>>
	class CompressedPair final : private FirstType
	{
	public:
		template <class FirstArgumentType, class... SecondArgumentType>
		constexpr CompressedPair(FirstArgumentType&& FirstArgument, SecondArgumentType&&... SecondArgument)
			: FirstType(std::forward<FirstArgumentType>(FirstArgument)), Second(std::forward<SecondArgumentType>(SecondArgument)...)
		{ }

		constexpr FirstType& GetFirst()
		{
			return *this;
		}

		constexpr const SecondType& GetSecond() const
		{
			return Second;
		}

	private:
		SecondType Second;
	};

	template <class FirstType, class SecondType>
	class CompressedPair<FirstType, SecondType, false> final
	{
	public:
		template <class FirstArgumentType, class... SecondArgumentType>
		constexpr CompressedPair(FirstArgumentType&& FirstArgument, SecondArgumentType&&... SecondArgument)
			: First(std::forward<FirstArgumentType>(FirstArgument)), Second(std::forward<SecondArgumentType>(SecondArgument)...)
		{ }

		constexpr FirstType& GetFirst() { return First; }
		constexpr const FirstType& GetFirst() const { return First; }
		constexpr SecondType& GetSecond() { return Second; }
		constexpr const SecondType& GetSecond() const { return Second; }

	private:
		FirstType First;
		SecondType Second;
	};

	template<typename ObjectType, ThreadSafetyPolicy ThreadSafety>
	struct TSharedControlBlockWithDefaultDeleter : TSharedControlBlock<ThreadSafety>
	{
		TSharedControlBlockWithDefaultDeleter(ObjectType* object)
			:mObject(object)
		{ }

		void DestroyOwnedObject() override
		{
			B3DDelete(mObject);
		}

		void DestroySelf() override
		{
			B3DDelete(this);
		}

	private:
		ObjectType* mObject; // TODO - Object pointer is duplicated here and in TSharedCommon
	};

	template<typename ObjectType, typename DeleterType, ThreadSafetyPolicy ThreadSafety>
	struct TSharedControlBlockWithCustomDeleter : TSharedControlBlock<ThreadSafety>
	{
		TSharedControlBlockWithCustomDeleter(ObjectType* object, DeleterType deleter)
			:mDeleterAndObject(std::move(deleter), object)
		{ }

		/**	Destroys the object the control block is pointing to. */
		void DestroyOwnedObject() override
		{
			mDeleterAndObject.GetFirst()(mDeleterAndObject.GetSecond());
		}

		/** Destroys the control block. */
		void DestroySelf() override
		{
			B3DDelete(this);
		}

	private:
		CompressedPair<DeleterType, ObjectType*> mDeleterAndObject; // TODO - Object pointer is duplicated here and in TSharedCommon
	};

	template <typename Type, ThreadSafetyPolicy ThreadSafety = ThreadSafe>
	class TSharedCommon
	{
	public:
		using ElementType = Type;

		u32 GetReferenceCount() const
		{
			if(mControlBlock != nullptr)
				return mControlBlock->GetStrongReferenceCount();

			return 0;
		}

		Type* Get() const { return mOwnedObject; }

	protected:
		~TSharedCommon() = default;

		template<typename OtherType>
		void MoveConstructFrom(TSharedCommon<OtherType, ThreadSafety>&& other)
		{
			mOwnedObject = std::exchange(other.mOwnedObject, nullptr);
			mControlBlock = std::exchange(other.mControlBlock, nullptr);
		}

		template<typename OtherType>
		void CopyConstructFrom(TSharedCommon<OtherType, ThreadSafety>& other)
		{
			other.IncrementStrongReferenceCount();

			mOwnedObject = other.mOwnedObject;
			mControlBlock = other.mControlBlock;
		}

		template<typename OtherType>
		void AliasMoveConstructFrom(TSharedCommon<OtherType, ThreadSafety>&& other, Type* object)
		{
			mOwnedObject = object;
			other.mOwnedObject = nullptr;

			mControlBlock = std::exchange(other.mControlBlock, nullptr);
		}

		template<typename OtherType>
		void AliasCopyConstructFrom(TSharedCommon<OtherType, ThreadSafety>&& other, Type* object)
		{
			other.IncrementStrongReferenceCount();

			mOwnedObject = object;
			mControlBlock = other.mControlBlock;
		}

		void IncrementStrongReferenceCount()
		{
			if(mControlBlock != nullptr)
				mControlBlock->IncrementStrongReferenceCount();
		}

		void DecrementStrongReferenceCount()
		{
			if(mControlBlock != nullptr)
				mControlBlock->DecrementStrongReferenceCount();
		}

		void Swap(TSharedCommon& other)
		{
			std::swap(mOwnedObject, other.mOwnedObject);
			std::swap(mControlBlock, other.mControlBlock);
		}


		Type* mOwnedObject = nullptr;
		TSharedControlBlock<ThreadSafety>* mControlBlock = nullptr;
	};


	template <typename Type, ThreadSafetyPolicy ThreadSafety = ThreadSafe>
	class TShared : public TSharedCommon<Type, ThreadSafety>
	{
	public:
		constexpr TShared() = default;
		constexpr TShared(nullptr_t) {}

		template<typename OtherType, std::enable_if_t<std::is_convertible_v<OtherType, Type>, int> = 0>
		explicit TShared(Type* pointer)
		{
			mOwnedObject = pointer;
			mControlBlock = B3DNew<TSharedControlBlockWithDefaultDeleter<Type, ThreadSafety>>(pointer);
			// TODO - Handle EnableSharedFromThis
		}

		template<typename OtherType>
		TShared(const TShared<OtherType>& other, Type* object)
		{
			AliasCopyConstructFrom(other, object);
		}

		template<typename OtherType>
		TShared(const TShared<OtherType>&& other, Type* object)
		{
			AliasMoveConstructFrom(std::move(other), object);
		}

		TShared(const TShared& other)
		{
			CopyConstructFrom(other);
		}

		template<typename OtherType, std::enable_if_t<std::is_convertible_v<OtherType, Type>, int> = 0>
		TShared(const TShared<OtherType>& other)
		{
			CopyConstructFrom(other);
		}

		TShared(TShared&& other)
		{
			MoveConstructFrom(std::move(other));
		}

		template<typename OtherType, std::enable_if_t<std::is_convertible_v<OtherType, Type>, int> = 0>
		TShared(TShared<OtherType>&& other)
		{
			MoveConstructFrom(std::move(other));
		}

		// TODO - Add ctors with deleter

		// TODO - Conversion from unique ptr to shared pointer
		// TODO - Conversion from weak ptr to shared_ptr

		~TShared()
		{
			DecrementStrongReferenceCount();
		}

		TShared& operator=(const TShared& rhs)
		{
			TShared(rhs).Swap(*this);
			return *this;
		}

		template<typename OtherType, std::enable_if_t<std::is_convertible_v<OtherType, Type>, int> = 0>
		TShared& operator=(const TShared<OtherType>& rhs)
		{
			TShared(rhs).Swap(*this);
			return *this;
		}

		TShared& operator=(TShared&& rhs)
		{
			TShared(std::move(rhs)).Swap(*this);
			return *this;
		}

		template<typename OtherType, std::enable_if_t<std::is_convertible_v<OtherType, Type>, int> = 0>
		TShared& operator=(TShared<OtherType>&& rhs)
		{
			TShared(std::move(rhs)).Swap(*this);
			return *this;
		}

		// TODO - Add assignment from unique pointer

		void Swap(TShared& other)
		{
			TSharedCommon<Type, ThreadSafety>::Swap(other);
		}

		void Reset()
		{
			TShared().Swap(*this);
		}

		template<typename OtherType, std::enable_if_t<std::is_convertible_v<OtherType, Type>, int> = 0>
		void Reset(OtherType* other)
		{
			TShared(other).Swap(*this);
		}

		// TODO - Add Reset with deleter

		template <class OtherType = Type>
		OtherType& operator*() const
		{
			return *mOwnedObject;
		}

		template <class OtherType = Type>
		OtherType* operator->() const
		{
			return mOwnedObject;
		}

		explicit operator bool() const 
		{
			return mOwnedObject != nullptr;
		}
	};

	template <class T, class U>
	bool operator==(const TShared<T>& lhs, const TShared<U>& rhs)
	{
		return lhs.Get() == rhs.Get();
	}

	template <class T, class U>
	bool operator!=(const TShared<T>& lhs, const TShared<U>& rhs)
	{
		return lhs.Get() != rhs.Get();
	}

	template <class T, class U>
	bool operator<=(const TShared<T>& lhs, const TShared<U>& rhs)
	{
		return lhs.Get() <= rhs.Get();
	}

	template <class T, class U>
	bool operator<(const TShared<T>& lhs, const TShared<U>& rhs)
	{
		return lhs.Get() < rhs.Get();
	}

	template <class T, class U>
	bool operator>=(const TShared<T>& lhs, const TShared<U>& rhs)
	{
		return lhs.Get() >= rhs.Get();
	}

	template <class T, class U>
	bool operator>(const TShared<T>& lhs, const TShared<U>& rhs)
	{
		return lhs.Get() > rhs.Get();
	}

	template <class T>
	bool operator==(nullptr_t, const TShared<T>& rhs)
	{
		return nullptr == rhs.Get();
	}

	template <class T>
	bool operator==(const TShared<T>& lhs, nullptr_t)
	{
		return lhs.Get() == nullptr;
	}

	template <class T>
	bool operator!=(nullptr_t, const TShared<T>& rhs)
	{
		return nullptr != rhs.Get();
	}

	template <class T>
	bool operator!=(const TShared<T>& lhs, nullptr_t)
	{
		return lhs.Get() != nullptr;
	}

	/** Cast a shared pointer from one type to another. */
	template <class T, class U>
	TShared<T> B3DStaticPointerCast(const TShared<U>& other)
	{
		const auto object = static_cast<typename TShared<T>::ElementType*>(other.Get());
		return TShared<T>(other, object);
	}

	/** Cast a shared pointer from one type to another. */
	template <class T, class U>
	TShared<T> B3DStaticPointerCast(TShared<U>&& other)
	{
		const auto object = static_cast<typename TShared<T>::ElementType*>(other.Get());
		return TShared<T>(std::move(other), object);
	}

	// TODO - Add Make global functions

	/** @} */
} // namespace bs
