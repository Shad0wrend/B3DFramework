//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "BsScriptObjectWrapper.h"
#include "../../../Foundation/bsfUtility/Math/BsVector2.h"
#include "BsScriptObjectWrapper.h"
#include "../../../Foundation/bsfUtility/Math/BsVector2.h"

namespace bs
{
	class B3D_SCRIPT_INTEROP_EXPORT ScriptVector2 : public TScriptTypeDefinition<ScriptVector2>
	{
	public:
		B3D_SCRIPT_TYPE_DEFINITION(kEngineAssembly, kEngineNs, "Vector2")

		static MonoObject* Box(const TVector2<float>& value);
		static TVector2<float> Unbox(MonoObject* value);

	private:
		ScriptVector2();

	};

	class B3D_SCRIPT_INTEROP_EXPORT ScriptVector2d : public TScriptTypeDefinition<ScriptVector2d>
	{
	public:
		B3D_SCRIPT_TYPE_DEFINITION(kEngineAssembly, kEngineNs, "Vector2d")

		static MonoObject* Box(const TVector2<double>& value);
		static TVector2<double> Unbox(MonoObject* value);

	private:
		ScriptVector2d();

	};
}
