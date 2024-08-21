//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Serialization/BsBuiltinReflectableTypesLookup.h"
#include "Reflection/BsRTTIType.h"
#include "../../../Foundation/bsfCore/Image/BsPixelData.h"
#include "BsScriptPixelData.generated.h"

namespace bs
{
	LOOKUP_BEGIN(BuiltinReflectableTypes)
		ADD_ENTRY(PixelData, ScriptPixelData)
	LOOKUP_END
}
#undef LOOKUP_BEGIN
#undef ADD_ENTRY
#undef LOOKUP_END
