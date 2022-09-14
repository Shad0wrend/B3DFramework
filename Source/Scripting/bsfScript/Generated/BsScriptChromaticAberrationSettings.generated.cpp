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

	void ScriptChromaticAberrationSettings::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_ChromaticAberrationSettings", (void*)&ScriptChromaticAberrationSettings::InternalChromaticAberrationSettings);
		metaData.scriptClass->addInternalCall("Internal_getfringeTexture", (void*)&ScriptChromaticAberrationSettings::Internal_getfringeTexture);
		metaData.scriptClass->addInternalCall("Internal_setfringeTexture", (void*)&ScriptChromaticAberrationSettings::Internal_setfringeTexture);
		metaData.scriptClass->addInternalCall("Internal_getenabled", (void*)&ScriptChromaticAberrationSettings::Internal_getenabled);
		metaData.scriptClass->addInternalCall("Internal_setenabled", (void*)&ScriptChromaticAberrationSettings::Internal_setenabled);
		metaData.scriptClass->addInternalCall("Internal_gettype", (void*)&ScriptChromaticAberrationSettings::Internal_gettype);
		metaData.scriptClass->addInternalCall("Internal_settype", (void*)&ScriptChromaticAberrationSettings::Internal_settype);
		metaData.scriptClass->addInternalCall("Internal_getshiftAmount", (void*)&ScriptChromaticAberrationSettings::Internal_getshiftAmount);
		metaData.scriptClass->addInternalCall("Internal_setshiftAmount", (void*)&ScriptChromaticAberrationSettings::Internal_setshiftAmount);

	}

	MonoObject* ScriptChromaticAberrationSettings::Create(const SPtr<ChromaticAberrationSettings>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
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
		tmp__output = thisPtr->getInternal()->fringeTexture;

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().getScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->getManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptChromaticAberrationSettings::InternalSetfringeTexture(ScriptChromaticAberrationSettings* thisPtr, MonoObject* value)
	{
		ResourceHandle<Texture> tmpvalue;
		ScriptRRefBase* scriptvalue;
		scriptvalue = ScriptRRefBase::toNative(value);
		if(scriptvalue != nullptr)
			tmpvalue = static_resource_cast<Texture>(scriptvalue->getHandle());
		thisPtr->getInternal()->fringeTexture = tmpvalue;
	}

	bool ScriptChromaticAberrationSettings::InternalGetenabled(ScriptChromaticAberrationSettings* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->getInternal()->enabled;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptChromaticAberrationSettings::InternalSetenabled(ScriptChromaticAberrationSettings* thisPtr, bool value)
	{
		thisPtr->getInternal()->enabled = value;
	}

	ChromaticAberrationType ScriptChromaticAberrationSettings::InternalGettype(ScriptChromaticAberrationSettings* thisPtr)
	{
		ChromaticAberrationType tmp__output;
		tmp__output = thisPtr->getInternal()->type;

		ChromaticAberrationType __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptChromaticAberrationSettings::InternalSettype(ScriptChromaticAberrationSettings* thisPtr, ChromaticAberrationType value)
	{
		thisPtr->getInternal()->type = value;
	}

	float ScriptChromaticAberrationSettings::InternalGetshiftAmount(ScriptChromaticAberrationSettings* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getInternal()->shiftAmount;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptChromaticAberrationSettings::InternalSetshiftAmount(ScriptChromaticAberrationSettings* thisPtr, float value)
	{
		thisPtr->getInternal()->shiftAmount = value;
	}
}
