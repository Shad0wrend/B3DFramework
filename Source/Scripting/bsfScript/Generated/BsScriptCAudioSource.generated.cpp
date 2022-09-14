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
			tmpclip = static_resource_cast<AudioClip>(scriptclip->getHandle());
		thisPtr->getHandle()->setClip(tmpclip);
	}

	MonoObject* ScriptCAudioSource::InternalGetClip(ScriptCAudioSource* thisPtr)
	{
		ResourceHandle<AudioClip> tmp__output;
		tmp__output = thisPtr->getHandle()->getClip();

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().getScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->getManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptCAudioSource::InternalSetVolume(ScriptCAudioSource* thisPtr, float volume)
	{
		thisPtr->getHandle()->setVolume(volume);
	}

	float ScriptCAudioSource::InternalGetVolume(ScriptCAudioSource* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getHandle()->getVolume();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCAudioSource::InternalSetPitch(ScriptCAudioSource* thisPtr, float pitch)
	{
		thisPtr->getHandle()->setPitch(pitch);
	}

	float ScriptCAudioSource::InternalGetPitch(ScriptCAudioSource* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getHandle()->getPitch();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCAudioSource::InternalSetIsLooping(ScriptCAudioSource* thisPtr, bool loop)
	{
		thisPtr->getHandle()->setIsLooping(loop);
	}

	bool ScriptCAudioSource::InternalGetIsLooping(ScriptCAudioSource* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->getHandle()->getIsLooping();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCAudioSource::InternalSetPriority(ScriptCAudioSource* thisPtr, uint32_t priority)
	{
		thisPtr->getHandle()->setPriority(priority);
	}

	uint32_t ScriptCAudioSource::InternalGetPriority(ScriptCAudioSource* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->getHandle()->getPriority();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCAudioSource::InternalSetMinDistance(ScriptCAudioSource* thisPtr, float distance)
	{
		thisPtr->getHandle()->setMinDistance(distance);
	}

	float ScriptCAudioSource::InternalGetMinDistance(ScriptCAudioSource* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getHandle()->getMinDistance();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCAudioSource::InternalSetAttenuation(ScriptCAudioSource* thisPtr, float attenuation)
	{
		thisPtr->getHandle()->setAttenuation(attenuation);
	}

	float ScriptCAudioSource::InternalGetAttenuation(ScriptCAudioSource* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getHandle()->getAttenuation();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCAudioSource::InternalSetTime(ScriptCAudioSource* thisPtr, float time)
	{
		thisPtr->getHandle()->setTime(time);
	}

	float ScriptCAudioSource::InternalGetTime(ScriptCAudioSource* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getHandle()->getTime();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCAudioSource::InternalSetPlayOnStart(ScriptCAudioSource* thisPtr, bool enable)
	{
		thisPtr->getHandle()->setPlayOnStart(enable);
	}

	bool ScriptCAudioSource::InternalGetPlayOnStart(ScriptCAudioSource* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->getHandle()->getPlayOnStart();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCAudioSource::InternalPlay(ScriptCAudioSource* thisPtr)
	{
		thisPtr->getHandle()->play();
	}

	void ScriptCAudioSource::InternalPause(ScriptCAudioSource* thisPtr)
	{
		thisPtr->getHandle()->pause();
	}

	void ScriptCAudioSource::InternalStop(ScriptCAudioSource* thisPtr)
	{
		thisPtr->getHandle()->stop();
	}

	AudioSourceState ScriptCAudioSource::InternalGetState(ScriptCAudioSource* thisPtr)
	{
		AudioSourceState tmp__output;
		tmp__output = thisPtr->getHandle()->getState();

		AudioSourceState __output;
		__output = tmp__output;

		return __output;
	}
}
