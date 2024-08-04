//********************************* bs::framework - Copyright 2024 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptObjectWrapper.h"

#include "BsMonoUtil.h"
#include "BsScriptObjectManager.h"
#include "Script/BsIScriptExportable.h"

using namespace bs;

ScriptObjectWrapper::ScriptObjectWrapper(IScriptExportable* nativeObject, MonoObject* scriptObject)
	:IScriptObjectWrapper(nativeObject)
{
	ScriptObjectManager::Instance().RegisterScriptObjectWrapper(this);

	if(B3D_ENSURE(scriptObject != nullptr))
		mStrongScriptObjectHandle = MonoUtil::NewGcHandle(scriptObject, false);
}

ScriptObjectWrapper::~ScriptObjectWrapper()
{
	ScriptObjectManager::Instance().UnregisterScriptObjectWrapper(this);
}

MonoObject* ScriptObjectWrapper::GetScriptObject() const
{
	if(mStrongScriptObjectHandle == ~0u)
		return nullptr;

	return MonoUtil::GetObjectFromGcHandle(mStrongScriptObjectHandle);
}

void ScriptObjectWrapper::NotifyScriptObjectDestroyed(bool isDestroyedDueToScriptReload)
{
	IScriptObjectWrapper::NotifyScriptObjectDestroyed();
	B3DDelete(this);
}

void ScriptObjectWrapper::NotifyNativeObjectDestroyed()
{
	if(mStrongScriptObjectHandle != ~0u)
	{
		MonoUtil::FreeGcHandle(mStrongScriptObjectHandle);
		mStrongScriptObjectHandle = ~0u;
	}

	IScriptObjectWrapper::NotifyNativeObjectDestroyed();
}

void ScriptObjectWrapper::ReleaseStrongHandlesBeforeScriptReload()
{
	if(mStrongScriptObjectHandle != ~0u)
	{
		MonoUtil::FreeGcHandle(mStrongScriptObjectHandle);
		mStrongScriptObjectHandle = ~0u;
	}
}
