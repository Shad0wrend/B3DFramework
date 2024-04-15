//************************************ bs::framework - Copyright 2024 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Scene/BsPrefabIdRemapper.h"
#include "BsPrefab.h"
#include "BsSceneObject.h"
#include "Resources/BsResources.h"

using namespace bs;

PrefabIdRemapper::PrefabIdRemapper(const HSceneObject& originalPrefabHierarchy, const UUID& rootPrefabId)
	:mRootPrefabId(rootPrefabId)
{
	PrefabInformation rootPrefabInformation;
	rootPrefabInformation.PrefabId = rootPrefabId;

	if(originalPrefabHierarchy.IsValid())
		rootPrefabInformation.PrefabHierarchyIds = PrefabUtility::GetInstanceToPrefabLinkInformationMap(originalPrefabHierarchy, true);

	mPrefabCache[rootPrefabId] = std::move(rootPrefabInformation);
}

UnorderedMap<UUID, UUID> PrefabIdRemapper::RestoreOriginalPrefabIds(const HSceneObject& hierarchyRoot)
{
	FrameScope frameScope;
	FrameStack<SceneObjectInformation> todo;
	todo.emplace(hierarchyRoot, UUID::kEmpty, -1);

	UnorderedMap<UUID, UUID> remappedGameObjectIDs;
	while(!todo.empty())
	{
		const SceneObjectInformation currentObjectToProcess = todo.top();
		todo.pop();

		HSceneObject sceneObject = currentObjectToProcess.SceneObject;

		// Deduce original IDs for the scene object
		const UUID originalSceneObjectId = currentObjectToProcess.SceneObject.GetId();

		const UnorderedMap<UUID, PrefabLinkInformation>& associatedPrefabOriginalHierarchyIds = GetOrPopulatePrefabInformation(currentObjectToProcess.SceneObject).PrefabHierarchyIds;
		const UUID sceneObjectIdInPrefab = DeduceInternalPrefabGameObjectId(currentObjectToProcess.SceneObject, associatedPrefabOriginalHierarchyIds);

		const PrefabLinkInformation sceneObjectLinkInformation = DeduceInternalPrefabLinkInformation(currentObjectToProcess.SceneObject, sceneObjectIdInPrefab, currentObjectToProcess, associatedPrefabOriginalHierarchyIds);

		// If we've reached a new prefab instance, increment nesting level.
		i32 nestingLevel = currentObjectToProcess.ParentNestingLevel;
		if(nestingLevel == -1) // -1 means parent resource ID has not been assigned yet
			nestingLevel = 0;
		else if(sceneObjectLinkInformation.PrefabResourceId != currentObjectToProcess.ParentPrefabResourceId)
			nestingLevel++;

		// Deduce and assign component IDs
		// Note: Important to process components before the scene object, as we depend on the owning scene object's IDs to be unchanged
		for(auto& component : sceneObject->GetComponents())
		{
			const UUID originalComponentId = component.GetId();
			const UUID componentIdInPrefab = DeduceInternalPrefabGameObjectId(component, associatedPrefabOriginalHierarchyIds);
			const PrefabLinkInformation componentLinkInformation = DeduceInternalPrefabLinkInformation(component, componentIdInPrefab, currentObjectToProcess, associatedPrefabOriginalHierarchyIds);

			AssignInternalPrefabIds(component, componentIdInPrefab, componentLinkInformation, mRootPrefabId, nestingLevel);

			remappedGameObjectIDs[originalComponentId] = componentIdInPrefab;
		}

		// Assign scene object IDs
		{
			AssignInternalPrefabIds(sceneObject, sceneObjectIdInPrefab, sceneObjectLinkInformation, mRootPrefabId, nestingLevel);

			remappedGameObjectIDs[originalSceneObjectId] = sceneObjectIdInPrefab;
		}

		const u32 childCount = currentObjectToProcess.SceneObject->GetChildCount();
		for(u32 childIndex = 0; childIndex < childCount; childIndex++)
		{
			const HSceneObject childSceneObject = currentObjectToProcess.SceneObject->GetChild(childIndex);
			todo.emplace(childSceneObject, sceneObjectLinkInformation.PrefabResourceId, nestingLevel);
		}
	}

	return remappedGameObjectIDs;
}

const PrefabIdRemapper::PrefabInformation& PrefabIdRemapper::GetOrPopulatePrefabInformation(const HSceneObject& sceneObject)
{
	static const PrefabInformation kEmpty;

	const UUID prefabResourceId = sceneObject->GetPrefabResourceId();
	if(prefabResourceId.Empty())
		return kEmpty;

	if(auto found = mPrefabCache.find(prefabResourceId); found != mPrefabCache.end())
		return found->second;

	PrefabInformation prefabInformation;
	prefabInformation.Prefab = B3DStaticResourceCast<Prefab>(GetResources().LoadFromUuid(prefabResourceId, false, ResourceLoadFlag::None));

	if(prefabInformation.Prefab.IsLoaded(false))
		prefabInformation.PrefabHierarchyIds = PrefabUtility::GetInstanceToPrefabLinkInformationMap(prefabInformation.Prefab->GetRoot(), true);

	auto result = mPrefabCache.insert(std::make_pair(prefabResourceId, std::move(prefabInformation)));
	return result.first->second;
}

UUID PrefabIdRemapper::DeduceInternalPrefabGameObjectId(const HGameObject& gameObject, const UnorderedMap<UUID, PrefabLinkInformation>& associatedPrefabOriginalHierarchyIds)
{
	// Try to find or generate a new scene object ID and prefab object ID
	const UUID& instancePrefabObjectId = gameObject->GetPrefabObjectId();
	if(gameObject->GetPrefabObjectId().Empty()) // This is a new object that was never part of a prefab (or its link was broken, in which case we treat it as new)
		return UUIDGenerator::GenerateRandom();
	else
	{
		auto found = associatedPrefabOriginalHierarchyIds.find(instancePrefabObjectId);
		if(found != associatedPrefabOriginalHierarchyIds.end()) // This is an object that was previously part of the prefab, retrieve its original ID
			return instancePrefabObjectId;
		else // This is an object with a prefab link, but not previously part of this prefab, which means its a newly added nested prefab and needs a new ID
			return UUIDGenerator::GenerateRandom();
	}
}

PrefabLinkInformation PrefabIdRemapper::DeduceInternalPrefabLinkInformation(const HGameObject& gameObject, const UUID& gameObjectIdInPrefab, const SceneObjectInformation& associatedSceneObjectInformation, const UnorderedMap<UUID, PrefabLinkInformation>& associatedPrefabOriginalHierarchyIds)
{
	PrefabLinkInformation prefabLinkInformation;

	const UUID& instancePrefabObjectId = gameObject->GetPrefabObjectId();
	if(gameObject->GetPrefabObjectId().Empty()) // Object with no prefab link inherits the parent prefab resource ID
	{
		prefabLinkInformation.PrefabObjectId = gameObjectIdInPrefab;
		prefabLinkInformation.PrefabResourceId = associatedSceneObjectInformation.ParentPrefabResourceId;
	}
	else
	{
		auto found = associatedPrefabOriginalHierarchyIds.find(instancePrefabObjectId);
		if(found != associatedPrefabOriginalHierarchyIds.end()) // This is an object that was previously part of the prefab, retrieve its original IDs
		{
			prefabLinkInformation.PrefabObjectId = found->second.PrefabObjectId;
			prefabLinkInformation.PrefabResourceId = found->second.PrefabResourceId;
		}
		else // This is an object with a prefab link, but not previously part of this prefab, which means its a newly added nested prefab. Those keep pointing to their original prefabs.
		{
			prefabLinkInformation.PrefabObjectId = gameObject->GetPrefabObjectId();
			prefabLinkInformation.PrefabResourceId = associatedSceneObjectInformation.SceneObject->GetPrefabResourceId();

			if(!B3D_ENSURE(!prefabLinkInformation.PrefabResourceId.Empty()))
				prefabLinkInformation.PrefabResourceId = associatedSceneObjectInformation.ParentPrefabResourceId; // Fallback, shouldn't happen
		}
	}

	return prefabLinkInformation;
}

void PrefabIdRemapper::AssignInternalPrefabIds(const GameObjectHandleBase& gameObject, const UUID& gameObjectIdInPrefab, const PrefabLinkInformation& linkInformationInPrefab, const UUID& rootPrefabResourceId, i32 nestingLevel)
{
	gameObject->SetId(gameObjectIdInPrefab);
	gameObject.GetSharedHandleData()->Id = gameObjectIdInPrefab;

	if(nestingLevel == 0) // Root objects just point to self
		gameObject->SetPrefabObjectId(gameObjectIdInPrefab);
	else
		gameObject->SetPrefabObjectId(linkInformationInPrefab.PrefabObjectId);

	// If scene object, set prefab resource IDs as well
	if(auto sceneObject = B3DRTTICast<SceneObject>(gameObject.Get()))
	{
		if(nestingLevel == 0) // Root objects point to root prefab
			sceneObject->SetPrefabResourceId(rootPrefabResourceId);
		else
			sceneObject->SetPrefabResourceId(linkInformationInPrefab.PrefabResourceId);
	}
}
