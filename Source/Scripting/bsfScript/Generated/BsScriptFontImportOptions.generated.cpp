//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptFontImportOptions.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptCharRange.generated.h"
#include "BsScriptFontImportOptions.generated.h"

namespace bs
{
#if !BS_IS_BANSHEE3D
	ScriptFontImportOptions::ScriptFontImportOptions(MonoObject* managedInstance, const SPtr<FontImportOptions>& value)
		:TScriptReflectable(managedInstance, value)
	{
		mInternal = value;
	}

	void ScriptFontImportOptions::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_getfontSizes", (void*)&ScriptFontImportOptions::InternalGetfontSizes);
		metaData.scriptClass->AddInternalCall("Internal_setfontSizes", (void*)&ScriptFontImportOptions::InternalSetfontSizes);
		metaData.scriptClass->AddInternalCall("Internal_getcharIndexRanges", (void*)&ScriptFontImportOptions::InternalGetcharIndexRanges);
		metaData.scriptClass->AddInternalCall("Internal_setcharIndexRanges", (void*)&ScriptFontImportOptions::InternalSetcharIndexRanges);
		metaData.scriptClass->AddInternalCall("Internal_getdpi", (void*)&ScriptFontImportOptions::InternalGetdpi);
		metaData.scriptClass->AddInternalCall("Internal_setdpi", (void*)&ScriptFontImportOptions::InternalSetdpi);
		metaData.scriptClass->AddInternalCall("Internal_getrenderMode", (void*)&ScriptFontImportOptions::InternalGetrenderMode);
		metaData.scriptClass->AddInternalCall("Internal_setrenderMode", (void*)&ScriptFontImportOptions::InternalSetrenderMode);
		metaData.scriptClass->AddInternalCall("Internal_getbold", (void*)&ScriptFontImportOptions::InternalGetbold);
		metaData.scriptClass->AddInternalCall("Internal_setbold", (void*)&ScriptFontImportOptions::InternalSetbold);
		metaData.scriptClass->AddInternalCall("Internal_getitalic", (void*)&ScriptFontImportOptions::InternalGetitalic);
		metaData.scriptClass->AddInternalCall("Internal_setitalic", (void*)&ScriptFontImportOptions::InternalSetitalic);
		metaData.scriptClass->AddInternalCall("Internal_create", (void*)&ScriptFontImportOptions::InternalCreate);

	}

	MonoObject* ScriptFontImportOptions::Create(const SPtr<FontImportOptions>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptFontImportOptions>()) ScriptFontImportOptions(managedInstance, value);
		return managedInstance;
	}
	void ScriptFontImportOptions::InternalCreate(MonoObject* managedInstance)
	{
		SPtr<FontImportOptions> instance = FontImportOptions::Create();
		new (bs_alloc<ScriptFontImportOptions>())ScriptFontImportOptions(managedInstance, instance);
	}
	MonoArray* ScriptFontImportOptions::InternalGetfontSizes(ScriptFontImportOptions* thisPtr)
	{
		Vector<uint32_t> vec__output;
		vec__output = thisPtr->GetInternal()->fontSizes;

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<uint32_t>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, vec__output[i]);
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptFontImportOptions::InternalSetfontSizes(ScriptFontImportOptions* thisPtr, MonoArray* value)
	{
		Vector<uint32_t> vecvalue;
		if(value != nullptr)
		{
			ScriptArray arrayvalue(value);
			vecvalue.resize(arrayvalue.size());
			for(int i = 0; i < (int)arrayvalue.size(); i++)
			{
				vecvalue[i] = arrayvalue.get<uint32_t>(i);
			}

		}
		thisPtr->GetInternal()->fontSizes = vecvalue;
	}

	MonoArray* ScriptFontImportOptions::InternalGetcharIndexRanges(ScriptFontImportOptions* thisPtr)
	{
		Vector<CharRange> vec__output;
		vec__output = thisPtr->GetInternal()->charIndexRanges;

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptCharRange>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, vec__output[i]);
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptFontImportOptions::InternalSetcharIndexRanges(ScriptFontImportOptions* thisPtr, MonoArray* value)
	{
		Vector<CharRange> vecvalue;
		if(value != nullptr)
		{
			ScriptArray arrayvalue(value);
			vecvalue.resize(arrayvalue.size());
			for(int i = 0; i < (int)arrayvalue.size(); i++)
			{
				vecvalue[i] = arrayvalue.get<CharRange>(i);
			}

		}
		thisPtr->GetInternal()->charIndexRanges = vecvalue;
	}

	uint32_t ScriptFontImportOptions::InternalGetdpi(ScriptFontImportOptions* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->dpi;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptFontImportOptions::InternalSetdpi(ScriptFontImportOptions* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->dpi = value;
	}

	FontRenderMode ScriptFontImportOptions::InternalGetrenderMode(ScriptFontImportOptions* thisPtr)
	{
		FontRenderMode tmp__output;
		tmp__output = thisPtr->GetInternal()->renderMode;

		FontRenderMode __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptFontImportOptions::InternalSetrenderMode(ScriptFontImportOptions* thisPtr, FontRenderMode value)
	{
		thisPtr->GetInternal()->renderMode = value;
	}

	bool ScriptFontImportOptions::InternalGetbold(ScriptFontImportOptions* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->bold;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptFontImportOptions::InternalSetbold(ScriptFontImportOptions* thisPtr, bool value)
	{
		thisPtr->GetInternal()->bold = value;
	}

	bool ScriptFontImportOptions::InternalGetitalic(ScriptFontImportOptions* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->italic;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptFontImportOptions::InternalSetitalic(ScriptFontImportOptions* thisPtr, bool value)
	{
		thisPtr->GetInternal()->italic = value;
	}
#endif
}
