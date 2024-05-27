//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "Renderer/BsRenderable.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Engine
	 *  @{
	 */

	class B3D_CORE_EXPORT RenderableRTTI : public TRTTIType<Renderable, IReflectable, RenderableRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER(mTransform, 0)
			B3D_RTTI_MEMBER(mActive, 1)
			B3D_RTTI_MEMBER(mMobility, 2)
			B3D_RTTI_MEMBER(mMesh, 3)
			B3D_RTTI_MEMBER(mLayer, 4)
			B3D_RTTI_MEMBER_CONTAINER(mMaterials, 5)
			B3D_RTTI_MEMBER(mCullDistanceFactor, 6)
			B3D_RTTI_MEMBER(mWriteVelocity, 7)
		B3D_RTTI_END_MEMBERS

	public:
		const String& GetRttiName()
		{
			static String name = "Renderable";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_Renderable;
		}

		SPtr<IReflectable> NewRttiObject()
		{
			return Renderable::CreateEmpty();
		}
	};

	/** @} */
	/** @endcond */
} // namespace bs
