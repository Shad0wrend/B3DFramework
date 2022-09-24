//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptChromaticAberrationSettings.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "../../../Foundation/bsfCore/Image/BsTexture.h"

namespace bs
{
	ScriptChromaticAberrationSettings::ScriptChromaticAberrationSettings(MonoObject* managedInstance, const SPtr<ChromaticAberrationSettings>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptChromaticAberrationSettings::InitRuntimeData()
	{
		metaData.ScriptClass->AddInternalCall("Internal_ChromaticAberrationSettings", (void*)&ScriptChromaticAberrationSettings::InternalChromaticAberrationSettings);
		metaData.ScriptClass->AddInternalCall("Internal_GetfringeTexture", (void*)&ScriptChromaticAberrationSettings::InternalGetfringeTexture);
		metaData.ScriptClass->AddInternalCall("Internal_SetfringeTexture", (void*)&ScriptChromaticAberrationSettings::InternalSetfringeTexture);
		metaData.ScriptClass->AddInternalCall("Internal_Getenabled", (void*)&ScriptChromaticAberrationSettings::InternalGetenabled);
		metaData.ScriptClass->AddInternalCall("Internal_Setenabled", (void*)&ScriptChromaticAberrationSettings::InternalSetenabled);
		metaData.ScriptClass->AddInternalCall("Internal_Gettype", (void*)&ScriptChromaticAberrationSettings::InternalGettype);
		metaData.ScriptClass->AddInternalCall("Internal_Settype", (void*)&ScriptChromaticAberrationSettings::InternalSettype);
		metaData.ScriptClass->AddInternalCall("Internal_GetshiftAmount", (void*)&ScriptChromaticAberrationSettings::InternalGetshiftAmount);
		metaData.ScriptClass->AddInternalCall("Internal_SetshiftAmount", (void*)&ScriptChromaticAberrationSettings::InternalSetshiftAmount);

	}

	MonoObject* ScriptChromaticAberrationSettings::Create(const SPtr<ChromaticAberrationSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.ScriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptChromaticAberrationSettings>()) ScriptChromaticAberrationSettings(managedInstance, value);
		return managedInstance;
	}
	void ScriptChromaticAberrationSettings::InternalChromaticAberrationSettings(MonoObject* managedInstance)
	{
		SPtr<ChromaticAberrationSettings> instance = bs_shared_ptr_new<ChromaticAberrationSettings>();
		new (bs_alloc<ScriptChromaticAberrationSettings>())ScriptChromaticAberrationSettings(managedInstance, instance);
	}

	MonoObject* ScriptChromaticAberrationSettings::InternalGetfringeTexture(ScriptChromaticAberrationSettings* thisPtr)
	{
		ResourceHandle<Texture> tmp__output;
		tmp__output = thisPtr->GetInternal()->FringeTexture;

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().GetScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptChromaticAberrationSettings::InternalSetfringeTexture(ScriptChromaticAberrationSettings* thisPtr, MonoObject* value)
	{
		ResourceHandle<Texture> tmpvalue;
		ScriptRRefBase* scriptvalue;
		scriptvalue = ScriptRRefBase::ToNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = static_resource_cast<Texture>(scriptvalue->GetHandle());
		thisPtr->GetInternal()->FringeTexture = tmpvalue;
	}

	bool ScriptChromaticAberrationSettings::InternalGetenabled(ScriptChromaticAberrationSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->Enabled;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptChromaticAberrationSettings::InternalSetenabled(ScriptChromaticAberrationSettings* thisPtr, bool value)
	{
		thisPtr->GetInternal()->Enabled = value;
	}

	ChromaticAberrationType ScriptChromaticAberrationSettings::InternalGettype(ScriptChromaticAberrationSettings* thisPtr)
	{
		ChromaticAberrationType tmp__output;
		tmp__output = thisPtr->GetInternal()->Type;

		ChromaticAberrationType __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptChromaticAberrationSettings::InternalSettype(ScriptChromaticAberrationSettings* thisPtr, ChromaticAberrationType value)
	{
		thisPtr->GetInternal()->Type = value;
	}

	float ScriptChromaticAberrationSettings::InternalGetshiftAmount(ScriptChromaticAberrationSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->ShiftAmount;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptChromaticAberrationSettings::InternalSetshiftAmount(ScriptChromaticAberrationSettings* thisPtr, float value)
	{
		thisPtr->GetInternal()->ShiftAmount = value;
	}
}
