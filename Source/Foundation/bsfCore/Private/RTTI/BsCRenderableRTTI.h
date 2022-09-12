//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "Components/BsCRenderable.h"
#include "Private/RTTI/BsGameObjectRTTI.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Engine
	 *  @{
	 */

	class BS_CORE_EXPORT CRenderableRTTI : public RTTIType<CRenderable, Component, CRenderableRTTI>
	{
	private:
		SPtr<Renderable> GetInternal(CRenderable* obj) { return obj->mInternal; }
		void SetInternal(CRenderable* obj, SPtr<Renderable> val) { obj->mInternal = val; }

	public:
		CRenderableRTTI()
		{
			addReflectablePtrField("mInternal", 0, &CRenderableRTTI::getInternal, &CRenderableRTTI::setInternal);
		}

		const String& GetRTTIName() override
		{
			static String name = "CRenderable";
			return name;
		}

		UINT32 GetRTTIId() override
		{
			return TID_CRenderable;
		}

		SPtr<IReflectable> NewRTTIObject() override
		{
			return SceneObject::createEmptyComponent<CRenderable>();
		}
	};

	/** @} */
	/** @endcond */
}
