//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Wrappers/BsScriptReflectable.h"
#include "BsScriptImportOptions.generated.h"
#include "../../../Foundation/bsfCore/Importer/BsTextureImportOptions.h"
#include "../../../Foundation/bsfCore/Image/BsPixelData.h"
#include "../../../Foundation/bsfCore/Utility/BsCommonTypes.h"

namespace bs { class TextureImportOptions; }
namespace bs
{
#if !BS_IS_BANSHEE3D
	class BS_SCR_BE_EXPORT ScriptTextureImportOptions : public TScriptReflectable<ScriptTextureImportOptions, TextureImportOptions, ScriptImportOptionsBase>
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, ENGINE_NS, "TextureImportOptions")

		ScriptTextureImportOptions(MonoObject* managedInstance, const SPtr<TextureImportOptions>& value);

		static MonoObject* Create(const SPtr<TextureImportOptions>& value);

	private:
		static PixelFormat InternalGetformat(ScriptTextureImportOptions* thisPtr);
		static void InternalSetformat(ScriptTextureImportOptions* thisPtr, PixelFormat value);
		static bool InternalGetgenerateMips(ScriptTextureImportOptions* thisPtr);
		static void InternalSetgenerateMips(ScriptTextureImportOptions* thisPtr, bool value);
		static uint32_t InternalGetmaxMip(ScriptTextureImportOptions* thisPtr);
		static void InternalSetmaxMip(ScriptTextureImportOptions* thisPtr, uint32_t value);
		static bool InternalGetcpuCached(ScriptTextureImportOptions* thisPtr);
		static void InternalSetcpuCached(ScriptTextureImportOptions* thisPtr, bool value);
		static bool InternalGetsRGB(ScriptTextureImportOptions* thisPtr);
		static void InternalSetsRGB(ScriptTextureImportOptions* thisPtr, bool value);
		static bool InternalGetcubemap(ScriptTextureImportOptions* thisPtr);
		static void InternalSetcubemap(ScriptTextureImportOptions* thisPtr, bool value);
		static CubemapSourceType InternalGetcubemapSourceType(ScriptTextureImportOptions* thisPtr);
		static void InternalSetcubemapSourceType(ScriptTextureImportOptions* thisPtr, CubemapSourceType value);
		static void InternalCreate(MonoObject* managedInstance);
	};
#endif
}
