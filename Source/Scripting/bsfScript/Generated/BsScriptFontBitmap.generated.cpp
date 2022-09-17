//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptFontBitmap.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "BsScriptCharDesc.generated.h"
#include "../../../Foundation/bsfCore/Image/BsTexture.h"

namespace bs
{
	ScriptFontBitmap::ScriptFontBitmap(MonoObject* managedInstance, const SPtr<FontBitmap>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptFontBitmap::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_getCharDesc", (void*)&ScriptFontBitmap::InternalGetCharDesc);
		metaData.scriptClass->AddInternalCall("Internal_getsize", (void*)&ScriptFontBitmap::InternalGetsize);
		metaData.scriptClass->AddInternalCall("Internal_setsize", (void*)&ScriptFontBitmap::InternalSetsize);
		metaData.scriptClass->AddInternalCall("Internal_getbaselineOffset", (void*)&ScriptFontBitmap::InternalGetbaselineOffset);
		metaData.scriptClass->AddInternalCall("Internal_setbaselineOffset", (void*)&ScriptFontBitmap::InternalSetbaselineOffset);
		metaData.scriptClass->AddInternalCall("Internal_getlineHeight", (void*)&ScriptFontBitmap::InternalGetlineHeight);
		metaData.scriptClass->AddInternalCall("Internal_setlineHeight", (void*)&ScriptFontBitmap::InternalSetlineHeight);
		metaData.scriptClass->AddInternalCall("Internal_getmissingGlyph", (void*)&ScriptFontBitmap::InternalGetmissingGlyph);
		metaData.scriptClass->AddInternalCall("Internal_setmissingGlyph", (void*)&ScriptFontBitmap::InternalSetmissingGlyph);
		metaData.scriptClass->AddInternalCall("Internal_getspaceWidth", (void*)&ScriptFontBitmap::InternalGetspaceWidth);
		metaData.scriptClass->AddInternalCall("Internal_setspaceWidth", (void*)&ScriptFontBitmap::InternalSetspaceWidth);
		metaData.scriptClass->AddInternalCall("Internal_gettexturePages", (void*)&ScriptFontBitmap::InternalGettexturePages);
		metaData.scriptClass->AddInternalCall("Internal_settexturePages", (void*)&ScriptFontBitmap::InternalSettexturePages);

	}

	MonoObject* ScriptFontBitmap::Create(const SPtr<FontBitmap>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptFontBitmap>()) ScriptFontBitmap(managedInstance, value);
		return managedInstance;
	}
	void ScriptFontBitmap::InternalGetCharDesc(ScriptFontBitmap* thisPtr, uint32_t charId, __CharDescInterop* __output)
	{
		CharDesc tmp__output;
		tmp__output = thisPtr->GetInternal()->GetCharDesc(charId);

		__CharDescInterop interop__output;
		interop__output = ScriptCharDesc::toInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptCharDesc::getMetaData()->scriptClass->GetInternalClassInternal());
	}

	uint32_t ScriptFontBitmap::InternalGetsize(ScriptFontBitmap* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->size;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptFontBitmap::InternalSetsize(ScriptFontBitmap* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->size = value;
	}

	int32_t ScriptFontBitmap::InternalGetbaselineOffset(ScriptFontBitmap* thisPtr)
	{
		int32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->baselineOffset;

		int32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptFontBitmap::InternalSetbaselineOffset(ScriptFontBitmap* thisPtr, int32_t value)
	{
		thisPtr->GetInternal()->baselineOffset = value;
	}

	uint32_t ScriptFontBitmap::InternalGetlineHeight(ScriptFontBitmap* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->lineHeight;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptFontBitmap::InternalSetlineHeight(ScriptFontBitmap* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->lineHeight = value;
	}

	void ScriptFontBitmap::InternalGetmissingGlyph(ScriptFontBitmap* thisPtr, __CharDescInterop* __output)
	{
		CharDesc tmp__output;
		tmp__output = thisPtr->GetInternal()->missingGlyph;

		__CharDescInterop interop__output;
		interop__output = ScriptCharDesc::toInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptCharDesc::getMetaData()->scriptClass->GetInternalClassInternal());


	}

	void ScriptFontBitmap::InternalSetmissingGlyph(ScriptFontBitmap* thisPtr, __CharDescInterop* value)
	{
		CharDesc tmpvalue;
		tmpvalue = ScriptCharDesc::fromInterop(*value);
		thisPtr->GetInternal()->missingGlyph = tmpvalue;
	}

	uint32_t ScriptFontBitmap::InternalGetspaceWidth(ScriptFontBitmap* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->spaceWidth;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptFontBitmap::InternalSetspaceWidth(ScriptFontBitmap* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->spaceWidth = value;
	}

	MonoArray* ScriptFontBitmap::InternalGettexturePages(ScriptFontBitmap* thisPtr)
	{
		Vector<ResourceHandle<Texture>> vec__output;
		vec__output = thisPtr->GetInternal()->texturePages;

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptRRefBase>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			ScriptRRefBase* script__output;
			script__output = ScriptResourceManager::Instance().getScriptRRef(vec__output[i]);
			if(script__output != nullptr)
				array__output.Set(i, script__output->GetManagedInstance());
			else
				array__output.Set(i, nullptr);
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptFontBitmap::InternalSettexturePages(ScriptFontBitmap* thisPtr, MonoArray* value)
	{
		Vector<ResourceHandle<Texture>> vecvalue;
		if(value != nullptr)
		{
			ScriptArray arrayvalue(value);
			vecvalue.resize(arrayvalue.size());
			for(int i = 0; i < (int)arrayvalue.size(); i++)
			{
				ScriptRRefBase* scriptvalue;
				scriptvalue = ScriptRRefBase::toNative(arrayvalue.get<MonoObject*>(i));
				if(scriptvalue != nullptr)
				{
					ResourceHandle<Texture> arrayElemPtrvalue = static_resource_cast<Texture>(scriptvalue->GetHandle());
					vecvalue[i] = arrayElemPtrvalue;
				}
			}

		}
		thisPtr->GetInternal()->texturePages = vecvalue;
	}
}
