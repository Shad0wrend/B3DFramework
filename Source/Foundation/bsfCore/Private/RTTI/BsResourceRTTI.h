//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "Reflection/BsRTTIPlain.h"
#include "Resources/BsResource.h"
#include "Resources/BsResourceMetaData.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Core
	 *  @{
	 */

	class BS_CORE_EXPORT ResourceRTTI : public RTTIType<Resource, IReflectable, ResourceRTTI>
	{
	private:
		UINT32& GetSize(Resource* obj) { return obj->mSize; }
		void SetSize(Resource* obj, UINT32& size) { obj->mSize = size; }

		SPtr<ResourceMetaData> GetMetaData(Resource* obj) { return obj->mMetaData; }
		void SetMetaData(Resource* obj, SPtr<ResourceMetaData> value) { obj->mMetaData = value; }

	public:
		ResourceRTTI()
		{
			AddPlainField("mSize", 0, &ResourceRTTI::GetSize, &ResourceRTTI::SetSize);
			AddReflectablePtrField("mMetaData", 1, &ResourceRTTI::GetMetaData, &ResourceRTTI::SetMetaData);
		}

		void OnDeserializationStarted(IReflectable* obj, SerializationContext* context) 
		{
			Resource* resource = static_cast<Resource*>(obj);

			if(context)
				resource->mKeepSourceData = (context->flags & SF_KeepResourceSourceData) != 0;
			else
				resource->mKeepSourceData = false;
		}

		const String& GetRttiName() 
		{
			static String name = "Resource";
			return name;
		}

		UINT32 GetRttiId() 
		{
			return 100;
		}

		SPtr<IReflectable> NewRttiObject() 
		{
			BS_EXCEPT(InternalErrorException, "Cannot instantiate an abstract class.");
			return nullptr;
		}
	};

	/** @} */
	/** @endcond */
}
