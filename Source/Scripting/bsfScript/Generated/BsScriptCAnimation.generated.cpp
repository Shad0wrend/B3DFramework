//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptCAnimation.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Components/BsCAnimation.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "../../../Foundation/bsfCore/Animation/BsAnimationClip.h"
#include "BsScriptBlend1DInfo.generated.h"
#include "BsScriptBlend2DInfo.generated.h"
#include "Wrappers/BsScriptVector.h"
#include "BsScriptAnimationClipState.generated.h"

namespace bs
{
	ScriptCAnimation::_scriptRebuildFloatPropertiesThunkDef ScriptCAnimation::_scriptRebuildFloatPropertiesThunk; 
	ScriptCAnimation::_scriptUpdateFloatPropertiesThunkDef ScriptCAnimation::_scriptUpdateFloatPropertiesThunk; 
	ScriptCAnimation::_scriptOnEventTriggeredThunkDef ScriptCAnimation::_scriptOnEventTriggeredThunk; 

	ScriptCAnimation::ScriptCAnimation(MonoObject* managedInstance, const GameObjectHandle<CAnimation>& value)
		:TScriptComponent(managedInstance, value)
	{
		value->ScriptOnEventTriggeredInternal = std::bind(&ScriptCAnimation::ScriptRebuildFloatPropertiesInternal, this, std::placeholders::_1);
		value->ScriptOnEventTriggeredInternal = std::bind(&ScriptCAnimation::ScriptUpdateFloatPropertiesInternal, this);
		value->ScriptOnEventTriggeredInternal = std::bind(&ScriptCAnimation::ScriptOnEventTriggeredInternal, this, std::placeholders::_1, std::placeholders::_2);
	}

	void ScriptCAnimation::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_setDefaultClip", (void*)&ScriptCAnimation::InternalSetDefaultClip);
		metaData.scriptClass->AddInternalCall("Internal_getDefaultClip", (void*)&ScriptCAnimation::InternalGetDefaultClip);
		metaData.scriptClass->AddInternalCall("Internal_setWrapMode", (void*)&ScriptCAnimation::InternalSetWrapMode);
		metaData.scriptClass->AddInternalCall("Internal_getWrapMode", (void*)&ScriptCAnimation::InternalGetWrapMode);
		metaData.scriptClass->AddInternalCall("Internal_setSpeed", (void*)&ScriptCAnimation::InternalSetSpeed);
		metaData.scriptClass->AddInternalCall("Internal_getSpeed", (void*)&ScriptCAnimation::InternalGetSpeed);
		metaData.scriptClass->AddInternalCall("Internal_play", (void*)&ScriptCAnimation::InternalPlay);
		metaData.scriptClass->AddInternalCall("Internal_blendAdditive", (void*)&ScriptCAnimation::InternalBlendAdditive);
		metaData.scriptClass->AddInternalCall("Internal_blend1D", (void*)&ScriptCAnimation::InternalBlend1D);
		metaData.scriptClass->AddInternalCall("Internal_blend2D", (void*)&ScriptCAnimation::InternalBlend2D);
		metaData.scriptClass->AddInternalCall("Internal_crossFade", (void*)&ScriptCAnimation::InternalCrossFade);
		metaData.scriptClass->AddInternalCall("Internal_sample", (void*)&ScriptCAnimation::InternalSample);
		metaData.scriptClass->AddInternalCall("Internal_stop", (void*)&ScriptCAnimation::InternalStop);
		metaData.scriptClass->AddInternalCall("Internal_stopAll", (void*)&ScriptCAnimation::InternalStopAll);
		metaData.scriptClass->AddInternalCall("Internal_isPlaying", (void*)&ScriptCAnimation::InternalIsPlaying);
		metaData.scriptClass->AddInternalCall("Internal_getState", (void*)&ScriptCAnimation::InternalGetState);
		metaData.scriptClass->AddInternalCall("Internal_setState", (void*)&ScriptCAnimation::InternalSetState);
		metaData.scriptClass->AddInternalCall("Internal_setMorphChannelWeight", (void*)&ScriptCAnimation::InternalSetMorphChannelWeight);
		metaData.scriptClass->AddInternalCall("Internal_setBounds", (void*)&ScriptCAnimation::InternalSetBounds);
		metaData.scriptClass->AddInternalCall("Internal_getBounds", (void*)&ScriptCAnimation::InternalGetBounds);
		metaData.scriptClass->AddInternalCall("Internal_setUseBounds", (void*)&ScriptCAnimation::InternalSetUseBounds);
		metaData.scriptClass->AddInternalCall("Internal_getUseBounds", (void*)&ScriptCAnimation::InternalGetUseBounds);
		metaData.scriptClass->AddInternalCall("Internal_setEnableCull", (void*)&ScriptCAnimation::InternalSetEnableCull);
		metaData.scriptClass->AddInternalCall("Internal_getEnableCull", (void*)&ScriptCAnimation::InternalGetEnableCull);
		metaData.scriptClass->AddInternalCall("Internal_getNumClips", (void*)&ScriptCAnimation::InternalGetNumClips);
		metaData.scriptClass->AddInternalCall("Internal_getClip", (void*)&ScriptCAnimation::InternalGetClip);
		metaData.scriptClass->AddInternalCall("Internal__refreshClipMappings", (void*)&ScriptCAnimation::InternalRefreshClipMappings);
		metaData.scriptClass->AddInternalCall("Internal__getGenericCurveValue", (void*)&ScriptCAnimation::InternalGetGenericCurveValue);
		metaData.scriptClass->AddInternalCall("Internal__togglePreviewMode", (void*)&ScriptCAnimation::InternalTogglePreviewMode);

		_scriptRebuildFloatPropertiesThunk = (_scriptRebuildFloatPropertiesThunkDef)metaData.scriptClass->GetMethodExact("Internal__scriptRebuildFloatProperties", "RRef`1<AnimationClip>")->GetThunk();
		_scriptUpdateFloatPropertiesThunk = (_scriptUpdateFloatPropertiesThunkDef)metaData.scriptClass->GetMethodExact("Internal__scriptUpdateFloatProperties", "")->GetThunk();
		_scriptOnEventTriggeredThunk = (_scriptOnEventTriggeredThunkDef)metaData.scriptClass->GetMethodExact("Internal__scriptOnEventTriggered", "RRef`1<AnimationClip>,string")->GetThunk();
	}

	void ScriptCAnimation::ScriptRebuildFloatPropertiesInternal(const ResourceHandle<AnimationClip>& p0)
	{
		MonoObject* tmpp0;
		ScriptRRefBase* scriptp0;
		scriptp0 = ScriptResourceManager::Instance().getScriptRRef(p0);
		if(scriptp0 != nullptr)
			tmpp0 = scriptp0->GetManagedInstance();
		else
			tmpp0 = nullptr;
		MonoUtil::InvokeThunk(_scriptRebuildFloatPropertiesThunk, GetManagedInstance(), tmpp0);
	}

	void ScriptCAnimation::ScriptUpdateFloatPropertiesInternal()
	{
		MonoUtil::InvokeThunk(_scriptUpdateFloatPropertiesThunk, GetManagedInstance());
	}

	void ScriptCAnimation::ScriptOnEventTriggeredInternal(const ResourceHandle<AnimationClip>& p0, const String& p1)
	{
		MonoObject* tmpp0;
		ScriptRRefBase* scriptp0;
		scriptp0 = ScriptResourceManager::Instance().getScriptRRef(p0);
		if(scriptp0 != nullptr)
			tmpp0 = scriptp0->GetManagedInstance();
		else
			tmpp0 = nullptr;
		MonoString* tmpp1;
		tmpp1 = MonoUtil::stringToMono(p1);
		MonoUtil::InvokeThunk(_scriptOnEventTriggeredThunk, GetManagedInstance(), tmpp0, tmpp1);
	}
	void ScriptCAnimation::InternalSetDefaultClip(ScriptCAnimation* thisPtr, MonoObject* clip)
	{
		ResourceHandle<AnimationClip> tmpclip;
		ScriptRRefBase* scriptclip;
		scriptclip = ScriptRRefBase::toNative(clip);
		if(scriptclip != nullptr)
			tmpclip = static_resource_cast<AnimationClip>(scriptclip->GetHandle());
		thisPtr->GetHandle()->SetDefaultClip(tmpclip);
	}

	MonoObject* ScriptCAnimation::InternalGetDefaultClip(ScriptCAnimation* thisPtr)
	{
		ResourceHandle<AnimationClip> tmp__output;
		tmp__output = thisPtr->GetHandle()->GetDefaultClip();

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().getScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptCAnimation::InternalSetWrapMode(ScriptCAnimation* thisPtr, AnimWrapMode wrapMode)
	{
		thisPtr->GetHandle()->SetWrapMode(wrapMode);
	}

	AnimWrapMode ScriptCAnimation::InternalGetWrapMode(ScriptCAnimation* thisPtr)
	{
		AnimWrapMode tmp__output;
		tmp__output = thisPtr->GetHandle()->GetWrapMode();

		AnimWrapMode __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCAnimation::InternalSetSpeed(ScriptCAnimation* thisPtr, float speed)
	{
		thisPtr->GetHandle()->SetSpeed(speed);
	}

	float ScriptCAnimation::InternalGetSpeed(ScriptCAnimation* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetHandle()->GetSpeed();

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCAnimation::InternalPlay(ScriptCAnimation* thisPtr, MonoObject* clip)
	{
		ResourceHandle<AnimationClip> tmpclip;
		ScriptRRefBase* scriptclip;
		scriptclip = ScriptRRefBase::toNative(clip);
		if(scriptclip != nullptr)
			tmpclip = static_resource_cast<AnimationClip>(scriptclip->GetHandle());
		thisPtr->GetHandle()->play(tmpclip);
	}

	void ScriptCAnimation::InternalBlendAdditive(ScriptCAnimation* thisPtr, MonoObject* clip, float weight, float fadeLength, uint32_t layer)
	{
		ResourceHandle<AnimationClip> tmpclip;
		ScriptRRefBase* scriptclip;
		scriptclip = ScriptRRefBase::toNative(clip);
		if(scriptclip != nullptr)
			tmpclip = static_resource_cast<AnimationClip>(scriptclip->GetHandle());
		thisPtr->GetHandle()->blendAdditive(tmpclip, weight, fadeLength, layer);
	}

	void ScriptCAnimation::InternalBlend1D(ScriptCAnimation* thisPtr, __Blend1DInfoInterop* info, float t)
	{
		Blend1DInfo tmpinfo;
		tmpinfo = ScriptBlend1DInfo::fromInterop(*info);
		thisPtr->GetHandle()->blend1D(tmpinfo, t);
	}

	void ScriptCAnimation::InternalBlend2D(ScriptCAnimation* thisPtr, __Blend2DInfoInterop* info, Vector2* t)
	{
		Blend2DInfo tmpinfo;
		tmpinfo = ScriptBlend2DInfo::fromInterop(*info);
		thisPtr->GetHandle()->blend2D(tmpinfo, *t);
	}

	void ScriptCAnimation::InternalCrossFade(ScriptCAnimation* thisPtr, MonoObject* clip, float fadeLength)
	{
		ResourceHandle<AnimationClip> tmpclip;
		ScriptRRefBase* scriptclip;
		scriptclip = ScriptRRefBase::toNative(clip);
		if(scriptclip != nullptr)
			tmpclip = static_resource_cast<AnimationClip>(scriptclip->GetHandle());
		thisPtr->GetHandle()->crossFade(tmpclip, fadeLength);
	}

	void ScriptCAnimation::InternalSample(ScriptCAnimation* thisPtr, MonoObject* clip, float time)
	{
		ResourceHandle<AnimationClip> tmpclip;
		ScriptRRefBase* scriptclip;
		scriptclip = ScriptRRefBase::toNative(clip);
		if(scriptclip != nullptr)
			tmpclip = static_resource_cast<AnimationClip>(scriptclip->GetHandle());
		thisPtr->GetHandle()->sample(tmpclip, time);
	}

	void ScriptCAnimation::InternalStop(ScriptCAnimation* thisPtr, uint32_t layer)
	{
		thisPtr->GetHandle()->stop(layer);
	}

	void ScriptCAnimation::InternalStopAll(ScriptCAnimation* thisPtr)
	{
		thisPtr->GetHandle()->stopAll();
	}

	bool ScriptCAnimation::InternalIsPlaying(ScriptCAnimation* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetHandle()->isPlaying();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptCAnimation::InternalGetState(ScriptCAnimation* thisPtr, MonoObject* clip, AnimationClipState* state)
	{
		bool tmp__output;
		ResourceHandle<AnimationClip> tmpclip;
		ScriptRRefBase* scriptclip;
		scriptclip = ScriptRRefBase::toNative(clip);
		if(scriptclip != nullptr)
			tmpclip = static_resource_cast<AnimationClip>(scriptclip->GetHandle());
		tmp__output = thisPtr->GetHandle()->GetState(tmpclip, *state);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCAnimation::InternalSetState(ScriptCAnimation* thisPtr, MonoObject* clip, AnimationClipState* state)
	{
		ResourceHandle<AnimationClip> tmpclip;
		ScriptRRefBase* scriptclip;
		scriptclip = ScriptRRefBase::toNative(clip);
		if(scriptclip != nullptr)
			tmpclip = static_resource_cast<AnimationClip>(scriptclip->GetHandle());
		thisPtr->GetHandle()->SetState(tmpclip, *state);
	}

	void ScriptCAnimation::InternalSetMorphChannelWeight(ScriptCAnimation* thisPtr, MonoString* name, float weight)
	{
		String tmpname;
		tmpname = MonoUtil::MonoToString(name);
		thisPtr->GetHandle()->SetMorphChannelWeight(tmpname, weight);
	}

	void ScriptCAnimation::InternalSetBounds(ScriptCAnimation* thisPtr, AABox* bounds)
	{
		thisPtr->GetHandle()->SetBounds(*bounds);
	}

	void ScriptCAnimation::InternalGetBounds(ScriptCAnimation* thisPtr, AABox* __output)
	{
		AABox tmp__output;
		tmp__output = thisPtr->GetHandle()->GetBounds();

		*__output = tmp__output;
	}

	void ScriptCAnimation::InternalSetUseBounds(ScriptCAnimation* thisPtr, bool enable)
	{
		thisPtr->GetHandle()->SetUseBounds(enable);
	}

	bool ScriptCAnimation::InternalGetUseBounds(ScriptCAnimation* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetHandle()->GetUseBounds();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCAnimation::InternalSetEnableCull(ScriptCAnimation* thisPtr, bool enable)
	{
		thisPtr->GetHandle()->SetEnableCull(enable);
	}

	bool ScriptCAnimation::InternalGetEnableCull(ScriptCAnimation* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetHandle()->GetEnableCull();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	uint32_t ScriptCAnimation::InternalGetNumClips(ScriptCAnimation* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetHandle()->GetNumClips();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	MonoObject* ScriptCAnimation::InternalGetClip(ScriptCAnimation* thisPtr, uint32_t idx)
	{
		ResourceHandle<AnimationClip> tmp__output;
		tmp__output = thisPtr->GetHandle()->GetClip(idx);

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().getScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->GetManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptCAnimation::InternalRefreshClipMappings(ScriptCAnimation* thisPtr)
	{
		thisPtr->GetHandle()->RefreshClipMappingsInternal();
	}

	bool ScriptCAnimation::InternalGetGenericCurveValue(ScriptCAnimation* thisPtr, uint32_t curveIdx, float* value)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetHandle()->GetGenericCurveValueInternal(curveIdx, *value);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptCAnimation::InternalTogglePreviewMode(ScriptCAnimation* thisPtr, bool enabled)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetHandle()->TogglePreviewModeInternal(enabled);

		bool __output;
		__output = tmp__output;

		return __output;
	}
}
