//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/GUI/BsScriptGUIFixedSpace.h"
#include "BsMonoField.h"
#include "BsMonoClass.h"
#include "BsMonoManager.h"
#include "Image/BsSpriteTexture.h"
#include "BsMonoUtil.h"
#include "GUI/BsGUILayout.h"
#include "GUI/BsGUISpace.h"
#include "Wrappers/GUI/BsScriptGUILayout.h"

using namespace bs;
ScriptGUIFixedSpace::ScriptGUIFixedSpace(GUIFixedSpace* nativeObject)
	: TScriptGUIElementWrapper(nativeObject)
{
}

void ScriptGUIFixedSpace::SetupScriptBindings()
{
	sInteropMetaData.ScriptClass->AddInternalCall("Internal_CreateInstance", (void*)&ScriptGUIFixedSpace::InternalCreateInstance);
	sInteropMetaData.ScriptClass->AddInternalCall("Internal_CreateInstance1", (void*)&ScriptGUIFixedSpace::InternalCreateInstance1);
	sInteropMetaData.ScriptClass->AddInternalCall("Internal_SetSize", (void*)&ScriptGUIFixedSpace::InternalSetSize);
	sInteropMetaData.ScriptClass->AddInternalCall("Internal_SetSize1", (void*)&ScriptGUIFixedSpace::InternalSetSize1);
}

MonoObject* ScriptGUIFixedSpace::CreateScriptObject(bool construct)
{
	// TODO - Add a ctor in C# we can call if needed
	return nullptr;
}

void ScriptGUIFixedSpace::InternalCreateInstance(MonoObject* instance, u32 size)
{
	GUIFixedSpace* space = GUIFixedSpace::Create((GUILogicalUnit)(i32)size);

	ScriptObjectWrapper::Create<ScriptGUIFixedSpace>(space, instance);
}

void ScriptGUIFixedSpace::InternalCreateInstance1(MonoObject* instance, GUILogicalUnit* size)
{
	GUIFixedSpace* space = GUIFixedSpace::Create(*size);

	ScriptObjectWrapper::Create<ScriptGUIFixedSpace>(space, instance);
}

void ScriptGUIFixedSpace::InternalSetSize(ScriptGUIFixedSpace* self, u32 size)
{
	self->GetNativeObject()->SetSize((i32)size);
}

void ScriptGUIFixedSpace::InternalSetSize1(ScriptGUIFixedSpace* self, GUILogicalUnit* size)
{
	self->GetNativeObject()->SetSize(*size);
}
