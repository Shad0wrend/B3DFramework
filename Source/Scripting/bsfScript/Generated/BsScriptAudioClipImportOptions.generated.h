//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "BsScriptImportOptions.generated.h"
#include "../../../Foundation/bsfCore/Audio/BsAudioClipImportOptions.h"
#include "../../../Foundation/bsfCore/Audio/BsAudioClip.h"
#include "../../../Foundation/bsfCore/Audio/BsAudioClip.h"

namespace bs { class AudioClipImportOptions; }
namespace bs
{
#if !BS_IS_BANSHEE3D
	class BS_SCR_BE_EXPORT ScriptAudioClipImportOptions : public TScriptReflectable<ScriptAudioClipImportOptions, AudioClipImportOptions, ScriptImportOptionsBase>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "AudioClipImportOptions")

		ScriptAudioClipImportOptions(MonoObject* managedInstance, const SPtr<AudioClipImportOptions>& value);

		static MonoObject* Create(const SPtr<AudioClipImportOptions>& value);

	private:
		static AudioFormat InternalGetformat(ScriptAudioClipImportOptions* thisPtr);
		static void InternalSetformat(ScriptAudioClipImportOptions* thisPtr, AudioFormat value);
		static AudioReadMode InternalGetreadMode(ScriptAudioClipImportOptions* thisPtr);
		static void InternalSetreadMode(ScriptAudioClipImportOptions* thisPtr, AudioReadMode value);
		static bool InternalGetis3D(ScriptAudioClipImportOptions* thisPtr);
		static void InternalSetis3D(ScriptAudioClipImportOptions* thisPtr, bool value);
		static uint32_t InternalGetbitDepth(ScriptAudioClipImportOptions* thisPtr);
		static void InternalSetbitDepth(ScriptAudioClipImportOptions* thisPtr, uint32_t value);
		static void InternalCreate(MonoObject* managedInstance);
	};
#endif
}
