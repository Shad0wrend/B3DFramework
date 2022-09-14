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
		metaData.scriptClass->addInternalCall("Internal_blend1D", (void*)&ScriptCAnimation::InternalBlend1D);
		metaData.scriptClass->addInternalCall("Internal_blend2D", (void*)&ScriptCAnimation::InternalBlend2D);
		metaData.scriptClass->addInternalCall("Internal_crossFade", (void*)&ScriptCAnimation::InternalCrossFade);
		metaData.scriptClass->addInternalCall("Internal_sample", (void*)&ScriptCAnimation::InternalSample);
		metaData.scriptClass->addInternalCall("Internal_stop", (void*)&ScriptCAnimation::InternalStop);
		metaData.scriptClass->addInternalCall("Internal_stopAll", (void*)&ScriptCAnimation::InternalStopAll);
		metaData.scriptClass->addInternalCall("Internal_isPlaying", (void*)&ScriptCAnimation::InternalIsPlaying);
		metaData.scriptClass->addInternalCall("Internal_getState", (void*)&ScriptCAnimation::InternalGetState);
		metaData.scriptClass->addInternalCall("Internal_setState", (void*)&ScriptCAnimation::InternalSetState);
		metaData.scriptClass->addInternalCall("Internal_setMorphChannelWeight", (void*)&ScriptCAnimation::InternalSetMorphChannelWeight);
		metaData.scriptClass->addInternalCall("Internal_setBounds", (void*)&ScriptCAnimation::InternalSetBounds);
		metaData.scriptClass->addInternalCall("Internal_getBounds", (void*)&ScriptCAnimation::InternalGetBounds);
		metaData.scriptClass->addInternalCall("Internal_setUseBounds", (void*)&ScriptCAnimation::InternalSetUseBounds);
		metaData.scriptClass->addInternalCall("Internal_getUseBounds", (void*)&ScriptCAnimation::InternalGetUseBounds);
		metaData.scriptClass->addInternalCall("Internal_setEnableCull", (void*)&ScriptCAnimation::InternalSetEnableCull);
		metaData.scriptClass->addInternalCall("Internal_getEnableCull", (void*)&ScriptCAnimation::InternalGetEnableCull);
		metaData.scriptClass->addInternalCall("Internal_getNumClips", (void*)&ScriptCAnimation::InternalGetNumClips);
		metaData.scriptClass->addInternalCall("Internal_getClip", (void*)&ScriptCAnimation::InternalGetClip);
		metaData.scriptClass->addInternalCall("Internal__refreshClipMappings", (void*)&ScriptCAnimation::InternalRefreshClipMappings);
		metaData.scriptClass->addInternalCall("Internal__getGenericCurveValue", (void*)&ScriptCAnimation::InternalGetGenericCurveValue);
		metaData.scriptClass->addInternalCall("Internal__togglePreviewMode", (void*)&ScriptCAnimation::InternalTogglePreviewMode);

		_scriptRebuildFloatPropertiesThunk = (_scriptRebuildFloatPropertiesThunkDef)metaData.scriptClass->getMethodExact("Internal__scriptRebuildFloatProperties", "RRef`1<AnimationClip>")->getThunk();
		_scriptUpdateFloatPropertiesThunk = (_scriptUpdateFloatPropertiesThunkDef)metaData.scriptClass->getMethodExact("Internal__scriptUpdateFloatProperties", "")->getThunk();
		_scriptOnEventTriggeredThunk = (_scriptOnEventTriggeredThunkDef)metaData.scriptClass->getMethodExact("Internal__scriptOnEventTriggered", "RRef`1<AnimationClip>,string")->getThunk();
	}

	void ScriptCAnimation::ScriptRebuildFloatPropertiesInternal(const ResourceHandle<AnimationClip>& p0)
	{
		MonoObject* tmpp0;
		ScriptRRefBase* scriptp0;
		scriptp0 = ScriptResourceManager::Instance().getScriptRRef(p0);
		if(scriptp0 != nullptr)
			tmpp0 = scriptp0->getManagedInstance();
		else
			tmpp0 = nullptr;
		MonoUtil::invokeThunk(_scriptRebuildFloatPropertiesThunk, getManagedInstance(), tmpp0);
	}

	void ScriptCAnimation::ScriptUpdateFloatPropertiesInternal()
	{
		MonoUtil::invokeThunk(_scriptUpdateFloatPropertiesThunk, getManagedInstance());
	}

	void ScriptCAnimation::ScriptOnEventTriggeredInternal(const ResourceHandle<AnimationClip>& p0, const String& p1)
	{
		MonoObject* tmpp0;
		ScriptRRefBase* scriptp0;
		scriptp0 = ScriptResourceManager::Instance().getScriptRRef(p0);
		if(scriptp0 != nullptr)
			tmpp0 = scriptp0->getManagedInstance();
		else
			tmpp0 = nullptr;
		MonoString* tmpp1;
		tmpp1 = MonoUtil::stringToMono(p1);
		MonoUtil::invokeThunk(_scriptOnEventTriggeredThunk, getManagedInstance(), tmpp0, tmpp1);
	}
	void ScriptCAnimation::InternalSetDefaultClip(ScriptCAnimation* thisPtr, MonoObject* clip)
	{
		ResourceHandle<AnimationClip> tmpclip;
		ScriptRRefBase* scriptclip;
		scriptclip = ScriptRRefBase::toNative(clip);
		if(scriptclip != nullptr)
			tmpclip = static_resource_cast<AnimationClip>(scriptclip->getHandle());
		thisPtr->getHandle()->setDefaultClip(tmpclip);
	}

	MonoObject* ScriptCAnimation::InternalGetDefaultClip(ScriptCAnimation* thisPtr)
	{
		ResourceHandle<AnimationClip> tmp__output;
		tmp__output = thisPtr->getHandle()->getDefaultClip();

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().getScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->getManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptCAnimation::InternalSetWrapMode(ScriptCAnimation* thisPtr, AnimWrapMode wrapMode)
	{
		thisPtr->getHandle()->setWrapMode(wrapMode);
	}

	AnimWrapMode ScriptCAnimation::InternalGetWrapMode(ScriptCAnimation* thisPtr)
	{
		AnimWrapMode tmp__output;
		tmp__output = thisPtr->getHandle()->getWrapMode();

		AnimWrapMode __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCAnimation::InternalSetSpeed(ScriptCAnimation* thisPtr, float speed)
	{
		thisPtr->getHandle()->setSpeed(speed);
	}

	float ScriptCAnimation::InternalGetSpeed(ScriptCAnimation* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->getHandle()->getSpeed();

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
			tmpclip = static_resource_cast<AnimationClip>(scriptclip->getHandle());
		thisPtr->getHandle()->play(tmpclip);
	}

	void ScriptCAnimation::InternalBlendAdditive(ScriptCAnimation* thisPtr, MonoObject* clip, float weight, float fadeLength, uint32_t layer)
	{
		ResourceHandle<AnimationClip> tmpclip;
		ScriptRRefBase* scriptclip;
		scriptclip = ScriptRRefBase::toNative(clip);
		if(scriptclip != nullptr)
			tmpclip = static_resource_cast<AnimationClip>(scriptclip->getHandle());
		thisPtr->getHandle()->blendAdditive(tmpclip, weight, fadeLength, layer);
	}

	void ScriptCAnimation::InternalBlend1D(ScriptCAnimation* thisPtr, __Blend1DInfoInterop* info, float t)
	{
		Blend1DInfo tmpinfo;
		tmpinfo = ScriptBlend1DInfo::fromInterop(*info);
		thisPtr->getHandle()->blend1D(tmpinfo, t);
	}

	void ScriptCAnimation::InternalBlend2D(ScriptCAnimation* thisPtr, __Blend2DInfoInterop* info, Vector2* t)
	{
		Blend2DInfo tmpinfo;
		tmpinfo = ScriptBlend2DInfo::fromInterop(*info);
		thisPtr->getHandle()->blend2D(tmpinfo, *t);
	}

	void ScriptCAnimation::InternalCrossFade(ScriptCAnimation* thisPtr, MonoObject* clip, float fadeLength)
	{
		ResourceHandle<AnimationClip> tmpclip;
		ScriptRRefBase* scriptclip;
		scriptclip = ScriptRRefBase::toNative(clip);
		if(scriptclip != nullptr)
			tmpclip = static_resource_cast<AnimationClip>(scriptclip->getHandle());
		thisPtr->getHandle()->crossFade(tmpclip, fadeLength);
	}

	void ScriptCAnimation::InternalSample(ScriptCAnimation* thisPtr, MonoObject* clip, float time)
	{
		ResourceHandle<AnimationClip> tmpclip;
		ScriptRRefBase* scriptclip;
		scriptclip = ScriptRRefBase::toNative(clip);
		if(scriptclip != nullptr)
			tmpclip = static_resource_cast<AnimationClip>(scriptclip->getHandle());
		thisPtr->getHandle()->sample(tmpclip, time);
	}

	void ScriptCAnimation::InternalStop(ScriptCAnimation* thisPtr, uint32_t layer)
	{
		thisPtr->getHandle()->stop(layer);
	}

	void ScriptCAnimation::InternalStopAll(ScriptCAnimation* thisPtr)
	{
		thisPtr->getHandle()->stopAll();
	}

	bool ScriptCAnimation::InternalIsPlaying(ScriptCAnimation* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->getHandle()->isPlaying();

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
			tmpclip = static_resource_cast<AnimationClip>(scriptclip->getHandle());
		tmp__output = thisPtr->getHandle()->getState(tmpclip, *state);

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
			tmpclip = static_resource_cast<AnimationClip>(scriptclip->getHandle());
		thisPtr->getHandle()->setState(tmpclip, *state);
	}

	void ScriptCAnimation::InternalSetMorphChannelWeight(ScriptCAnimation* thisPtr, MonoString* name, float weight)
	{
		String tmpname;
		tmpname = MonoUtil::monoToString(name);
		thisPtr->getHandle()->setMorphChannelWeight(tmpname, weight);
	}

	void ScriptCAnimation::InternalSetBounds(ScriptCAnimation* thisPtr, AABox* bounds)
	{
		thisPtr->getHandle()->setBounds(*bounds);
	}

	void ScriptCAnimation::InternalGetBounds(ScriptCAnimation* thisPtr, AABox* __output)
	{
		AABox tmp__output;
		tmp__output = thisPtr->getHandle()->getBounds();

		*__output = tmp__output;
	}

	void ScriptCAnimation::InternalSetUseBounds(ScriptCAnimation* thisPtr, bool enable)
	{
		thisPtr->getHandle()->setUseBounds(enable);
	}

	bool ScriptCAnimation::InternalGetUseBounds(ScriptCAnimation* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->getHandle()->getUseBounds();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptCAnimation::InternalSetEnableCull(ScriptCAnimation* thisPtr, bool enable)
	{
		thisPtr->getHandle()->setEnableCull(enable);
	}

	bool ScriptCAnimation::InternalGetEnableCull(ScriptCAnimation* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->getHandle()->getEnableCull();

		bool __output;
		__output = tmp__output;

		return __output;
	}

	uint32_t ScriptCAnimation::InternalGetNumClips(ScriptCAnimation* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->getHandle()->getNumClips();

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	MonoObject* ScriptCAnimation::InternalGetClip(ScriptCAnimation* thisPtr, uint32_t idx)
	{
		ResourceHandle<AnimationClip> tmp__output;
		tmp__output = thisPtr->getHandle()->getClip(idx);

		MonoObject* __output;
		ScriptRRefBase* script__output;
		script__output = ScriptResourceManager::Instance().getScriptRRef(tmp__output);
		if(script__output != nullptr)
			__output = script__output->getManagedInstance();
		else
			__output = nullptr;

		return __output;
	}

	void ScriptCAnimation::InternalRefreshClipMappings(ScriptCAnimation* thisPtr)
	{
		thisPtr->getHandle()->RefreshClipMappingsInternal();
	}

	bool ScriptCAnimation::InternalGetGenericCurveValue(ScriptCAnimation* thisPtr, uint32_t curveIdx, float* value)
	{
		bool tmp__output;
		tmp__output = thisPtr->getHandle()->GetGenericCurveValueInternal(curveIdx, *value);

		bool __output;
		__output = tmp__output;

		return __output;
	}

	bool ScriptCAnimation::InternalTogglePreviewMode(ScriptCAnimation* thisPtr, bool enabled)
	{
		bool tmp__output;
		tmp__output = thisPtr->getHandle()->TogglePreviewModeInternal(enabled);

		bool __output;
		__output = tmp__output;

		return __output;
	}
}
