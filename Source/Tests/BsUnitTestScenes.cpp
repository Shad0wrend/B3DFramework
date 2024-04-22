//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsUnitTestScenes.h"
#include "Scene/BsPrefab.h"
#include "Testing/BsTestSuite.h"

using namespace bs;

UnitTestSceneA::UnitTestSceneA(const SPtr<SceneInstance>& sceneInstance)
{
	SceneObject_0 = sceneInstance->CreateSceneObject("SceneA_SceneObject_0");
	SceneObject_0_0 = sceneInstance->CreateSceneObject("SceneA_SceneObject_0_0");
	SceneObject_0_1 = sceneInstance->CreateSceneObject("SceneA_SceneObject_0_1");
	SceneObject_0_1_0 = sceneInstance->CreateSceneObject("SceneA_SceneObject_0_1_0");
	SceneObject_1 = sceneInstance->CreateSceneObject("SceneA_SceneObject_1");
	SceneObject_1_0 = sceneInstance->CreateSceneObject("SceneA_SceneObject_1_0");

	SceneObject_0_0->SetParent(SceneObject_0);
	SceneObject_0_1->SetParent(SceneObject_0);
	SceneObject_1_0->SetParent(SceneObject_1);
	SceneObject_0_1_0->SetParent(SceneObject_0_1);

	SceneObject_0_1_0->SetPosition(Vector3(10.0f, 15.0f, 20.0f));
	SceneObject_0_1->SetPosition(Vector3(1.0f, 2.0f, 3.0f));
	SceneObject_1_0->SetPosition(Vector3(0, 123.0f, 0.0f));

	Component_0 = SceneObject_0->AddComponent<UnitTestComponentA>();
	Component_1 = SceneObject_1->AddComponent<UnitTestComponentB>();
	Component_0_1 = SceneObject_0_1->AddComponent<UnitTestComponentA>();
	Component_0_1_0 = SceneObject_0_1_0->AddComponent<UnitTestComponentA>();
}

UnitTestSceneA::UnitTestSceneA(const HSceneObject& root)
{
	SceneObject_0 = root->FindChild("SceneA_SceneObject_0", false);
	SceneObject_0_0 = SceneObject_0->FindChild("SceneA_SceneObject_0_0", false);
	SceneObject_0_1 = SceneObject_0->FindChild("SceneA_SceneObject_0_1", false);
	SceneObject_0_1_0 = SceneObject_0_1->FindChild("SceneA_SceneObject_0_1_0", false);
	SceneObject_1 = root->FindChild("SceneA_SceneObject_1", false);
	SceneObject_1_0 = SceneObject_1->FindChild("SceneA_SceneObject_1_0", false);

	Component_0 = SceneObject_0->GetComponent<UnitTestComponentA>();
	Component_1 = SceneObject_1->GetComponent<UnitTestComponentB>();
	Component_0_1 = SceneObject_0_1->GetComponent<UnitTestComponentA>();
	Component_0_1_0 = SceneObject_0_1_0->GetComponent<UnitTestComponentA>();
}

/** Populates the scene objects and components by looking them up in the provided hierarchy. */
UnitTestSceneB::UnitTestSceneB(const HSceneObject& root)
	: SceneInstance(root->GetScene()), Root(root)
{
	SceneObject_0 = root->FindChild("SceneB_SceneObject_0", false);
	SceneObject_1 = root->FindChild("SceneB_SceneObject_1", false);

	if(SceneObject_1.IsValid())
	{
		SceneObject_1_0 = SceneObject_1->FindChild("SceneB_SceneObject_1_0", false);

		if(SceneObject_1_0.IsValid())
			Component_1_0 = SceneObject_1_0->GetComponent<UnitTestComponentA>();

		const u32 childCount = SceneObject_1->GetChildCount();
		for(u32 childIndex = 0; childIndex < childCount; childIndex++)
		{
			HSceneObject child = SceneObject_1->GetChild(childIndex);
			if(child != SceneObject_1_0)
			{
				OptionalSceneObject_1_1_PrefabInstance = child;
				break;
			}
		}
	}

	if(SceneObject_0->GetChildCount() > 0)
		OptionalSceneObject_0_0_PrefabInstance = SceneObject_0->GetChild(0);

	OptionalSceneObject_2 = root->FindChild("SceneB_SceneObject_2", false);
	if(OptionalSceneObject_2.IsValid())
		OptionalComponent_2 = OptionalSceneObject_2->GetComponent<UnitTestComponentA>();

	// Record IDs
	PerformSceneObjectUnaryOperation([this](const HSceneObject& sceneObject) {
		OriginalObjectIds[sceneObject.GetId()] = PrefabLinkInformation(sceneObject->GetPrefabObjectId(), sceneObject->GetPrefabResourceId());
		for(const auto& component : sceneObject->GetComponents())
			OriginalObjectIds[component.GetId()] = PrefabLinkInformation(component->GetPrefabObjectId(), sceneObject->GetPrefabResourceId());
	 });
}

HSceneObject UnitTestSceneB::PopulateNewSceneInstance(const char* name)
{
	SPtr<class SceneInstance> sceneInstance = SceneInstance::Create(name);
	UnitTestSceneB wrapper = PopulateParent(sceneInstance->GetRoot());

	return sceneInstance->GetRoot();
}

UnitTestSceneB UnitTestSceneB::PopulateParent(const HSceneObject& parent)
{
	UnitTestSceneB scene;

	scene.SceneInstance = parent->GetScene();
	scene.Root = parent;

	scene.SceneObject_0 = scene.SceneInstance->CreateSceneObject("SceneB_SceneObject_0");
	scene.SceneObject_0->SetParent(parent);

	scene.SceneObject_1 = scene.SceneInstance->CreateSceneObject("SceneB_SceneObject_1");
	scene.SceneObject_1->SetParent(parent);

	scene.SceneObject_1_0 = scene.SceneInstance->CreateSceneObject("SceneB_SceneObject_1_0");
	scene.SceneObject_1_0->SetParent(scene.SceneObject_1);

	scene.SceneObject_0->SetPosition(Vector3(50.0f, 50.0f, 50.0f));
	scene.Component_1_0 = scene.SceneObject_1_0->AddComponent<UnitTestComponentA>();
	scene.Component_1_0->SetName("SceneB_Component_1_0");

	// Record IDs
	scene.PerformSceneObjectUnaryOperation([&scene](const HSceneObject& sceneObject) {
		scene.OriginalObjectIds[sceneObject.GetId()] = PrefabLinkInformation(sceneObject->GetPrefabObjectId(), sceneObject->GetPrefabResourceId());
		for(const auto& component : sceneObject->GetComponents())
			scene.OriginalObjectIds[component.GetId()] = PrefabLinkInformation(component->GetPrefabObjectId(), sceneObject->GetPrefabResourceId());
	 });

	return scene;
}

HSceneObject UnitTestSceneB::SetUnitTestSceneAChildPrefab_0_0(const Prefab& prefab)
{
	B3D_ASSERT(!OptionalSceneObject_0_0_PrefabInstance.IsValid());

	OptionalSceneObject_0_0_PrefabInstance = prefab.Instantiate(SceneInstance);
	OptionalSceneObject_0_0_PrefabInstance->SetParent(SceneObject_0);
	OptionalSceneObject_0_0_PrefabInstance->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

	return OptionalSceneObject_0_0_PrefabInstance;
}

HSceneObject UnitTestSceneB::SetUnitTestSceneBChildPrefab_0_0(const Prefab& prefab)
{
	B3D_ASSERT(!OptionalSceneObject_0_0_PrefabInstance.IsValid());

	OptionalSceneObject_0_0_PrefabInstance = prefab.Instantiate(SceneInstance);
	OptionalSceneObject_0_0_PrefabInstance->SetParent(SceneObject_0);
	OptionalSceneObject_0_0_PrefabInstance->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

	OptionalPrefabInstance_0_0 = B3DMakeShared<UnitTestSceneB>(OptionalSceneObject_0_0_PrefabInstance);
	return OptionalSceneObject_0_0_PrefabInstance;
}

HSceneObject UnitTestSceneB::SetUnitTestSceneBChildPrefab_1_1(const Prefab& prefab)
{
	B3D_ASSERT(!OptionalSceneObject_1_1_PrefabInstance.IsValid());

	OptionalSceneObject_1_1_PrefabInstance = prefab.Instantiate(SceneInstance);
	OptionalSceneObject_1_1_PrefabInstance->SetParent(SceneObject_1);
	OptionalSceneObject_1_1_PrefabInstance->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

	OptionalPrefabInstance_1_1 = B3DMakeShared<UnitTestSceneB>(OptionalSceneObject_1_1_PrefabInstance);
	return OptionalSceneObject_1_1_PrefabInstance;
}

void UnitTestSceneB::CreateOptionalSceneObject_2()
{
	OptionalSceneObject_2 = SceneInstance->CreateSceneObject("SceneB_SceneObject_2");
	OptionalSceneObject_2->SetParent(Root);

	OptionalComponent_2 = OptionalSceneObject_2->AddComponent<UnitTestComponentA>();
	OptionalComponent_2->SetName("SceneB_Component_2");
}

void UnitTestSceneB::DestroySceneObject_1_0()
{
	OriginalObjectIds.erase(SceneObject_1_0.GetId());
	OriginalObjectIds.erase(Component_1_0.GetId());

	SceneObject_1_0->Destroy();
	Component_1_0->Destroy();
}

void UnitTestSceneB::TestAssertOriginalIds(TestSuite& testSuite)
{
	PerformSceneObjectUnaryOperation([this, &testSuite](const HSceneObject& sceneObject) {
		auto foundSceneObject = OriginalObjectIds.find(sceneObject.GetId());
		B3D_TEST_ASSERT_EXTERNAL(testSuite, foundSceneObject != OriginalObjectIds.end())

		if(foundSceneObject != OriginalObjectIds.end())
		{
			B3D_TEST_ASSERT_EXTERNAL(testSuite, foundSceneObject->second.PrefabObjectId == sceneObject->GetPrefabObjectId())
			B3D_TEST_ASSERT_EXTERNAL(testSuite, foundSceneObject->second.PrefabResourceId == sceneObject->GetPrefabResourceId())
		}

		for(const auto& component : sceneObject->GetComponents())
		{
			auto foundComponent = OriginalObjectIds.find(component.GetId());
			B3D_TEST_ASSERT_EXTERNAL(testSuite, foundComponent != OriginalObjectIds.end())

			if(foundComponent != OriginalObjectIds.end())
				B3D_TEST_ASSERT_EXTERNAL(testSuite, foundComponent->second.PrefabObjectId == component->GetPrefabObjectId())
		}
	 });
	
}
