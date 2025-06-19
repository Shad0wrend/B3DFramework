//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "Components/BsCBoxCollider.h"
#include "Private/RTTI/BsGameObjectRTTI.h"
#include "RTTI/BsMathRTTI.h"

namespace b3d
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Core
	 *  @{
	 */

	class B3D_CORE_EXPORT CBoxColliderRTTI : public TRTTIType<CBoxCollider, CCollider, CBoxColliderRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER(mExtents, 0)
		B3D_RTTI_END_MEMBERS
	public:
		const String& GetRttiName() override
		{
			static String name = "CBoxCollider";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_CBoxCollider;
		}

		SPtr<IReflectable> NewRttiObject() override
		{
			return SceneObject::CreateEmptyComponent<CBoxCollider>();
		}
	};

	/** @} */
	/** @endcond */
} // namespace b3d
