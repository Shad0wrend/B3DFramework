//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptMotionBlurSettings.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptMotionBlurSettings::ScriptMotionBlurSettings(MonoObject* managedInstance, const SPtr<MotionBlurSettings>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptMotionBlurSettings::InitRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_MotionBlurSettings", (void*)&ScriptMotionBlurSettings::InternalMotionBlurSettings);
		metaData.scriptClass->AddInternalCall("Internal_Getenabled", (void*)&ScriptMotionBlurSettings::InternalGetenabled);
		metaData.scriptClass->AddInternalCall("Internal_Setenabled", (void*)&ScriptMotionBlurSettings::InternalSetenabled);
		metaData.scriptClass->AddInternalCall("Internal_Getdomain", (void*)&ScriptMotionBlurSettings::InternalGetdomain);
		metaData.scriptClass->AddInternalCall("Internal_Setdomain", (void*)&ScriptMotionBlurSettings::InternalSetdomain);
		metaData.scriptClass->AddInternalCall("Internal_Getfilter", (void*)&ScriptMotionBlurSettings::InternalGetfilter);
		metaData.scriptClass->AddInternalCall("Internal_Setfilter", (void*)&ScriptMotionBlurSettings::InternalSetfilter);
		metaData.scriptClass->AddInternalCall("Internal_Getquality", (void*)&ScriptMotionBlurSettings::InternalGetquality);
		metaData.scriptClass->AddInternalCall("Internal_Setquality", (void*)&ScriptMotionBlurSettings::InternalSetquality);
		metaData.scriptClass->AddInternalCall("Internal_GetmaximumRadius", (void*)&ScriptMotionBlurSettings::InternalGetmaximumRadius);
		metaData.scriptClass->AddInternalCall("Internal_SetmaximumRadius", (void*)&ScriptMotionBlurSettings::InternalSetmaximumRadius);

	}

	MonoObject* ScriptMotionBlurSettings::Create(const SPtr<MotionBlurSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptMotionBlurSettings>()) ScriptMotionBlurSettings(managedInstance, value);
		return managedInstance;
	}
	void ScriptMotionBlurSettings::InternalMotionBlurSettings(MonoObject* managedInstance)
	{
		SPtr<MotionBlurSettings> instance = bs_shared_ptr_new<MotionBlurSettings>();
		new (bs_alloc<ScriptMotionBlurSettings>())ScriptMotionBlurSettings(managedInstance, instance);
	}

	bool ScriptMotionBlurSettings::InternalGetenabled(ScriptMotionBlurSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->enabled;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptMotionBlurSettings::InternalSetenabled(ScriptMotionBlurSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->enabled = value;
	}

	MotionBlurDomain ScriptMotionBlurSettings::InternalGetdomain(ScriptMotionBlurSettings* thisPtr)
	{
		MotionBlurDomain tmp__output;
		tmp__output = thisPtr->GetInternal()->domain;

		MotionBlurDomain __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptMotionBlurSettings::InternalSetdomain(ScriptMotionBlurSettings* thisPtr, MotionBlurDomain value)
	{
		thisPtr->GetInternal()->domain = value;
	}

	MotionBlurFilter ScriptMotionBlurSettings::InternalGetfilter(ScriptMotionBlurSettings* thisPtr)
	{
		MotionBlurFilter tmp__output;
		tmp__output = thisPtr->GetInternal()->filter;

		MotionBlurFilter __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptMotionBlurSettings::InternalSetfilter(ScriptMotionBlurSettings* thisPtr, MotionBlurFilter value)
	{
		thisPtr->GetInternal()->filter = value;
	}

	MotionBlurQuality ScriptMotionBlurSettings::InternalGetquality(ScriptMotionBlurSettings* thisPtr)
	{
		MotionBlurQuality tmp__output;
		tmp__output = thisPtr->GetInternal()->quality;

		MotionBlurQuality __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptMotionBlurSettings::InternalSetquality(ScriptMotionBlurSettings* thisPtr, MotionBlurQuality value)
	{
		thisPtr->GetInternal()->quality = value;
	}

	float ScriptMotionBlurSettings::InternalGetmaximumRadius(ScriptMotionBlurSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->maximumRadius;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptMotionBlurSettings::InternalSetmaximumRadius(ScriptMotionBlurSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->maximumRadius = value;
	}
}
