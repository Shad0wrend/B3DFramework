//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptApplicationEx.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../Extensions/BsApplicationEx.h"
#include "BsScriptVideoMode.generated.h"
#include "BsScriptSTART_UP_DESC.generated.h"

namespace bs
{
#if !BS_IS_BANSHEE3D
	ScriptApplicationEx::ScriptApplicationEx(MonoObject* managedInstance, const SPtr<ApplicationEx>& value)
		:ScriptObject(managedInstance), mInternal(value)
	{
	}

	void ScriptApplicationEx::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_startUp", (void*)&ScriptApplicationEx::InternalStartUp);
		metaData.scriptClass->AddInternalCall("Internal_startUp0", (void*)&ScriptApplicationEx::InternalStartUp0);
		metaData.scriptClass->AddInternalCall("Internal_runMainLoop", (void*)&ScriptApplicationEx::InternalRunMainLoop);
		metaData.scriptClass->AddInternalCall("Internal_shutDown", (void*)&ScriptApplicationEx::InternalShutDown);

	}

	MonoObject* ScriptApplicationEx::Create(const SPtr<ApplicationEx>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptApplicationEx>()) ScriptApplicationEx(managedInstance, value);
		return managedInstance;
	}
	void ScriptApplicationEx::InternalStartUp(__START_UP_DESCInterop* desc)
	{
		START_UP_DESC tmpdesc;
		tmpdesc = ScriptSTART_UP_DESC::fromInterop(*desc);
		ApplicationEx::StartUp(tmpdesc);
	}

	void ScriptApplicationEx::InternalStartUp0(__VideoModeInterop* videoMode, MonoString* title, bool fullscreen)
	{
		VideoMode tmpvideoMode;
		tmpvideoMode = ScriptVideoMode::fromInterop(*videoMode);
		String tmptitle;
		tmptitle = MonoUtil::MonoToString(title);
		ApplicationEx::StartUp(tmpvideoMode, tmptitle, fullscreen);
	}

	void ScriptApplicationEx::InternalRunMainLoop()
	{
		ApplicationEx::runMainLoop();
	}

	void ScriptApplicationEx::InternalShutDown()
	{
		ApplicationEx::ShutDown();
	}
#endif
}
