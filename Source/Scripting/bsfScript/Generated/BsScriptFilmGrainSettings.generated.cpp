//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptFilmGrainSettings.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptFilmGrainSettings::ScriptFilmGrainSettings(MonoObject* managedInstance, const SPtr<FilmGrainSettings>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptFilmGrainSettings::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_FilmGrainSettings", (void*)&ScriptFilmGrainSettings::InternalFilmGrainSettings);
		metaData.scriptClass->AddInternalCall("Internal_getenabled", (void*)&ScriptFilmGrainSettings::InternalGetenabled);
		metaData.scriptClass->AddInternalCall("Internal_setenabled", (void*)&ScriptFilmGrainSettings::InternalSetenabled);
		metaData.scriptClass->AddInternalCall("Internal_getintensity", (void*)&ScriptFilmGrainSettings::InternalGetintensity);
		metaData.scriptClass->AddInternalCall("Internal_setintensity", (void*)&ScriptFilmGrainSettings::InternalSetintensity);
		metaData.scriptClass->AddInternalCall("Internal_getspeed", (void*)&ScriptFilmGrainSettings::InternalGetspeed);
		metaData.scriptClass->AddInternalCall("Internal_setspeed", (void*)&ScriptFilmGrainSettings::InternalSetspeed);

	}

	MonoObject* ScriptFilmGrainSettings::Create(const SPtr<FilmGrainSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptFilmGrainSettings>()) ScriptFilmGrainSettings(managedInstance, value);
		return managedInstance;
	}
	void ScriptFilmGrainSettings::InternalFilmGrainSettings(MonoObject* managedInstance)
	{
		SPtr<FilmGrainSettings> instance = bs_shared_ptr_new<FilmGrainSettings>();
		new (bs_alloc<ScriptFilmGrainSettings>())ScriptFilmGrainSettings(managedInstance, instance);
	}

	bool ScriptFilmGrainSettings::InternalGetenabled(ScriptFilmGrainSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->enabled;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptFilmGrainSettings::InternalSetenabled(ScriptFilmGrainSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->enabled = value;
	}

	float ScriptFilmGrainSettings::InternalGetintensity(ScriptFilmGrainSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->intensity;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptFilmGrainSettings::InternalSetintensity(ScriptFilmGrainSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->intensity = value;
	}

	float ScriptFilmGrainSettings::InternalGetspeed(ScriptFilmGrainSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->speed;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptFilmGrainSettings::InternalSetspeed(ScriptFilmGrainSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->speed = value;
	}
}
