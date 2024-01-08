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

	class B3D_CORE_EXPORT ResourceManifestRTTI : public RTTIType<ResourceManifest, IReflectable, ResourceManifestRTTI>
	{
	private:
		String& GetName(ResourceManifest* obj) { return obj->mName; }

		void SetName(ResourceManifest* obj, String& val) { obj->mName = val; }

		UnorderedMap<UUID, Path>& GetUuidMap(ResourceManifest* obj) { return obj->mUUIDToFilePath; }

		void SetUuidMap(ResourceManifest* obj, UnorderedMap<UUID, Path>& val)
		{
			obj->mUUIDToFilePath = val;

			obj->mFilePathToUUID.clear();

			for(auto& entry : obj->mUUIDToFilePath)
			{
				obj->mFilePathToUUID[entry.second] = entry.first;
			}
		}

	public:
		ResourceManifestRTTI()
		{
			AddPlainField("mName", 0, &ResourceManifestRTTI::GetName, &ResourceManifestRTTI::SetName);
			AddPlainField("mUUIDToFilePath", 1, &ResourceManifestRTTI::GetUuidMap, &ResourceManifestRTTI::SetUuidMap);
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
