//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsUnitTestPrefabUpdateHelper.h"
#include "BsUnitTestScenes.h"
#include "Testing/BsTestSuite.h"
#include "Scene/BsSceneObject.h"
#include "Scene/BsPrefab.h"

namespace bs
{
	template <typename SceneWrapperType>
	void UnitTestPrefabUpdateHelper::TestAssertPrefabLinkValid(TestSuite& testSuite, SceneWrapperType& instanceWrapper, SceneWrapperType& prefabWrapper, const UUID& prefabId, bool skipOptional)
	{
		instanceWrapper.PerformSceneObjectBinaryOperation(
			prefabWrapper, [prefabId, &testSuite](const HSceneObject& instanceSceneObject, const HSceneObject& prefabSceneObject)
			{
				B3D_TEST_ASSERT_EXTERNAL(testSuite, !instanceSceneObject->GetPrefabObjectId().Empty())
				B3D_TEST_ASSERT_EXTERNAL(testSuite, instanceSceneObject->GetPrefabObjectId() == prefabSceneObject->GetId())
				B3D_TEST_ASSERT_EXTERNAL(testSuite, instanceSceneObject->GetPrefabResourceId() == prefabId) },
			skipOptional);

		instanceWrapper.PerformComponentBinaryOperation(
			prefabWrapper, [&testSuite](const HComponent& instanceComponent, const HComponent& prefabComponent)
			{
				B3D_TEST_ASSERT_EXTERNAL(testSuite, !instanceComponent->GetPrefabObjectId().Empty())
				B3D_TEST_ASSERT_EXTERNAL(testSuite, instanceComponent->GetPrefabObjectId() == prefabComponent->GetId()) },
			skipOptional);
	}

	/** Asserts that prefab & resource IDs are empty. */
	template <typename SceneWrapperType>
	void UnitTestPrefabUpdateHelper::TestAssetPrefabLinkEmpty(TestSuite& testSuite, SceneWrapperType& prefabWrapper, bool skipOptional)
	{
		prefabWrapper.PerformSceneObjectUnaryOperation([&testSuite](const HSceneObject& sceneObject) {
			B3D_TEST_ASSERT_EXTERNAL(testSuite, sceneObject->GetPrefabObjectId().Empty())
			B3D_TEST_ASSERT_EXTERNAL(testSuite, sceneObject->GetPrefabResourceId().Empty()) }, skipOptional);
	}

	void UnitTestPrefabUpdateHelper::TestAssertScene1WrapperPrefabLinks(TestSuite& testSuite, const HSceneObject& sourceInstanceRoot, const HSceneObject& newInstanceRoot, const HPrefab& rootPrefab, const HPrefab& firstNestedPrefab, const HPrefab& secondNestedPrefab, PrefabLinkCheckType checkType)
	{
		// Wrappers for original scene object instances
		UnitTestSceneB sourceInstanceWrapper_Root(sourceInstanceRoot);
		UnitTestSceneB sourceInstanceWrapper_Child0(sourceInstanceWrapper_Root.OptionalSceneObject_0_0_PrefabInstance);

		// Wrappers for prefab internals
		UnitTestSceneB prefabInternalsWrapper_Root(rootPrefab->GetRoot());
		UnitTestSceneB prefabInternalsWrapper_Root_Child0(prefabInternalsWrapper_Root.OptionalSceneObject_0_0_PrefabInstance);
		UnitTestSceneB prefabInternalsWrapper_Child0(firstNestedPrefab->GetRoot());

		// Wrapper for instantiated scene objects
		UnitTestSceneB instancedWrapper_Root(newInstanceRoot);
		UnitTestSceneB instancedWrapper_Child0(instancedWrapper_Root.OptionalSceneObject_0_0_PrefabInstance);

		// Ensure that original prefab instances have correct prefab object & resource IDs
		TestAssertPrefabLinkValid(testSuite, sourceInstanceWrapper_Child0, prefabInternalsWrapper_Root_Child0, rootPrefab->GetId());
		TestAssertPrefabLinkValid(testSuite, sourceInstanceWrapper_Root, prefabInternalsWrapper_Root, rootPrefab->GetId());

		// Ensure that newly instantiated prefab instances have correct prefab object & resource IDs
		TestAssertPrefabLinkValid(testSuite, instancedWrapper_Child0, prefabInternalsWrapper_Root_Child0, rootPrefab->GetId());
		TestAssertPrefabLinkValid(testSuite, instancedWrapper_Root, prefabInternalsWrapper_Root, rootPrefab->GetId());

		// Ensure that prefab internals have correct prefab object & resource IDs (empty for root, nested otherwise)
		TestAssetPrefabLinkEmpty(testSuite, prefabInternalsWrapper_Root);

		if(checkType == PrefabLinkCheckType::Regular || checkType == PrefabLinkCheckType::SecondNestedPrefabIsInstanceModification)
			TestAssertPrefabLinkValid(testSuite, prefabInternalsWrapper_Root_Child0, prefabInternalsWrapper_Child0, firstNestedPrefab->GetId());
		else if(checkType == PrefabLinkCheckType::OptionalsAreInstanceModifications)
		{
			TestAssertPrefabLinkValid(testSuite, prefabInternalsWrapper_Root_Child0, prefabInternalsWrapper_Child0, firstNestedPrefab->GetId(), true);

			// Optional object is not part of the child prefab resource, so it should have no prefab object (it should be treated as an instance modification)
			B3D_TEST_ASSERT_EXTERNAL(testSuite, prefabInternalsWrapper_Root_Child0.OptionalSceneObject_2->GetPrefabObjectId().Empty())
			B3D_TEST_ASSERT_EXTERNAL(testSuite, prefabInternalsWrapper_Root_Child0.OptionalSceneObject_2->GetPrefabResourceId() == firstNestedPrefab->GetId())
			B3D_TEST_ASSERT_EXTERNAL(testSuite, prefabInternalsWrapper_Root_Child0.OptionalComponent_2->GetPrefabObjectId().Empty())
		}

		// If it has second nested prefab, check that as well
		if(sourceInstanceWrapper_Child0.OptionalSceneObject_0_0_PrefabInstance.IsValid())
		{
			B3D_ASSERT(secondNestedPrefab.IsLoaded());

			UnitTestSceneB sourceInstanceWrapper_Child1(sourceInstanceWrapper_Child0.OptionalSceneObject_0_0_PrefabInstance);
			UnitTestSceneB instancedWrapper_Child1(instancedWrapper_Child0.OptionalSceneObject_0_0_PrefabInstance);

			UnitTestSceneB prefabInternalsWrapper_Root_Child1(prefabInternalsWrapper_Root_Child0.OptionalSceneObject_0_0_PrefabInstance);
			UnitTestSceneB prefabInternalsWrapper_Child0_Child1(prefabInternalsWrapper_Child0.OptionalSceneObject_0_0_PrefabInstance);
			UnitTestSceneB prefabInternalsWrapper_Child1(secondNestedPrefab->GetRoot());

			TestAssertPrefabLinkValid(testSuite, sourceInstanceWrapper_Child1, prefabInternalsWrapper_Root_Child1, rootPrefab->GetId());
			TestAssertPrefabLinkValid(testSuite, instancedWrapper_Child1, prefabInternalsWrapper_Root_Child1, rootPrefab->GetId());

			if(checkType == PrefabLinkCheckType::SecondNestedPrefabIsInstanceModification)
			{
				prefabInternalsWrapper_Root_Child1.PerformSceneObjectUnaryOperation(
					[nestedPrefabId = firstNestedPrefab->GetId(), &testSuite](const HSceneObject& sceneObject)
																					{
				B3D_TEST_ASSERT_EXTERNAL(testSuite, sceneObject->GetPrefabObjectId().Empty())
				B3D_TEST_ASSERT_EXTERNAL(testSuite, sceneObject->GetPrefabResourceId() == nestedPrefabId) });

				prefabInternalsWrapper_Root_Child1.PerformComponentUnaryOperation([&testSuite](const HComponent& component)
																				  { B3D_TEST_ASSERT_EXTERNAL(testSuite, component->GetPrefabObjectId().Empty()) });
			}
			else
			{
				TestAssertPrefabLinkValid(testSuite, prefabInternalsWrapper_Root_Child1, prefabInternalsWrapper_Child0, firstNestedPrefab->GetId());

				// Check internals of first nested prefab compared to second nested prefab
				TestAssertPrefabLinkValid(testSuite, prefabInternalsWrapper_Child0_Child1, prefabInternalsWrapper_Child1, secondNestedPrefab->GetId());
			}
		}
	}
} // namespace bs
