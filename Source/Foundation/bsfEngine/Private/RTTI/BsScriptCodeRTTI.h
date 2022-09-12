//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsPrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "RTTI/BsStringRTTI.h"
#include "Resources/BsScriptCode.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Engine
	 *  @{
	 */

	class BS_EXPORT ScriptCodeRTTI : public RTTIType <ScriptCode, Resource, ScriptCodeRTTI>
	{
	private:
		BS_BEGIN_RTTI_MEMBERS
			BS_RTTI_MEMBER_PLAIN(mString, 0)
			BS_RTTI_MEMBER_PLAIN(mEditorScript, 1)
		BS_END_RTTI_MEMBERS

	public:
		const String& GetRTTIName() override
		{
			static String name = "ScriptCode";
			return name;
		}

		UINT32 GetRTTIId() override
		{
			return TID_ScriptCode;
		}

		SPtr<IReflectable> NewRTTIObject() override
		{
			return ScriptCode::_createPtr(L""); // Initial string doesn't matter, it'll get overwritten
		}
	};

	/** @} */
	/** @endcond */
}
