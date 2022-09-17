//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptImporter.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Importer/BsImporter.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "Wrappers/BsScriptAsyncOp.h"
#include "Reflection/BsRTTIType.h"
#include "../../../Foundation/bsfCore/Text/BsFontImportOptions.h"
#include "BsScriptFontImportOptions.generated.h"
#include "../../../Foundation/bsfCore/Importer/BsShaderImportOptions.h"
#include "BsScriptShaderImportOptions.generated.h"
#include "Wrappers/BsScriptResource.h"
#include "BsScriptImportOptions.generated.h"
#include "../../../Foundation/bsfEngine/Resources/BsScriptCodeImportOptions.h"
#include "BsScriptScriptCodeImportOptions.generated.h"
#include "../../../Foundation/bsfCore/Audio/BsAudioClipImportOptions.h"
#include "BsScriptAudioClipImportOptions.generated.h"
#include "../../../Foundation/bsfCore/Importer/BsMeshImportOptions.h"
#include "BsScriptMeshImportOptions.generated.h"
#include "../../../Foundation/bsfCore/Importer/BsTextureImportOptions.h"
#include "BsScriptTextureImportOptions.generated.h"
#include "BsScriptMultiResource.generated.h"

namespace bs
{
#if !BS_IS_BANSHEE3D
	ScriptImporter::ScriptImporter(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{
	}

	void ScriptImporter::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_import", (void*)&ScriptImporter::InternalImport);
		metaData.scriptClass->AddInternalCall("Internal_importAsync", (void*)&ScriptImporter::InternalImportAsync);
		metaData.scriptClass->AddInternalCall("Internal_importAll", (void*)&ScriptImporter::InternalImportAll);
		metaData.scriptClass->AddInternalCall("Internal_importAllAsync", (void*)&ScriptImporter::InternalImportAllAsync);
		metaData.scriptClass->AddInternalCall("Internal_supportsFileType", (void*)&ScriptImporter::InternalSupportsFileType);

	}

	MonoObject* ScriptImporter::InternalImport(MonoString* inputFilePath, MonoObject* importOptions, UUID* UUID)
	{
		ResourceHandle<Resource> tmp__output;
		Path tmpinputFilePath;
		tmpinputFilePath = MonoUtil::MonoToString(inputFilePath);
		SPtr<ImportOptions> tmpimportOptions;
		ScriptImportOptionsBase* scriptimportOptions;
		scriptimportOptions = (ScriptImportOptionsBase*)ScriptImportOptions::toNative(importOptions);
		if(scriptimportOptions != nullptr)
			tmpimportOptions = scriptimportOptions->GetInternal();
		tmp__output = Importer::Instance().import(tmpinputFilePath, tmpimportOptions, *UUID);

		MonoObject* __output;
		ScriptResourceBase* script__output;
		script__output = ScriptResourceManager::Instance().getScriptResource(tmp__output, true);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	MonoObject* ScriptImporter::InternalImportAsync(MonoString* inputFilePath, MonoObject* importOptions, UUID* UUID)
	{
		TAsyncOp<ResourceHandle<Resource>> tmp__output;
		Path tmpinputFilePath;
		tmpinputFilePath = MonoUtil::MonoToString(inputFilePath);
		SPtr<ImportOptions> tmpimportOptions;
		ScriptImportOptionsBase* scriptimportOptions;
		scriptimportOptions = (ScriptImportOptionsBase*)ScriptImportOptions::toNative(importOptions);
		if(scriptimportOptions != nullptr)
			tmpimportOptions = scriptimportOptions->GetInternal();
		tmp__output = Importer::Instance().importAsync(tmpinputFilePath, tmpimportOptions, *UUID);

		MonoObject* __output;
		auto convertCallback = [](const Any& returnVal)
		{
			ResourceHandle<Resource> nativeObj = any_cast<ResourceHandle<Resource>>(returnVal);
			MonoObject* monoObj;
			ScriptRRefBase* scriptObj;
			scriptObj = ScriptResourceManager::Instance().getScriptRRef(nativeObj);
			if(scriptObj != nullptr)
				monoObj = scriptObj->GetManagedInstance();
			else
				monoObj = nullptr;
			return monoObj;
		};

;		__output = ScriptAsyncOpBase::Create(tmp__output, convertCallback, ScriptRRefBase::getMetaData()->scriptClass);

		return __output;
	}

	MonoObject* ScriptImporter::InternalImportAll(MonoString* inputFilePath, MonoObject* importOptions)
	{
		SPtr<MultiResource> tmp__output;
		Path tmpinputFilePath;
		tmpinputFilePath = MonoUtil::MonoToString(inputFilePath);
		SPtr<ImportOptions> tmpimportOptions;
		ScriptImportOptionsBase* scriptimportOptions;
		scriptimportOptions = (ScriptImportOptionsBase*)ScriptImportOptions::toNative(importOptions);
		if(scriptimportOptions != nullptr)
			tmpimportOptions = scriptimportOptions->GetInternal();
		tmp__output = Importer::Instance().importAll(tmpinputFilePath, tmpimportOptions);

		MonoObject* __output;
		__output = ScriptMultiResource::Create(tmp__output);

		return __output;
	}

	MonoObject* ScriptImporter::InternalImportAllAsync(MonoString* inputFilePath, MonoObject* importOptions)
	{
		TAsyncOp<SPtr<MultiResource>> tmp__output;
		Path tmpinputFilePath;
		tmpinputFilePath = MonoUtil::MonoToString(inputFilePath);
		SPtr<ImportOptions> tmpimportOptions;
		ScriptImportOptionsBase* scriptimportOptions;
		scriptimportOptions = (ScriptImportOptionsBase*)ScriptImportOptions::toNative(importOptions);
		if(scriptimportOptions != nullptr)
			tmpimportOptions = scriptimportOptions->GetInternal();
		tmp__output = Importer::Instance().importAllAsync(tmpinputFilePath, tmpimportOptions);

		MonoObject* __output;
		auto convertCallback = [](const Any& returnVal)
		{
			SPtr<MultiResource> nativeObj = any_cast<SPtr<MultiResource>>(returnVal);
			MonoObject* monoObj;
			monoObj = ScriptMultiResource::Create(nativeObj);
			return monoObj;
		};

;		__output = ScriptAsyncOpBase::Create(tmp__output, convertCallback, ScriptMultiResource::getMetaData()->scriptClass);

		return __output;
	}

	bool ScriptImporter::InternalSupportsFileType(MonoString* extension)
	{
		bool tmp__output;
		String tmpextension;
		tmpextension = MonoUtil::MonoToString(extension);
		tmp__output = Importer::Instance().supportsFileType(tmpextension);

		bool __output;
		__output = tmp__output;

		return __output;
	}

#endif
}
