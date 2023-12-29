//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "BsScriptObject.h"
#include "../../../Foundation/bsfEngine/GUI/BsGUIInputBox.h"

namespace bs
{
	class B3D_SCRIPT_INTEROP_EXPORT ScriptGUIInputBoxContent : public ScriptObject<ScriptGUIInputBoxContent>
	{
	public:
		SCRIPT_OBJ(kEngineAssembly, kEngineNs, "GUIInputBoxContent")

		static MonoObject* Box(const GUIInputBoxContent& value);
		static GUIInputBoxContent Unbox(MonoObject* value);

	private:
		ScriptGUIInputBoxContent(MonoObject* managedInstance);

	};
}
