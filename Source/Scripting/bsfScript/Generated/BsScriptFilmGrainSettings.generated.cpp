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

	void ScriptFilmGrainSettings::InitRuntimeData()
	{
		metaData.ScriptClass->AddInternalCall("Internal_FilmGrainSettings", (void*)&ScriptFilmGrainSettings::InternalFilmGrainSettings);
		metaData.ScriptClass->AddInternalCall("Internal_Getenabled", (void*)&ScriptFilmGrainSettings::InternalGetenabled);
		metaData.ScriptClass->AddInternalCall("Internal_Setenabled", (void*)&ScriptFilmGrainSettings::InternalSetenabled);
		metaData.ScriptClass->AddInternalCall("Internal_Getintensity", (void*)&ScriptFilmGrainSettings::InternalGetintensity);
		metaData.ScriptClass->AddInternalCall("Internal_Setintensity", (void*)&ScriptFilmGrainSettings::InternalSetintensity);
		metaData.ScriptClass->AddInternalCall("Internal_Getspeed", (void*)&ScriptFilmGrainSettings::InternalGetspeed);
		metaData.ScriptClass->AddInternalCall("Internal_Setspeed", (void*)&ScriptFilmGrainSettings::InternalSetspeed);

	}

	MonoObject* ScriptFilmGrainSettings::Create(const SPtr<FilmGrainSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.ScriptClass->CreateInstance("bool", ctorParams);
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
		tmp__output = thisPtr->GetInternal()->Enabled;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptFilmGrainSettings::InternalSetenabled(ScriptFilmGrainSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->Enabled = value;
	}

	float ScriptFilmGrainSettings::InternalGetintensity(ScriptFilmGrainSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->Intensity;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptFilmGrainSettings::InternalSetintensity(ScriptFilmGrainSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->Intensity = value;
	}

	float ScriptFilmGrainSettings::InternalGetspeed(ScriptFilmGrainSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->Speed;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptFilmGrainSettings::InternalSetspeed(ScriptFilmGrainSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->Speed = value;
	}
}
