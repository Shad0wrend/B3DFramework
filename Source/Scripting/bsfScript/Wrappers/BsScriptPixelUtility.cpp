//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/BsScriptPixelUtility.h"
#include "BsMonoManager.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "Debug/BsDebug.h"
#include "Generated/BsScriptPixelData.generated.h"

namespace bs
{
	ScriptPixelUtility::ScriptPixelUtility(MonoObject* instance)
		:ScriptObject(instance)
	{ }

	void ScriptPixelUtility::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_GetMemorySize", (void*)&ScriptPixelUtility::InternalGetMemorySize);
		metaData.scriptClass->AddInternalCall("Internal_HasAlpha", (void*)&ScriptPixelUtility::InternalHasAlpha);
		metaData.scriptClass->AddInternalCall("Internal_IsFloatingPoint", (void*)&ScriptPixelUtility::InternalIsFloatingPoint);
		metaData.scriptClass->AddInternalCall("Internal_IsCompressed", (void*)&ScriptPixelUtility::InternalIsCompressed);
		metaData.scriptClass->AddInternalCall("Internal_IsDepth", (void*)&ScriptPixelUtility::InternalIsDepth);
		metaData.scriptClass->AddInternalCall("Internal_GetMaxMipmaps", (void*)&ScriptPixelUtility::InternalGetMaxMipmaps);
		metaData.scriptClass->AddInternalCall("Internal_ConvertFormat", (void*)&ScriptPixelUtility::InternalConvertFormat);
		metaData.scriptClass->AddInternalCall("Internal_Compress", (void*)&ScriptPixelUtility::InternalCompress);
		metaData.scriptClass->AddInternalCall("Internal_GenerateMipmaps", (void*)&ScriptPixelUtility::InternalGenerateMipmaps);
		metaData.scriptClass->AddInternalCall("Internal_Scale", (void*)&ScriptPixelUtility::InternalScale);
		metaData.scriptClass->AddInternalCall("Internal_LinearToSRGB", (void*)&ScriptPixelUtility::InternalLinearToSrgb);
		metaData.scriptClass->AddInternalCall("Internal_SRGBToLinear", (void*)&ScriptPixelUtility::InternalSrgbToLinear);
	}

	void ScriptPixelUtility::InternalGetMemorySize(UINT32 width, UINT32 height, UINT32 depth, PixelFormat format, UINT32* value)
	{
		*value = PixelUtil::GetMemorySize(width, height, depth, format);
	}

	void ScriptPixelUtility::InternalHasAlpha(PixelFormat format, bool* value)
	{
		*value = PixelUtil::HasAlpha(format);
	}

	void ScriptPixelUtility::InternalIsFloatingPoint(PixelFormat format, bool* value)
	{
		*value = PixelUtil::IsFloatingPoint(format);
	}

	void ScriptPixelUtility::InternalIsCompressed(PixelFormat format, bool* value)
	{
		*value = PixelUtil::IsCompressed(format);
	}

	void ScriptPixelUtility::InternalIsDepth(PixelFormat format, bool* value)
	{
		*value = PixelUtil::IsDepth(format);
	}

	void ScriptPixelUtility::InternalGetMaxMipmaps(UINT32 width, UINT32 height, UINT32 depth, PixelFormat format, UINT32* value)
	{
		*value = PixelUtil::GetMaxMipmaps(width, height, depth, format);
	}

	MonoObject* ScriptPixelUtility::InternalConvertFormat(MonoObject* source, PixelFormat newFormat)
	{
		ScriptPixelData* sourceScriptPixelData = ScriptPixelData::ToNative(source);
		if (sourceScriptPixelData == nullptr)
			return nullptr;

		SPtr<PixelData> sourcePixelData = sourceScriptPixelData->getInternal();
		SPtr<PixelData> outputData = bs_shared_ptr_new<PixelData>(sourcePixelData->getWidth(), sourcePixelData->getHeight(),
			sourcePixelData->getDepth(), newFormat);
		outputData->allocateInternalBuffer();

		PixelUtil::bulkPixelConversion(*sourcePixelData, *outputData);

		return ScriptPixelData::Create(outputData);
	}

	MonoObject* ScriptPixelUtility::InternalCompress(MonoObject* source, CompressionOptions* options)
	{
		ScriptPixelData* sourceScriptPixelData = ScriptPixelData::toNative(source);
		if (sourceScriptPixelData == nullptr)
			return nullptr;

		SPtr<PixelData> sourcePixelData = sourceScriptPixelData->getInternal();
		SPtr<PixelData> outputData = bs_shared_ptr_new<PixelData>(sourcePixelData->getWidth(), sourcePixelData->getHeight(),
			sourcePixelData->getDepth(), options->format);
		outputData->allocateInternalBuffer();

		PixelUtil::compress(*sourcePixelData, *outputData, *options);

		return ScriptPixelData::Create(outputData);
	}

	MonoArray* ScriptPixelUtility::InternalGenerateMipmaps(MonoObject* source, MipMapGenOptions* options)
	{
		ScriptPixelData* sourceScriptPixelData = ScriptPixelData::toNative(source);
		if (sourceScriptPixelData == nullptr)
			return nullptr;

		SPtr<PixelData> sourcePixelData = sourceScriptPixelData->getInternal();
		Vector<SPtr<PixelData>> mipmaps = PixelUtil::genMipmaps(*sourcePixelData, *options);

		UINT32 numElements = (UINT32)mipmaps.size();
		ScriptArray scriptArray(ScriptPixelData::getMetaData()->scriptClass->GetInternalClassInternal(), numElements);

		for (UINT32 i = 0; i < numElements; i++)
		{
			MonoObject* managedPixelData = ScriptPixelData::Create(mipmaps[i]);
			scriptArray.set(i, managedPixelData);
		}

		return scriptArray.getInternal();
	}

	MonoObject* ScriptPixelUtility::InternalScale(MonoObject* source, PixelVolume* newSize, PixelUtil::Filter filter)
	{
		ScriptPixelData* sourceScriptPixelData = ScriptPixelData::toNative(source);
		if (sourceScriptPixelData == nullptr)
			return nullptr;

		SPtr<PixelData> sourcePixelData = sourceScriptPixelData->getInternal();
		SPtr<PixelData> outputData = bs_shared_ptr_new<PixelData>(newSize->getWidth(), newSize->getHeight(),
			newSize->getDepth(), sourcePixelData->getFormat());
		outputData->allocateInternalBuffer();

		PixelUtil::scale(*sourcePixelData, *outputData, filter);

		return ScriptPixelData::Create(outputData);
	}

	void ScriptPixelUtility::InternalLinearToSrgb(MonoObject* source)
	{
		ScriptPixelData* sourceScriptPixelData = ScriptPixelData::toNative(source);
		if (sourceScriptPixelData == nullptr)
			return;

		SPtr<PixelData> pixelData = sourceScriptPixelData->getInternal();
		PixelUtil::linearToSRGB(*pixelData);
	}

	void ScriptPixelUtility::InternalSrgbToLinear(MonoObject* source)
	{
		ScriptPixelData* sourceScriptPixelData = ScriptPixelData::toNative(source);
		if (sourceScriptPixelData == nullptr)
			return;

		SPtr<PixelData> pixelData = sourceScriptPixelData->getInternal();
		PixelUtil::SRGBToLinear(*pixelData);
	}
}
