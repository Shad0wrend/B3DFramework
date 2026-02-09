//************************************ B3D Framework - Copyright 2026 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DUtilityPrerequisites.h"
#include "ECS/B3DRegistry.h"
#include "ECS/B3DEntity.h"
#include "Reflection/B3DRTTIIterator.h"

namespace b3d
{
	/** @addtogroup Internal-Utility
	 *  @{
	 */

	/** @addtogroup RTTI-Internal
	 *  @{
	 */

	template<typename ComponentType>
	class TRTTIECSIterator;

	/** Deleter for TRTTIECSIterator allocated via FrameAllocator. */
	template<typename ComponentType>
	struct TRTTIECSIteratorDeleter
	{
		TRTTIECSIteratorDeleter(FrameAllocator* allocator = nullptr)
			: mAllocator(allocator)
		{ }

		void operator()(TRTTIECSIterator<ComponentType>* iterator)
		{
			if(B3D_ENSURE(mAllocator != nullptr))
				mAllocator->Destruct(iterator);
		}

	private:
		FrameAllocator* mAllocator;
	};

	/**
	 * RTTI iterator for a single ECS component. Behaves as a faux single-element iterator (always 1 element),
	 * matching the behavior of TRTTIIteratorAdapter<T, false>. Reads directly from ECS storage and writes
	 * via AddOrReplaceComponent.
	 */
	template<typename ComponentType>
	class TRTTIECSIterator : public IRTTIIterator
	{
	public:
		TRTTIECSIterator(ecs::Registry* registry, ecs::Entity entity)
			: mRegistry(registry), mEntity(entity)
		{ }

		bool IsValid() const override { return mIsValid; }
		u64 GetElementCount() const override { return 1; }
		void SeekToBeginning() override { mIsValid = true; }
		void SeekToEnd() override { mIsValid = false; }
		void Increment() override { mIsValid = false; }
		bool SeekToIndex(u64 index) override { mIsValid = (index == 0); return mIsValid; }
		bool SeekToKey(const void*) override { return false; }

		const void* GetValue() const override
		{
			if(mRegistry != nullptr && mEntity != ecs::kNullEntity && mRegistry->HasAllOf<ComponentType>(mEntity))
				return &mRegistry->GetComponents<ComponentType>(mEntity);

			B3D_ASSERT(false);
			return nullptr;
		}

		void Erase() override { }
		void Clear() override { }

		SPtr<IRTTIIterator> Clone(FrameAllocator& allocator) const override
		{
			auto* it = allocator.Construct<TRTTIECSIterator>(mRegistry, mEntity);
			it->mIsValid = mIsValid;
			return B3DMakeSharedFromExisting<TRTTIECSIterator>(it, TRTTIECSIteratorDeleter<ComponentType>(&allocator));
		}

		/** Writes a component value to the ECS registry via AddOrReplaceComponent. */
		void WriteComponent(const ComponentType& value)
		{
			if(mRegistry != nullptr && mEntity != ecs::kNullEntity)
			{
				ComponentType copy = value;
				mRegistry->AddOrReplaceComponent<ComponentType>(mEntity, std::move(copy));
			}
		}

	private:
		ecs::Registry* mRegistry = nullptr;
		ecs::Entity mEntity = ecs::kNullEntity;
		bool mIsValid = true;
	};

	/** @} */
	/** @} */
} // namespace b3d
