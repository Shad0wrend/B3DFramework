//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Scene/BsGameObject.h"
#include "Private/RTTI/BsGameObjectRTTI.h"
#include "Scene/BsGameObjectManager.h"

namespace bs
{
	void GameObject::Initialize(const SPtr<GameObject>& object, UINT64 instanceId)
	{
		mInstanceData = bs_shared_ptr_new<GameObjectInstanceData>();
		mInstanceData->Object = object;
		mInstanceData->MInstanceId = instanceId;
	}

	void GameObject::SetInstanceDataInternal(GameObjectInstanceDataPtr& other)
	{
		SPtr<GameObject> myPtr = mInstanceData->Object;
		UINT64 oldId = mInstanceData->MInstanceId;

		mInstanceData = other;
		mInstanceData->Object = myPtr;

		GameObjectManager::Instance().RemapId(oldId, mInstanceData->MInstanceId);
	}
	
	RTTITypeBase* GameObject::GetRttiStatic()
	{
		return GameObjectRTTI::Instance();
	}

	RTTITypeBase* GameObject::GetRtti() const
	{
		return GameObject::GetRttiStatic();
	}
}
