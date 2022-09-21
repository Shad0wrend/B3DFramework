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

	void ScriptAnimationClip::InitRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_GetRef", (void*)&ScriptAnimationClip::InternalGetRef);
		metaData.scriptClass->AddInternalCall("Internal_GetCurves", (void*)&ScriptAnimationClip::InternalGetCurves);
		metaData.scriptClass->AddInternalCall("Internal_SetCurves", (void*)&ScriptAnimationClip::InternalSetCurves);
		metaData.scriptClass->AddInternalCall("Internal_GetEvents", (void*)&ScriptAnimationClip::InternalGetEvents);
		metaData.scriptClass->AddInternalCall("Internal_SetEvents", (void*)&ScriptAnimationClip::InternalSetEvents);
		metaData.scriptClass->AddInternalCall("Internal_GetRootMotion", (void*)&ScriptAnimationClip::InternalGetRootMotion);
		metaData.scriptClass->AddInternalCall("Internal_HasRootMotion", (void*)&ScriptAnimationClip::InternalHasRootMotion);
		metaData.scriptClass->AddInternalCall("Internal_IsAdditive", (void*)&ScriptAnimationClip::InternalIsAdditive);
		metaData.scriptClass->AddInternalCall("Internal_GetLength", (void*)&ScriptAnimationClip::InternalGetLength);
		metaData.scriptClass->AddInternalCall("Internal_GetSampleRate", (void*)&ScriptAnimationClip::InternalGetSampleRate);
		metaData.scriptClass->AddInternalCall("Internal_SetSampleRate", (void*)&ScriptAnimationClip::InternalSetSampleRate);
		metaData.scriptClass->AddInternalCall("Internal_Create", (void*)&ScriptAnimationClip::InternalCreate);
		metaData.scriptClass->AddInternalCall("Internal_Create0", (void*)&ScriptAnimationClip::InternalCreate0);

	}

	 MonoObject*ScriptAnimationClip::CreateInstance()
	{
		bool dummy = false;
		void* ctorParams[2] = { &dummy, &dummy };

		return metaData.scriptClass->CreateInstance("bool,bool", ctorParams);
	}
	MonoObject* ScriptAnimationClip::InternalGetRef(ScriptAnimationClip* thisPtr)
	{
		return thisPtr->GetRRef();
	}

	MonoObject* ScriptAnimationClip::InternalGetCurves(ScriptAnimationClip* thisPtr)
	{
		SPtr<AnimationCurves> tmp__output;
		tmp__output = thisPtr->GetHandle()->GetCurves();

		MonoObject* __output;
		__output = ScriptAnimationCurves::Create(tmp__output);

		return __output;
	}

	void ScriptAnimationClip::InternalSetCurves(ScriptAnimationClip* thisPtr, MonoObject* curves)
	{
		SPtr<AnimationCurves> tmpcurves;
		ScriptAnimationCurves* scriptcurves;
		scriptcurves = ScriptAnimationCurves::ToNative(curves);
		if(scriptcurves != nullptr)
			tmpcurves = scriptcurves->GetInternal();
		thisPtr->GetHandle()->SetCurves(*tmpcurves);
	}

	MonoArray* ScriptAnimationClip::InternalGetEvents(ScriptAnimationClip* thisPtr)
	{
		Vector<AnimationEvent> vec__output;
		vec__output = thisPtr->GetHandle()->GetEvents();

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::Create<ScriptAnimationEvent>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, ScriptAnimationEvent::ToInterop(vec__output[i]));
		}
		__output = array__output.GetInternal();

		return __output;
	}

	void ScriptAnimationClip::InternalSetEvents(ScriptAnimationClip* thisPtr, MonoArray* events)
	{
		Vector<AnimationEvent> vecevents;
		if(events != nullptr)
		{
			ScriptArray arrayevents(events);
			vecevents.resize(arrayevents.Size());
			for(int i = 0; i < (int)arrayevents.Size(); i++)
			{
				vecevents[i] = ScriptAnimationEvent::FromInterop(arrayevents.Get<__AnimationEventInterop>(i));
			}
		}
		thisPtr->GetHandle()->SetEvents(vecevents);
	}

	MonoObject* ScriptAnimationClip::InternalGetRootMotion(ScriptAnimationClip* thisPtr)
	{
		SPtr<RootMotion> tmp__output;
		tmp__output = thisPtr->GetHandle()->GetRootMotion();

		MonoObject* __output;
		__output = ScriptRootMotion::Create(tmp__output);

		return __output;
	}

	bool ScriptAnimationClip::InternalHasRootMotion(ScriptAnimationClip* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetHandle()->HasRootMotion();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptAnimationClip::InternalIsAdditive(ScriptAnimationClip* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetHandle()->IsAdditive();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	float ScriptAnimationClip::InternalGetLength(ScriptAnimationClip* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetLength();

		float __output;
		__output = tmp__output;

		return __output;
	}

	uint32_t ScriptAnimationClip::InternalGetSampleRate(ScriptAnimationClip* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetHandle()->GetSampleRate();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAnimationClip::InternalSetSampleRate(ScriptAnimationClip* thisPtr, uint32_t sampleRate)
	{
		thisPtr->GetHandle()->SetSampleRate(sampleRate);
	}

	void ScriptAnimationClip::InternalCreate(MonoObject* managedInstance, bool isAdditive)
	{
		ResourceHandle<AnimationClip> instance = AnimationClip::Create(isAdditive);
		ScriptResourceManager::Instance().CreateBuiltinScriptResource(instance, managedInstance);
	}

	void ScriptAnimationClip::InternalCreate0(MonoObject* managedInstance, MonoObject* curves, bool isAdditive, uint32_t sampleRate, MonoObject* rootMotion)
	{
		SPtr<AnimationCurves> tmpcurves;
		ScriptAnimationCurves* scriptcurves;
		scriptcurves = ScriptAnimationCurves::ToNative(curves);
		if(scriptcurves != nullptr)
			tmpcurves = scriptcurves->GetInternal();
		SPtr<RootMotion> tmprootMotion;
		ScriptRootMotion* scriptrootMotion;
		scriptrootMotion = ScriptRootMotion::ToNative(rootMotion);
		if(scriptrootMotion != nullptr)
			tmprootMotion = scriptrootMotion->GetInternal();
		ResourceHandle<AnimationClip> instance = AnimationClip::Create(tmpcurves, isAdditive, sampleRate, tmprootMotion);
		ScriptResourceManager::Instance().CreateBuiltinScriptResource(instance, managedInstance);
	}
}
