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

	void ScriptFontBitmap::InitRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_GetCharDesc", (void*)&ScriptFontBitmap::InternalGetCharDesc);
		metaData.scriptClass->AddInternalCall("Internal_Getsize", (void*)&ScriptFontBitmap::InternalGetsize);
		metaData.scriptClass->AddInternalCall("Internal_Setsize", (void*)&ScriptFontBitmap::InternalSetsize);
		metaData.scriptClass->AddInternalCall("Internal_GetbaselineOffset", (void*)&ScriptFontBitmap::InternalGetbaselineOffset);
		metaData.scriptClass->AddInternalCall("Internal_SetbaselineOffset", (void*)&ScriptFontBitmap::InternalSetbaselineOffset);
		metaData.scriptClass->AddInternalCall("Internal_GetlineHeight", (void*)&ScriptFontBitmap::InternalGetlineHeight);
		metaData.scriptClass->AddInternalCall("Internal_SetlineHeight", (void*)&ScriptFontBitmap::InternalSetlineHeight);
		metaData.scriptClass->AddInternalCall("Internal_GetmissingGlyph", (void*)&ScriptFontBitmap::InternalGetmissingGlyph);
		metaData.scriptClass->AddInternalCall("Internal_SetmissingGlyph", (void*)&ScriptFontBitmap::InternalSetmissingGlyph);
		metaData.scriptClass->AddInternalCall("Internal_GetspaceWidth", (void*)&ScriptFontBitmap::InternalGetspaceWidth);
		metaData.scriptClass->AddInternalCall("Internal_SetspaceWidth", (void*)&ScriptFontBitmap::InternalSetspaceWidth);
		metaData.scriptClass->AddInternalCall("Internal_GettexturePages", (void*)&ScriptFontBitmap::InternalGettexturePages);
		metaData.scriptClass->AddInternalCall("Internal_SettexturePages", (void*)&ScriptFontBitmap::InternalSettexturePages);

	}

	MonoObject* ScriptFontBitmap::Create(const SPtr<FontBitmap>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptFontBitmap>()) ScriptFontBitmap(managedInstance, value);
		return managedInstance;
	}
	void ScriptFontBitmap::InternalGetCharDesc(ScriptFontBitmap* thisPtr, uint32_t charId, __CharDescInterop* __output)
	{
		CharDesc tmp__output;
		tmp__output = thisPtr->GetInternal()->GetCharDesc(charId);

		__CharDescInterop interop__output;
		interop__output = ScriptCharDesc::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptCharDesc::GetMetaData()->scriptClass->GetInternalClassInternal());
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
		interop__output = ScriptCharDesc::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptCharDesc::GetMetaData()->scriptClass->GetInternalClassInternal());


	}

	void ScriptFontBitmap::InternalSetmissingGlyph(ScriptFontBitmap* thisPtr, __CharDescInterop* value)
	{
		CharDesc tmpvalue;
		tmpvalue = ScriptCharDesc::FromInterop(*value);
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
		ScriptArray array__output = ScriptArray::Create<ScriptRRefBase>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			ScriptRRefBase* script__output;
			script__output = ScriptResourceManager::Instance().GetScriptRRef(vec__output[i]);
			if(script__output != nullptr)
				array__output.Set(i, script__output->GetManagedInstance());
			else
				array__output.Set(i, nullptr);
		}
		__output = array__output.GetInternal();

		return __output;
	}

	void ScriptFontBitmap::InternalSettexturePages(ScriptFontBitmap* thisPtr, MonoArray* value)
	{
		Vector<ResourceHandle<Texture>> vecvalue;
		if(value != nullptr)
		{
			ScriptArray arrayvalue(value);
			vecvalue.resize(arrayvalue.Size());
			for(int i = 0; i < (int)arrayvalue.Size(); i++)
			{
				ScriptRRefBase* scriptvalue;
				scriptvalue = ScriptRRefBase::ToNative(arrayvalue.Get<MonoObject*>(i));
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
