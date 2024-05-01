//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Scene/BsPrefabUtility.h"

#include "BsGameObjectCollection.h"
#include "BsPrefabIdRemapper.h"
#include "BsSceneManager.h"
#include "Scene/BsSceneObjectHierarchyDelta.h"
#include "Scene/BsPrefab.h"
#include "Scene/BsSceneObject.h"
#include "Resources/BsResources.h"

using namespace bs;

/** Contains game-object instance data and UUID. */
struct PrefabInstanceData
{
	PrefabInstanceData(const SPtr<GameObjectInstanceData>& instanceData = nullptr, const UUID& id = UUID::kEmpty)
		: InstanceData(instanceData), Id(id)
	{ }

	SPtr<GameObjectInstanceData> InstanceData;
	UUID Id;
};

/**
 * Traverses the object hierarchy, finds all child objects and components and records their instance data. Instance data essentially holds the object's "identity"
 * and by restoring it we ensure any handles pointing to the object earlier will still point to the new version.
 *
 * @param	sceneObject			Root object to traverse and record.
 * @param	outInstanceData		A map of object IDs to instance data. 
 *
 * @note	Does not recurse into child prefab instances.
 */
static void RecordInstanceData(const HSceneObject& sceneObject, UnorderedMap<UUID, PrefabInstanceData>& outInstanceData)
{
	sceneObject->IterateHierarchy(
		[&outInstanceData](const HSceneObject& sceneObject)
		{
			const UUID& prefabObjectId = sceneObject->GetPrefabObjectId();
			if(!prefabObjectId.Empty())
				outInstanceData[prefabObjectId] = PrefabInstanceData(sceneObject->GetInstanceData(), sceneObject->GetId());

			return true;
		},
		[&outInstanceData](const HComponent& component)
		{
			const UUID& prefabObjectId = component->GetPrefabObjectId();
			if(!prefabObjectId.Empty())
				outInstanceData[prefabObjectId] = PrefabInstanceData(component->GetInstanceData(), component->GetId());
		});
}

/**
 * Restores instance data in the provided hierarchy, using prefab ids to determine what data maps to which objects.
 *
 * @param	sceneObject		Root object to traverse and restore.
 * @param	instanceData	A map of prefab IDs to instance data, as output by RecordInstanceData() method.
 *
 * @note	Does not recurse into child prefab instances.
 */
static void RestoreInstanceData(const HSceneObject& sceneObject, const UnorderedMap<UUID, PrefabInstanceData>& instanceData)
{
	SPtr<GameObjectCollection> gameObjectCollection = sceneObject->GetOwnerCollection().lock();
	if(!B3D_ENSURE(gameObjectCollection))
		return;

	sceneObject->IterateHierarchy(
		[&instanceData, &gameObjectCollection](const HSceneObject& sceneObject)
		{
			const UUID& prefabObjectId = sceneObject->GetPrefabObjectId();
			if(!prefabObjectId.Empty())
			{
				if(auto found = instanceData.find(prefabObjectId); found != instanceData.end())
				{
					sceneObject->SetId(found->second.Id); // ID must be set before calling ReplaceGameObjectInstance 

					HSceneObject sceneObjectMutableHandle = sceneObject;
					gameObjectCollection->ReplaceGameObjectInstance(sceneObjectMutableHandle, found->second.InstanceData);
				}
			}

			return true;
		},
		[&instanceData, &gameObjectCollection](const HComponent& component)
		{
			const UUID& prefabObjectId = component->GetPrefabObjectId();
			if(!prefabObjectId.Empty())
			{
				if(auto found = instanceData.find(prefabObjectId); found != instanceData.end())
				{
					component->SetId(found->second.Id); // ID must be set before calling ReplaceGameObjectInstance 

					HComponent componentMutableHandle = component;
					gameObjectCollection->ReplaceGameObjectInstance(componentMutableHandle, found->second.InstanceData);
				}
			}
		});
}

/** Contains a reference to a scene object representing a prefab instance root, and a prefab resource associated with that root. */
struct PrefabInstanceRoot
{
	PrefabInstanceRoot(const HPrefab& parentPrefab, const HSceneObject& sceneObjectInParentPrefab, const HPrefab& prefabToUpdateFrom)
		: ParentPrefab(parentPrefab), SceneObjectInParentPrefab(sceneObjectInParentPrefab), PrefabToUpdateFrom(prefabToUpdateFrom)
	{ }

	HPrefab ParentPrefab;
	HSceneObject SceneObjectInParentPrefab;
	HPrefab PrefabToUpdateFrom;
};

struct ObjectInPrefab
{
	ObjectInPrefab(HPrefab prefab, HGameObject instanceInPrefab)
		: Prefab(prefab), InstanceInPrefab(instanceInPrefab)
	{ }

	HPrefab Prefab;
	HGameObject InstanceInPrefab;
};

/** Looks up a counterpart of the provided game object in the specified prefab. Under the hood loads the prefab as required. */
static Optional<ObjectInPrefab> FindInstanceInPrefab(const GameObjectHandleBase& gameObject, const UUID& prefabResourceId)
{
	const HPrefab prefab = B3DStaticResourceCast<Prefab>(GetResources().LoadFromUuid(prefabResourceId, false, ResourceLoadFlag::None));

	if(!prefab.IsLoaded())
	{
		B3D_LOG(Error, Prefab, "Unable to find instance in prefab. Prefab {0} cannot be loaded.", prefabResourceId);
		return {};
	}

	const SPtr<GameObjectCollection>& gameObjectCollection = prefab->GetGameObjectCollection();
	if(!B3D_ENSURE(gameObjectCollection))
		return {};

	HGameObject instanceInPrefab;
	if(!gameObjectCollection->TryGetObject(gameObject->GetPrefabObjectId(), instanceInPrefab))
		return {};

	return ObjectInPrefab(prefab, instanceInPrefab);
}

/** Looks up a counterpart of the provided scene object in the prefab it is an instance of. Under the hood loads the prefab as required. */
static Optional<ObjectInPrefab> FindInstanceInPrefab(const HSceneObject& sceneObject)
{
	if(!sceneObject->IsPrefabInstance())
		return {};

	return FindInstanceInPrefab(sceneObject, sceneObject->GetPrefabResourceId());
}

/** Looks up a counterpart of the provided component in the prefab it is an instance of. Under the hood loads the prefab as required. */
static Optional<ObjectInPrefab> FindInstanceInPrefab(const HComponent& component)
{
	HSceneObject sceneObject = component->SceneObject();
	if(!sceneObject->IsPrefabInstance())
		return {};

	return FindInstanceInPrefab(component, sceneObject->GetPrefabResourceId());
}

HPrefab PrefabCache::FindOrLoadPrefab(const UUID& prefabId)
{
	if(auto found = mPrefabs.find(prefabId); found != mPrefabs.end())
		return found->second;

	HPrefab prefab = B3DStaticResourceCast<Prefab>(GetResources().LoadFromUuid(prefabId, false, ResourceLoadFlag::None));

	if(!prefab.IsLoaded(false))
	{
		B3D_LOG(Error, Scene, "Prefab with ID: {0} cannot be loaded.", prefabId);
		return nullptr;
	}

	mPrefabs.insert(std::make_pair(prefab->GetId(), prefab));
	return prefab;
}

void PrefabCache::AddToCache(const HPrefab& prefab)
{
	mPrefabs.insert(std::make_pair(prefab.GetId(), prefab));
}

bool PrefabCache::ExistsInCache(const UUID& prefabId)
{
	return mPrefabs.find(prefabId) != mPrefabs.end();
}

void PrefabUtility::RevertToPrefab(const HSceneObject& sceneObject)
{
	if(!B3D_ENSURE(sceneObject.IsValid()))
		return;

	HSceneObject prefabInstanceRoot = sceneObject->GetPrefabInstanceRoot();
	if(!prefabInstanceRoot.IsValid())
		return;

	const UUID& prefabResourceId = sceneObject->GetPrefabResourceId();
	if(!B3D_ENSURE(!prefabResourceId.Empty()))
		return;

	HPrefab linkedPrefab = B3DStaticResourceCast<Prefab>(GetResources().LoadFromUuid(prefabResourceId, false, ResourceLoadFlag::None));
	if(!linkedPrefab.IsLoaded(false))
	{
		B3D_LOG(Warning, Prefab, "Cannot revert scene object '{0}' to prefab. Failed to load prefab with ID: '{1}'.", sceneObject.GetId(), prefabResourceId);
		return;
	}

	// Save IDs, destroy original, create new, restore IDs
	UnorderedMap<UUID, PrefabInstanceData> instanceData;
	RecordInstanceData(sceneObject, instanceData);

	HSceneObject parent = sceneObject->GetParent();

	// This will destroy the object but keep it in the parent's child list
	HSceneObject currentSceneObject = sceneObject;
	sceneObject->DestroyInternal(currentSceneObject, true);

	HSceneObject newInstance = linkedPrefab->Instantiate(nullptr, false);

	// Remove default parent, and replace with original one
	newInstance->mParent->RemoveChild(newInstance);
	newInstance->mParent = parent;

	RestoreInstanceData(newInstance, instanceData);
}

HSceneObject PrefabUtility::UpdateInstanceFromPrefab(const HSceneObject& instance, const Prefab& prefab)
{
	if(!B3D_ENSURE(instance.IsValid()))
		return HSceneObject();

	if(!instance->IsPrefabInstanceRoot())
	{
		B3D_LOG(Warning, Scene, "Cannot update scene object from prefab. Provided scene object '{0}' ({1}) is not a prefab instance root.", instance->GetName(), instance.GetId());
		return HSceneObject();
	}

	if(instance->GetPrefabResourceId() != prefab.GetId())
	{
		B3D_LOG(Warning, Scene, "Cannot update scene object from prefab. Provided scene object '{0}' ({1}) is referencing prefab '{2}', but the provided prefab is '{3}'.", instance->GetName(), instance.GetId(), instance->GetPrefabResourceId(), prefab.GetId());
		return HSceneObject();
	}

	if(instance->GetPrefabVersion() == prefab.GetPrefabVersion())
		return HSceneObject();

	// Save IDs, destroy original, create new, restore IDs
	UnorderedMap<UUID, PrefabInstanceData> instanceData;
	RecordInstanceData(instance, instanceData);

	HSceneObject parent = instance->GetParent();
	SPtr<SceneObjectHierarchyDelta> prefabDelta = instance->GetPrefabDelta();
	Transform transform = instance->GetLocalTransform();

	const SPtr<GameObjectCollection> gameObjectCollection = instance->GetOwnerCollection().lock();

	instance->Destroy(true);
	HSceneObject newInstance = prefab.Clone(gameObjectCollection);
	AssignPrefabInstanceIds(newInstance, prefab.GetRoot(), prefab.GetId());

	// When restoring instance IDs it is important to make all the new handles point to the old GameObjectInstanceData.
	// This is because old handles will have different GameObjectHandleData and we have no easy way of accessing it to
	// change to which GameObjectInstanceData it points. But the GameObjectCollection ensures that all handles deserialized
	// at once (i.e. during the Clone() call above) will share GameObjectHandleData so we can simply replace
	// to what they point to, affecting all of the handles to that object. (In another words, we can modify the
	// new handles at this point, but old ones must keep referencing what they already were.)
	RestoreInstanceData(newInstance, instanceData);

	newInstance->SetParent(parent, false);

	//if(prefabDelta != nullptr)
	//	prefabDelta->Apply(newInstance, SceneObjectHierarchyDeltaFlag::PrefabDelta);

	newInstance->SetLocalTransform(transform);
	newInstance->mPrefabDelta = prefabDelta;

	return newInstance;
}

/**
 * Iterates the provided hierarchy and for each object visits the prefab resource as referenced by the object. If the visited object
 * itself contains a prefab link, its prefab resource is visited as well. Returns the game object ID within the last prefab that
 * is referencing the object, excluding the prefabs that own the object (cases where object links to its own prefab).
 *
 * e.g. if we have this hierarchy:
 *   ***Scene***
 *   Root
 *     PFB1 Instance [Game Object ID = OBI1, Prefab Object ID = OB11, Prefab Resource ID = PFB1]
 *       PFB2 Instance [Game Object ID = OBI2, Prefab Object ID = OB12, Prefab Resource ID = PFB1]
 *         Optional object [Game Object ID = OBI3, Prefab Object ID = OB13, Prefab Resource ID = PFB1]
 *
 *   ***PFB1***
 *   PFB1 Instance [Game Object ID = OB11, Prefab Object ID = OB11, Prefab Resource ID = PFB1]
 *     PFB2 Instance [Game Object ID = OB12, Prefab Object ID = OB21, Prefab Resource ID = PFB2]
 *       Optional object [Game Object ID = OB13, Prefab Object ID = OB13, Prefab Resource ID = PFB2]
 *
 *   ***PFB2***
 *   PFB2 Instance [Game Object ID = OB21, Prefab Object ID = OB21, Prefab Resource ID = PFB2]
 *
 * For Scene/Optional object, this method would return { OBI3 -> OB13 }. As PFB1 is the last object
 * that links to Optional object.
 *
 * Another example with a nested prefab:
 *   ***Scene***
 *   Root
 *     PFB1 Instance [Game Object ID = OBI1, Prefab Object ID = OB11, Prefab Resource ID = PFB1]
 *       PFB2 Instance [Game Object ID = OBI2, Prefab Object ID = OB12, Prefab Resource ID = PFB1]
 *         PFB3 Instance [Game Object ID = OBI3, Prefab Object ID = OB31, Prefab Resource ID = PFB2]
 *
 *   ***PFB1***
 *   PFB1 Instance [Game Object ID = OB11, Prefab Object ID = OB11, Prefab Resource ID = PFB1]
 *     PFB2 Instance [Game Object ID = OB12, Prefab Object ID = OB21, Prefab Resource ID = PFB2]
 *
 *   ***PFB2***
 *   PFB2 Instance [Game Object ID = OB21, Prefab Object ID = OB21, Prefab Resource ID = PFB2]
 *
 *   ***PFB3***
 *   PFB3 Instance [Game Object ID = OB31, Prefab Object ID = OB31, Prefab Resource ID = PFB3]
 *
 * For Scene/PFB3 Instance this method would return { OBI3 -> OBI3 }. Even though PFB3 is the last prefab to reference the object,
 * it's a self reference. Instead we return the parent - which in this case is the scene itself as neither PFB1 or PFB2 reference
 * the object. This ensures that if we ever update PFB2 or PFB3 with the object, the prefab object ID of OBI3 is the one that
 * gets remapped.
 */
static UnorderedMap<UUID, UUID> GenerateInstanceIdToPrefabThatNeedsRemappingInstanceId(const HSceneObject& root)
{
	// First iterate parents of @p sceneObjectToUpdateWith, and find the root-most prefab instance
	UUID rootInstancePrefabId;
	{
		// Start with the parent, because we don't care if sceneObjectToUpdateWith is an instance itself, as that doesn't require special handling
		HSceneObject currentObject = root->GetParent();
		while(currentObject.IsValid())
		{
			if(!currentObject->IsPrefabInstance())
				break;

			rootInstancePrefabId = currentObject->GetPrefabResourceId();
			currentObject = currentObject->GetParent();
		}
	}

	// For each object in the provided instance hierarchy, determine the bottom-most prefab it has been defined in, relative to the root prefab instance
	UnorderedMap<UUID, UUID> instanceIdToBottomMostPrefab;
	root->IterateHierarchy([&rootInstancePrefabId, &instanceIdToBottomMostPrefab](const HSceneObject& sceneObject) {
		// Map scene object
		{
			HPrefab bottomMostPrefab = nullptr;
			HSceneObject sceneObjectInBottomMostPrefab = sceneObject;

			if(!rootInstancePrefabId.Empty() && sceneObject->GetPrefabResourceId() == rootInstancePrefabId) // If an object is not part of the root instance, we don't need special handling
			{
				while(Optional<ObjectInPrefab> found = FindInstanceInPrefab(sceneObjectInBottomMostPrefab))
				{
					if(!found.has_value())
						break;

					HSceneObject instanceInPrefab = B3DStaticGameObjectCast<SceneObject>(found->InstanceInPrefab);

					// Reached the bottom-most prefab, as the instance points to itself
					if(found->Prefab->GetId() == instanceInPrefab->GetPrefabResourceId())
						break;

					sceneObjectInBottomMostPrefab = instanceInPrefab;
					bottomMostPrefab = found->Prefab;
				}
			}

			if(bottomMostPrefab != nullptr)
				instanceIdToBottomMostPrefab[sceneObject.GetId()] = sceneObjectInBottomMostPrefab->GetId();
			else
				instanceIdToBottomMostPrefab[sceneObject.GetId()] = sceneObject.GetId();
		}

		// Map components
		for(const auto& component : sceneObject->GetComponents())
		{
			HPrefab bottomMostPrefab = nullptr;
			HComponent componentInBottomMostPrefab = component;

			if(!rootInstancePrefabId.Empty() && sceneObject->GetPrefabResourceId() == rootInstancePrefabId) // If an object is not part of the root instance, we don't need special handling
			{
				while(Optional<ObjectInPrefab> found = FindInstanceInPrefab(componentInBottomMostPrefab))
				{
					if(!found.has_value())
						break;

					HComponent instanceInPrefab = B3DStaticGameObjectCast<Component>(found->InstanceInPrefab);

					// Reached the bottom-most prefab, as the instance points to itself
					if(found->Prefab->GetId() == instanceInPrefab->SceneObject()->GetPrefabResourceId())
						break;

					componentInBottomMostPrefab = instanceInPrefab;
					bottomMostPrefab = found->Prefab;
				}
			}

			if(bottomMostPrefab != nullptr)
				instanceIdToBottomMostPrefab[component.GetId()] = componentInBottomMostPrefab->GetId();
			else
				instanceIdToBottomMostPrefab[component.GetId()] = component.GetId();
		}

		return true;
	}, nullptr, true);

	return instanceIdToBottomMostPrefab;
}

/**
 * Iterates the hierarchy in @p root, and if a scene object or component is found in @p remappingTable, assigns it the prefab object ID
 * from the remapping table, and provided @p prefabId as the prefab resource ID.
 */
static void RemapSceneObjectHierarchyIds(const HSceneObject& root, const UnorderedMap<UUID, UUID>& remappingTable, const UUID& prefabId)
{
	root->IterateHierarchy(
		[&remappingTable, prefabId](const HSceneObject& sceneObject)
		{
			if(sceneObject->HasFlag(SOF_DontSave))
				return false;

			if(auto found = remappingTable.find(sceneObject.GetId()); found != remappingTable.end())
			{
				sceneObject->SetPrefabObjectId(found->second);
				sceneObject->SetPrefabResourceId(prefabId);
			}

			return true; },
		[&remappingTable](const HComponent& component)
		{
			if(auto found = remappingTable.find(component.GetId()); found != remappingTable.end())
				component->SetPrefabObjectId(found->second);
		});
}

// -------------------------------------------------------------------
// At the highest level this method performs the following actions:
// 1. Updates the provided prefab
// 2. Ensures that parent prefab instances of @p sceneObjectToUpdateWith are updated
//   - This means iterating bottom up from the prefab we updated, to the root instance
// 3. Ensures that prefab object IDs in @p sceneObjectToUpdateWith and parent prefab instances are updated accordingly
// 4. Updates all loaded instances of the prefab we updated
//
// -------------------------------------------------------------------
// #2 and #3 requires further explanation. Suppose we have a scene such as this:
//   ***Scene***
//   Root
//     PFB1 Instance [Game Object ID = OBI1, Prefab Object ID = OB11, Prefab Resource ID = PFB1]
//       PFB2 Instance [Game Object ID = OBI2, Prefab Object ID = OB12, Prefab Resource ID = PFB1]
//
//   ***PFB1***
//   PFB1 Instance [Game Object ID = OB11, Prefab Object ID = OB11, Prefab Resource ID = PFB1]
//     PFB2 Instance [Game Object ID = OB12, Prefab Object ID = OB21, Prefab Resource ID = PFB2]
//
//   ***PFB2***
//   PFB2 Instance [Game Object ID = OB21, Prefab Object ID = OB21, Prefab Resource ID = PFB2]
//
// We then add an new object as a child of PFB2 Instance, and update PFB1 with PFB1 Instance:
//   ***PFB1***
//   PFB1 Instance [Game Object ID = OB11, Prefab Object ID = OB11, Prefab Resource ID = PFB1]
//     PFB2 Instance [Game Object ID = OB12, Prefab Object ID = OB21, Prefab Resource ID = PFB2]
//       Optional object [Game Object ID = OB13, Prefab Object ID = OB13, Prefab Resource ID = PFB2]
//
//   ***Scene***
//   Root
//     PFB1 Instance [Game Object ID = OBI1, Prefab Object ID = OB11, Prefab Resource ID = PFB1]
//       PFB2 Instance [Game Object ID = OBI2, Prefab Object ID = OB12, Prefab Resource ID = PFB1]
//         Optional object [Game Object ID = OBI3, Prefab Object ID = OB13, Prefab Resource ID = PFB1]
//
// If we then update PFB2 with PFB2 Instance:
//   ***PFB2***
//   PFB2 Instance [Game Object ID = OB21, Prefab Object ID = OB21, Prefab Resource ID = PFB2]
//     Optional object [Game Object ID = OB22, Prefab Object ID = OB22, Prefab Resource ID = PFB2]
//
//   ***PFB1***
//   PFB1 Instance [Game Object ID = OB11, Prefab Object ID = OB11, Prefab Resource ID = PFB1]
//     PFB2 Instance #1 [Game Object ID = OB12, Prefab Object ID = OB21, Prefab Resource ID = PFB2]
//       Optional object [Game Object ID = OB13, Prefab Object ID = OB22, Prefab Resource ID = PFB2]
//
// Note that when updating PFB2 we also had to update PFB1, and change the Prefab Object ID of OB13 to point
// to OB22 in PFB2.
//
// -------------------------------------------------------------------
// #2 and #3 are also required when adding nested prefabs. Suppose we have a scene such as this:
//   ***Scene***
//   Root
//     PFB1 Instance [Game Object ID = OBI1, Prefab Object ID = OB11, Prefab Resource ID = PFB1]
//       PFB2 Instance [Game Object ID = OBI2, Prefab Object ID = OB12, Prefab Resource ID = PFB1]
//
//   ***PFB1***
//   PFB1 Instance [Game Object ID = OB11, Prefab Object ID = OB11, Prefab Resource ID = PFB1]
//     PFB2 Instance [Game Object ID = OB12, Prefab Object ID = OB21, Prefab Resource ID = PFB2]
//
//   ***PFB2***
//   PFB2 Instance [Game Object ID = OB21, Prefab Object ID = OB21, Prefab Resource ID = PFB2]
//
//   ***PFB3***
//   PFB3 Instance [Game Object ID = OB31, Prefab Object ID = OB31, Prefab Resource ID = PFB3]
//
// We then add an instance of PFB3 as a child to PFB2 Instance
//   ***Scene***
//   Root
//     PFB1 Instance [Game Object ID = OBI1, Prefab Object ID = OB11, Prefab Resource ID = PFB1]
//       PFB2 Instance [Game Object ID = OBI2, Prefab Object ID = OB12, Prefab Resource ID = PFB1]
//         PFB3 Instance [Game Object ID = OBI3, Prefab Object ID = OB31, Prefab Resource ID = PFB2]
//
// And the proceed to update PFB2 with PFB2 Instance:
//   ***PFB2***
//   PFB2 Instance [Game Object ID = OB21, Prefab Object ID = OB21, Prefab Resource ID = PFB2]
//     PFB3 Instance [Game Object ID = OB22, Prefab Object ID = OB31, Prefab Resource ID = PFB3]
//
//   ***PFB1***
//   PFB1 Instance [Game Object ID = OB11, Prefab Object ID = OB11, Prefab Resource ID = PFB1]
//     PFB2 Instance [Game Object ID = OB12, Prefab Object ID = OB21, Prefab Resource ID = PFB2]
//       PFB3 Instance [Game Object ID = OB13, Prefab Object ID = OB22, Prefab Resource ID = PFB2]
//
//   ***Scene***
//   Root
//     PFB1 Instance [Game Object ID = OBI1, Prefab Object ID = OB11, Prefab Resource ID = PFB1]
//       PFB2 Instance [Game Object ID = OBI2, Prefab Object ID = OB12, Prefab Resource ID = PFB1]
//         PFB3 Instance [Game Object ID = OBI3, Prefab Object ID = OB13, Prefab Resource ID = PFB1]
//
// In this case after updating PFB2, we need to update both PFB1 and Scene.
//  - PFB1 will be updated with the new version of PFB2, adding PFB3 Instance [Game Object ID = OB13], linking to PFB2. 
//  - PFB3 Instance [Game Object ID = OBI3] in the Scene must be updated so it now links to PFB1.
//   - This is because PFB2 update triggered the update to PFB1, linking OB13 -> OB22, and our scene must now link OBI3 -> OB13
//
// This means the code below must keep careful track of the prefab object IDs as we're updating the prefab hierarchy chain.
void PrefabUtility::UpdatePrefab(const HPrefab& prefabToUpdate, const HSceneObject& sceneObjectToUpdateWith)
{
	if(!B3D_ENSURE(prefabToUpdate.IsLoaded(false)))
		return;

	// As described above, the code must keep careful track of how to update prefab object IDs in @p sceneObjectToUpdateWith hierarchy. Note this also
	// may include parents of @p sceneObjectToUpdateWith, as changes need to be propagated to parent prefab instances.

	// Find the object ID in the last (bottom-most, most-nested) prefab that an object is linked to (ignoring links to itself). If the object is not linked
	// to any prefab, or is just linked to itself, this will be its own ID. This ID represents the object whose prefab object ID we will need to update.
	UnorderedMap<UUID, UUID> instanceIdToPrefabThatNeedsRemappingInstanceId = GenerateInstanceIdToPrefabThatNeedsRemappingInstanceId(sceneObjectToUpdateWith);

	FrameScope frameScope;

	PrefabCache prefabCache;
	prefabCache.AddToCache(prefabToUpdate);

	// Record any parent prefab instances. As described above, we need to update these recursively bottom to top, in order to maintain prefab object ID links.
	FrameVector<PrefabInstanceRoot> prefabInstanceParents;
	if(sceneObjectToUpdateWith->IsPrefabInstance())
	{
		HSceneObject currentSceneObject = sceneObjectToUpdateWith;
		HPrefab parentPrefab = nullptr;
		while(Optional<ObjectInPrefab> found = FindInstanceInPrefab(currentSceneObject))
		{
			if(!found.has_value())
				break;

			currentSceneObject = B3DStaticGameObjectCast<SceneObject>(found->InstanceInPrefab);
			parentPrefab = found->Prefab;

			// Current prefab becomes the prefab to update from for the parent
			if(!prefabInstanceParents.empty())
				prefabInstanceParents.back().PrefabToUpdateFrom = parentPrefab;

			if(parentPrefab == prefabToUpdate)
				break;

			prefabInstanceParents.push_back(PrefabInstanceRoot(found->Prefab, currentSceneObject, nullptr));
		}
	}

	// Update the prefab, and retrieve the map containing the prefab object IDs that we assigned. 
	UnorderedMap<UUID, UUID> instanceIdToPrefabToUpdateInstanceId = prefabToUpdate->ReplaceInternalHierarchy(sceneObjectToUpdateWith, false);

	// Combine @p instanceIdToFirstRelevantPrefab with @p instanceIdToPrefabToUpdateInstanceId to give us a map that tells us which objects need
	// their prefab object ID remapped (value in @p instanceIdToPrefabThatNeedsRemappingInstanceId), and what is prefab object ID to remap to (value in @p instanceIdToPrefabToUpdateInstanceId).
	UnorderedMap<UUID, UUID> prefabObjectIdRemapping;
	sceneObjectToUpdateWith->IterateHierarchy([&instanceIdToPrefabToUpdateInstanceId, &prefabObjectIdRemapping, &instanceIdToPrefabThatNeedsRemappingInstanceId](const HSceneObject& child) mutable -> bool
	{
		UUID idInFirstRelevantPrefab;
		if(auto found = instanceIdToPrefabThatNeedsRemappingInstanceId.find(child.GetId()); B3D_ENSURE(found != instanceIdToPrefabThatNeedsRemappingInstanceId.end()))
			idInFirstRelevantPrefab = found->second;

		if(auto found = instanceIdToPrefabToUpdateInstanceId.find(child.GetId()); found != instanceIdToPrefabToUpdateInstanceId.end())
		{
			auto result = prefabObjectIdRemapping.insert(std::make_pair(idInFirstRelevantPrefab, found->second));
			B3D_ENSURE(result.second);
		}

		return true;
	},
	   [&instanceIdToPrefabToUpdateInstanceId, &prefabObjectIdRemapping, &instanceIdToPrefabThatNeedsRemappingInstanceId](const HComponent& component) mutable -> void
	   {
			UUID lookupId;
		    if(auto found = instanceIdToPrefabThatNeedsRemappingInstanceId.find(component.GetId()); B3D_ENSURE(found != instanceIdToPrefabThatNeedsRemappingInstanceId.end()))
				lookupId = found->second;

	   		if(auto found = instanceIdToPrefabToUpdateInstanceId.find(component.GetId()); found != instanceIdToPrefabToUpdateInstanceId.end())
	   		{
	   			auto result = prefabObjectIdRemapping.insert(std::make_pair(lookupId, found->second));
				B3D_ENSURE(result.second);
	   		}
	   },
	   true);

	// In order to keep prefab object IDs valid, we need to update all parent prefab instances of the object we just updated, starting from most nested (bottom-most) to root (top-most).
	for(auto it = prefabInstanceParents.rbegin(); it != prefabInstanceParents.rend(); ++it)
	{
		const PrefabInstanceRoot& prefabInstanceRoot = *it;

		// If this prefab contains any object in the @p prefabObjectIdRemapping, this means this is the last prefab that has a link to this object (excluding
		// owner prefab of the object, if any). We need to update its prefab object ID, as it could have changed during ReplaceInternalHierarchy call above,
		// or during previous iterations of this loop when UpdateInstanceFromPrefab was called.
		// 
		// We need to perform the prefab object ID remapping before the UpdateInstanceFromPrefab call, otherwise game object instance data will not be restored
		// correctly as it relies on matching prefab object IDs.
		RemapSceneObjectHierarchyIds(prefabInstanceRoot.SceneObjectInParentPrefab, prefabObjectIdRemapping, prefabInstanceRoot.PrefabToUpdateFrom.GetId());

		HSceneObject newHierarchy = UpdateInstanceFromPrefab(prefabInstanceRoot.SceneObjectInParentPrefab, *prefabInstanceRoot.PrefabToUpdateFrom);
		if(!B3D_ENSURE(newHierarchy != nullptr))
			continue;

		// UpdateInstanceFromPrefab could have resulted in new prefab instance objects getting created, with their own unique IDs. We need to make sure
		// that parent prefab instances wanting to linking to those objects (as stored in @p prefabObjectIdRemapping) now link to the new prefab instance.
		// This action is performed for each iteration as we go up the prefab instance hierarchy, ensuring the parent prefab instance links to the
		// last (top-most, least-nested) instance of the object.
		const UnorderedMap<UUID, UUID> prefabObjectToInstanceIdMap = GetPrefabToInstanceIdMap(prefabInstanceRoot.SceneObjectInParentPrefab, true);
		for(auto& entry : prefabObjectIdRemapping)
		{
			if(auto found = prefabObjectToInstanceIdMap.find(entry.second); found != prefabObjectToInstanceIdMap.end())
				entry.second = found->second;
		}

		// Note: We purposefully don't tick the prefab version here, and do another update that takes care of that below. This is because
		// the prefab could contain multiple instances of our update prefab, and we're only updating one that's related to @p root
	}

	// Same as we do above for prefab instance roots, but ensures that any objects in the scene itself also have prefab object IDs assigned
	const UUID rootPrefabId = prefabInstanceParents.empty() ? prefabToUpdate.GetId() : prefabInstanceParents.back().ParentPrefab->GetId();
	RemapSceneObjectHierarchyIds(sceneObjectToUpdateWith, prefabObjectIdRemapping, rootPrefabId);

	// Ensure all instances of @p prefabToUpdate are up to date in all live scene instances
	const UnorderedMap<SceneInstance*, WeakSPtr<SceneInstance>>& sceneInstances = GetSceneManager().GetAllSceneInstances();
	for(const auto& pair : sceneInstances)
	{
		SPtr<SceneInstance> scene = pair.second.lock();
		if(B3D_ENSURE(scene != nullptr))
		{
			FrameVector<UUID> parentPrefabChain;
			UpdateNestedPrefabInstancesRecursive(scene->GetRoot(), prefabCache);
		}
	}

	// Ensure all instances of @p prefabToUpdate are up to date in all live prefabs
	UnorderedSet<Prefab*> livePrefabs = PrefabManager::Instance().GetLivePrefabs();
	for(const auto& entry : livePrefabs)
	{
		const UUID& prefabResourceId = entry->GetId();
		if(prefabCache.ExistsInCache(prefabResourceId))
			continue;

		if(UpdateNestedPrefabInstancesRecursive(entry->GetRoot(), prefabCache))
		{
			entry->TickPrefabVersion();
			entry->RecordNestedPrefabInstanceDeltas();
		}
	}
}

bool PrefabUtility::UpdateNestedPrefabInstances(const HSceneObject& sceneObject)
{
	FrameScope frameScope;

	PrefabCache prefabCache;
	return UpdateNestedPrefabInstancesRecursive(sceneObject, prefabCache);
}

bool PrefabUtility::UpdateNestedPrefabInstancesRecursive(const HSceneObject& root, PrefabCache& inOutPrefabCache)
{
	if(!root.IsValid())
		return false;

	// Find all the prefab instances we need to update. This includes the hierarchy of @p root, but also of any prefab that's referenced by root,
	// or nested prefabs of those prefabs, recursively
	FrameVector<PrefabInstanceRoot> prefabInstanceRootsToUpdate;
	bool foundCircularDependency = false;
	auto fnVisitPrefabInstances = [&inOutPrefabCache, &prefabInstanceRootsToUpdate, &foundCircularDependency](const HSceneObject& instanceRoot, const HPrefab& parentPrefab, FrameVector<UUID>& inOutParentPrefabChain, auto& fnVisitPrefabInstances) -> void {

		instanceRoot->IterateHierarchy([&inOutPrefabCache, &prefabInstanceRootsToUpdate, &foundCircularDependency, &parentPrefab, &inOutParentPrefabChain, &fnVisitPrefabInstances](const HSceneObject& child) mutable -> bool
		{
			if(!child->IsPrefabInstanceRoot())
				return true;

			const UUID& nestedPrefabId = child->GetPrefabResourceId();
			if(auto found = std::find(inOutParentPrefabChain.begin(), inOutParentPrefabChain.end(), nestedPrefabId); found != inOutParentPrefabChain.end())
			{
				B3D_LOG(Error, Scene, "Failed to update instance from prefab. Detected circular dependency for prefab with ID:{0}.)", nestedPrefabId);
				foundCircularDependency = true;
				return false;
			}

			const HPrefab nestedPrefab = inOutPrefabCache.FindOrLoadPrefab(nestedPrefabId);
			if(nestedPrefab.IsLoaded(false))
			{
				FrameVector<UUID> parentPrefabChainCopy = inOutParentPrefabChain;
				parentPrefabChainCopy.push_back(nestedPrefabId);

				HSceneObject nestedPrefabInternalsRoot = nestedPrefab->GetRoot();
				if(B3D_ENSURE(nestedPrefabInternalsRoot.IsValid()))
					fnVisitPrefabInstances(nestedPrefabInternalsRoot, nestedPrefab, parentPrefabChainCopy, fnVisitPrefabInstances);
			}

			prefabInstanceRootsToUpdate.emplace_back(parentPrefab, child, nestedPrefab);
			return true;

		},
		nullptr, false);
	};

	FrameVector<UUID> parentPrefabChain;
	fnVisitPrefabInstances(root, nullptr, parentPrefabChain, fnVisitPrefabInstances);

	if(foundCircularDependency)
		return false;

	if(prefabInstanceRootsToUpdate.empty())
		return false;

	bool isAnythingModified = false;
	FrameUnorderedMap<UUID, HPrefab> updatedPrefabs;
	for(const auto& entry : prefabInstanceRootsToUpdate)
	{
		HSceneObject objectToUpdate = entry.SceneObjectInParentPrefab;
		if(!B3D_ENSURE(objectToUpdate.IsValid()))
			continue;

		if(!entry.PrefabToUpdateFrom.IsLoaded(false))
			continue;

		if(UpdateInstanceFromPrefab(objectToUpdate, *entry.PrefabToUpdateFrom) != nullptr)
		{
			isAnythingModified = true;

			if(entry.ParentPrefab.IsLoaded(false))
			{
				entry.ParentPrefab->TickPrefabVersion(); // Technically we only need to tick this once, but this is easier
				updatedPrefabs.insert(std::make_pair(entry.ParentPrefab.GetId(), entry.ParentPrefab));
			}
		}
	}

	for(const auto& entry : updatedPrefabs)
		entry.second->RecordNestedPrefabInstanceDeltas();

	return isAnythingModified;
}

void PrefabUtility::AssignPrefabResourceId(const HSceneObject& sceneObject, const UUID& newPrefabResourceId)
{
	const UUID originalResourceId = sceneObject->GetPrefabResourceId();

	sceneObject->IterateHierarchy(
		[&originalResourceId, &newPrefabResourceId](const HSceneObject& sceneObject)
		{
			if(sceneObject->HasFlag(SOF_DontSave))
				return false;

			const UUID& currentResourceId = sceneObject->GetPrefabResourceId();

			// Assign IDs while the resource ID matches, or if the object is not associated with a prefab (i.e. has an empty ID)
			if(!currentResourceId.Empty() && (currentResourceId != originalResourceId))
				return false;

			sceneObject->SetPrefabResourceId(newPrefabResourceId);
			sceneObject->SetPrefabObjectId(sceneObject->GetId());

			return true;
		},
		[](const HComponent& component)
		{
			component->SetPrefabObjectId(component->GetId());
		});
}


void PrefabUtility::AssignPrefabInstanceIds(const HSceneObject& instanceRoot, const HSceneObject& prefabRoot, const UUID& prefabResourceId)
{
	struct MatchingSceneObjects
	{
		MatchingSceneObjects(HSceneObject instance, HSceneObject prefab)
			: InstanceSceneObject(std::move(instance)), PrefabSceneObject(std::move(prefab))
		{ }

		HSceneObject InstanceSceneObject;
		HSceneObject PrefabSceneObject;
	};

	FrameScope frameScope;
	FrameStack<MatchingSceneObjects> todo;
	todo.emplace(instanceRoot, prefabRoot);

	while(!todo.empty())
	{
		MatchingSceneObjects currentMatchingSceneObjects = todo.top();
		todo.pop();

		currentMatchingSceneObjects.InstanceSceneObject->SetPrefabResourceId(prefabResourceId);
		currentMatchingSceneObjects.InstanceSceneObject->SetPrefabObjectId(currentMatchingSceneObjects.PrefabSceneObject.GetId());

		const Vector<HComponent>& instanceComponents = currentMatchingSceneObjects.InstanceSceneObject->GetComponents();
		const Vector<HComponent>& prefabComponents = currentMatchingSceneObjects.PrefabSceneObject->GetComponents();

		const u32 componentCount = (u32)instanceComponents.size();
		if(!B3D_ENSURE(componentCount == prefabComponents.size()))
			return;

		for(u32 componentIndex = 0; componentIndex < componentCount; ++componentIndex)
		{
			const HComponent& instanceComponent = instanceComponents[componentIndex];
			const HComponent& prefabComponent = prefabComponents[componentIndex];

			instanceComponent->SetPrefabObjectId(prefabComponent.GetId());
		}

		const u32 childCount = currentMatchingSceneObjects.InstanceSceneObject->GetChildCount();
		if(!B3D_ENSURE(childCount == currentMatchingSceneObjects.PrefabSceneObject->GetChildCount()))
			return;

		for(u32 childIndex = 0; childIndex < childCount; ++childIndex)
		{
			const HSceneObject& instanceChild = currentMatchingSceneObjects.InstanceSceneObject->GetChild(childIndex);
			const HSceneObject& prefabChild = currentMatchingSceneObjects.PrefabSceneObject->GetChild(childIndex);

			todo.emplace(instanceChild, prefabChild);
		}
	}
}

void PrefabUtility::ClearPrefabIds(const HSceneObject& sceneObject)
{
	const UUID originalResourceId = sceneObject->GetPrefabResourceId();

	sceneObject->SetPrefabObjectId(UUID::kEmpty);
	sceneObject->SetPrefabResourceId(UUID::kEmpty);

	sceneObject->IterateHierarchy(
		[&originalResourceId](const HSceneObject& sceneObject)
		{
			if(sceneObject->GetPrefabResourceId() != originalResourceId)
				return false;

			sceneObject->SetPrefabObjectId(UUID::kEmpty);
			sceneObject->SetPrefabResourceId(UUID::kEmpty);
			return true;
		},
		[](const HComponent& component)
		{
			component->SetPrefabObjectId(UUID::kEmpty);
		});
}

UnorderedMap<UUID, PrefabLinkInformation> PrefabUtility::GetInstanceToPrefabLinkInformationMap(const HSceneObject& sceneObject, bool visitChildPrefabs)
{
	UnorderedMap<UUID, PrefabLinkInformation> output;
	if(!B3D_ENSURE(sceneObject.IsValid()))
		return output;

	const UUID rootPrefabResourceId = sceneObject->GetPrefabResourceId();

	sceneObject->IterateHierarchy([&output, &rootPrefabResourceId, visitChildPrefabs](const HSceneObject& sceneObject)
	{
		if(sceneObject->GetPrefabResourceId() != rootPrefabResourceId && !visitChildPrefabs)
			return false;

		if(!sceneObject->IsPrefabInstance())
			return true;

		const UUID& prefabObjectId = sceneObject->GetPrefabObjectId();
		const UUID& prefabResourceId = sceneObject->GetPrefabResourceId();

		output[sceneObject.GetId()] = PrefabLinkInformation(prefabObjectId, prefabResourceId);
		return true;
	},
	[&output](const HComponent& component) {
		if(!component->IsPrefabInstance())
			return;

		const UUID& prefabObjectId = component->GetPrefabObjectId();
		const UUID& prefabResourceId = component->SceneObject()->GetPrefabResourceId();

		output[component.GetId()] = PrefabLinkInformation(prefabObjectId, prefabResourceId);

	});

	return output;
}

UnorderedMap<UUID, UUID> PrefabUtility::GetPrefabToInstanceIdMap(const HSceneObject& sceneObject, bool visitChildPrefabs)
{
	UnorderedMap<UUID, UUID> output;
	if(!B3D_ENSURE(sceneObject.IsValid()))
		return output;

	const UUID rootPrefabResourceId = sceneObject->GetPrefabResourceId();

	sceneObject->IterateHierarchy([&output, &rootPrefabResourceId, visitChildPrefabs](const HSceneObject& sceneObject)
	{
		if(sceneObject->GetPrefabResourceId() != rootPrefabResourceId && !visitChildPrefabs)
			return false;

		if(!sceneObject->IsPrefabInstance())
			return true;

		const UUID& prefabObjectId = sceneObject->GetPrefabObjectId();
		output[prefabObjectId] = sceneObject.GetId();

		return true;
	},
	[&output](const HComponent& component) {
		if(!component->IsPrefabInstance())
			return;

		const UUID& prefabObjectId = component->GetPrefabObjectId();
		output[prefabObjectId] = component.GetId();

	});

	return output;
}
