//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptPixelData.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "Wrappers/BsScriptColor.h"
#include "BsScriptPixelVolume.generated.h"
#include "BsScriptPixelData.generated.h"
#include "../Extensions/BsPixelDataEx.h"

namespace bs
{
	ScriptPixelData::ScriptPixelData(MonoObject* managedInstance, const SPtr<PixelData>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptPixelData::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_getRowPitch", (void*)&ScriptPixelData::InternalGetRowPitch);
		metaData.scriptClass->AddInternalCall("Internal_getSlicePitch", (void*)&ScriptPixelData::InternalGetSlicePitch);
		metaData.scriptClass->AddInternalCall("Internal_getFormat", (void*)&ScriptPixelData::InternalGetFormat);
		metaData.scriptClass->AddInternalCall("Internal_getExtents", (void*)&ScriptPixelData::InternalGetExtents);
		metaData.scriptClass->AddInternalCall("Internal_isConsecutive", (void*)&ScriptPixelData::InternalIsConsecutive);
		metaData.scriptClass->AddInternalCall("Internal_getSize", (void*)&ScriptPixelData::InternalGetSize);
		metaData.scriptClass->AddInternalCall("Internal_create", (void*)&ScriptPixelData::InternalCreate);
		metaData.scriptClass->AddInternalCall("Internal_create0", (void*)&ScriptPixelData::InternalCreate0);
		metaData.scriptClass->addInternalCall("Internal_getPixel", (void*)&ScriptPixelData::Internal_getPixel);
		metaData.scriptClass->addInternalCall("Internal_setPixel", (void*)&ScriptPixelData::Internal_setPixel);
		metaData.scriptClass->addInternalCall("Internal_getPixels", (void*)&ScriptPixelData::Internal_getPixels);
		metaData.scriptClass->addInternalCall("Internal_setPixels", (void*)&ScriptPixelData::Internal_setPixels);
		metaData.scriptClass->addInternalCall("Internal_getRawPixels", (void*)&ScriptPixelData::Internal_getRawPixels);
		metaData.scriptClass->addInternalCall("Internal_setRawPixels", (void*)&ScriptPixelData::Internal_setRawPixels);

	}

	MonoObject* ScriptPixelData::Create(const SPtr<PixelData>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptPixelData>()) ScriptPixelData(managedInstance, value);
		return managedInstance;
	}
	uint32_t ScriptPixelData::InternalGetRowPitch(ScriptPixelData* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->getInternal()->getRowPitch();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	uint32_t ScriptPixelData::InternalGetSlicePitch(ScriptPixelData* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->getInternal()->getSlicePitch();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	PixelFormat ScriptPixelData::InternalGetFormat(ScriptPixelData* thisPtr)
	{
		PixelFormat tmp__output;
		tmp__output = thisPtr->getInternal()->getFormat();

		PixelFormat __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptPixelData::InternalGetExtents(ScriptPixelData* thisPtr, PixelVolume* __output)
	{
		PixelVolume tmp__output;
		tmp__output = thisPtr->getInternal()->getExtents();

		*__output = tmp__output;
	}

	bool ScriptPixelData::InternalIsConsecutive(ScriptPixelData* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->getInternal()->isConsecutive();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	uint32_t ScriptPixelData::InternalGetSize(ScriptPixelData* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->getInternal()->getSize();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptPixelData::InternalCreate(MonoObject* managedInstance, PixelVolume* volume, PixelFormat format)
	{
		SPtr<PixelData> instance = PixelDataEx::Create(*volume, format);
		new (bs_alloc<ScriptPixelData>())ScriptPixelData(managedInstance, instance);
	}

	void ScriptPixelData::InternalCreate0(MonoObject* managedInstance, uint32_t width, uint32_t height, uint32_t depth, PixelFormat pixelFormat)
	{
		SPtr<PixelData> instance = PixelDataEx::Create(width, height, depth, pixelFormat);
		new (bs_alloc<ScriptPixelData>())ScriptPixelData(managedInstance, instance);
	}

	void ScriptPixelData::InternalGetPixel(ScriptPixelData* thisPtr, int32_t x, int32_t y, int32_t z, Color* __output)
	{
		Color tmp__output;
		tmp__output = PixelDataEx::getPixel(thisPtr->getInternal(), x, y, z);

		*__output = tmp__output;
	}

	void ScriptPixelData::InternalSetPixel(ScriptPixelData* thisPtr, Color* value, int32_t x, int32_t y, int32_t z)
	{
		PixelDataEx::setPixel(thisPtr->getInternal(), *value, x, y, z);
	}

	MonoArray* ScriptPixelData::InternalGetPixels(ScriptPixelData* thisPtr)
	{
		Vector<Color> vec__output;
		vec__output = PixelDataEx::getPixels(thisPtr->getInternal());

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptColor>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.set(i, vec__output[i]);
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptPixelData::InternalSetPixels(ScriptPixelData* thisPtr, MonoArray* value)
	{
		Vector<Color> vecvalue;
		if(value != nullptr)
		{
			ScriptArray arrayvalue(value);
			vecvalue.resize(arrayvalue.size());
			for(int i = 0; i < (int)arrayvalue.size(); i++)
			{
				vecvalue[i] = arrayvalue.get<Color>(i);
			}
		}
		PixelDataEx::setPixels(thisPtr->getInternal(), vecvalue);
	}

	MonoArray* ScriptPixelData::InternalGetRawPixels(ScriptPixelData* thisPtr)
	{
		Vector<char> vec__output;
		vec__output = PixelDataEx::getRawPixels(thisPtr->getInternal());

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<char>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.set(i, vec__output[i]);
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptPixelData::InternalSetRawPixels(ScriptPixelData* thisPtr, MonoArray* value)
	{
		Vector<char> vecvalue;
		if(value != nullptr)
		{
			ScriptArray arrayvalue(value);
			vecvalue.resize(arrayvalue.size());
			for(int i = 0; i < (int)arrayvalue.size(); i++)
			{
				vecvalue[i] = arrayvalue.get<char>(i);
			}
		}
		PixelDataEx::setRawPixels(thisPtr->getInternal(), vecvalue);
	}
}
