//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "Renderer/BsDecal.h"
#include "Renderer/BsRenderer.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Engine
	 *  @{
	 */

	class B3D_CORE_EXPORT DecalRTTI : public TRTTIType<Decal, IReflectable, DecalRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER(mTransform, 0)
			B3D_RTTI_MEMBER(mActive, 1)
			B3D_RTTI_MEMBER(mMobility, 2)
			B3D_RTTI_MEMBER(mSize, 3)
			B3D_RTTI_MEMBER(mMaxDistance, 4)
			B3D_RTTI_MEMBER(mMaterial, 5)
			B3D_RTTI_MEMBER(mLayer, 6)
			B3D_RTTI_MEMBER(mLayerMask, 7)
		B3D_RTTI_END_MEMBERS
	public:
		const String& GetRttiName() override
		{
			static String name = "Decal";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_Decal;
		}

		SPtr<IReflectable> NewRttiObject()
		{
			return Decal::CreateEmpty();
		}
	};

	/** @} */
	/** @endcond */
} // namespace bs
