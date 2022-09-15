//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptCAudioSource.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Components/BsCAudioSource.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "../../../Foundation/bsfCore/Audio/BsAudioClip.h"

namespace bs
{
	ScriptCAudioSource::ScriptCAudioSource(MonoObject* managedInstance, const GameObjectHandle<CAudioSource>& value)
		:TScriptComponent(managedInstance, value)
	{
	}

	void ScriptCAudioSource::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_setClip", (void*)&ScriptCAudioSource::InternalSetClip);
		metaData.scriptClass->AddInternalCall("Internal_getClip", (void*)&ScriptCAudioSource::InternalGetClip);
		metaData.scriptClass->AddInternalCall("Internal_setVolume", (void*)&ScriptCAudioSource::InternalSetVolume);
		metaData.scriptClass->AddInternalCall("Internal_getVolume", (void*)&ScriptCAudioSource::InternalGetVolume);
		metaData.scriptClass->AddInternalCall("Internal_setPitch", (void*)&ScriptCAudioSource::InternalSetPitch);
		metaData.scriptClass->AddInternalCall("Internal_getPitch", (void*)&ScriptCAudioSource::InternalGetPitch);
		metaData.scriptClass->AddInternalCall("Internal_setIsLooping", (void*)&ScriptCAudioSource::InternalSetIsLooping);
		metaData.scriptClass->AddInternalCall("Internal_getIsLooping", (void*)&ScriptCAudioSource::InternalGetIsLooping);
		metaData.scriptClass->AddInternalCall("Internal_setPriority", (void*)&ScriptCAudioSource::InternalSetPriority);
		metaData.scriptClass->addInternalCall("Internal_getPriority", (void*)&ScriptCAudioSource::InternalGetPriority);
		metaData.scriptClass->addInternalCall("Internal_setMinDistance", (void*)&ScriptCAudioSource::InternalSetMinDistance);
		metaData.scriptClass->addInternalCall("Internal_getMinDistance", (void*)&ScriptCAudioSource::InternalGetMinDistance);
		metaData.scriptClass->addInternalCall("Internal_setAttenuation", (void*)&ScriptCAudioSource::InternalSetAttenuation);
		metaData.scriptClass->addInternalCall("Internal_getAttenuation", (void*)&ScriptCAudioSource::InternalGetAttenuation);
		metaData.scriptClass->addInternalCall("Internal_setTime", (void*)&ScriptCAudioSource::InternalSetTime);
		metaData.scriptClass->addInternalCall("Internal_getTime", (void*)&ScriptCAudioSource::InternalGetTime);
		metaData.scriptClass->addInternalCall("Internal_setPlayOnStart", (void*)&ScriptCAudioSource::InternalSetPlayOnStart);
		metaData.scriptClass->addInternalCall("Internal_getPlayOnStart", (void*)&ScriptCAudioSource::InternalGetPlayOnStart);
		metaData.scriptClass->addInternalCall("Internal_play", (void*)&ScriptCAudioSource::InternalPlay);
		metaData.scriptClass->addInternalCall("Internal_pause", (void*)&ScriptCAudioSource::InternalPause);
		metaData.scriptClass->addInternalCall("Internal_stop", (void*)&ScriptCAudioSource::InternalStop);
		metaData.scriptClass->addInternalCall("Internal_getState", (void*)&ScriptCAudioSource::InternalGetState);

	}

	void ScriptCAudioSource::InternalSetClip(ScriptCAudioSource* thisPtr, MonoObject* clip)
	{
		ResourceHandle<AudioClip> tmpclip;
		ScriptRRefBase* scriptclip;
		scriptclip = ScriptRRefBase::toNative(clip);
		if(scriptclip != nullptr)
			tmpclip = static_resource_cast<AudioClip>(scriptclip->GetHandle());
		thisPtr->GetHandle()->SetClip(tmpclip);
	}

	MonoObject* ScriptCAudioSource::InternalGetClip(ScriptCAudioSource* thisPtr)
	{
		ResourceHandle<AudioClip> tmp__output;
		tmp__output = thisPtr->GetHandle()->GetClip();

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().getScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptCAudioSource::InternalSetVolume(ScriptCAudioSource* thisPtr, float volume)
	{
		thisPtr->GetHandle()->SetVolume(volume);
	}

	float ScriptCAudioSource::InternalGetVolume(ScriptCAudioSource* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetVolume();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCAudioSource::InternalSetPitch(ScriptCAudioSource* thisPtr, float pitch)
	{
		thisPtr->GetHandle()->SetPitch(pitch);
	}

	float ScriptCAudioSource::InternalGetPitch(ScriptCAudioSource* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetPitch();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCAudioSource::InternalSetIsLooping(ScriptCAudioSource* thisPtr, bool loop)
	{
		thisPtr->GetHandle()->SetIsLooping(loop);
	}

	bool ScriptCAudioSource::InternalGetIsLooping(ScriptCAudioSource* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetHandle()->GetIsLooping();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCAudioSource::InternalSetPriority(ScriptCAudioSource* thisPtr, uint32_t priority)
	{
		thisPtr->GetHandle()->SetPriority(priority);
	}

	uint32_t ScriptCAudioSource::InternalGetPriority(ScriptCAudioSource* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetHandle()->GetPriority();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCAudioSource::InternalSetMinDistance(ScriptCAudioSource* thisPtr, float distance)
	{
		thisPtr->GetHandle()->SetMinDistance(distance);
	}

	float ScriptCAudioSource::InternalGetMinDistance(ScriptCAudioSource* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetMinDistance();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCAudioSource::InternalSetAttenuation(ScriptCAudioSource* thisPtr, float attenuation)
	{
		thisPtr->GetHandle()->SetAttenuation(attenuation);
	}

	float ScriptCAudioSource::InternalGetAttenuation(ScriptCAudioSource* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetAttenuation();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCAudioSource::InternalSetTime(ScriptCAudioSource* thisPtr, float time)
	{
		thisPtr->GetHandle()->SetTime(time);
	}

	float ScriptCAudioSource::InternalGetTime(ScriptCAudioSource* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetTime();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCAudioSource::InternalSetPlayOnStart(ScriptCAudioSource* thisPtr, bool enable)
	{
		thisPtr->GetHandle()->SetPlayOnStart(enable);
	}

	bool ScriptCAudioSource::InternalGetPlayOnStart(ScriptCAudioSource* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetHandle()->GetPlayOnStart();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCAudioSource::InternalPlay(ScriptCAudioSource* thisPtr)
	{
		thisPtr->GetHandle()->play();
	}

	void ScriptCAudioSource::InternalPause(ScriptCAudioSource* thisPtr)
	{
		thisPtr->GetHandle()->pause();
	}

	void ScriptCAudioSource::InternalStop(ScriptCAudioSource* thisPtr)
	{
		thisPtr->GetHandle()->stop();
	}

	AudioSourceState ScriptCAudioSource::InternalGetState(ScriptCAudioSource* thisPtr)
	{
		AudioSourceState tmp__output;
		tmp__output = thisPtr->GetHandle()->GetState();

		AudioSourceState __output;
		__output = tmp__output;

		return __output;
	}
}
