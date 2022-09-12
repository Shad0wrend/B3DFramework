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

	class BS_CORE_EXPORT ShaderIncludeRTTI : public RTTIType <ShaderInclude, Resource, ShaderIncludeRTTI>
	{
	private:
		String& GetString(ShaderInclude* obj) { return obj->mString; }
		void SetString(ShaderInclude* obj, String& val) { obj->mString = val; }
	public:
		ShaderIncludeRTTI()
		{
			addPlainField("mString", 0, &ShaderIncludeRTTI::getString, &ShaderIncludeRTTI::setString);
		}

		const String& GetRTTIName() override
		{
			static String name = "ShaderInclude";
			return name;
		}

		UINT32 GetRTTIId() override
		{
			return TID_ShaderInclude;
		}

		SPtr<IReflectable> NewRTTIObject() override
		{
			return ShaderInclude::_createPtr(""); // Initial string doesn't matter, it'll get overwritten
		}
	};

	/** @} */
	/** @endcond */
}
