//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "Reflection/BsRTTIPlain.h"
#include "RenderAPI/BsDepthStencilState.h"
#include "Managers/BsRenderStateManager.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Core
	 *  @{
	 */

	BS_ALLOW_MEMCPY_SERIALIZATION(DEPTH_STENCIL_STATE_DESC);

	class BS_CORE_EXPORT DepthStencilStateRTTI : public RTTIType<DepthStencilState, IReflectable, DepthStencilStateRTTI>
	{
	private:
		DEPTH_STENCIL_STATE_DESC& GetData(DepthStencilState* obj) { return obj->mProperties.mData; }
		void SetData(DepthStencilState* obj, DEPTH_STENCIL_STATE_DESC& val) { obj->mProperties.mData = val; }

	public:
		DepthStencilStateRTTI()
		{
			addPlainField("mData", 0, &DepthStencilStateRTTI::GetData, &DepthStencilStateRTTI::SetData);
		}

		void OnDeserializationEnded(IReflectable* obj, SerializationContext* context) override
		{
			DepthStencilState* depthStencilState = static_cast<DepthStencilState*>(obj);
			depthStencilState->initialize();
		}

		const String& GetRttiName() override
		{
			static String name = "DepthStencilState";
			return name;
		}

		UINT32 GetRttiId() override
		{
			return TID_DepthStencilState;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return RenderStateManager::Instance().CreateDepthStencilStatePtrInternal(DEPTH_STENCIL_STATE_DESC());
		}
	};

	/** @} */
	/** @endcond */
}
