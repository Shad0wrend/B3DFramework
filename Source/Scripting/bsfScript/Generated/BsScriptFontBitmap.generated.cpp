//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptFontBitmap.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "BsScriptCharacterInformation.generated.h"
#include "../../../Foundation/bsfCore/Image/BsTexture.h"

namespace bs
{
	ScriptFontBitmap::ScriptFontBitmap(MonoObject* managedInstance, const SPtr<FontBitmap>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptFontBitmap::InitRuntimeData()
	{
		metaData.ScriptClass->AddInternalCall("Internal_GetCharacterInformation", (void*)&ScriptFontBitmap::InternalGetCharacterInformation);
		metaData.ScriptClass->AddInternalCall("Internal_GetSize", (void*)&ScriptFontBitmap::InternalGetSize);
		metaData.ScriptClass->AddInternalCall("Internal_SetSize", (void*)&ScriptFontBitmap::InternalSetSize);
		metaData.ScriptClass->AddInternalCall("Internal_GetBaselineOffset", (void*)&ScriptFontBitmap::InternalGetBaselineOffset);
		metaData.ScriptClass->AddInternalCall("Internal_SetBaselineOffset", (void*)&ScriptFontBitmap::InternalSetBaselineOffset);
		metaData.ScriptClass->AddInternalCall("Internal_GetLineHeight", (void*)&ScriptFontBitmap::InternalGetLineHeight);
		metaData.ScriptClass->AddInternalCall("Internal_SetLineHeight", (void*)&ScriptFontBitmap::InternalSetLineHeight);
		metaData.ScriptClass->AddInternalCall("Internal_GetMissingGlyph", (void*)&ScriptFontBitmap::InternalGetMissingGlyph);
		metaData.ScriptClass->AddInternalCall("Internal_SetMissingGlyph", (void*)&ScriptFontBitmap::InternalSetMissingGlyph);
		metaData.ScriptClass->AddInternalCall("Internal_GetSpaceWidth", (void*)&ScriptFontBitmap::InternalGetSpaceWidth);
		metaData.ScriptClass->AddInternalCall("Internal_SetSpaceWidth", (void*)&ScriptFontBitmap::InternalSetSpaceWidth);
		metaData.ScriptClass->AddInternalCall("Internal_GetTexturePages", (void*)&ScriptFontBitmap::InternalGetTexturePages);
		metaData.ScriptClass->AddInternalCall("Internal_SetTexturePages", (void*)&ScriptFontBitmap::InternalSetTexturePages);

	}

	MonoObject* ScriptFontBitmap::Create(const SPtr<FontBitmap>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.ScriptClass->CreateInstance("bool", ctorParams);
		new (B3DAllocate<ScriptFontBitmap>()) ScriptFontBitmap(managedInstance, value);
		return managedInstance;
	}
	void ScriptFontBitmap::InternalGetCharacterInformation(ScriptFontBitmap* thisPtr, uint32_t characterId, __CharacterInformationInterop* __output)
	{
		CharacterInformation tmp__output;
		tmp__output = thisPtr->GetInternal()->GetCharacterInformation(characterId);

		__CharacterInformationInterop interop__output;
		interop__output = ScriptCharacterInformation::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptCharacterInformation::GetMetaData()->ScriptClass->GetInternalClassInternal());
	}

	uint32_t ScriptFontBitmap::InternalGetSize(ScriptFontBitmap* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->Size;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptFontBitmap::InternalSetSize(ScriptFontBitmap* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->Size = value;
	}

	float ScriptFontBitmap::InternalGetBaselineOffset(ScriptFontBitmap* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->BaselineOffset;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptFontBitmap::InternalSetBaselineOffset(ScriptFontBitmap* thisPtr, float value)
	{
		thisPtr->GetInternal()->BaselineOffset = value;
	}

	float ScriptFontBitmap::InternalGetLineHeight(ScriptFontBitmap* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->LineHeight;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptFontBitmap::InternalSetLineHeight(ScriptFontBitmap* thisPtr, float value)
	{
		thisPtr->GetInternal()->LineHeight = value;
	}

	void ScriptFontBitmap::InternalGetMissingGlyph(ScriptFontBitmap* thisPtr, __CharacterInformationInterop* __output)
	{
		CharacterInformation tmp__output;
		tmp__output = thisPtr->GetInternal()->MissingGlyph;

		__CharacterInformationInterop interop__output;
		interop__output = ScriptCharacterInformation::ToInterop(tmp__output);
		MonoUtil::ValueCopy(__output, &interop__output, ScriptCharacterInformation::GetMetaData()->ScriptClass->GetInternalClassInternal());


	}

	void ScriptFontBitmap::InternalSetMissingGlyph(ScriptFontBitmap* thisPtr, __CharacterInformationInterop* value)
	{
		CharacterInformation tmpvalue;
		tmpvalue = ScriptCharacterInformation::FromInterop(*value);
		thisPtr->GetInternal()->MissingGlyph = tmpvalue;
	}

	float ScriptFontBitmap::InternalGetSpaceWidth(ScriptFontBitmap* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->SpaceWidth;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptFontBitmap::InternalSetSpaceWidth(ScriptFontBitmap* thisPtr, float value)
	{
		thisPtr->GetInternal()->SpaceWidth = value;
	}

	MonoArray* ScriptFontBitmap::InternalGetTexturePages(ScriptFontBitmap* thisPtr)
	{
		Vector<ResourceHandle<Texture>> vec__output;
		vec__output = thisPtr->GetInternal()->TexturePages;

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

	void ScriptFontBitmap::InternalSetTexturePages(ScriptFontBitmap* thisPtr, MonoArray* value)
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
					ResourceHandle<Texture> arrayElemPtrvalue = B3DStaticResourceCast<Texture>(scriptvalue->GetHandle());
					vecvalue[i] = arrayElemPtrvalue;
				}
			}

		}
		thisPtr->GetInternal()->TexturePages = vecvalue;
	}
}
