//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "RTTI/BsStringRTTI.h"
#include "Material/BsShaderInclude.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Core
	 *  @{
	 */

	class B3D_CORE_EXPORT ShaderIncludeRTTI : public RTTIType<ShaderInclude, Resource, ShaderIncludeRTTI>
	{
	private:
		String& GetString(ShaderInclude* obj) { return obj->mString; }

		void SetString(ShaderInclude* obj, String& val) { obj->mString = val; }

	public:
		ShaderIncludeRTTI()
		{
			AddPlainField("mString", 0, &ShaderIncludeRTTI::GetString, &ShaderIncludeRTTI::SetString);
		}

		const String& GetRttiName()
		{
			static String name = "ShaderInclude";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_ShaderInclude;
		}

		SPtr<IReflectable> NewRttiObject()
		{
			return ShaderInclude::CreatePtrInternal(""); // Initial string doesn't matter, it'll get overwritten
		}
	};

	/** @} */
	/** @endcond */
} // namespace bs
