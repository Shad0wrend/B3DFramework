//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptAudioClip.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Audio/BsAudioClip.h"

namespace bs
{
	ScriptAudioClip::ScriptAudioClip(MonoObject* managedInstance, const ResourceHandle<AudioClip>& value)
		:TScriptResource(managedInstance, value)
	{
	}

	void ScriptAudioClip::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_GetRef", (void*)&ScriptAudioClip::InternalGetRef);
		metaData.scriptClass->AddInternalCall("Internal_getBitDepth", (void*)&ScriptAudioClip::InternalGetBitDepth);
		metaData.scriptClass->AddInternalCall("Internal_getFrequency", (void*)&ScriptAudioClip::InternalGetFrequency);
		metaData.scriptClass->AddInternalCall("Internal_getNumChannels", (void*)&ScriptAudioClip::InternalGetNumChannels);
		metaData.scriptClass->AddInternalCall("Internal_getFormat", (void*)&ScriptAudioClip::InternalGetFormat);
		metaData.scriptClass->AddInternalCall("Internal_getReadMode", (void*)&ScriptAudioClip::InternalGetReadMode);
		metaData.scriptClass->AddInternalCall("Internal_getLength", (void*)&ScriptAudioClip::InternalGetLength);
		metaData.scriptClass->AddInternalCall("Internal_getNumSamples", (void*)&ScriptAudioClip::InternalGetNumSamples);
		metaData.scriptClass->AddInternalCall("Internal_is3D", (void*)&ScriptAudioClip::InternalIs3D);

	}

	 MonoObject*ScriptAudioClip::CreateInstance()
	{
		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		return metaData.scriptClass->createInstance("bool", ctorParams);
	}
	MonoObject* ScriptAudioClip::InternalGetRef(ScriptAudioClip* thisPtr)
	{
		return thisPtr->GetRRef();
	}

	uint32_t ScriptAudioClip::InternalGetBitDepth(ScriptAudioClip* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetHandle()->GetBitDepth();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	uint32_t ScriptAudioClip::InternalGetFrequency(ScriptAudioClip* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetHandle()->GetFrequency();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	uint32_t ScriptAudioClip::InternalGetNumChannels(ScriptAudioClip* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetHandle()->GetNumChannels();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	AudioFormat ScriptAudioClip::InternalGetFormat(ScriptAudioClip* thisPtr)
	{
		AudioFormat tmp__output;
		tmp__output = thisPtr->GetHandle()->GetFormat();

		AudioFormat __output;
		__output = tmp__output;

		return __output;
	}

	AudioReadMode ScriptAudioClip::InternalGetReadMode(ScriptAudioClip* thisPtr)
	{
		AudioReadMode tmp__output;
		tmp__output = thisPtr->GetHandle()->GetReadMode();

		AudioReadMode __output;
		__output = tmp__output;

		return __output;
	}

	float ScriptAudioClip::InternalGetLength(ScriptAudioClip* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetLength();

		float __output;
		__output = tmp__output;

		return __output;
	}

	uint32_t ScriptAudioClip::InternalGetNumSamples(ScriptAudioClip* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetHandle()->GetNumSamples();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptAudioClip::InternalIs3D(ScriptAudioClip* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetHandle()->is3D();

		bool __output;
		__output = tmp__output;

		return __output;
	}
}
