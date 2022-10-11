//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "Reflection/BsRTTIPlain.h"
#include "Components/BsCCharacterController.h"
#include "Private/RTTI/BsGameObjectRTTI.h"
#include "RTTI/BsMathRTTI.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Core
	 *  @{
	 */

	class BS_CORE_EXPORT CCharacterControllerRTTI : public RTTIType<CCharacterController, Component, CCharacterControllerRTTI>
	{
	private:
		BS_BEGIN_RTTI_MEMBERS
			BS_RTTI_MEMBER_PLAIN_NAMED(mPosition, mDesc.Position, 0)
			BS_RTTI_MEMBER_PLAIN_NAMED(mContactOffset, mDesc.ContactOffset, 1)
			BS_RTTI_MEMBER_PLAIN_NAMED(mStepOffset, mDesc.StepOffset, 2)
			BS_RTTI_MEMBER_PLAIN_NAMED(mSlopeLimit, mDesc.SlopeLimit, 3)
			BS_RTTI_MEMBER_PLAIN_NAMED(mMinMoveDistance, mDesc.MinMoveDistance, 4)
			BS_RTTI_MEMBER_PLAIN_NAMED(mHeight, mDesc.Height, 5)
			BS_RTTI_MEMBER_PLAIN_NAMED(mRadius, mDesc.Radius, 6)
			BS_RTTI_MEMBER_PLAIN_NAMED(mUp, mDesc.Up, 7)
			BS_RTTI_MEMBER_PLAIN_NAMED(mClimbingMode, mDesc.ClimbingMode, 8)
			BS_RTTI_MEMBER_PLAIN_NAMED(mNonWalkableMode, mDesc.NonWalkableMode, 9)
			BS_RTTI_MEMBER_PLAIN(mLayer, 10)
		BS_END_RTTI_MEMBERS

	public:
		const String& GetRttiName() override
		{
			static String name = "CCharacterController";
			return name;
		}

		u32 GetRttiId() override
		{
			return TID_CCharacterController;
		}

		SPtr<IReflectable> NewRttiObject() 
		{
			return SceneObject::CreateEmptyComponent<CCharacterController>();
		}
	};

	/** @} */
	/** @endcond */
}
