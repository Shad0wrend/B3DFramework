//************************************ bs::framework - Copyright 2023 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/BsScriptSize.h"
#include "BsMonoManager.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

using namespace bs;

ScriptSize2::ScriptSize2(MonoObject* instance)
	: ScriptObject(instance)
{}

void ScriptSize2::InitRuntimeData()
{}

MonoObject* ScriptSize2::Box(const Size2& value)
{
	// We're casting away const but it's fine since structs are passed by value anyway
	return MonoUtil::Box(metaData.ScriptClass->GetInternalClassInternal(), (void*)&value);
}

Size2 ScriptSize2::Unbox(MonoObject* obj)
{
	return *(Size2*)MonoUtil::Unbox(obj);
}

ScriptSize2UI::ScriptSize2UI(MonoObject* instance)
	: ScriptObject(instance)
{}

void ScriptSize2UI::InitRuntimeData()
{}

MonoObject* ScriptSize2UI::Box(const Size2UI& value)
{
	// We're casting away const but it's fine since structs are passed by value anyway
	return MonoUtil::Box(metaData.ScriptClass->GetInternalClassInternal(), (void*)&value);
}

Size2UI ScriptSize2UI::Unbox(MonoObject* obj)
{
	return *(Size2UI*)MonoUtil::Unbox(obj);
}
