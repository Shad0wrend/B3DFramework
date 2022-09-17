//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptColorGradientHDR.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfUtility/Image/BsColorGradient.h"
#include "Wrappers/BsScriptColor.h"
#include "../Extensions/BsColorGradientEx.h"
#include "BsScriptColorGradientKey.generated.h"

namespace bs
{
	ScriptColorGradientHDR::ScriptColorGradientHDR(MonoObject* managedInstance, const SPtr<ColorGradientHDR>& value)
		:ScriptObject(managedInstance), mInternal(value)
	{
	}

	void ScriptColorGradientHDR::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_ColorGradientHDR", (void*)&ScriptColorGradientHDR::InternalColorGradientHDR);
		metaData.scriptClass->AddInternalCall("Internal_ColorGradientHDR0", (void*)&ScriptColorGradientHDR::InternalColorGradientHdR0);
		metaData.scriptClass->AddInternalCall("Internal_ColorGradientHDR1", (void*)&ScriptColorGradientHDR::InternalColorGradientHdR1);
		metaData.scriptClass->AddInternalCall("Internal_setKeys", (void*)&ScriptColorGradientHDR::InternalSetKeys);
		metaData.scriptClass->AddInternalCall("Internal_getKeys", (void*)&ScriptColorGradientHDR::InternalGetKeys);
		metaData.scriptClass->AddInternalCall("Internal_getNumKeys", (void*)&ScriptColorGradientHDR::InternalGetNumKeys);
		metaData.scriptClass->AddInternalCall("Internal_getKey", (void*)&ScriptColorGradientHDR::InternalGetKey);
		metaData.scriptClass->AddInternalCall("Internal_setConstant", (void*)&ScriptColorGradientHDR::InternalSetConstant);
		metaData.scriptClass->AddInternalCall("Internal_evaluate", (void*)&ScriptColorGradientHDR::InternalEvaluate);

	}

	MonoObject* ScriptColorGradientHDR::Create(const SPtr<ColorGradientHDR>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptColorGradientHDR>()) ScriptColorGradientHDR(managedInstance, value);
		return managedInstance;
	}
	void ScriptColorGradientHDR::InternalColorGradientHdr(MonoObject* managedInstance)
	{
		SPtr<ColorGradientHDR> instance = bs_shared_ptr_new<ColorGradientHDR>();
		new (bs_alloc<ScriptColorGradientHDR>())ScriptColorGradientHDR(managedInstance, instance);
	}

	void ScriptColorGradientHDR::InternalColorGradientHdR0(MonoObject* managedInstance, Color* color)
	{
		SPtr<ColorGradientHDR> instance = bs_shared_ptr_new<ColorGradientHDR>(*color);
		new (bs_alloc<ScriptColorGradientHDR>())ScriptColorGradientHDR(managedInstance, instance);
	}

	void ScriptColorGradientHDR::InternalColorGradientHdR1(MonoObject* managedInstance, MonoArray* keys)
	{
		Vector<ColorGradientKey> veckeys;
		if(keys != nullptr)
		{
			ScriptArray arraykeys(keys);
			veckeys.resize(arraykeys.size());
			for(int i = 0; i < (int)arraykeys.size(); i++)
			{
				veckeys[i] = ScriptColorGradientKey::fromInterop(arraykeys.get<__ColorGradientKeyInterop>(i));
			}
		}
		SPtr<ColorGradientHDR> instance = bs_shared_ptr_new<ColorGradientHDR>(veckeys);
		new (bs_alloc<ScriptColorGradientHDR>())ScriptColorGradientHDR(managedInstance, instance);
	}

	void ScriptColorGradientHDR::InternalSetKeys(ScriptColorGradientHDR* thisPtr, MonoArray* keys, float duration)
	{
		Vector<ColorGradientKey> veckeys;
		if(keys != nullptr)
		{
			ScriptArray arraykeys(keys);
			veckeys.resize(arraykeys.size());
			for(int i = 0; i < (int)arraykeys.size(); i++)
			{
				veckeys[i] = ScriptColorGradientKey::fromInterop(arraykeys.get<__ColorGradientKeyInterop>(i));
			}

		}
		thisPtr->GetInternal()->SetKeys(veckeys, duration);
	}

	MonoArray* ScriptColorGradientHDR::InternalGetKeys(ScriptColorGradientHDR* thisPtr)
	{
		Vector<ColorGradientKey> vec__output;
		vec__output = thisPtr->GetInternal()->GetKeys();

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptColorGradientKey>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, ScriptColorGradientKey::toInterop(vec__output[i]));
		}
		__output = array__output.getInternal();

		return __output;
	}

	uint32_t ScriptColorGradientHDR::InternalGetNumKeys(ScriptColorGradientHDR* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->GetNumKeys();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptColorGradientHDR::InternalGetKey(ScriptColorGradientHDR* thisPtr, uint32_t idx, __ColorGradientKeyInterop* __output)
	{
		ColorGradientKey tmp__output;
		tmp__output = thisPtr->GetInternal()->GetKey(idx);

		__ColorGradientKeyInterop interop__output;
		interop__output = ScriptColorGradientKey::toInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptColorGradientKey::getMetaData()->scriptClass->GetInternalClassInternal());
	}

	void ScriptColorGradientHDR::InternalSetConstant(ScriptColorGradientHDR* thisPtr, Color* color)
	{
		thisPtr->GetInternal()->SetConstant(*color);
	}

	void ScriptColorGradientHDR::InternalEvaluate(ScriptColorGradientHDR* thisPtr, float t, Color* __output)
	{
		Color tmp__output;
		tmp__output = ColorGradientHDREx::evaluate(thisPtr->GetInternal(), t);

		*__output = tmp__output;
	}
}
