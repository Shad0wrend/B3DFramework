//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptAudio.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Audio/BsAudio.h"
#include "BsScriptAudioDevice.generated.h"

namespace bs
{
	ScriptAudio::ScriptAudio(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{
	}

	void ScriptAudio::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_setVolume", (void*)&ScriptAudio::InternalSetVolume);
		metaData.scriptClass->AddInternalCall("Internal_getVolume", (void*)&ScriptAudio::InternalGetVolume);
		metaData.scriptClass->AddInternalCall("Internal_setPaused", (void*)&ScriptAudio::InternalSetPaused);
		metaData.scriptClass->AddInternalCall("Internal_isPaused", (void*)&ScriptAudio::InternalIsPaused);
		metaData.scriptClass->AddInternalCall("Internal_setActiveDevice", (void*)&ScriptAudio::InternalSetActiveDevice);
		metaData.scriptClass->AddInternalCall("Internal_getActiveDevice", (void*)&ScriptAudio::InternalGetActiveDevice);
		metaData.scriptClass->AddInternalCall("Internal_getDefaultDevice", (void*)&ScriptAudio::InternalGetDefaultDevice);
		metaData.scriptClass->AddInternalCall("Internal_getAllDevices", (void*)&ScriptAudio::InternalGetAllDevices);

	}

	void ScriptAudio::InternalSetVolume(float volume)
	{
		Audio::Instance().SetVolume(volume);
	}

	float ScriptAudio::InternalGetVolume()
	{
		float tmp__output;
		tmp__output = Audio::Instance().GetVolume();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAudio::InternalSetPaused(bool paused)
	{
		Audio::Instance().SetPaused(paused);
	}

	bool ScriptAudio::InternalIsPaused()
	{
		bool tmp__output;
		tmp__output = Audio::Instance().IsPaused();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAudio::InternalSetActiveDevice(__AudioDeviceInterop* device)
	{
		AudioDevice tmpdevice;
		tmpdevice = ScriptAudioDevice::FromInterop(*device);
		Audio::Instance().SetActiveDevice(tmpdevice);
	}

	void ScriptAudio::InternalGetActiveDevice(__AudioDeviceInterop* __output)
	{
		AudioDevice tmp__output;
		tmp__output = Audio::Instance().getActiveDevice();

		__AudioDeviceInterop interop__output;
		interop__output = ScriptAudioDevice::toInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptAudioDevice::getMetaData()->scriptClass->GetInternalClassInternal());
	}

	void ScriptAudio::InternalGetDefaultDevice(__AudioDeviceInterop* __output)
	{
		AudioDevice tmp__output;
		tmp__output = Audio::Instance().getDefaultDevice();

		__AudioDeviceInterop interop__output;
		interop__output = ScriptAudioDevice::toInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptAudioDevice::getMetaData()->scriptClass->GetInternalClassInternal());
	}

	MonoArray* ScriptAudio::InternalGetAllDevices()
	{
		Vector<AudioDevice> vec__output;
		vec__output = Audio::Instance().getAllDevices();

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptAudioDevice>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.set(i, ScriptAudioDevice::toInterop(vec__output[i]));
		}
		__output = array__output.getInternal();

		return __output;
	}
}
