//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptAudioClipImportOptions.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptAudioClipImportOptions.generated.h"

namespace bs
{
#if !BS_IS_BANSHEE3D
	ScriptAudioClipImportOptions::ScriptAudioClipImportOptions(MonoObject* managedInstance, const SPtr<AudioClipImportOptions>& value)
		:TScriptReflectable(managedInstance, value)
	{
		mInternal = value;
	}

	void ScriptAudioClipImportOptions::InitRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_Getformat", (void*)&ScriptAudioClipImportOptions::InternalGetformat);
		metaData.scriptClass->AddInternalCall("Internal_Setformat", (void*)&ScriptAudioClipImportOptions::InternalSetformat);
		metaData.scriptClass->AddInternalCall("Internal_GetreadMode", (void*)&ScriptAudioClipImportOptions::InternalGetreadMode);
		metaData.scriptClass->AddInternalCall("Internal_SetreadMode", (void*)&ScriptAudioClipImportOptions::InternalSetreadMode);
		metaData.scriptClass->AddInternalCall("Internal_Getis3D", (void*)&ScriptAudioClipImportOptions::InternalGetis3D);
		metaData.scriptClass->AddInternalCall("Internal_Setis3D", (void*)&ScriptAudioClipImportOptions::InternalSetis3D);
		metaData.scriptClass->AddInternalCall("Internal_GetbitDepth", (void*)&ScriptAudioClipImportOptions::InternalGetbitDepth);
		metaData.scriptClass->AddInternalCall("Internal_SetbitDepth", (void*)&ScriptAudioClipImportOptions::InternalSetbitDepth);
		metaData.scriptClass->AddInternalCall("Internal_Create", (void*)&ScriptAudioClipImportOptions::InternalCreate);

	}

	MonoObject* ScriptAudioClipImportOptions::Create(const SPtr<AudioClipImportOptions>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptAudioClipImportOptions>()) ScriptAudioClipImportOptions(managedInstance, value);
		return managedInstance;
	}
	void ScriptAudioClipImportOptions::InternalCreate(MonoObject* managedInstance)
	{
		SPtr<AudioClipImportOptions> instance = AudioClipImportOptions::Create();
		new (bs_alloc<ScriptAudioClipImportOptions>())ScriptAudioClipImportOptions(managedInstance, instance);
	}
	AudioFormat ScriptAudioClipImportOptions::InternalGetformat(ScriptAudioClipImportOptions* thisPtr)
	{
		AudioFormat tmp__output;
		tmp__output = thisPtr->GetInternal()->format;

		AudioFormat __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAudioClipImportOptions::InternalSetformat(ScriptAudioClipImportOptions* thisPtr, AudioFormat value)
	{
		thisPtr->GetInternal()->format = value;
	}

	AudioReadMode ScriptAudioClipImportOptions::InternalGetreadMode(ScriptAudioClipImportOptions* thisPtr)
	{
		AudioReadMode tmp__output;
		tmp__output = thisPtr->GetInternal()->readMode;

		AudioReadMode __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAudioClipImportOptions::InternalSetreadMode(ScriptAudioClipImportOptions* thisPtr, AudioReadMode value)
	{
		thisPtr->GetInternal()->readMode = value;
	}

	bool ScriptAudioClipImportOptions::InternalGetis3D(ScriptAudioClipImportOptions* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->is3D;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAudioClipImportOptions::InternalSetis3D(ScriptAudioClipImportOptions* thisPtr, bool value)
	{
		thisPtr->GetInternal()->is3D = value;
	}

	uint32_t ScriptAudioClipImportOptions::InternalGetbitDepth(ScriptAudioClipImportOptions* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->bitDepth;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAudioClipImportOptions::InternalSetbitDepth(ScriptAudioClipImportOptions* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->bitDepth = value;
	}
#endif
}
