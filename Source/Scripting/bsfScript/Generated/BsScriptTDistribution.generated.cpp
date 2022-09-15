//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptTDistribution.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptRandom.generated.h"
#include "BsScriptTAnimationCurve.generated.h"
#include "Wrappers/BsScriptVector.h"
#include "Wrappers/BsScriptVector.h"
#include "BsScriptTAnimationCurve.generated.h"
#include "BsScriptTAnimationCurve.generated.h"

namespace bs
{
	ScriptTDistributionfloat::ScriptTDistributionfloat(MonoObject* managedInstance, const SPtr<TDistribution<float>>& value)
		:ScriptObject(managedInstance), mInternal(value)
	{
	}

	void ScriptTDistributionfloat::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_TDistribution", (void*)&ScriptTDistributionfloat::InternalTDistribution);
		metaData.scriptClass->AddInternalCall("Internal_TDistribution0", (void*)&ScriptTDistributionfloat::InternalTDistribution0);
		metaData.scriptClass->AddInternalCall("Internal_TDistribution1", (void*)&ScriptTDistributionfloat::InternalTDistribution1);
		metaData.scriptClass->addInternalCall("Internal_TDistribution2", (void*)&ScriptTDistributionfloat::Internal_TDistribution2);
		metaData.scriptClass->addInternalCall("Internal_TDistribution3", (void*)&ScriptTDistributionfloat::Internal_TDistribution3);
		metaData.scriptClass->addInternalCall("Internal_getType", (void*)&ScriptTDistributionfloat::Internal_getType);
		metaData.scriptClass->addInternalCall("Internal_getMinConstant", (void*)&ScriptTDistributionfloat::Internal_getMinConstant);
		metaData.scriptClass->addInternalCall("Internal_getMaxConstant", (void*)&ScriptTDistributionfloat::Internal_getMaxConstant);
		metaData.scriptClass->addInternalCall("Internal_getMinCurve", (void*)&ScriptTDistributionfloat::Internal_getMinCurve);
		metaData.scriptClass->addInternalCall("Internal_getMaxCurve", (void*)&ScriptTDistributionfloat::Internal_getMaxCurve);
		metaData.scriptClass->addInternalCall("Internal_evaluate", (void*)&ScriptTDistributionfloat::Internal_evaluate);
		metaData.scriptClass->addInternalCall("Internal_evaluate0", (void*)&ScriptTDistributionfloat::Internal_evaluate0);

	}

	MonoObject* ScriptTDistributionfloat::Create(const SPtr<TDistribution<float>>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptTDistributionfloat>()) ScriptTDistributionfloat(managedInstance, value);
		return managedInstance;
	}
	void ScriptTDistributionfloat::InternalTDistribution(MonoObject* managedInstance)
	{
		SPtr<TDistribution<float>> instance = bs_shared_ptr_new<TDistribution<float>>();
		new (bs_alloc<ScriptTDistributionfloat>())ScriptTDistributionfloat(managedInstance, instance);
	}

	void ScriptTDistributionfloat::InternalTDistribution0(MonoObject* managedInstance, float value)
	{
		SPtr<TDistribution<float>> instance = bs_shared_ptr_new<TDistribution<float>>(value);
		new (bs_alloc<ScriptTDistributionfloat>())ScriptTDistributionfloat(managedInstance, instance);
	}

	void ScriptTDistributionfloat::InternalTDistribution1(MonoObject* managedInstance, float minValue, float maxValue)
	{
		SPtr<TDistribution<float>> instance = bs_shared_ptr_new<TDistribution<float>>(minValue, maxValue);
		new (bs_alloc<ScriptTDistributionfloat>())ScriptTDistributionfloat(managedInstance, instance);
	}

	void ScriptTDistributionfloat::InternalTDistribution2(MonoObject* managedInstance, MonoObject* curve)
	{
		SPtr<TAnimationCurve<float>> tmpcurve;
		ScriptTAnimationCurvefloat* scriptcurve;
		scriptcurve = ScriptTAnimationCurvefloat::toNative(curve);
		if(scriptcurve != nullptr)
			tmpcurve = scriptcurve->GetInternal();
		SPtr<TDistribution<float>> instance = bs_shared_ptr_new<TDistribution<float>>(*tmpcurve);
		new (bs_alloc<ScriptTDistributionfloat>())ScriptTDistributionfloat(managedInstance, instance);
	}

	void ScriptTDistributionfloat::InternalTDistribution3(MonoObject* managedInstance, MonoObject* minCurve, MonoObject* maxCurve)
	{
		SPtr<TAnimationCurve<float>> tmpminCurve;
		ScriptTAnimationCurvefloat* scriptminCurve;
		scriptminCurve = ScriptTAnimationCurvefloat::toNative(minCurve);
		if(scriptminCurve != nullptr)
			tmpminCurve = scriptminCurve->GetInternal();
		SPtr<TAnimationCurve<float>> tmpmaxCurve;
		ScriptTAnimationCurvefloat* scriptmaxCurve;
		scriptmaxCurve = ScriptTAnimationCurvefloat::toNative(maxCurve);
		if(scriptmaxCurve != nullptr)
			tmpmaxCurve = scriptmaxCurve->GetInternal();
		SPtr<TDistribution<float>> instance = bs_shared_ptr_new<TDistribution<float>>(*tmpminCurve, *tmpmaxCurve);
		new (bs_alloc<ScriptTDistributionfloat>())ScriptTDistributionfloat(managedInstance, instance);
	}

	PropertyDistributionType ScriptTDistributionfloat::InternalGetType(ScriptTDistributionfloat* thisPtr)
	{
		PropertyDistributionType tmp__output;
		tmp__output = thisPtr->GetInternal()->GetType();

		PropertyDistributionType __output;
		__output = tmp__output;

		return __output;
	}

	float ScriptTDistributionfloat::InternalGetMinConstant(ScriptTDistributionfloat* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->GetMinConstant();

		float __output;
		__output = tmp__output;

		return __output;
	}

	float ScriptTDistributionfloat::InternalGetMaxConstant(ScriptTDistributionfloat* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->GetMaxConstant();

		float __output;
		__output = tmp__output;

		return __output;
	}

	MonoObject* ScriptTDistributionfloat::InternalGetMinCurve(ScriptTDistributionfloat* thisPtr)
	{
		SPtr<TAnimationCurve<float>> tmp__output = bs_shared_ptr_new<TAnimationCurve<float>>();
		*tmp__output = thisPtr->GetInternal()->GetMinCurve();

		MonoObject* __output;
		__output = ScriptTAnimationCurvefloat::Create(tmp__output);

		return __output;
	}

	MonoObject* ScriptTDistributionfloat::InternalGetMaxCurve(ScriptTDistributionfloat* thisPtr)
	{
		SPtr<TAnimationCurve<float>> tmp__output = bs_shared_ptr_new<TAnimationCurve<float>>();
		*tmp__output = thisPtr->GetInternal()->GetMaxCurve();

		MonoObject* __output;
		__output = ScriptTAnimationCurvefloat::Create(tmp__output);

		return __output;
	}

	float ScriptTDistributionfloat::InternalEvaluate(ScriptTDistributionfloat* thisPtr, float t, float factor)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->evaluate(t, factor);

		float __output;
		__output = tmp__output;

		return __output;
	}

	float ScriptTDistributionfloat::InternalEvaluate0(ScriptTDistributionfloat* thisPtr, float t, MonoObject* factor)
	{
		float tmp__output;
		SPtr<Random> tmpfactor;
		ScriptRandom* scriptfactor;
		scriptfactor = ScriptRandom::toNative(factor);
		if(scriptfactor != nullptr)
			tmpfactor = scriptfactor->GetInternal();
		tmp__output = thisPtr->GetInternal()->evaluate(t, *tmpfactor);

		float __output;
		__output = tmp__output;

		return __output;
	}

	ScriptTDistributionVector3::ScriptTDistributionVector3(MonoObject* managedInstance, const SPtr<TDistribution<Vector3>>& value)
		:ScriptObject(managedInstance), mInternal(value)
	{
	}

	void ScriptTDistributionVector3::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_TDistribution", (void*)&ScriptTDistributionVector3::Internal_TDistribution);
		metaData.scriptClass->addInternalCall("Internal_TDistribution0", (void*)&ScriptTDistributionVector3::Internal_TDistribution0);
		metaData.scriptClass->addInternalCall("Internal_TDistribution1", (void*)&ScriptTDistributionVector3::Internal_TDistribution1);
		metaData.scriptClass->addInternalCall("Internal_TDistribution2", (void*)&ScriptTDistributionVector3::Internal_TDistribution2);
		metaData.scriptClass->addInternalCall("Internal_TDistribution3", (void*)&ScriptTDistributionVector3::Internal_TDistribution3);
		metaData.scriptClass->addInternalCall("Internal_getType", (void*)&ScriptTDistributionVector3::Internal_getType);
		metaData.scriptClass->addInternalCall("Internal_getMinConstant", (void*)&ScriptTDistributionVector3::Internal_getMinConstant);
		metaData.scriptClass->addInternalCall("Internal_getMaxConstant", (void*)&ScriptTDistributionVector3::Internal_getMaxConstant);
		metaData.scriptClass->addInternalCall("Internal_getMinCurve", (void*)&ScriptTDistributionVector3::Internal_getMinCurve);
		metaData.scriptClass->addInternalCall("Internal_getMaxCurve", (void*)&ScriptTDistributionVector3::Internal_getMaxCurve);
		metaData.scriptClass->addInternalCall("Internal_evaluate", (void*)&ScriptTDistributionVector3::Internal_evaluate);
		metaData.scriptClass->addInternalCall("Internal_evaluate0", (void*)&ScriptTDistributionVector3::Internal_evaluate0);

	}

	MonoObject* ScriptTDistributionVector3::Create(const SPtr<TDistribution<Vector3>>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptTDistributionVector3>()) ScriptTDistributionVector3(managedInstance, value);
		return managedInstance;
	}
	void ScriptTDistributionVector3::InternalTDistribution(MonoObject* managedInstance)
	{
		SPtr<TDistribution<Vector3>> instance = bs_shared_ptr_new<TDistribution<Vector3>>();
		new (bs_alloc<ScriptTDistributionVector3>())ScriptTDistributionVector3(managedInstance, instance);
	}

	void ScriptTDistributionVector3::InternalTDistribution0(MonoObject* managedInstance, Vector3* value)
	{
		SPtr<TDistribution<Vector3>> instance = bs_shared_ptr_new<TDistribution<Vector3>>(*value);
		new (bs_alloc<ScriptTDistributionVector3>())ScriptTDistributionVector3(managedInstance, instance);
	}

	void ScriptTDistributionVector3::InternalTDistribution1(MonoObject* managedInstance, Vector3* minValue, Vector3* maxValue)
	{
		SPtr<TDistribution<Vector3>> instance = bs_shared_ptr_new<TDistribution<Vector3>>(*minValue, *maxValue);
		new (bs_alloc<ScriptTDistributionVector3>())ScriptTDistributionVector3(managedInstance, instance);
	}

	void ScriptTDistributionVector3::InternalTDistribution2(MonoObject* managedInstance, MonoObject* curve)
	{
		SPtr<TAnimationCurve<Vector3>> tmpcurve;
		ScriptTAnimationCurveVector3* scriptcurve;
		scriptcurve = ScriptTAnimationCurveVector3::toNative(curve);
		if(scriptcurve != nullptr)
			tmpcurve = scriptcurve->GetInternal();
		SPtr<TDistribution<Vector3>> instance = bs_shared_ptr_new<TDistribution<Vector3>>(*tmpcurve);
		new (bs_alloc<ScriptTDistributionVector3>())ScriptTDistributionVector3(managedInstance, instance);
	}

	void ScriptTDistributionVector3::InternalTDistribution3(MonoObject* managedInstance, MonoObject* minCurve, MonoObject* maxCurve)
	{
		SPtr<TAnimationCurve<Vector3>> tmpminCurve;
		ScriptTAnimationCurveVector3* scriptminCurve;
		scriptminCurve = ScriptTAnimationCurveVector3::toNative(minCurve);
		if(scriptminCurve != nullptr)
			tmpminCurve = scriptminCurve->GetInternal();
		SPtr<TAnimationCurve<Vector3>> tmpmaxCurve;
		ScriptTAnimationCurveVector3* scriptmaxCurve;
		scriptmaxCurve = ScriptTAnimationCurveVector3::toNative(maxCurve);
		if(scriptmaxCurve != nullptr)
			tmpmaxCurve = scriptmaxCurve->GetInternal();
		SPtr<TDistribution<Vector3>> instance = bs_shared_ptr_new<TDistribution<Vector3>>(*tmpminCurve, *tmpmaxCurve);
		new (bs_alloc<ScriptTDistributionVector3>())ScriptTDistributionVector3(managedInstance, instance);
	}

	PropertyDistributionType ScriptTDistributionVector3::InternalGetType(ScriptTDistributionVector3* thisPtr)
	{
		PropertyDistributionType tmp__output;
		tmp__output = thisPtr->GetInternal()->GetType();

		PropertyDistributionType __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTDistributionVector3::InternalGetMinConstant(ScriptTDistributionVector3* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->GetMinConstant();

		*__output = tmp__output;
	}

	void ScriptTDistributionVector3::InternalGetMaxConstant(ScriptTDistributionVector3* thisPtr, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->GetMaxConstant();

		*__output = tmp__output;
	}

	MonoObject* ScriptTDistributionVector3::InternalGetMinCurve(ScriptTDistributionVector3* thisPtr)
	{
		SPtr<TAnimationCurve<Vector3>> tmp__output = bs_shared_ptr_new<TAnimationCurve<Vector3>>();
		*tmp__output = thisPtr->GetInternal()->GetMinCurve();

		MonoObject* __output;
		__output = ScriptTAnimationCurveVector3::Create(tmp__output);

		return __output;
	}

	MonoObject* ScriptTDistributionVector3::InternalGetMaxCurve(ScriptTDistributionVector3* thisPtr)
	{
		SPtr<TAnimationCurve<Vector3>> tmp__output = bs_shared_ptr_new<TAnimationCurve<Vector3>>();
		*tmp__output = thisPtr->GetInternal()->GetMaxCurve();

		MonoObject* __output;
		__output = ScriptTAnimationCurveVector3::Create(tmp__output);

		return __output;
	}

	void ScriptTDistributionVector3::InternalEvaluate(ScriptTDistributionVector3* thisPtr, float t, float factor, Vector3* __output)
	{
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->evaluate(t, factor);

		*__output = tmp__output;
	}

	void ScriptTDistributionVector3::InternalEvaluate0(ScriptTDistributionVector3* thisPtr, float t, MonoObject* factor, Vector3* __output)
	{
		SPtr<Random> tmpfactor;
		ScriptRandom* scriptfactor;
		scriptfactor = ScriptRandom::toNative(factor);
		if(scriptfactor != nullptr)
			tmpfactor = scriptfactor->GetInternal();
		Vector3 tmp__output;
		tmp__output = thisPtr->GetInternal()->evaluate(t, *tmpfactor);

		*__output = tmp__output;
	}

	ScriptTDistributionVector2::ScriptTDistributionVector2(MonoObject* managedInstance, const SPtr<TDistribution<Vector2>>& value)
		:ScriptObject(managedInstance), mInternal(value)
	{
	}

	void ScriptTDistributionVector2::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_TDistribution", (void*)&ScriptTDistributionVector2::Internal_TDistribution);
		metaData.scriptClass->addInternalCall("Internal_TDistribution0", (void*)&ScriptTDistributionVector2::Internal_TDistribution0);
		metaData.scriptClass->addInternalCall("Internal_TDistribution1", (void*)&ScriptTDistributionVector2::Internal_TDistribution1);
		metaData.scriptClass->addInternalCall("Internal_TDistribution2", (void*)&ScriptTDistributionVector2::Internal_TDistribution2);
		metaData.scriptClass->addInternalCall("Internal_TDistribution3", (void*)&ScriptTDistributionVector2::Internal_TDistribution3);
		metaData.scriptClass->addInternalCall("Internal_getType", (void*)&ScriptTDistributionVector2::Internal_getType);
		metaData.scriptClass->addInternalCall("Internal_getMinConstant", (void*)&ScriptTDistributionVector2::Internal_getMinConstant);
		metaData.scriptClass->addInternalCall("Internal_getMaxConstant", (void*)&ScriptTDistributionVector2::Internal_getMaxConstant);
		metaData.scriptClass->addInternalCall("Internal_getMinCurve", (void*)&ScriptTDistributionVector2::Internal_getMinCurve);
		metaData.scriptClass->addInternalCall("Internal_getMaxCurve", (void*)&ScriptTDistributionVector2::Internal_getMaxCurve);
		metaData.scriptClass->addInternalCall("Internal_evaluate", (void*)&ScriptTDistributionVector2::Internal_evaluate);
		metaData.scriptClass->addInternalCall("Internal_evaluate0", (void*)&ScriptTDistributionVector2::Internal_evaluate0);

	}

	MonoObject* ScriptTDistributionVector2::Create(const SPtr<TDistribution<Vector2>>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptTDistributionVector2>()) ScriptTDistributionVector2(managedInstance, value);
		return managedInstance;
	}
	void ScriptTDistributionVector2::InternalTDistribution(MonoObject* managedInstance)
	{
		SPtr<TDistribution<Vector2>> instance = bs_shared_ptr_new<TDistribution<Vector2>>();
		new (bs_alloc<ScriptTDistributionVector2>())ScriptTDistributionVector2(managedInstance, instance);
	}

	void ScriptTDistributionVector2::InternalTDistribution0(MonoObject* managedInstance, Vector2* value)
	{
		SPtr<TDistribution<Vector2>> instance = bs_shared_ptr_new<TDistribution<Vector2>>(*value);
		new (bs_alloc<ScriptTDistributionVector2>())ScriptTDistributionVector2(managedInstance, instance);
	}

	void ScriptTDistributionVector2::InternalTDistribution1(MonoObject* managedInstance, Vector2* minValue, Vector2* maxValue)
	{
		SPtr<TDistribution<Vector2>> instance = bs_shared_ptr_new<TDistribution<Vector2>>(*minValue, *maxValue);
		new (bs_alloc<ScriptTDistributionVector2>())ScriptTDistributionVector2(managedInstance, instance);
	}

	void ScriptTDistributionVector2::InternalTDistribution2(MonoObject* managedInstance, MonoObject* curve)
	{
		SPtr<TAnimationCurve<Vector2>> tmpcurve;
		ScriptTAnimationCurveVector2* scriptcurve;
		scriptcurve = ScriptTAnimationCurveVector2::toNative(curve);
		if(scriptcurve != nullptr)
			tmpcurve = scriptcurve->GetInternal();
		SPtr<TDistribution<Vector2>> instance = bs_shared_ptr_new<TDistribution<Vector2>>(*tmpcurve);
		new (bs_alloc<ScriptTDistributionVector2>())ScriptTDistributionVector2(managedInstance, instance);
	}

	void ScriptTDistributionVector2::InternalTDistribution3(MonoObject* managedInstance, MonoObject* minCurve, MonoObject* maxCurve)
	{
		SPtr<TAnimationCurve<Vector2>> tmpminCurve;
		ScriptTAnimationCurveVector2* scriptminCurve;
		scriptminCurve = ScriptTAnimationCurveVector2::toNative(minCurve);
		if(scriptminCurve != nullptr)
			tmpminCurve = scriptminCurve->GetInternal();
		SPtr<TAnimationCurve<Vector2>> tmpmaxCurve;
		ScriptTAnimationCurveVector2* scriptmaxCurve;
		scriptmaxCurve = ScriptTAnimationCurveVector2::toNative(maxCurve);
		if(scriptmaxCurve != nullptr)
			tmpmaxCurve = scriptmaxCurve->GetInternal();
		SPtr<TDistribution<Vector2>> instance = bs_shared_ptr_new<TDistribution<Vector2>>(*tmpminCurve, *tmpmaxCurve);
		new (bs_alloc<ScriptTDistributionVector2>())ScriptTDistributionVector2(managedInstance, instance);
	}

	PropertyDistributionType ScriptTDistributionVector2::InternalGetType(ScriptTDistributionVector2* thisPtr)
	{
		PropertyDistributionType tmp__output;
		tmp__output = thisPtr->GetInternal()->GetType();

		PropertyDistributionType __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptTDistributionVector2::InternalGetMinConstant(ScriptTDistributionVector2* thisPtr, Vector2* __output)
	{
		Vector2 tmp__output;
		tmp__output = thisPtr->GetInternal()->GetMinConstant();

		*__output = tmp__output;
	}

	void ScriptTDistributionVector2::InternalGetMaxConstant(ScriptTDistributionVector2* thisPtr, Vector2* __output)
	{
		Vector2 tmp__output;
		tmp__output = thisPtr->GetInternal()->GetMaxConstant();

		*__output = tmp__output;
	}

	MonoObject* ScriptTDistributionVector2::InternalGetMinCurve(ScriptTDistributionVector2* thisPtr)
	{
		SPtr<TAnimationCurve<Vector2>> tmp__output = bs_shared_ptr_new<TAnimationCurve<Vector2>>();
		*tmp__output = thisPtr->GetInternal()->GetMinCurve();

		MonoObject* __output;
		__output = ScriptTAnimationCurveVector2::Create(tmp__output);

		return __output;
	}

	MonoObject* ScriptTDistributionVector2::InternalGetMaxCurve(ScriptTDistributionVector2* thisPtr)
	{
		SPtr<TAnimationCurve<Vector2>> tmp__output = bs_shared_ptr_new<TAnimationCurve<Vector2>>();
		*tmp__output = thisPtr->GetInternal()->GetMaxCurve();

		MonoObject* __output;
		__output = ScriptTAnimationCurveVector2::Create(tmp__output);

		return __output;
	}

	void ScriptTDistributionVector2::InternalEvaluate(ScriptTDistributionVector2* thisPtr, float t, float factor, Vector2* __output)
	{
		Vector2 tmp__output;
		tmp__output = thisPtr->GetInternal()->evaluate(t, factor);

		*__output = tmp__output;
	}

	void ScriptTDistributionVector2::InternalEvaluate0(ScriptTDistributionVector2* thisPtr, float t, MonoObject* factor, Vector2* __output)
	{
		SPtr<Random> tmpfactor;
		ScriptRandom* scriptfactor;
		scriptfactor = ScriptRandom::toNative(factor);
		if(scriptfactor != nullptr)
			tmpfactor = scriptfactor->GetInternal();
		Vector2 tmp__output;
		tmp__output = thisPtr->GetInternal()->evaluate(t, *tmpfactor);

		*__output = tmp__output;
	}
}
