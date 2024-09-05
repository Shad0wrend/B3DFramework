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

	CreateScriptObjectHandle(scriptObject);
}

ScriptObjectWrapper::~ScriptObjectWrapper()
{
	ScriptObjectManager::Instance().UnregisterScriptObjectWrapper(this);
}

MonoObject* ScriptObjectWrapper::GetScriptObject() const
{
	if(mScriptObjectHandle == ~0u)
		return nullptr;

	return MonoUtil::GetObjectFromGcHandle(mScriptObjectHandle);
}

void ScriptObjectWrapper::NotifyScriptObjectDestroyed(bool isDestroyedDueToScriptReload)
{
	IScriptObjectWrapper::NotifyScriptObjectDestroyed();
	B3DDelete(this);
}

void ScriptObjectWrapper::NotifyNativeObjectDestroyed()
{
	ReleaseStrongScriptObjectHandle();

	IScriptObjectWrapper::NotifyNativeObjectDestroyed();
}

void ScriptObjectWrapper::CreateScriptObjectHandle(MonoObject* scriptObject)
{
	if(B3D_ENSURE(scriptObject != nullptr))
	{
		B3D_ENSURE(mScriptObjectHandle == ~0u);

		if(GetLifetimeTrackingMode() == ScriptObjectLifetimeTrackingMode::WeakHandle)
		{
			mScriptObjectHandle = MonoUtil::NewWeakGcHandle(scriptObject);
			mRequiresStrongHandle = false; // Not used in WeakHandle mode, but set it anyway
		}
		else
		{
			mScriptObjectHandle = MonoUtil::NewGcHandle(scriptObject, false);
			mRequiresStrongHandle = true; // Always start of as a strong handle, GC can transition to weak if needed
		}
	}
}

void ScriptObjectWrapper::TransitionToWeakHandle()
{
	if(!mRequiresStrongHandle)
		return;

	if(mScriptObjectHandle == ~0u)
		return;

	if(!B3D_ENSURE(GetLifetimeTrackingMode() == ScriptObjectLifetimeTrackingMode::StrongHandleWithGarbageCollection))
		return;

	MonoObject* const scriptObject = GetScriptObject();
	if(!B3D_ENSURE(scriptObject != nullptr))
		return;

	const u32 weakHandle = MonoUtil::NewWeakGcHandle(scriptObject);
	MonoUtil::FreeGcHandle(mScriptObjectHandle);

	mScriptObjectHandle = weakHandle;
	mRequiresStrongHandle = false;
}

void ScriptObjectWrapper::TransitionToStrongHandle()
{
	if(mRequiresStrongHandle)
		return;

	if(mScriptObjectHandle == ~0u)
		return;

	if(!B3D_ENSURE(GetLifetimeTrackingMode() == ScriptObjectLifetimeTrackingMode::StrongHandleWithGarbageCollection))
		return;

	MonoObject* const scriptObject = GetScriptObject();
	if(scriptObject == nullptr)
		return;

	const u32 strongHandle = MonoUtil::NewGcHandle(scriptObject);
	MonoUtil::FreeGcHandle(mScriptObjectHandle);

	mScriptObjectHandle = strongHandle;
	mRequiresStrongHandle = true;
}

void ScriptObjectWrapper::ReleaseStrongScriptObjectHandle()
{
	if(mScriptObjectHandle != ~0u)
	{
		MonoUtil::FreeGcHandle(mScriptObjectHandle);
		mScriptObjectHandle = ~0u;
	}
}


ScriptScriptObject::ScriptScriptObject(MonoObject* scriptObject)
	: TNonInstantiableScriptObjectWrapper(scriptObject)
{}

void ScriptScriptObject::SetupScriptBindings()
{
	sInteropMetaData.ScriptClass->AddInternalCall("Internal_ScriptObjectFinalizerCalled", (void*)&ScriptScriptObject::Internal_ScriptObjectFinalizerCalled);
}

void ScriptScriptObject::Internal_ScriptObjectFinalizerCalled(ScriptObjectWrapper* scriptObjectWrapper)
{
	// This method gets called on the finalizer thread, but so that we don't need to deal
	// with multi-threading issues we just delay it and execute it on the main thread.
	ScriptObjectManager::Instance().NotifyObjectFinalized(scriptObjectWrapper);
}
