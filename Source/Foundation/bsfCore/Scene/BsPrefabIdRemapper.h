//************************************ bs::framework - Copyright 2024 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once
#include "BsCorePrerequisites.h"
#include "BsPrefabUtility.h"

namespace bs
{
	/**
	 * Helper class that allows prefab instance hierarchy remap their IDs for use within prefab internals. This includes game object ID, prefab object ID and prefab resource ID.
	 * Examples are provided below for better understanding - the main purpose of this class is find the original IDs from within the prefab, from an instance of that prefab
	 * (or assign new IDs if that is not possible).
	 *
	 * //////////////////////////////////////////////////////////////////////////////////////////////////////////
	 * The mapping of instance -> prefab is as follows:
	 *  All objects within the prefab internal hierarchy will have a unique game object ID.
	 *	All instance objects of that prefab will have a prefab object ID pointing to that game object ID, and prefab resource ID pointing to the prefab resource.
	 *
	 * For example:
	 * Prefab #1 (Resource ID=PFB1) internal hierarchy:
	 *  Root [Game Object ID=OB11, Prefab Object ID=OB11, Prefab Resource ID=PFB1]
	 *   My object [Game Object ID=OB12, Prefab Object ID=OB12, Prefab Resource ID=PFB1]
	 *
	 * Instance of Prefab #1
	 *  Root [Game Object ID=OBI1, Prefab Object ID=OB11, Prefab Resource ID=PFB1]
	 *   My object [Game Object ID=OBI2, Prefab Object ID=OB12, Prefab Resource ID=PFB1]
	 *
	 * //////////////////////////////////////////////////////////////////////////////////////////////////////////
	 * A prefab may contain nested prefabs, but this is not reflected in the instance of that prefab. An instance of a prefab will link directly to its parent prefab. Information
	 * about nested prefabs is instead stored in the parent prefab resource's internal prefab hierarchy. There prefab object ID and prefab resource ID will point to the nested prefab.
	 * If that nested prefab contains nested prefabs in itself, that information is stored in the nested prefab resource's internal prefab hierarchy, and so on.
	 *
	 * For example:
	 * Prefab #2 (Resource ID=PFB2) internal hierarchy:
	 *  Root [Game Object ID=OB21, Prefab Object ID=OB21, Prefab Resource ID=PFB2]
	 *
	 * Prefab #1 (Resource ID=PFB1) internal hierarchy:
	 *  Root [Game Object ID=OB11, Prefab Object ID=OB11, Prefab Resource ID=PFB1]
	 *   My object [Game Object ID=OB12, Prefab Object ID=OB12, Prefab Resource ID=PFB1]
	 *   Prefab #2 instance [Game Object ID=OB13, Prefab Object ID=OB21, Prefab Resource ID=PFB2]
	 *
	 * Instance of Prefab #1
	 *  Root [Game Object ID=OBI1, Prefab Object ID=OB11, Prefab Resource ID=PFB1]
	 *   My object [Game Object ID=OBI2, Prefab Object ID=OB12, Prefab Resource ID=PFB1]
	 *	 Prefab #2 instance [Game Object ID=OBI3, Prefab Object ID=OB13, Prefab Resource ID=PFB1]
	 *
	 * //////////////////////////////////////////////////////////////////////////////////////////////////////////
	 * The above means that generally there is only ever a single level of prefab nesting as visible from the prefab internals hierarchy. One exception are prefabs that are added to a
	 * nested prefab, but this change doesn't get applied to the nested prefab resource. In such case those prefabs are treated as instance modifications, and will result in multiple
	 * levels of nesting within the hierarchy.
	 *
	 * For example:
	 * Prefab #3 (Resource ID=PFB3) internal hierarchy:
	 *  Root [Game Object ID=OB31, Prefab Object ID=OB31, Prefab Resource ID=PFB3]
	 *
	 * Prefab #2 (Resource ID=PFB2) internal hierarchy:
	 *  Root [Game Object ID=OB21, Prefab Object ID=OB21, Prefab Resource ID=PFB2]
	 *  // Note PFB3 is not part of this prefab
	 *
	 * Prefab #1 (Resource ID=PFB1) internal hierarchy:
	 *  Root [Game Object ID=OB11, Prefab Object ID=OB11, Prefab Resource ID=PFB1]
	 *   My object [Game Object ID=OB12, Prefab Object ID=OB12, Prefab Resource ID=PFB1]
	 *   Prefab #2 instance [Game Object ID=OB13, Prefab Object ID=OB21, Prefab Resource ID=PFB2]
	 *    Prefab #3 instance [Game Object ID=OB14, Prefab Object ID=OB31, Prefab Resource ID=PFB3] // Links directly to PFB3, as it's an instance modification of PFB1, and not part of PFB2
	 *
	 * Instance of Prefab #1
	 *  Root [Game Object ID=OBI1, Prefab Object ID=OB11, Prefab Resource ID=PFB1]
	 *   My object [Game Object ID=OBI2, Prefab Object ID=OB12, Prefab Resource ID=PFB1]
	 *	 Prefab #2 instance [Game Object ID=OBI3, Prefab Object ID=OB13, Prefab Resource ID=PFB1]
	 *    Prefab #3 instance [Game Object ID=OBI4, Prefab Object ID=OB14, Prefab Resource ID=PFB1]
	 *
	 * If we were to apply the change to PFB2 within PFB1, PFB3 becomes part of PFB2, and PFB1 then links only to PFB2:
	 * Prefab #3 (Resource ID=PFB3) internal hierarchy:
	 *  Root [Game Object ID=OB31, Prefab Object ID=OB31, Prefab Resource ID=PFB3]
	 *
	 * Prefab #2 (Resource ID=PFB2) internal hierarchy:
	 *  Root [Game Object ID=OB21, Prefab Object ID=OB21, Prefab Resource ID=PFB2]
	 *   Prefab #3 instance [Game Object ID=OB22, Prefab Object ID=OB31, Prefab Resource ID=PFB3] // PFB3 now nested prefab of PFB2
	 *
	 * Prefab #1 (Resource ID=PFB1) internal hierarchy:
	 *  Root [Game Object ID=OB11, Prefab Object ID=OB11, Prefab Resource ID=PFB1]
	 *   My object [Game Object ID=OB12, Prefab Object ID=OB12, Prefab Resource ID=PFB1]
	 *   Prefab #2 instance [Game Object ID=OB13, Prefab Object ID=OB21, Prefab Resource ID=PFB2]
	 *    Prefab #3 instance [Game Object ID=OB14, Prefab Object ID=OB22, Prefab Resource ID=PFB2] // This instance now links to PFB2
	 *
	 * Instance of Prefab #1
	 *  Root [Game Object ID=OBI1, Prefab Object ID=OB11, Prefab Resource ID=PFB1]
	 *   My object [Game Object ID=OBI2, Prefab Object ID=OB12, Prefab Resource ID=PFB1]
	 *	 Prefab #2 instance [Game Object ID=OBI3, Prefab Object ID=OB13, Prefab Resource ID=PFB1]
	 *    Prefab #3 instance [Game Object ID=OBI4, Prefab Object ID=OB14, Prefab Resource ID=PFB1]
	 *
	 * //////////////////////////////////////////////////////////////////////////////////////////////////////////
	 * As may be seen from examples above, if an object within internal prefab hierarchy is not part of a nested prefab, its prefab object ID will be equal to its game object ID,
	 * and resource ID point to the prefab resource it is contained in.
	 */
	class B3D_CORE_EXPORT PrefabIdRemapper
	{
	public:
		struct SceneObjectInformation
		{
			SceneObjectInformation(const HSceneObject& sceneObject, const UUID& parentPrefabResourceId, i32 prefabNestingLevel)
				: SceneObject(sceneObject), ParentPrefabResourceId(parentPrefabResourceId), ParentNestingLevel(prefabNestingLevel)
			{}

			HSceneObject SceneObject;
			UUID ParentPrefabResourceId;
			i32 ParentNestingLevel = 0;
		};

		struct PrefabInformation
		{
			HPrefab Prefab;
			UUID PrefabId;
			UnorderedMap<UUID, PrefabLinkInformation> PrefabHierarchyIds; /**< Game object ID -> { Prefab object ID, prefab resource ID } map for all objects in the prefab hierarchy. */
		};

		PrefabIdRemapper(const HSceneObject& originalPrefabHierarchy, const UUID& rootPrefabId);

		/**
		 * Updates all objects in @p hierarchyRoot with IDs so they match previously stored prefab hierarchy.
		 *
		 * Returns a map containing a remapping from instance game object IDs to prefab game object IDs, which may be used for updating the original
		 * scene object hierarchy, so it links to the created/updated prefab hierarchy.
		 */
		UnorderedMap<UUID, UUID> RestoreOriginalPrefabIds(const HSceneObject& hierarchyRoot);

	private:
		/**
		 * Finds the prefab associated with the scene object instance, and populates a lookup from instance ID to { prefab object id, prefab resource id }.
		 * If a prefab has already been processed, cached data will be returned.
		 */
		const PrefabInformation& GetOrPopulatePrefabInformation(const HSceneObject& sceneObject);

		/**
		 * Deduces game object ID to used when referencing the object in the internal prefab hierarchy. This will be the matching prefab object ID, if one is provided and still
		 * exists in the current prefab hierarchy, or a brand new ID.
		 *
		 * @param gameObject							Game object instance for which to deduce the ID.
		 * @param associatedPrefabOriginalHierarchyIds	Game object ID -> { prefab object ID, prefab resource ID } mapping for all objects in the associated internal prefab hierarchy.
		 *												This is the prefab hierarchy of the prefab that is currently linked to the associated scene object.
		 * @return										Game object ID to assign to the game object.
		 */
		static UUID DeduceInternalPrefabGameObjectId(const HGameObject& gameObject, const UnorderedMap<UUID, PrefabLinkInformation>& associatedPrefabOriginalHierarchyIds);

		/**
		 * Deduces original prefab object ID and prefab resource ID, as they would be stored in the internal prefab resource, based on a prefab instance and original prefab hierarchy.
		 *
		 * @param gameObject							Game object instance for which to deduce the IDs.
		 * @param gameObjectIdInPrefab					Game object ID that will be used for @p gameObject in the internal prefab hierarchy.
		 * @param associatedSceneObjectInformation		Information about the parent scene object that owns @p gameObject. If @p gameObject is a SceneObject, this should point to the same object.
		 * @param associatedPrefabOriginalHierarchyIds	Game object ID -> { prefab object ID, prefab resource ID } mapping for all objects in the associated internal prefab hierarchy.
		 *												This is the prefab hierarchy of the prefab that is currently linked to the associated scene object.
		 * @return										Prefab object ID and prefab resource ID, as they are stored in the associated prefab. Or empty IDs if not found.
		 */
		static PrefabLinkInformation DeduceInternalPrefabLinkInformation(const HGameObject& gameObject, const UUID& gameObjectIdInPrefab, const SceneObjectInformation& associatedSceneObjectInformation, const UnorderedMap<UUID, PrefabLinkInformation>& associatedPrefabOriginalHierarchyIds);

		/**
		 * Assigns original prefab game object ID, prefab object ID and prefab resource ID to the provided game object. The original prefab IDs refers to the IDs stored when a game object
		 * is part of the internal prefab hierarchy.
		 *
		 * @param gameObject					Game object to which to assign the IDs to.
		 * @param gameObjectIdInPrefab			ID of the game object within the prefab hierarchy, as output by DeduceOriginalPrefabIds().
		 * @param linkInformationInPrefab		Link information to nested prefabs, as output by DeduceOriginalPrefabIds().
		 * @param rootPrefabResourceId			ID of the prefab resource that the resulting hierarchy will be stored in.
		 * @param nestingLevel					Current depth of nested prefab we are at. 0 for root, 1 for first nested prefab, etc.
		 */
		static void AssignInternalPrefabIds(const GameObjectHandleBase& gameObject, const UUID& gameObjectIdInPrefab, const PrefabLinkInformation& linkInformationInPrefab, const UUID& rootPrefabResourceId, i32 nestingLevel);

		UnorderedMap<UUID, PrefabInformation> mPrefabCache;
		UUID mRootPrefabId;
	};
} // namespace bs
