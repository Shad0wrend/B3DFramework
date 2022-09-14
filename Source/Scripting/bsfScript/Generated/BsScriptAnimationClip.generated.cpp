//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptAnimationClip.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Animation/BsAnimationClip.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "BsScriptAnimationCurves.generated.h"
#include "BsScriptRootMotion.generated.h"
#include "BsScriptAnimationEvent.generated.h"
#include "../../../Foundation/bsfCore/Animation/BsAnimationClip.h"

namespace bs
{
	ScriptAnimationClip::ScriptAnimationClip(MonoObject* managedInstance, const ResourceHandle<AnimationClip>& value)
		:TScriptResource(managedInstance, value)
	{
	}

	void ScriptAnimationClip::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_GetRef", (void*)&ScriptAnimationClip::InternalGetRef);
		metaData.scriptClass->addInternalCall("Internal_getCurves", (void*)&ScriptAnimationClip::Internal_getCurves);
		metaData.scriptClass->addInternalCall("Internal_setCurves", (void*)&ScriptAnimationClip::Internal_setCurves);
		metaData.scriptClass->addInternalCall("Internal_getEvents", (void*)&ScriptAnimationClip::Internal_getEvents);
		metaData.scriptClass->addInternalCall("Internal_setEvents", (void*)&ScriptAnimationClip::Internal_setEvents);
		metaData.scriptClass->addInternalCall("Internal_getRootMotion", (void*)&ScriptAnimationClip::Internal_getRootMotion);
		metaData.scriptClass->addInternalCall("Internal_hasRootMotion", (void*)&ScriptAnimationClip::Internal_hasRootMotion);
		metaData.scriptClass->addInternalCall("Internal_isAdditive", (void*)&ScriptAnimationClip::Internal_isAdditive);
		metaData.scriptClass->addInternalCall("Internal_getLength", (void*)&ScriptAnimationClip::Internal_getLength);
		metaData.scriptClass->addInternalCall("Internal_getSampleRate", (void*)&ScriptAnimationClip::Internal_getSampleRate);
		metaData.scriptClass->addInternalCall("Internal_setSampleRate", (void*)&ScriptAnimationClip::Internal_setSampleRate);
		metaData.scriptClass->addInternalCall("Internal_create", (void*)&ScriptAnimationClip::Internal_create);
		metaData.scriptClass->addInternalCall("Internal_create0", (void*)&ScriptAnimationClip::Internal_create0);

	}

	 MonoObject*ScriptAnimationClip::CreateInstance()
	{
		bool dummy = false;
		void* ctorParams[2] = { &dummy, &dummy };

		return metaData.scriptClass->createInstance("bool,bool", ctorParams);
	}
	MonoObject* ScriptAnimationClip::InternalGetRef(ScriptAnimationClip* thisPtr)
	{
		return thisPtr->getRRef();
	}

	MonoObject* ScriptAnimationClip::InternalGetCurves(ScriptAnimationClip* thisPtr)
	{
		SPtr<AnimationCurves> tmp__output;
		tmp__output = thisPtr->getHandle()->getCurves();

		MonoObject* __output;
		__output = ScriptAnimationCurves::Create(tmp__output);

		return __output;
	}

	void ScriptAnimationClip::InternalSetCurves(ScriptAnimationClip* thisPtr, MonoObject* curves)
	{
		SPtr<AnimationCurves> tmpcurves;
		ScriptAnimationCurves* scriptcurves;
		scriptcurves = ScriptAnimationCurves::toNative(curves);
		if(scriptcurves != nullptr)
			tmpcurves = scriptcurves->getInternal();
		thisPtr->getHandle()->setCurves(*tmpcurves);
	}

	MonoArray* ScriptAnimationClip::InternalGetEvents(ScriptAnimationClip* thisPtr)
	{
		Vector<AnimationEvent> vec__output;
		vec__output = thisPtr->getHandle()->getEvents();

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptAnimationEvent>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.set(i, ScriptAnimationEvent::toInterop(vec__output[i]));
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptAnimationClip::InternalSetEvents(ScriptAnimationClip* thisPtr, MonoArray* events)
	{
		Vector<AnimationEvent> vecevents;
		if(events != nullptr)
		{
			ScriptArray arrayevents(events);
			vecevents.resize(arrayevents.size());
			for(int i = 0; i < (int)arrayevents.size(); i++)
			{
				vecevents[i] = ScriptAnimationEvent::fromInterop(arrayevents.get<__AnimationEventInterop>(i));
			}
		}
		thisPtr->getHandle()->setEvents(vecevents);
	}

	MonoObject* ScriptAnimationClip::InternalGetRootMotion(ScriptAnimationClip* thisPtr)
	{
		SPtr<RootMotion> tmp__output;
		tmp__output = thisPtr->getHandle()->getRootMotion();

		MonoObject* __output;
		__output = ScriptRootMotion::Create(tmp__output);

		return __output;
	}

	bool ScriptAnimationClip::InternalHasRootMotion(ScriptAnimationClip* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->getHandle()->hasRootMotion();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptAnimationClip::InternalIsAdditive(ScriptAnimationClip* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->getHandle()->isAdditive();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	float ScriptAnimationClip::InternalGetLength(ScriptAnimationClip* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getHandle()->getLength();

		float __output;
		__output = tmp__output;

		return __output;
	}

	uint32_t ScriptAnimationClip::InternalGetSampleRate(ScriptAnimationClip* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->getHandle()->getSampleRate();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAnimationClip::InternalSetSampleRate(ScriptAnimationClip* thisPtr, uint32_t sampleRate)
	{
		thisPtr->getHandle()->setSampleRate(sampleRate);
	}

	void ScriptAnimationClip::InternalCreate(MonoObject* managedInstance, bool isAdditive)
	{
		ResourceHandle<AnimationClip> instance = AnimationClip::Create(isAdditive);
		ScriptResourceManager::Instance().createBuiltinScriptResource(instance, managedInstance);
	}

	void ScriptAnimationClip::InternalCreate0(MonoObject* managedInstance, MonoObject* curves, bool isAdditive, uint32_t sampleRate, MonoObject* rootMotion)
	{
		SPtr<AnimationCurves> tmpcurves;
		ScriptAnimationCurves* scriptcurves;
		scriptcurves = ScriptAnimationCurves::toNative(curves);
		if(scriptcurves != nullptr)
			tmpcurves = scriptcurves->getInternal();
		SPtr<RootMotion> tmprootMotion;
		ScriptRootMotion* scriptrootMotion;
		scriptrootMotion = ScriptRootMotion::toNative(rootMotion);
		if(scriptrootMotion != nullptr)
			tmprootMotion = scriptrootMotion->getInternal();
		ResourceHandle<AnimationClip> instance = AnimationClip::Create(tmpcurves, isAdditive, sampleRate, tmprootMotion);
		ScriptResourceManager::Instance().createBuiltinScriptResource(instance, managedInstance);
	}
}
