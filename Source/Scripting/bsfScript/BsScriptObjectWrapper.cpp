//********************************* bs::framework - Copyright 2024 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptObjectWrapper.h"
#include "BsScriptObjectManager.h"

using namespace bs;

ScriptObjectWrapper::ScriptObjectWrapper(IScriptExportable* nativeObject, MonoObject* scriptObject)
	:IScriptObjectWrapper(nativeObject)
{
	ScriptObjectManager::Instance().RegisterScriptObjectWrapper(this);

	// TODO - Acquire strong handle on the scriptObject?
}

ScriptObjectWrapper::~ScriptObjectWrapper()
{
	ScriptObjectManager::Instance().UnregisterScriptObjectWrapper(this);
}

void ScriptObjectWrapper::NotifyScriptObjectDestroyed(bool isDestroyedDueToScriptReload)
{
	// TODO - Need to do something else where?

	B3DDelete(this);
}
