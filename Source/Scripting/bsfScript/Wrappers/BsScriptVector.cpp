//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/BsScriptVector.h"
#include "BsMonoManager.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

using namespace bs;

ScriptVector4::ScriptVector4(MonoObject* instance)
	: ScriptObject(instance)
{}

void ScriptVector4::InitRuntimeData()
{}

MonoObject* ScriptVector4::Box(const Vector4& value)
{
	// We're casting away const but it's fine since structs are passed by value anyway
	return MonoUtil::Box(metaData.ScriptClass->GetInternalClass(), (void*)&value);
}

Vector4 ScriptVector4::Unbox(MonoObject* obj)
{
	return *(Vector4*)MonoUtil::Unbox(obj);
}
