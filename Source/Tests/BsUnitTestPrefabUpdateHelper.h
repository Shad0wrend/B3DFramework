//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once
#include "BsCorePrerequisites.h"

namespace bs
{
	/** Type of check to perform when verifying prefab links. */
	enum class PrefabLinkCheckType
	{
		/**
		 * Instances must match object/resource ID of the prefab resource.
		 * Prefab resource internal root must have empty object/resource id.
		 * Nested prefab resource internal must have object and resource id pointing to the nested prefab resource.
		 */
		Regular,

		/**
		 * Same as regular, except for treatment of optionals in the nested prefab.
		 * Optionals in the nested prefab resource must have empty object id, and resource id pointing to the nested prefab resource.
		 */
		OptionalsAreInstanceModifications,

		/**
		 * Same as regular, except if there is a second nested prefab, it will be treated as an instance modification.
		 * Its objects must have empty object id, and resource id pointing to the (first) nested prefab resource.
		 */
		SecondNestedPrefabIsInstanceModification
	};

	struct UnitTestPrefabUpdateHelper
	{
		/** Asserts that instance prefab & resource IDs point to the prefab. */
		template <typename UnitTestSceneType>
		static void TestAssertPrefabLinkValid(TestSuite& testSuite, UnitTestSceneType& instanceWrapper, UnitTestSceneType& prefabWrapper, const UUID& prefabId, bool skipOptional = false);

		/** Asserts that prefab & resource IDs are empty. */
		template <typename UnitTestSceneType>
		static void TestAssetPrefabLinkEmpty(TestSuite& testSuite, UnitTestSceneType& prefabWrapper, bool skipOptional = false);

		static void TestAssertScene1WrapperPrefabLinks(TestSuite& testSuite, const HSceneObject& sourceInstanceRoot, const HSceneObject& newInstanceRoot, const HPrefab& rootPrefab, const HPrefab& firstNestedPrefab, const HPrefab& secondNestedPrefab, PrefabLinkCheckType checkType);
	};
} // namespace bs
