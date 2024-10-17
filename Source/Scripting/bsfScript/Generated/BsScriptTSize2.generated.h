//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "BsScriptObjectWrapper.h"
#include "../../../Foundation/bsfUtility/Utility/BsUtil.h"
#include "BsScriptObjectWrapper.h"
#include "../../../Foundation/bsfUtility/Utility/BsUtil.h"

namespace bs
{
	class B3D_SCRIPT_INTEROP_EXPORT ScriptSize2 : public TScriptTypeDefinition<ScriptSize2>
	{
	public:
		B3D_SCRIPT_TYPE_DEFINITION(kEngineAssembly, kEngineNs, "Size2")

		static MonoObject* Box(const TSize2<float>& value);
		static TSize2<float> Unbox(MonoObject* value);

	private:
		ScriptSize2();

	};

	class B3D_SCRIPT_INTEROP_EXPORT ScriptSize2UI : public TScriptTypeDefinition<ScriptSize2UI>
	{
	public:
		B3D_SCRIPT_TYPE_DEFINITION(kEngineAssembly, kEngineNs, "Size2UI")

		static MonoObject* Box(const TSize2<uint32_t>& value);
		static TSize2<uint32_t> Unbox(MonoObject* value);

	private:
		ScriptSize2UI();

	};
}
