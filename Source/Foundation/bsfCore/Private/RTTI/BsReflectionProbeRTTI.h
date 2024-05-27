//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "Renderer/BsReflectionProbe.h"
#include "Renderer/BsRenderer.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Engine
	 *  @{
	 */

	class B3D_CORE_EXPORT ReflectionProbeRTTI : public TRTTIType<ReflectionProbe, IReflectable, ReflectionProbeRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER(mTransform, 0)
			B3D_RTTI_MEMBER(mActive, 1)
			B3D_RTTI_MEMBER(mMobility, 2)
			B3D_RTTI_MEMBER(mType, 3)
			B3D_RTTI_MEMBER(mRadius, 4)
			B3D_RTTI_MEMBER(mExtents, 5)
			B3D_RTTI_MEMBER(mTransitionDistance, 6)
			B3D_RTTI_MEMBER(mCustomTexture, 7)
			B3D_RTTI_MEMBER(mFilteredTexture, 8)
		B3D_RTTI_END_MEMBERS
	public:
		void OnOperationStarted(ReflectionProbe& object, RTTIOperationTypeFlags operationType, RTTIOperationContext& context) override
		{
			if(operationType.IsSet(RTTIOperationType::ReadBit))
			{
				// Force the renderer task to complete, so the filtered texture is up to date
				if(object.mRendererTask != nullptr)
					object.mRendererTask->Wait();
			}
		}

		const String& GetRttiName()
		{
			static String name = "ReflectionProbe";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_ReflectionProbe;
		}

		SPtr<IReflectable> NewRttiObject()
		{
			return ReflectionProbe::CreateEmpty();
		}
	};

	/** @} */
	/** @endcond */
} // namespace bs
