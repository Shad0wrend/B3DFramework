//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptAnimationCurves.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Animation/BsAnimationClip.h"
#include "../Extensions/BsAnimationEx.h"
#include "BsScriptTAnimationCurve.generated.h"
#include "BsScriptTAnimationCurve.generated.h"
#include "BsScriptTAnimationCurve.generated.h"
#include "BsScriptTNamedAnimationCurve.generated.h"
#include "BsScriptTNamedAnimationCurve.generated.h"
#include "BsScriptTNamedAnimationCurve.generated.h"

namespace bs
{
	ScriptAnimationCurves::ScriptAnimationCurves(MonoObject* managedInstance, const SPtr<AnimationCurves>& value)
		:ScriptObject(managedInstance), mInternal(value)
	{
	}

	void ScriptAnimationCurves::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_AnimationCurves", (void*)&ScriptAnimationCurves::InternalAnimationCurves);
		metaData.scriptClass->AddInternalCall("Internal_addPositionCurve", (void*)&ScriptAnimationCurves::InternalAddPositionCurve);
		metaData.scriptClass->AddInternalCall("Internal_addRotationCurve", (void*)&ScriptAnimationCurves::InternalAddRotationCurve);
		metaData.scriptClass->AddInternalCall("Internal_addScaleCurve", (void*)&ScriptAnimationCurves::InternalAddScaleCurve);
		metaData.scriptClass->AddInternalCall("Internal_addGenericCurve", (void*)&ScriptAnimationCurves::InternalAddGenericCurve);
		metaData.scriptClass->AddInternalCall("Internal_removePositionCurve", (void*)&ScriptAnimationCurves::InternalRemovePositionCurve);
		metaData.scriptClass->AddInternalCall("Internal_removeRotationCurve", (void*)&ScriptAnimationCurves::InternalRemoveRotationCurve);
		metaData.scriptClass->AddInternalCall("Internal_removeScaleCurve", (void*)&ScriptAnimationCurves::InternalRemoveScaleCurve);
		metaData.scriptClass->addInternalCall("Internal_removeGenericCurve", (void*)&ScriptAnimationCurves::InternalRemoveGenericCurve);
		metaData.scriptClass->addInternalCall("Internal_getPositionCurves", (void*)&ScriptAnimationCurves::InternalGetPositionCurves);
		metaData.scriptClass->addInternalCall("Internal_setPositionCurves", (void*)&ScriptAnimationCurves::InternalSetPositionCurves);
		metaData.scriptClass->addInternalCall("Internal_getRotationCurves", (void*)&ScriptAnimationCurves::InternalGetRotationCurves);
		metaData.scriptClass->addInternalCall("Internal_setRotationCurves", (void*)&ScriptAnimationCurves::InternalSetRotationCurves);
		metaData.scriptClass->addInternalCall("Internal_getScaleCurves", (void*)&ScriptAnimationCurves::InternalGetScaleCurves);
		metaData.scriptClass->addInternalCall("Internal_setScaleCurves", (void*)&ScriptAnimationCurves::InternalSetScaleCurves);
		metaData.scriptClass->addInternalCall("Internal_getGenericCurves", (void*)&ScriptAnimationCurves::InternalGetGenericCurves);
		metaData.scriptClass->addInternalCall("Internal_setGenericCurves", (void*)&ScriptAnimationCurves::InternalSetGenericCurves);

	}

	MonoObject* ScriptAnimationCurves::Create(const SPtr<AnimationCurves>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptAnimationCurves>()) ScriptAnimationCurves(managedInstance, value);
		return managedInstance;
	}
	void ScriptAnimationCurves::InternalAnimationCurves(MonoObject* managedInstance)
	{
		SPtr<AnimationCurves> instance = bs_shared_ptr_new<AnimationCurves>();
		new (bs_alloc<ScriptAnimationCurves>())ScriptAnimationCurves(managedInstance, instance);
	}

	void ScriptAnimationCurves::InternalAddPositionCurve(ScriptAnimationCurves* thisPtr, MonoString* name, MonoObject* curve)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		SPtr<TAnimationCurve<Vector3>> tmpcurve;
		ScriptTAnimationCurveVector3* scriptcurve;
		scriptcurve = ScriptTAnimationCurveVector3::toNative(curve);
		if(scriptcurve != nullptr)
			tmpcurve = scriptcurve->GetInternal();
		thisPtr->GetInternal()->addPositionCurve(tmpname, *tmpcurve);
	}

	void ScriptAnimationCurves::InternalAddRotationCurve(ScriptAnimationCurves* thisPtr, MonoString* name, MonoObject* curve)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		SPtr<TAnimationCurve<Quaternion>> tmpcurve;
		ScriptTAnimationCurveQuaternion* scriptcurve;
		scriptcurve = ScriptTAnimationCurveQuaternion::toNative(curve);
		if(scriptcurve != nullptr)
			tmpcurve = scriptcurve->GetInternal();
		thisPtr->GetInternal()->addRotationCurve(tmpname, *tmpcurve);
	}

	void ScriptAnimationCurves::InternalAddScaleCurve(ScriptAnimationCurves* thisPtr, MonoString* name, MonoObject* curve)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		SPtr<TAnimationCurve<Vector3>> tmpcurve;
		ScriptTAnimationCurveVector3* scriptcurve;
		scriptcurve = ScriptTAnimationCurveVector3::toNative(curve);
		if(scriptcurve != nullptr)
			tmpcurve = scriptcurve->GetInternal();
		thisPtr->GetInternal()->addScaleCurve(tmpname, *tmpcurve);
	}

	void ScriptAnimationCurves::InternalAddGenericCurve(ScriptAnimationCurves* thisPtr, MonoString* name, MonoObject* curve)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		SPtr<TAnimationCurve<float>> tmpcurve;
		ScriptTAnimationCurvefloat* scriptcurve;
		scriptcurve = ScriptTAnimationCurvefloat::toNative(curve);
		if(scriptcurve != nullptr)
			tmpcurve = scriptcurve->GetInternal();
		thisPtr->GetInternal()->addGenericCurve(tmpname, *tmpcurve);
	}

	void ScriptAnimationCurves::InternalRemovePositionCurve(ScriptAnimationCurves* thisPtr, MonoString* name)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		thisPtr->GetInternal()->removePositionCurve(tmpname);
	}

	void ScriptAnimationCurves::InternalRemoveRotationCurve(ScriptAnimationCurves* thisPtr, MonoString* name)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		thisPtr->GetInternal()->removeRotationCurve(tmpname);
	}

	void ScriptAnimationCurves::InternalRemoveScaleCurve(ScriptAnimationCurves* thisPtr, MonoString* name)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		thisPtr->GetInternal()->removeScaleCurve(tmpname);
	}

	void ScriptAnimationCurves::InternalRemoveGenericCurve(ScriptAnimationCurves* thisPtr, MonoString* name)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		thisPtr->GetInternal()->removeGenericCurve(tmpname);
	}

	MonoArray* ScriptAnimationCurves::InternalGetPositionCurves(ScriptAnimationCurves* thisPtr)
	{
		Vector<TNamedAnimationCurve<Vector3>> vec__output;
		vec__output = AnimationCurvesEx::getPositionCurves(thisPtr->GetInternal());

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptTNamedAnimationCurveVector3>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, ScriptTNamedAnimationCurveVector3::toInterop(vec__output[i]));
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptAnimationCurves::InternalSetPositionCurves(ScriptAnimationCurves* thisPtr, MonoArray* value)
	{
		Vector<TNamedAnimationCurve<Vector3>> vecvalue;
		if(value != nullptr)
		{
			ScriptArray arrayvalue(value);
			vecvalue.resize(arrayvalue.size());
			for(int i = 0; i < (int)arrayvalue.size(); i++)
			{
				vecvalue[i] = ScriptTNamedAnimationCurveVector3::fromInterop(arrayvalue.get<__TNamedAnimationCurveVector3Interop>(i));
			}
		}
		AnimationCurvesEx::setPositionCurves(thisPtr->GetInternal(), vecvalue);
	}

	MonoArray* ScriptAnimationCurves::InternalGetRotationCurves(ScriptAnimationCurves* thisPtr)
	{
		Vector<TNamedAnimationCurve<Quaternion>> vec__output;
		vec__output = AnimationCurvesEx::getRotationCurves(thisPtr->GetInternal());

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptTNamedAnimationCurveQuaternion>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, ScriptTNamedAnimationCurveQuaternion::toInterop(vec__output[i]));
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptAnimationCurves::InternalSetRotationCurves(ScriptAnimationCurves* thisPtr, MonoArray* value)
	{
		Vector<TNamedAnimationCurve<Quaternion>> vecvalue;
		if(value != nullptr)
		{
			ScriptArray arrayvalue(value);
			vecvalue.resize(arrayvalue.size());
			for(int i = 0; i < (int)arrayvalue.size(); i++)
			{
				vecvalue[i] = ScriptTNamedAnimationCurveQuaternion::fromInterop(arrayvalue.get<__TNamedAnimationCurveQuaternionInterop>(i));
			}
		}
		AnimationCurvesEx::setRotationCurves(thisPtr->GetInternal(), vecvalue);
	}

	MonoArray* ScriptAnimationCurves::InternalGetScaleCurves(ScriptAnimationCurves* thisPtr)
	{
		Vector<TNamedAnimationCurve<Vector3>> vec__output;
		vec__output = AnimationCurvesEx::getScaleCurves(thisPtr->GetInternal());

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptTNamedAnimationCurveVector3>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, ScriptTNamedAnimationCurveVector3::toInterop(vec__output[i]));
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptAnimationCurves::InternalSetScaleCurves(ScriptAnimationCurves* thisPtr, MonoArray* value)
	{
		Vector<TNamedAnimationCurve<Vector3>> vecvalue;
		if(value != nullptr)
		{
			ScriptArray arrayvalue(value);
			vecvalue.resize(arrayvalue.size());
			for(int i = 0; i < (int)arrayvalue.size(); i++)
			{
				vecvalue[i] = ScriptTNamedAnimationCurveVector3::fromInterop(arrayvalue.get<__TNamedAnimationCurveVector3Interop>(i));
			}
		}
		AnimationCurvesEx::setScaleCurves(thisPtr->GetInternal(), vecvalue);
	}

	MonoArray* ScriptAnimationCurves::InternalGetGenericCurves(ScriptAnimationCurves* thisPtr)
	{
		Vector<TNamedAnimationCurve<float>> vec__output;
		vec__output = AnimationCurvesEx::getGenericCurves(thisPtr->GetInternal());

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptTNamedAnimationCurvefloat>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, ScriptTNamedAnimationCurvefloat::toInterop(vec__output[i]));
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptAnimationCurves::InternalSetGenericCurves(ScriptAnimationCurves* thisPtr, MonoArray* value)
	{
		Vector<TNamedAnimationCurve<float>> vecvalue;
		if(value != nullptr)
		{
			ScriptArray arrayvalue(value);
			vecvalue.resize(arrayvalue.size());
			for(int i = 0; i < (int)arrayvalue.size(); i++)
			{
				vecvalue[i] = ScriptTNamedAnimationCurvefloat::fromInterop(arrayvalue.get<__TNamedAnimationCurvefloatInterop>(i));
			}
		}
		AnimationCurvesEx::setGenericCurves(thisPtr->GetInternal(), vecvalue);
	}
}
