//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/BsScriptInputConfiguration.h"
#include "BsMonoManager.h"
#include "BsMonoClass.h"
#include "BsMonoMethod.h"
#include "BsMonoUtil.h"
#include "Input/BsVirtualInput.h"

namespace bs
{
	Map<UINT64, ScriptInputConfiguration*> ScriptInputConfiguration::ScriptInputConfigurations;

	ScriptInputConfiguration::ScriptInputConfiguration(MonoObject* instance, const SPtr<InputConfiguration>& inputConfig)
		:ScriptObject(instance), mInputConfig(inputConfig)
	{
		mGCHandle = MonoUtil::NewWeakGcHandle(instance);

		UINT64 configId = (UINT64)inputConfig.get();
		ScriptInputConfigurations[configId] = this;
	}

	void ScriptInputConfiguration::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_CreateInstance", (void*)&ScriptInputConfiguration::InternalCreateInstance);
		metaData.scriptClass->AddInternalCall("Internal_RegisterButton", (void*)&ScriptInputConfiguration::InternalRegisterButton);
		metaData.scriptClass->AddInternalCall("Internal_UnregisterButton", (void*)&ScriptInputConfiguration::InternalUnregisterButton);
		metaData.scriptClass->AddInternalCall("Internal_RegisterAxis", (void*)&ScriptInputConfiguration::InternalRegisterAxis);
		metaData.scriptClass->AddInternalCall("Internal_UnregisterAxis", (void*)&ScriptInputConfiguration::InternalUnregisterAxis);
		metaData.scriptClass->AddInternalCall("Internal_SetRepeatInterval", (void*)&ScriptInputConfiguration::InternalSetRepeatInterval);
		metaData.scriptClass->AddInternalCall("Internal_GetRepeatInterval", (void*)&ScriptInputConfiguration::InternalGetRepeatInterval);
	}

	MonoObject* ScriptInputConfiguration::GetManagedInstance() const
	{
		return MonoUtil::GetObjectFromGcHandle(mGCHandle);
	}

	ScriptInputConfiguration* ScriptInputConfiguration::GetScriptInputConfig(const SPtr<InputConfiguration>& inputConfig)
	{
		UINT64 configId = (UINT64)inputConfig.get();

		auto iterFind = ScriptInputConfigurations.find(configId);
		if (iterFind != ScriptInputConfigurations.end())
			return iterFind->second;

		return nullptr;
	}

	ScriptInputConfiguration* ScriptInputConfiguration::CreateScriptInputConfig(const SPtr<InputConfiguration>& inputConfig)
	{
		MonoObject* instance = metaData.scriptClass->createInstance(false);

		ScriptInputConfiguration* nativeInstance = new (bs_alloc<ScriptInputConfiguration>()) ScriptInputConfiguration(instance, inputConfig);
		return nativeInstance;
	}

	void ScriptInputConfiguration::InternalCreateInstance(MonoObject* object)
	{
		SPtr<InputConfiguration> inputConfig = VirtualInput::createConfiguration();

		new (bs_alloc<ScriptInputConfiguration>()) ScriptInputConfiguration(object, inputConfig);
	}

	void ScriptInputConfiguration::InternalRegisterButton(ScriptInputConfiguration* thisPtr, MonoString* name, ButtonCode buttonCode,
		ButtonModifier modifiers, bool repeatable)
	{
		String nameStr = MonoUtil::monoToString(name);

		thisPtr->GetInternalValue()->registerButton(nameStr, buttonCode, modifiers, repeatable);
	}

	void ScriptInputConfiguration::InternalUnregisterButton(ScriptInputConfiguration* thisPtr, MonoString* name)
	{
		String nameStr = MonoUtil::monoToString(name);

		thisPtr->GetInternalValue()->unregisterButton(nameStr);
	}

	void ScriptInputConfiguration::InternalRegisterAxis(ScriptInputConfiguration* thisPtr, MonoString* name, InputAxis type, float deadZone,
		float sensitivity, bool invert)
	{
		String nameStr = MonoUtil::monoToString(name);

		VIRTUAL_AXIS_DESC axisDesc;
		axisDesc.type = (UINT32)type;
		axisDesc.deadZone = deadZone;
		axisDesc.invert = invert;
		axisDesc.sensitivity = sensitivity;

		thisPtr->GetInternalValue()->registerAxis(nameStr, axisDesc);
	}

	void ScriptInputConfiguration::InternalUnregisterAxis(ScriptInputConfiguration* thisPtr, MonoString* name)
	{
		String nameStr = MonoUtil::monoToString(name);

		thisPtr->GetInternalValue()->unregisterAxis(nameStr);
	}

	void ScriptInputConfiguration::InternalSetRepeatInterval(ScriptInputConfiguration* thisPtr, UINT64 milliseconds)
	{
		thisPtr->GetInternalValue()->SetRepeatInterval(milliseconds);
	}

	UINT64 ScriptInputConfiguration::InternalGetRepeatInterval(ScriptInputConfiguration* thisPtr)
	{
		return thisPtr->GetInternalValue()->GetRepeatInterval();
	}

	void ScriptInputConfiguration::OnManagedInstanceDeletedInternal(bool assemblyRefresh)
	{
		UINT64 configId = (UINT64)mInputConfig.get();
		ScriptInputConfigurations.erase(configId);

		ScriptObject::OnManagedInstanceDeletedInternal(assemblyRefresh);
	}

	ScriptVirtualAxis::ScriptVirtualAxis(MonoObject* instance)
		:ScriptObject(instance)
	{ }

	void ScriptVirtualAxis::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_InitVirtualAxis", (void*)&ScriptVirtualAxis::internal_InitVirtualAxis);
	}

	UINT32 ScriptVirtualAxis::InternalInitVirtualAxis(MonoString* name)
	{
		String nameStr = MonoUtil::monoToString(name);

		VirtualAxis vb(nameStr);
		return vb.axisIdentifier;
	}
}
