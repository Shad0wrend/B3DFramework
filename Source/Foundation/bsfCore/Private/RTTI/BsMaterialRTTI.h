//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "Material/BsMaterial.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Core
	 *  @{
	 */

	class BS_CORE_EXPORT MaterialRTTI : public RTTIType<Material, Resource, MaterialRTTI>
	{
	private:
		HShader& GetShader(Material* obj) { return obj->mShader; }
		void SetShader(Material* obj, HShader& val) { obj->mShader = val; }

		SPtr<MaterialParams> GetMaterialParams(Material* obj) { return obj->mParams; }
		void SetMaterialParams(Material* obj, SPtr<MaterialParams> value) { mMatParams = value; }

	public:
		MaterialRTTI()
		{
			addReflectableField("mShader", 0, &MaterialRTTI::getShader, &MaterialRTTI::setShader);
			addReflectablePtrField("mMaterialParams", 2, &MaterialRTTI::getMaterialParams, &MaterialRTTI::setMaterialParams);
		}

		void OnDeserializationEnded(IReflectable* obj, SerializationContext* context) override;

		const String& GetRTTIName() override
		{
			static String name = "Material";
			return name;
		}

		UINT32 GetRTTIId() override
		{
			return TID_Material;
		}

		SPtr<IReflectable> NewRTTIObject() override;

	private:
		SPtr<MaterialParams> mMatParams;
	};

	/** @} */
	/** @endcond */
}
