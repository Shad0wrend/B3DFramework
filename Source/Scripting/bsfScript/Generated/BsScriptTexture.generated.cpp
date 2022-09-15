//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptTexture.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Image/BsTexture.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "Wrappers/BsScriptAsyncOp.h"
#include "BsScriptPixelData.generated.h"
#include "../../../Foundation/bsfCore/Image/BsTexture.h"
#include "Wrappers/BsScriptColor.h"
#include "../Extensions/BsTextureEx.h"

namespace bs
{
	ScriptTexture::ScriptTexture(MonoObject* managedInstance, const ResourceHandle<Texture>& value)
		:TScriptResource(managedInstance, value)
	{
	}

	void ScriptTexture::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_GetRef", (void*)&ScriptTexture::InternalGetRef);
		metaData.scriptClass->AddInternalCall("Internal_readData", (void*)&ScriptTexture::InternalReadData);
		metaData.scriptClass->AddInternalCall("Internal_create", (void*)&ScriptTexture::InternalCreate);
		metaData.scriptClass->AddInternalCall("Internal_getPixelFormat", (void*)&ScriptTexture::InternalGetPixelFormat);
		metaData.scriptClass->AddInternalCall("Internal_getUsage", (void*)&ScriptTexture::Internal_getUsage);
		metaData.scriptClass->addInternalCall("Internal_getType", (void*)&ScriptTexture::Internal_getType);
		metaData.scriptClass->addInternalCall("Internal_getWidth", (void*)&ScriptTexture::Internal_getWidth);
		metaData.scriptClass->addInternalCall("Internal_getHeight", (void*)&ScriptTexture::Internal_getHeight);
		metaData.scriptClass->addInternalCall("Internal_getDepth", (void*)&ScriptTexture::Internal_getDepth);
		metaData.scriptClass->addInternalCall("Internal_getGammaCorrection", (void*)&ScriptTexture::Internal_getGammaCorrection);
		metaData.scriptClass->addInternalCall("Internal_getSampleCount", (void*)&ScriptTexture::Internal_getSampleCount);
		metaData.scriptClass->addInternalCall("Internal_getMipmapCount", (void*)&ScriptTexture::Internal_getMipmapCount);
		metaData.scriptClass->addInternalCall("Internal_getPixels", (void*)&ScriptTexture::Internal_getPixels);
		metaData.scriptClass->addInternalCall("Internal_setPixels", (void*)&ScriptTexture::Internal_setPixels);
		metaData.scriptClass->addInternalCall("Internal_setPixelsArray", (void*)&ScriptTexture::Internal_setPixelsArray);

	}

	 MonoObject*ScriptTexture::CreateInstance()
	{
		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		return metaData.scriptClass->createInstance("bool", ctorParams);
	}
	MonoObject* ScriptTexture::InternalGetRef(ScriptTexture* thisPtr)
	{
		return thisPtr->GetRRef();
	}

	MonoObject* ScriptTexture::InternalReadData(ScriptTexture* thisPtr, uint32_t face, uint32_t mipLevel)
	{
		TAsyncOp<SPtr<PixelData>> tmp__output;
		tmp__output = thisPtr->GetHandle()->readData(face, mipLevel);

		MonoObject* __output;
		auto convertCallback = [](const Any& returnVal)
		{
			SPtr<PixelData> nativeObj = any_cast<SPtr<PixelData>>(returnVal);
			MonoObject* monoObj;
			monoObj = ScriptPixelData::Create(nativeObj);
			return monoObj;
		};

;		__output = ScriptAsyncOpBase::Create(tmp__output, convertCallback, ScriptPixelData::getMetaData()->scriptClass);

		return __output;
	}

	void ScriptTexture::InternalCreate(MonoObject* managedInstance, PixelFormat format, uint32_t width, uint32_t height, uint32_t depth, TextureType texType, TextureUsage usage, uint32_t numSamples, bool hasMipmaps, bool gammaCorrection)
	{
		ResourceHandle<Texture> instance = TextureEx::Create(format, width, height, depth, texType, usage, numSamples, hasMipmaps, gammaCorrection);
		ScriptResourceManager::Instance().createBuiltinScriptResource(instance, managedInstance);
	}

	PixelFormat ScriptTexture::InternalGetPixelFormat(ScriptTexture* thisPtr)
	{
		PixelFormat tmp__output;
		tmp__output = TextureEx::getPixelFormat(thisPtr->GetHandle());

		PixelFormat __output;
		__output = tmp__output;

		return __output;
	}

	TextureUsage ScriptTexture::InternalGetUsage(ScriptTexture* thisPtr)
	{
		TextureUsage tmp__output;
		tmp__output = TextureEx::getUsage(thisPtr->GetHandle());

		TextureUsage __output;
		__output = tmp__output;

		return __output;
	}

	TextureType ScriptTexture::InternalGetType(ScriptTexture* thisPtr)
	{
		TextureType tmp__output;
		tmp__output = TextureEx::getType(thisPtr->GetHandle());

		TextureType __output;
		__output = tmp__output;

		return __output;
	}

	uint32_t ScriptTexture::InternalGetWidth(ScriptTexture* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = TextureEx::getWidth(thisPtr->GetHandle());

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	uint32_t ScriptTexture::InternalGetHeight(ScriptTexture* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = TextureEx::getHeight(thisPtr->GetHandle());

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	uint32_t ScriptTexture::InternalGetDepth(ScriptTexture* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = TextureEx::getDepth(thisPtr->GetHandle());

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptTexture::InternalGetGammaCorrection(ScriptTexture* thisPtr)
	{
		bool tmp__output;
		tmp__output = TextureEx::getGammaCorrection(thisPtr->GetHandle());

		bool __output;
		__output = tmp__output;

		return __output;
	}

	uint32_t ScriptTexture::InternalGetSampleCount(ScriptTexture* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = TextureEx::getSampleCount(thisPtr->GetHandle());

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	uint32_t ScriptTexture::InternalGetMipmapCount(ScriptTexture* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = TextureEx::getMipmapCount(thisPtr->GetHandle());

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	MonoObject* ScriptTexture::InternalGetPixels(ScriptTexture* thisPtr, uint32_t face, uint32_t mipLevel)
	{
		SPtr<PixelData> tmp__output;
		tmp__output = TextureEx::getPixels(thisPtr->GetHandle(), face, mipLevel);

		MonoObject* __output;
		__output = ScriptPixelData::Create(tmp__output);

		return __output;
	}

	void ScriptTexture::InternalSetPixels(ScriptTexture* thisPtr, MonoObject* data, uint32_t face, uint32_t mipLevel)
	{
		SPtr<PixelData> tmpdata;
		ScriptPixelData* scriptdata;
		scriptdata = ScriptPixelData::toNative(data);
		if(scriptdata != nullptr)
			tmpdata = scriptdata->GetInternal();
		TextureEx::setPixels(thisPtr->GetHandle(), tmpdata, face, mipLevel);
	}

	void ScriptTexture::InternalSetPixelsArray(ScriptTexture* thisPtr, MonoArray* colors, uint32_t face, uint32_t mipLevel)
	{
		Vector<Color> veccolors;
		if(colors != nullptr)
		{
			ScriptArray arraycolors(colors);
			veccolors.resize(arraycolors.size());
			for(int i = 0; i < (int)arraycolors.size(); i++)
			{
				veccolors[i] = arraycolors.get<Color>(i);
			}

		}
		TextureEx::setPixelsArray(thisPtr->GetHandle(), veccolors, face, mipLevel);
	}
}
