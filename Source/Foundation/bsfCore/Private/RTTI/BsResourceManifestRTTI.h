//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "Reflection/BsRTTIPlain.h"
#include "RTTI/BsStringRTTI.h"
#include "RTTI/BsStdRTTI.h"
#include "RTTI/BsPathRTTI.h"
#include "RTTI/BsUUIDRTTI.h"
#include "Resources/BsResourceManifest.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Core
	 *  @{
	 */

	class B3D_CORE_EXPORT ResourceManifestRTTI : public TRTTIType<ResourceManifest, IReflectable, ResourceManifestRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER(mName, 0)
			B3D_RTTI_MEMBER(mUUIDToFilePath, 1)
		B3D_RTTI_END_MEMBERS

	public:
		void OnOperationEnded(ResourceManifest& object, RTTIOperationTypeFlags operationType, RTTIOperationContext& context) override
		{
			if(operationType.IsSet(RTTIOperationType::WriteBit))
			{
				object.mFilePathToUUID.clear();

				for(auto& entry : object.mUUIDToFilePath)
					object.mFilePathToUUID[entry.second] = entry.first;
			}
		}

		const String& GetRttiName()
		{
			static String name = "ResourceManifest";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_ResourceManifest;
		}

		SPtr<IReflectable> NewRttiObject()
		{
			return ResourceManifest::CreateEmpty();
		}
	};

	/** @} */
	/** @endcond */
} // namespace bs
