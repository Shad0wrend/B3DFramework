//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "Reflection/BsRTTIPlain.h"
#include "Components/BsCHingeJoint.h"
#include "Private/RTTI/BsGameObjectRTTI.h"
#include "RTTI/BsMathRTTI.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Core
	 *  @{
	 */

	class BS_CORE_EXPORT CHingeJointRTTI : public RTTIType<CHingeJoint, CJoint, CHingeJointRTTI>
	{
		BS_BEGIN_RTTI_MEMBERS
			BS_RTTI_MEMBER_PLAIN_NAMED(mFlag, mDesc.Flag, 0)
			BS_RTTI_MEMBER_PLAIN_NAMED(mDriveSpeed, mDesc.Drive.Speed, 1)
			BS_RTTI_MEMBER_PLAIN_NAMED(mDriveForceLimit, mDesc.Drive.ForceLimit, 2)
			BS_RTTI_MEMBER_PLAIN_NAMED(mDriveGearRatio, mDesc.Drive.GearRatio, 3)
			BS_RTTI_MEMBER_PLAIN_NAMED(mDriveFreeSpin, mDesc.Drive.FreeSpin, 4)
			BS_RTTI_MEMBER_PLAIN_NAMED(mLimitLower, mDesc.Limit.Lower, 5)
			BS_RTTI_MEMBER_PLAIN_NAMED(mLimitUpper, mDesc.Limit.Upper, 6)
			BS_RTTI_MEMBER_PLAIN_NAMED(mLimitContactDist, mDesc.Limit.ContactDist, 7)
			BS_RTTI_MEMBER_PLAIN_NAMED(mLimitRestitution, mDesc.Limit.Restitution, 8)
			BS_RTTI_MEMBER_PLAIN_NAMED(mSpringDamping, mDesc.Limit.Spring.Damping, 9)
			BS_RTTI_MEMBER_PLAIN_NAMED(mSpringStiffness, mDesc.Limit.Spring.Stiffness, 10)
		BS_END_RTTI_MEMBERS

	public:
		const String& GetRttiName() override
		{
			static String name = "CHingeJoint";
			return name;
		}

		u32 GetRttiId() override
		{
			return TID_CHingeJoint;
		}

		SPtr<IReflectable> NewRttiObject() override
		{
			return SceneObject::CreateEmptyComponent<CHingeJoint>();
		}
	};

	/** @} */
	/** @endcond */
} // namespace bs
