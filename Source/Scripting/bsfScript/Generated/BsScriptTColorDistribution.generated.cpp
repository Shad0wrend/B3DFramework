//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptTColorDistribution.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptColorGradient.generated.h"
#include "Wrappers/BsScriptColor.h"
#include "BsScriptColorGradientHDR.generated.h"

namespace bs
{
	ScriptTColorDistributionColorGradient::ScriptTColorDistributionColorGradient(MonoObject* managedInstance, const SPtr<TColorDistribution<ColorGradient>>& value)
		:ScriptObject(managedInstance), mInternal(value)
	{
	}

	void ScriptTColorDistributionColorGradient::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_TColorDistribution", (void*)&ScriptTColorDistributionColorGradient::InternalTColorDistribution);
		metaData.scriptClass->AddInternalCall("Internal_TColorDistribution0", (void*)&ScriptTColorDistributionColorGradient::InternalTColorDistribution0);
		metaData.scriptClass->AddInternalCall("Internal_TColorDistribution1", (void*)&ScriptTColorDistributionColorGradient::InternalTColorDistribution1);
		metaData.scriptClass->addInternalCall("Internal_TColorDistribution2", (void*)&ScriptTColorDistributionColorGradient::Internal_TColorDistribution2);
		metaData.scriptClass->addInternalCall("Internal_TColorDistribution3", (void*)&ScriptTColorDistributionColorGradient::Internal_TColorDistribution3);
		metaData.scriptClass->addInternalCall("Internal_getType", (void*)&ScriptTColorDistributionColorGradient::Internal_getType);
		metaData.scriptClass->addInternalCall("Internal_getMinConstant", (void*)&ScriptTColorDistributionColorGradient::Internal_getMinConstant);
		metaData.scriptClass->addInternalCall("Internal_getMaxConstant", (void*)&ScriptTColorDistributionColorGradient::Internal_getMaxConstant);
		metaData.scriptClass->addInternalCall("Internal_getMinGradient", (void*)&ScriptTColorDistributionColorGradient::Internal_getMinGradient);
		metaData.scriptClass->addInternalCall("Internal_getMaxGradient", (void*)&ScriptTColorDistributionColorGradient::Internal_getMaxGradient);

	}

	MonoObject* ScriptTColorDistributionColorGradient::Create(const SPtr<TColorDistribution<ColorGradient>>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptTColorDistributionColorGradient>()) ScriptTColorDistributionColorGradient(managedInstance, value);
		return managedInstance;
	}
	void ScriptTColorDistributionColorGradient::InternalTColorDistribution(MonoObject* managedInstance)
	{
		SPtr<TColorDistribution<ColorGradient>> instance = bs_shared_ptr_new<TColorDistribution<ColorGradient>>();
		new (bs_alloc<ScriptTColorDistributionColorGradient>())ScriptTColorDistributionColorGradient(managedInstance, instance);
	}

	void ScriptTColorDistributionColorGradient::InternalTColorDistribution0(MonoObject* managedInstance, Color* color)
	{
		SPtr<TColorDistribution<ColorGradient>> instance = bs_shared_ptr_new<TColorDistribution<ColorGradient>>(*color);
		new (bs_alloc<ScriptTColorDistributionColorGradient>())ScriptTColorDistributionColorGradient(managedInstance, instance);
	}

	void ScriptTColorDistributionColorGradient::InternalTColorDistribution1(MonoObject* managedInstance, Color* minColor, Color* maxColor)
	{
		SPtr<TColorDistribution<ColorGradient>> instance = bs_shared_ptr_new<TColorDistribution<ColorGradient>>(*minColor, *maxColor);
		new (bs_alloc<ScriptTColorDistributionColorGradient>())ScriptTColorDistributionColorGradient(managedInstance, instance);
	}

	void ScriptTColorDistributionColorGradient::InternalTColorDistribution2(MonoObject* managedInstance, MonoObject* gradient)
	{
		SPtr<ColorGradient> tmpgradient;
		ScriptColorGradient* scriptgradient;
		scriptgradient = ScriptColorGradient::toNative(gradient);
		if(scriptgradient != nullptr)
			tmpgradient = scriptgradient->GetInternal();
		SPtr<TColorDistribution<ColorGradient>> instance = bs_shared_ptr_new<TColorDistribution<ColorGradient>>(*tmpgradient);
		new (bs_alloc<ScriptTColorDistributionColorGradient>())ScriptTColorDistributionColorGradient(managedInstance, instance);
	}

	void ScriptTColorDistributionColorGradient::InternalTColorDistribution3(MonoObject* managedInstance, MonoObject* minGradient, MonoObject* maxGradient)
	{
		SPtr<ColorGradient> tmpminGradient;
		ScriptColorGradient* scriptminGradient;
		scriptminGradient = ScriptColorGradient::toNative(minGradient);
		if(scriptminGradient != nullptr)
			tmpminGradient = scriptminGradient->GetInternal();
		SPtr<ColorGradient> tmpmaxGradient;
		ScriptColorGradient* scriptmaxGradient;
		scriptmaxGradient = ScriptColorGradient::toNative(maxGradient);
		if(scriptmaxGradient != nullptr)
			tmpmaxGradient = scriptmaxGradient->GetInternal();
		SPtr<TColorDistribution<ColorGradient>> instance = bs_shared_ptr_new<TColorDistribution<ColorGradient>>(*tmpminGradient, *tmpmaxGradient);
		new (bs_alloc<ScriptTColorDistributionColorGradient>())ScriptTColorDistributionColorGradient(managedInstance, instance);
	}

	PropertyDistributionType ScriptTColorDistributionColorGradient::InternalGetType(ScriptTColorDistributionColorGradient* thisPtr)
	{
		PropertyDistributionType tmp__output;
		tmp__output = thisPtr->GetInternal()->GetType();

		PropertyDistributionType __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTColorDistributionColorGradient::InternalGetMinConstant(ScriptTColorDistributionColorGradient* thisPtr, Color* __output)
	{
		Color tmp__output;
		tmp__output = thisPtr->GetInternal()->GetMinConstant();

		*__output = tmp__output;
	}

	void ScriptTColorDistributionColorGradient::InternalGetMaxConstant(ScriptTColorDistributionColorGradient* thisPtr, Color* __output)
	{
		Color tmp__output;
		tmp__output = thisPtr->GetInternal()->GetMaxConstant();

		*__output = tmp__output;
	}

	MonoObject* ScriptTColorDistributionColorGradient::InternalGetMinGradient(ScriptTColorDistributionColorGradient* thisPtr)
	{
		SPtr<ColorGradient> tmp__output = bs_shared_ptr_new<ColorGradient>();
		*tmp__output = thisPtr->GetInternal()->GetMinGradient();

		MonoObject* __output;
		__output = ScriptColorGradient::Create(tmp__output);

		return __output;
	}

	MonoObject* ScriptTColorDistributionColorGradient::InternalGetMaxGradient(ScriptTColorDistributionColorGradient* thisPtr)
	{
		SPtr<ColorGradient> tmp__output = bs_shared_ptr_new<ColorGradient>();
		*tmp__output = thisPtr->GetInternal()->GetMaxGradient();

		MonoObject* __output;
		__output = ScriptColorGradient::Create(tmp__output);

		return __output;
	}

	ScriptTColorDistributionColorGradientHDR::ScriptTColorDistributionColorGradientHDR(MonoObject* managedInstance, const SPtr<TColorDistribution<ColorGradientHDR>>& value)
		:ScriptObject(managedInstance), mInternal(value)
	{
	}

	void ScriptTColorDistributionColorGradientHDR::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_TColorDistribution", (void*)&ScriptTColorDistributionColorGradientHDR::Internal_TColorDistribution);
		metaData.scriptClass->addInternalCall("Internal_TColorDistribution0", (void*)&ScriptTColorDistributionColorGradientHDR::Internal_TColorDistribution0);
		metaData.scriptClass->addInternalCall("Internal_TColorDistribution1", (void*)&ScriptTColorDistributionColorGradientHDR::Internal_TColorDistribution1);
		metaData.scriptClass->addInternalCall("Internal_TColorDistribution2", (void*)&ScriptTColorDistributionColorGradientHDR::Internal_TColorDistribution2);
		metaData.scriptClass->addInternalCall("Internal_TColorDistribution3", (void*)&ScriptTColorDistributionColorGradientHDR::Internal_TColorDistribution3);
		metaData.scriptClass->addInternalCall("Internal_getType", (void*)&ScriptTColorDistributionColorGradientHDR::Internal_getType);
		metaData.scriptClass->addInternalCall("Internal_getMinConstant", (void*)&ScriptTColorDistributionColorGradientHDR::Internal_getMinConstant);
		metaData.scriptClass->addInternalCall("Internal_getMaxConstant", (void*)&ScriptTColorDistributionColorGradientHDR::Internal_getMaxConstant);
		metaData.scriptClass->addInternalCall("Internal_getMinGradient", (void*)&ScriptTColorDistributionColorGradientHDR::Internal_getMinGradient);
		metaData.scriptClass->addInternalCall("Internal_getMaxGradient", (void*)&ScriptTColorDistributionColorGradientHDR::Internal_getMaxGradient);

	}

	MonoObject* ScriptTColorDistributionColorGradientHDR::Create(const SPtr<TColorDistribution<ColorGradientHDR>>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptTColorDistributionColorGradientHDR>()) ScriptTColorDistributionColorGradientHDR(managedInstance, value);
		return managedInstance;
	}
	void ScriptTColorDistributionColorGradientHDR::InternalTColorDistribution(MonoObject* managedInstance)
	{
		SPtr<TColorDistribution<ColorGradientHDR>> instance = bs_shared_ptr_new<TColorDistribution<ColorGradientHDR>>();
		new (bs_alloc<ScriptTColorDistributionColorGradientHDR>())ScriptTColorDistributionColorGradientHDR(managedInstance, instance);
	}

	void ScriptTColorDistributionColorGradientHDR::InternalTColorDistribution0(MonoObject* managedInstance, Color* color)
	{
		SPtr<TColorDistribution<ColorGradientHDR>> instance = bs_shared_ptr_new<TColorDistribution<ColorGradientHDR>>(*color);
		new (bs_alloc<ScriptTColorDistributionColorGradientHDR>())ScriptTColorDistributionColorGradientHDR(managedInstance, instance);
	}

	void ScriptTColorDistributionColorGradientHDR::InternalTColorDistribution1(MonoObject* managedInstance, Color* minColor, Color* maxColor)
	{
		SPtr<TColorDistribution<ColorGradientHDR>> instance = bs_shared_ptr_new<TColorDistribution<ColorGradientHDR>>(*minColor, *maxColor);
		new (bs_alloc<ScriptTColorDistributionColorGradientHDR>())ScriptTColorDistributionColorGradientHDR(managedInstance, instance);
	}

	void ScriptTColorDistributionColorGradientHDR::InternalTColorDistribution2(MonoObject* managedInstance, MonoObject* gradient)
	{
		SPtr<ColorGradientHDR> tmpgradient;
		ScriptColorGradientHDR* scriptgradient;
		scriptgradient = ScriptColorGradientHDR::toNative(gradient);
		if(scriptgradient != nullptr)
			tmpgradient = scriptgradient->GetInternal();
		SPtr<TColorDistribution<ColorGradientHDR>> instance = bs_shared_ptr_new<TColorDistribution<ColorGradientHDR>>(*tmpgradient);
		new (bs_alloc<ScriptTColorDistributionColorGradientHDR>())ScriptTColorDistributionColorGradientHDR(managedInstance, instance);
	}

	void ScriptTColorDistributionColorGradientHDR::InternalTColorDistribution3(MonoObject* managedInstance, MonoObject* minGradient, MonoObject* maxGradient)
	{
		SPtr<ColorGradientHDR> tmpminGradient;
		ScriptColorGradientHDR* scriptminGradient;
		scriptminGradient = ScriptColorGradientHDR::toNative(minGradient);
		if(scriptminGradient != nullptr)
			tmpminGradient = scriptminGradient->GetInternal();
		SPtr<ColorGradientHDR> tmpmaxGradient;
		ScriptColorGradientHDR* scriptmaxGradient;
		scriptmaxGradient = ScriptColorGradientHDR::toNative(maxGradient);
		if(scriptmaxGradient != nullptr)
			tmpmaxGradient = scriptmaxGradient->GetInternal();
		SPtr<TColorDistribution<ColorGradientHDR>> instance = bs_shared_ptr_new<TColorDistribution<ColorGradientHDR>>(*tmpminGradient, *tmpmaxGradient);
		new (bs_alloc<ScriptTColorDistributionColorGradientHDR>())ScriptTColorDistributionColorGradientHDR(managedInstance, instance);
	}

	PropertyDistributionType ScriptTColorDistributionColorGradientHDR::InternalGetType(ScriptTColorDistributionColorGradientHDR* thisPtr)
	{
		PropertyDistributionType tmp__output;
		tmp__output = thisPtr->GetInternal()->GetType();

		PropertyDistributionType __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTColorDistributionColorGradientHDR::InternalGetMinConstant(ScriptTColorDistributionColorGradientHDR* thisPtr, Color* __output)
	{
		Color tmp__output;
		tmp__output = thisPtr->GetInternal()->GetMinConstant();

		*__output = tmp__output;
	}

	void ScriptTColorDistributionColorGradientHDR::InternalGetMaxConstant(ScriptTColorDistributionColorGradientHDR* thisPtr, Color* __output)
	{
		Color tmp__output;
		tmp__output = thisPtr->GetInternal()->GetMaxConstant();

		*__output = tmp__output;
	}

	MonoObject* ScriptTColorDistributionColorGradientHDR::InternalGetMinGradient(ScriptTColorDistributionColorGradientHDR* thisPtr)
	{
		SPtr<ColorGradientHDR> tmp__output = bs_shared_ptr_new<ColorGradientHDR>();
		*tmp__output = thisPtr->GetInternal()->GetMinGradient();

		MonoObject* __output;
		__output = ScriptColorGradientHDR::Create(tmp__output);

		return __output;
	}

	MonoObject* ScriptTColorDistributionColorGradientHDR::InternalGetMaxGradient(ScriptTColorDistributionColorGradientHDR* thisPtr)
	{
		SPtr<ColorGradientHDR> tmp__output = bs_shared_ptr_new<ColorGradientHDR>();
		*tmp__output = thisPtr->GetInternal()->GetMaxGradient();

		MonoObject* __output;
		__output = ScriptColorGradientHDR::Create(tmp__output);

		return __output;
	}
}
