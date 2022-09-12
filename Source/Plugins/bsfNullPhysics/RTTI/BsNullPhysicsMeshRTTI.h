//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsNullPhysicsPrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "BsNullPhysicsMesh.h"
#include "FileSystem/BsDataStream.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-NullPhysics
	 *  @{
	 */

	class FNullPhysicsMeshRTTI : public RTTIType<FNullPhysicsMesh, FPhysicsMesh, FNullPhysicsMeshRTTI>
	{
	public:
		const String& GetRTTIName() override
		{
			static String name = "FNullPhysicsMesh";
			return name;
		}

		UINT32 GetRTTIId() override
		{
			return TID_FNullPhysicsMesh;
		}

		SPtr<IReflectable> NewRTTIObject() override
		{
			return bs_shared_ptr_new<FNullPhysicsMesh>();
		}
	};

	/** @} */
	/** @endcond */
}
