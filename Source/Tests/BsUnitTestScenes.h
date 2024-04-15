//************************************ bs::framework - Copyright 2024 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once
#include "Prerequisites/BsPrerequisitesUtil.h"
#include "Scene/BsSceneManager.h"
#include "Scene/BsSceneObject.h"
#include "BsUnitTestComponents.h"

namespace bs
{
	/** Wrapper for easier scene creation and object access. */
	struct UnitTestSceneA
	{
		/** Populates the provided scene instance with the scene. */
		UnitTestSceneA(const SPtr<SceneInstance>& sceneInstance);

		/** Populates the scene objects and components by looking them up in the provided hierarchy. */
		UnitTestSceneA(const HSceneObject& root);

		template <class T>
		void PerformSceneObjectUnaryOperation(T&& predicate)
		{
			predicate(SceneObject_0);
			predicate(SceneObject_0_0);
			predicate(SceneObject_0_1);
			predicate(SceneObject_0_1_0);
			predicate(SceneObject_1);
			predicate(SceneObject_1_0);
		}

		template <class T>
		void PerformComponentUnaryOperation(T&& predicate)
		{
			predicate(Component_0);
			predicate(Component_1);
			predicate(Component_0_1);
			predicate(Component_0_1_0);
		}

		template <class T>
		void PerformSceneObjectBinaryOperation(UnitTestSceneA& other, T&& predicate)
		{
			predicate(SceneObject_0, other.SceneObject_0);
			predicate(SceneObject_0_0, other.SceneObject_0_0);
			predicate(SceneObject_0_1, other.SceneObject_0_1);
			predicate(SceneObject_0_1_0, other.SceneObject_0_1_0);
			predicate(SceneObject_1, other.SceneObject_1);
			predicate(SceneObject_1_0, other.SceneObject_1_0);
		}

		template <class T>
		void PerformComponentBinaryOperation(UnitTestSceneA& other, T&& predicate)
		{
			predicate(Component_0, other.Component_0);
			predicate(Component_1, other.Component_1);
			predicate(Component_0_1, other.Component_0_1);
			predicate(Component_0_1_0, other.Component_0_1_0);
		}

		HSceneObject SceneObject_0;
		HSceneObject SceneObject_0_0;
		HSceneObject SceneObject_0_1;
		HSceneObject SceneObject_0_1_0;
		HSceneObject SceneObject_1;
		HSceneObject SceneObject_1_0;

		HUnitTestComponentA Component_0;
		HUnitTestComponentB Component_1;
		HUnitTestComponentA Component_0_1;
		HUnitTestComponentA Component_0_1_0;
	};

	/**
	 * Allows you to easily set up the following scene object hierarchy:
	 * Root
	 *  SceneObject_0 [Components: None]
	 *    OptionalSceneObject_0_0_PrefabInstance (Optional child prefab instance, if prefab is provided)
	 *  SceneObject_1 [Components: None]
	 *    SceneObject_1_0 [Components: UnitTestComponentA]
	 *  OptionalSceneObject_2 [Components: UnitTestComponentA] (Optional object that can be created after initial construction)
	 */
	struct UnitTestSceneB
	{
		UnitTestSceneB() = default;

		/** Populates the provided parent with the scene. */
		UnitTestSceneB(const HSceneObject& parent, const SPtr<Prefab>& childPrefab);

		/** Populates the scene objects and components by looking them up in the provided hierarchy. */
		UnitTestSceneB(const HSceneObject& root);

		virtual ~UnitTestSceneB() = default;

		static HSceneObject PopulateNewSceneInstance(const char* name, const SPtr<Prefab>& childPrefab);
		static void PopulateParent(const HSceneObject& parent, const SPtr<Prefab>& childPrefab);

		/** Creates OptionalSceneObect_2 and its associated components. */
		void CreateOptionalObjects();

		/** Performs an operation over scene objects in the scene. If an object has been destroyed, the predicate won't be called on it. If @p skipOptional is true, OptionalSceneObject_2 will not be visited. */
		template <class T>
		void PerformSceneObjectUnaryOperation(T&& predicate, bool skipOptional = false)
		{
			predicate(Root);
			if(SceneObject_0.IsValid()) predicate(SceneObject_0);
			if(SceneObject_1.IsValid()) predicate(SceneObject_1);
			if(SceneObject_1_0.IsValid()) predicate(SceneObject_1_0);
			if(!skipOptional && OptionalSceneObject_2.IsValid()) predicate(OptionalSceneObject_2);
		}

		/** Performs an operation over components in the scene. If a component has been destroyed, the predicate won't be called on it. If @p skipOptional is true, OptionalComponent_2 will not be visited. */
		template <class T>
		void PerformComponentUnaryOperation(T&& predicate, bool skipOptional = false)
		{
			if(Component_1_0.IsValid()) predicate(Component_1_0);
			if(!skipOptional && OptionalComponent_2.IsValid()) predicate(OptionalComponent_2);
		}

		/** Performs an operation over matching scene objects in two scenes. If an object has been destroyed in this scene, the predicate won't be called on it. If @p skipOptional is true, OptionalSceneObject_2 will not be visited. */
		template <class T>
		void PerformSceneObjectBinaryOperation(UnitTestSceneB& other, T&& predicate, bool skipOptional = false)
		{
			predicate(Root, other.Root);
			if(SceneObject_0.IsValid()) predicate(SceneObject_0, other.SceneObject_0);
			if(SceneObject_1.IsValid()) predicate(SceneObject_1, other.SceneObject_1);
			if(SceneObject_1_0.IsValid()) predicate(SceneObject_1_0, other.SceneObject_1_0);
			if(!skipOptional && OptionalSceneObject_2.IsValid()) predicate(OptionalSceneObject_2, other.OptionalSceneObject_2);
		}

		/** Performs an operation over matching components in two scenes. If a component has been destroyed in this scene, the predicate won't be called on it. If @p skipOptional is true, OptionalComponent_2 will not be visited. */
		template <class T>
		void PerformComponentBinaryOperation(UnitTestSceneB& other, T&& predicate, bool skipOptional = false)
		{
			if(Component_1_0.IsValid()) predicate(Component_1_0, other.Component_1_0);
			if(!skipOptional && OptionalComponent_2.IsValid()) predicate(OptionalComponent_2, other.OptionalComponent_2);
		}

		SPtr<SceneInstance> SceneInstance;
		HSceneObject Root;

		HSceneObject SceneObject_0;
		HSceneObject OptionalSceneObject_0_0_PrefabInstance;
		HSceneObject SceneObject_1;
		HSceneObject SceneObject_1_0;
		HUnitTestComponentA Component_1_0;

		// These objects may be created after initial construction
		HSceneObject OptionalSceneObject_2;
		HComponent OptionalComponent_2;
	};
} // namespace bs
