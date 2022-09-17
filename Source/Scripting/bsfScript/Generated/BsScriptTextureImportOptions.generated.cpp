//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptTextureImportOptions.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptTextureImportOptions.generated.h"

namespace bs
{
#if !BS_IS_BANSHEE3D
	ScriptTextureImportOptions::ScriptTextureImportOptions(MonoObject* managedInstance, const SPtr<TextureImportOptions>& value)
		:TScriptReflectable(managedInstance, value)
	{
		mInternal = value;
	}

	void ScriptTextureImportOptions::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_getformat", (void*)&ScriptTextureImportOptions::InternalGetformat);
		metaData.scriptClass->AddInternalCall("Internal_setformat", (void*)&ScriptTextureImportOptions::InternalSetformat);
		metaData.scriptClass->AddInternalCall("Internal_getgenerateMips", (void*)&ScriptTextureImportOptions::InternalGetgenerateMips);
		metaData.scriptClass->AddInternalCall("Internal_setgenerateMips", (void*)&ScriptTextureImportOptions::InternalSetgenerateMips);
		metaData.scriptClass->AddInternalCall("Internal_getmaxMip", (void*)&ScriptTextureImportOptions::InternalGetmaxMip);
		metaData.scriptClass->AddInternalCall("Internal_setmaxMip", (void*)&ScriptTextureImportOptions::InternalSetmaxMip);
		metaData.scriptClass->AddInternalCall("Internal_getcpuCached", (void*)&ScriptTextureImportOptions::InternalGetcpuCached);
		metaData.scriptClass->AddInternalCall("Internal_setcpuCached", (void*)&ScriptTextureImportOptions::InternalSetcpuCached);
		metaData.scriptClass->AddInternalCall("Internal_getsRGB", (void*)&ScriptTextureImportOptions::InternalGetsRGB);
		metaData.scriptClass->AddInternalCall("Internal_setsRGB", (void*)&ScriptTextureImportOptions::InternalSetsRGB);
		metaData.scriptClass->AddInternalCall("Internal_getcubemap", (void*)&ScriptTextureImportOptions::InternalGetcubemap);
		metaData.scriptClass->AddInternalCall("Internal_setcubemap", (void*)&ScriptTextureImportOptions::InternalSetcubemap);
		metaData.scriptClass->AddInternalCall("Internal_getcubemapSourceType", (void*)&ScriptTextureImportOptions::InternalGetcubemapSourceType);
		metaData.scriptClass->AddInternalCall("Internal_setcubemapSourceType", (void*)&ScriptTextureImportOptions::InternalSetcubemapSourceType);
		metaData.scriptClass->AddInternalCall("Internal_create", (void*)&ScriptTextureImportOptions::InternalCreate);

	}

	MonoObject* ScriptTextureImportOptions::Create(const SPtr<TextureImportOptions>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptTextureImportOptions>()) ScriptTextureImportOptions(managedInstance, value);
		return managedInstance;
	}
	void ScriptTextureImportOptions::InternalCreate(MonoObject* managedInstance)
	{
		SPtr<TextureImportOptions> instance = TextureImportOptions::Create();
		new (bs_alloc<ScriptTextureImportOptions>())ScriptTextureImportOptions(managedInstance, instance);
	}
	PixelFormat ScriptTextureImportOptions::InternalGetformat(ScriptTextureImportOptions* thisPtr)
	{
		PixelFormat tmp__output;
		tmp__output = thisPtr->GetInternal()->format;

		PixelFormat __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTextureImportOptions::InternalSetformat(ScriptTextureImportOptions* thisPtr, PixelFormat value)
	{
		thisPtr->GetInternal()->format = value;
	}

	bool ScriptTextureImportOptions::InternalGetgenerateMips(ScriptTextureImportOptions* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->generateMips;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTextureImportOptions::InternalSetgenerateMips(ScriptTextureImportOptions* thisPtr, bool value)
	{
		thisPtr->GetInternal()->generateMips = value;
	}

	uint32_t ScriptTextureImportOptions::InternalGetmaxMip(ScriptTextureImportOptions* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->maxMip;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTextureImportOptions::InternalSetmaxMip(ScriptTextureImportOptions* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->maxMip = value;
	}

	bool ScriptTextureImportOptions::InternalGetcpuCached(ScriptTextureImportOptions* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->cpuCached;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTextureImportOptions::InternalSetcpuCached(ScriptTextureImportOptions* thisPtr, bool value)
	{
		thisPtr->GetInternal()->cpuCached = value;
	}

	bool ScriptTextureImportOptions::InternalGetsRGB(ScriptTextureImportOptions* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->sRGB;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTextureImportOptions::InternalSetsRGB(ScriptTextureImportOptions* thisPtr, bool value)
	{
		thisPtr->GetInternal()->sRGB = value;
	}

	bool ScriptTextureImportOptions::InternalGetcubemap(ScriptTextureImportOptions* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->cubemap;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTextureImportOptions::InternalSetcubemap(ScriptTextureImportOptions* thisPtr, bool value)
	{
		thisPtr->GetInternal()->cubemap = value;
	}

	CubemapSourceType ScriptTextureImportOptions::InternalGetcubemapSourceType(ScriptTextureImportOptions* thisPtr)
	{
		CubemapSourceType tmp__output;
		tmp__output = thisPtr->GetInternal()->cubemapSourceType;

		CubemapSourceType __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTextureImportOptions::InternalSetcubemapSourceType(ScriptTextureImportOptions* thisPtr, CubemapSourceType value)
	{
		thisPtr->GetInternal()->cubemapSourceType = value;
	}
#endif
}
