//************************************ B3D Framework - Copyright 2024 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsUtilityPrerequisites.h"
#include "Debug/BsDebug.h"
#include "ECS/BsEntity.h"
#include "ECS/BsUtility.h"
#include "ECS/BsSparseSet.h"
#include "BsUtility.h"

#include <iterator>

namespace b3d::ecs
{
	/** @addtogroup General
	 *  @{
	 */

	// Note: Based on EnTT (https://github.com/skypjack/entt)

	template<typename IteratorType, typename OwnedStorageType, typename IncludedStorageType>
	struct TGroupIteratorAdapter {};

	template<typename IteratorType, typename... OwnedStorageTypes, typename... IncludedStorageTypes>
	struct TGroupIteratorAdapter<IteratorType, TOwnedTypes<OwnedStorageTypes...>, TIncludedTypes<IncludedStorageTypes...>>
	{
		using iterator_type = IteratorType;
		using value_type = decltype(std::tuple_cat(std::make_tuple(*std::declval<IteratorType>()), GetAsTuple<OwnedStorageTypes>(nullptr, {})..., GetAsTuple<IncludedStorageTypes>(nullptr, {})...));
		using pointer = TPointerToTemporary<value_type>;
		using reference = value_type;
		using difference_type = typename std::iterator_traits<IteratorType>::difference_type;
		using iterator_category = std::input_iterator_tag;

		constexpr TGroupIteratorAdapter() = default;
		constexpr TGroupIteratorAdapter(IteratorType iterator, std::tuple<OwnedStorageTypes*..., IncludedStorageTypes*> ownedAndIncludedTypeStorage)
			: mIterator(iterator), mOwnedAndIncludedTypeStorage(std::move(ownedAndIncludedTypeStorage))
		{ }

		constexpr TGroupIteratorAdapter& operator++()
		{
			++mIterator;
			return *this;
		}

		constexpr pointer operator->() const
		{
			return operator*();
		}

		constexpr reference operator*() const
		{
			return std::tuple_cat(
				std::make_tuple(*mIterator),
				GetOwnedStorageElementTuple<OwnedStorageTypes>(*std::get<OwnedStorageTypes*>(mOwnedAndIncludedTypeStorage))...,
				GetAsTuple(std::get<IncludedStorageTypes*>(mOwnedAndIncludedTypeStorage), *mIterator)...);
		}

		constexpr iterator_type GetUnderlyingIterator() const
		{
			return mIterator;
		}

		template <typename... LeftIteratorTypes, typename... RightIteratorTypes>
		friend constexpr bool operator==(const TGroupIteratorAdapter<LeftIteratorTypes...>&, const TGroupIteratorAdapter<RightIteratorTypes...>&);

	private:
		template<typename OwnedStorageType>
		auto GetOwnedStorageElementTuple(OwnedStorageType& storage)
		{
			if constexpr(std::is_void_v<typename OwnedStorageType::ValueType>)
				return std::make_tuple();
			else
				return GetAsTuple(storage[mIterator.Index()]);
		}

		IteratorType mIterator;
		std::tuple<OwnedStorageTypes*..., IncludedStorageTypes*> mOwnedAndIncludedTypeStorage;
	};

	template <typename... LeftIteratorTypes, typename... RightIteratorTypes>
	constexpr bool operator==(const TGroupIteratorAdapter<LeftIteratorTypes...>& lhs, const TGroupIteratorAdapter<RightIteratorTypes...>& rhs)
	{
		return lhs.GetUnderlyingIterator() == rhs.GetUnderlyingIterator();
	}

	template <typename... LeftIteratorTypes, typename... RightIteratorTypes>
	constexpr bool operator!=(const TGroupIteratorAdapter<LeftIteratorTypes...>& lhs, const TGroupIteratorAdapter<RightIteratorTypes...>& rhs)
	{
		return !(lhs == rhs);
	}
	/** @} */
} // namespace b3d::ecs
