//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "Components/BsCCamera.h"
#include "Private/RTTI/BsGameObjectRTTI.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Engine
	 *  @{
	 */

	class BS_CORE_EXPORT CCameraRTTI : public RTTIType<CCamera, Component, CCameraRTTI>
	{
	private:
		SPtr<Camera> GetInternal(CCamera* obj) { return obj->mInternal; }
		void SetInternal(CCamera* obj, SPtr<Camera> val) { obj->mInternal = val; }

	public:
		CCameraRTTI()
		{
			addReflectablePtrField("mInternal", 0, &CCameraRTTI::getInternal, &CCameraRTTI::setInternal);
		}

		const String& GetRttiName() override
		{
			static String name = "CCamera";
			return name;
		}

		UINT32 GetRttiId() override
		{
			return TID_CCamera;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return SceneObject::CreateEmptyComponent<CCamera>();
		}
	};

	/** @} */
	/** @endcond */
}
