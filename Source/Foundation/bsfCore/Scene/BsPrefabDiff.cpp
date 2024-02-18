//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Scene/BsPrefabDiff.h"
#include "Private/RTTI/BsPrefabDiffRTTI.h"
#include "Scene/BsSceneObject.h"
#include "Serialization/BsBinarySerializer.h"
#include "Serialization/BsBinaryDiff.h"
#include "Scene/BsSceneManager.h"
#include "Utility/BsUtility.h"
#include "BsPrefab.h"

using namespace bs;

RTTITypeBase* PrefabComponentDiff::GetRttiStatic()
{
	return PrefabComponentDiffRTTI::Instance();
}

RTTITypeBase* PrefabComponentDiff::GetRtti() const
{
	return PrefabComponentDiff::GetRttiStatic();
}

RTTITypeBase* PrefabObjectDiff::GetRttiStatic()
{
	return PrefabObjectDiffRTTI::Instance();
}

RTTITypeBase* PrefabObjectDiff::GetRtti() const
{
	return PrefabObjectDiff::GetRttiStatic();
}

SPtr<PrefabDiff> PrefabDiff::Create(const HSceneObject& prefab, const HSceneObject& instance)
{
	if(prefab->GetPrefabResourceId() != instance->GetPrefabResourceId())
	{
		B3D_LOG(Warning, Prefab, "Cannot create a delta between objects not linked to the same prefab.");
		return nullptr;
	}

	SPtr<PrefabDiff> output = B3DMakeShared<PrefabDiff>();
	output->mRoot = GenerateDelta(prefab, instance);

	return output;
}

void PrefabDiff::Apply(const HSceneObject& object)
{
	if(mRoot == nullptr)
		return;

	CoreSerializationContext serzContext;
	serzContext.GoState = B3DMakeShared<GameObjectDeserializationState>(GODM_UseNewIds | GODM_RestoreExternal);
	serzContext.GoDeserializationActive = true;

	ApplyDiff(mRoot, object, &serzContext);

	serzContext.GoState->Resolve();
}

void PrefabDiff::ApplyDiff(const SPtr<PrefabObjectDiff>& diff, const HSceneObject& object, SerializationContext* context)
{
	if((diff->SoFlags & (u32)SceneObjectDiffFlags::Name) != 0)
		object->SetName(diff->Name);

	if((diff->SoFlags & (u32)SceneObjectDiffFlags::Position) != 0)
		object->SetPosition(diff->Position);

	if((diff->SoFlags & (u32)SceneObjectDiffFlags::Rotation) != 0)
		object->SetRotation(diff->Rotation);

	if((diff->SoFlags & (u32)SceneObjectDiffFlags::Scale) != 0)
		object->SetScale(diff->Scale);

	if((diff->SoFlags & (u32)SceneObjectDiffFlags::Active) != 0)
		object->SetActive(diff->IsActive);

	// Note: It is important to remove objects and components first, before adding them.
	//		 Some systems rely on the fact that applyDiff added components/objects are
	//       always at the end.
	const Vector<HComponent>& components = object->GetComponents();
	for(auto& removedId : diff->RemovedComponents)
	{
		for(auto component : components)
		{
			if(removedId == component->GetPrefabObjectId())
			{
				component->Destroy(true);
				break;
			}
		}
	}

	for(auto& removedId : diff->RemovedChildren)
	{
		u32 childCount = object->GetChildCount();
		for(u32 i = 0; i < childCount; i++)
		{
			HSceneObject child = object->GetChild(i);
			if(removedId == child->GetPrefabObjectId())
			{
				child->Destroy(true);
				break;
			}
		}
	}

	for(auto& addedComponentData : diff->AddedComponents)
	{
		SPtr<Component> component = std::static_pointer_cast<Component>(addedComponentData->Decode(context));

		object->AddAndInitializeComponent(component);
	}

	for(auto& addedChildData : diff->AddedChildren)
	{
		SPtr<SceneObject> sceneObject = std::static_pointer_cast<SceneObject>(addedChildData->Decode(context));
		sceneObject->SetParent(object);

		if(object->IsInstantiated())
			sceneObject->InstantiateInternal();
	}

	for(auto& componentDiff : diff->ComponentDeltas)
	{
		for(auto& component : components)
		{
			if(componentDiff->Id == component->GetPrefabObjectId())
			{
				IDiff& diffHandler = component->GetRtti()->GetDiffHandler();
				diffHandler.ApplyDiff(component.GetShared(), componentDiff->Data, context);
				break;
			}
		}
	}

	for(auto& childDiff : diff->ChildDeltas)
	{
		u32 childCount = object->GetChildCount();
		for(u32 i = 0; i < childCount; i++)
		{
			HSceneObject child = object->GetChild(i);
			if(childDiff->Id == child->GetPrefabObjectId())
			{
				ApplyDiff(childDiff, child, context);
				break;
			}
		}
	}
}

SPtr<PrefabObjectDiff> PrefabDiff::GenerateDelta(const HSceneObject& prefab, const HSceneObject& instance)
{
	SPtr<PrefabObjectDiff> output;

	if(prefab->GetName() != instance->GetName())
	{
		if(output == nullptr)
			output = B3DMakeShared<PrefabObjectDiff>();

		output->Name = instance->GetName();
		output->SoFlags |= (u32)SceneObjectDiffFlags::Name;
	}

	const Transform& prefabTfrm = prefab->GetLocalTransform();
	const Transform& instanceTfrm = instance->GetLocalTransform();
	if(prefabTfrm.GetPosition() != instanceTfrm.GetPosition())
	{
		if(output == nullptr)
			output = B3DMakeShared<PrefabObjectDiff>();

		output->Position = instanceTfrm.GetPosition();
		output->SoFlags |= (u32)SceneObjectDiffFlags::Position;
	}

	if(prefabTfrm.GetRotation() != instanceTfrm.GetRotation())
	{
		if(output == nullptr)
			output = B3DMakeShared<PrefabObjectDiff>();

		output->Rotation = instanceTfrm.GetRotation();
		output->SoFlags |= (u32)SceneObjectDiffFlags::Rotation;
	}

	if(prefabTfrm.GetScale() != instanceTfrm.GetScale())
	{
		if(output == nullptr)
			output = B3DMakeShared<PrefabObjectDiff>();

		output->Scale = instanceTfrm.GetScale();
		output->SoFlags |= (u32)SceneObjectDiffFlags::Scale;
	}

	if(prefab->GetActive() != instance->GetActive())
	{
		if(output == nullptr)
			output = B3DMakeShared<PrefabObjectDiff>();

		output->IsActive = instance->GetActive();
		output->SoFlags |= (u32)SceneObjectDiffFlags::Active;
	}

	u32 prefabChildCount = prefab->GetChildCount();
	u32 instanceChildCount = instance->GetChildCount();

	// Find modified and removed children
	for(u32 i = 0; i < prefabChildCount; i++)
	{
		HSceneObject prefabChild = prefab->GetChild(i);

		SPtr<PrefabObjectDiff> childDiff;
		bool foundMatching = false;
		for(u32 j = 0; j < instanceChildCount; j++)
		{
			HSceneObject instanceChild = instance->GetChild(j);

			if(prefabChild->GetId() == instanceChild->GetPrefabObjectId())
			{
				if(!instanceChild->IsPrefabInstanceRoot())
					childDiff = GenerateDelta(prefabChild, instanceChild);

				foundMatching = true;
				break;
			}
		}

		if(foundMatching)
		{
			if(childDiff != nullptr)
			{
				if(output == nullptr)
					output = B3DMakeShared<PrefabObjectDiff>();

				output->ChildDeltas.push_back(childDiff);
			}
		}
		else
		{
			if(output == nullptr)
				output = B3DMakeShared<PrefabObjectDiff>();

			output->RemovedChildren.push_back(prefabChild->GetId());
		}
	}

	// Find added children
	for(u32 i = 0; i < instanceChildCount; i++)
	{
		HSceneObject instanceChild = instance->GetChild(i);

		if(instanceChild->HasFlag(SOF_DontSave))
			continue;

		bool foundMatching = false;
		if(instanceChild->IsPrefabInstance())
		{
			for(u32 j = 0; j < prefabChildCount; j++)
			{
				HSceneObject prefabChild = prefab->GetChild(j);

				if(prefabChild->GetId() == instanceChild->GetPrefabObjectId())
				{
					foundMatching = true;
					break;
				}
			}
		}

		if(!foundMatching)
		{
			SPtr<SerializedObject> obj = SerializedObject::Create(*instanceChild);

			if(output == nullptr)
				output = B3DMakeShared<PrefabObjectDiff>();

			output->AddedChildren.push_back(obj);
		}
	}

	const Vector<HComponent>& prefabComponents = prefab->GetComponents();
	const Vector<HComponent>& instanceComponents = instance->GetComponents();

	u32 prefabComponentCount = (u32)prefabComponents.size();
	u32 instanceComponentCount = (u32)instanceComponents.size();

	// Find modified and removed components
	for(u32 i = 0; i < prefabComponentCount; i++)
	{
		HComponent prefabComponent = prefabComponents[i];

		SPtr<PrefabComponentDiff> childDiff;
		bool foundMatching = false;
		for(u32 j = 0; j < instanceComponentCount; j++)
		{
			HComponent instanceComponent = instanceComponents[j];

			if(prefabComponent->GetId() == instanceComponent->GetPrefabObjectId())
			{
				SPtr<SerializedObject> encodedPrefab = SerializedObject::Create(*prefabComponent);
				SPtr<SerializedObject> encodedInstance = SerializedObject::Create(*instanceComponent);

				IDiff& diffHandler = prefabComponent->GetRtti()->GetDiffHandler();
				SPtr<SerializedObject> diff = diffHandler.GenerateDiff(encodedPrefab, encodedInstance);

				if(diff != nullptr)
				{
					childDiff = B3DMakeShared<PrefabComponentDiff>();
					childDiff->Id = prefabComponent->GetId();
					childDiff->Data = diff;
				}

				foundMatching = true;
				break;
			}
		}

		if(foundMatching)
		{
			if(childDiff != nullptr)
			{
				if(output == nullptr)
					output = B3DMakeShared<PrefabObjectDiff>();

				output->ComponentDeltas.push_back(childDiff);
			}
		}
		else
		{
			if(output == nullptr)
				output = B3DMakeShared<PrefabObjectDiff>();

			output->RemovedComponents.push_back(prefabComponent->GetId());
		}
	}

	// Find added components
	for(u32 i = 0; i < instanceComponentCount; i++)
	{
		HComponent instanceComponent = instanceComponents[i];

		bool foundMatching = false;
		if(instanceComponent->IsPrefabInstance())
		{
			for(u32 j = 0; j < prefabComponentCount; j++)
			{
				HComponent prefabComponent = prefabComponents[j];

				if(prefabComponent->GetId() == instanceComponent->GetPrefabObjectId())
				{
					foundMatching = true;
					break;
				}
			}
		}

		if(!foundMatching)
		{
			SPtr<SerializedObject> obj = SerializedObject::Create(*instanceComponent);

			if(output == nullptr)
				output = B3DMakeShared<PrefabObjectDiff>();

			output->AddedComponents.push_back(obj);
		}
	}

	if(output != nullptr)
		output->Id = instance->GetPrefabObjectId();

	return output;
}

RTTITypeBase* PrefabDiff::GetRttiStatic()
{
	return PrefabDiffRTTI::Instance();
}

RTTITypeBase* PrefabDiff::GetRtti() const
{
	return PrefabDiff::GetRttiStatic();
}
