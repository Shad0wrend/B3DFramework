//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "Components/BsCPlaneCollider.h"
#include "Private/RTTI/BsGameObjectRTTI.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Core
	 *  @{
	 */

	class BS_CORE_EXPORT CPlaneColliderRTTI : public RTTIType<CPlaneCollider, CCollider, CPlaneColliderRTTI>
	{
	private:
		BS_BEGIN_RTTI_MEMBERS
			BS_RTTI_MEMBER_PLAIN(mNormal, 0)
			BS_RTTI_MEMBER_PLAIN(mDistance, 1)
		BS_END_RTTI_MEMBERS
	public:
		const String& GetRTTIName() override
		{
			static String name = "CPlaneCollider";
			return name;
		}

		UINT32 GetRTTIId() override
		{
			return TID_CPlaneCollider;
		}

		SPtr<IReflectable> NewRTTIObject() override
		{
			return SceneObject::createEmptyComponent<CPlaneCollider>();
		}
	};

	/** @} */
	/** @endcond */
}
