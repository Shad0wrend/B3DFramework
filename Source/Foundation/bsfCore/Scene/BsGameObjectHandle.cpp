//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsCorePrerequisites.h"
#include "Scene/BsGameObject.h"
#include "Scene/BsGameObjectHandle.h"
#include "Error/BsException.h"
#include "Scene/BsGameObject.h"
#include "Private/RTTI/BsGameObjectHandleRTTI.h"

namespace bs
{
	GameObjectHandleBase::GameObjectHandleBase(const SPtr<GameObject>& ptr)
	{
		mData = bs_shared_ptr_new<GameObjectHandleData>(ptr->mInstanceData);
	}

	bool GameObjectHandleBase::IsDestroyed(bool checkQueued) const
	{
		return mData->mPtr == nullptr || mData->mPtr->object == nullptr
			|| (checkQueued && mData->mPtr->object->GetIsDestroyedInternal());
	}

	void GameObjectHandleBase::SetHandleDataInternal(const SPtr<GameObject>& object)
	{
		mData->mPtr = object->mInstanceData;
	}

	void GameObjectHandleBase::ThrowIfDestroyed() const
	{
		if(IsDestroyed())
		{
			BS_EXCEPT(InternalErrorException, "Trying to access an object that has been destroyed.");
		}
	}

	RTTITypeBase* GameObjectHandleBase::GetRttiStatic()
	{
		return GameObjectHandleRTTI::Instance();
	}

	RTTITypeBase* GameObjectHandleBase::GetRtti() const
	{
		return GameObjectHandleBase::GetRttiStatic();
	}
}
