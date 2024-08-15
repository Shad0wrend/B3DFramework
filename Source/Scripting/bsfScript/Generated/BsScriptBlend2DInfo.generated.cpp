//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptBlend2DInfo.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "../../../Foundation/bsfCore/Animation/BsAnimationClip.h"
#include "BsScriptAnimationClip.generated.h"

namespace bs
{
	ScriptBlend2DInfo::ScriptBlend2DInfo(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptBlend2DInfo::InitRuntimeData()
	{ }

	MonoObject*ScriptBlend2DInfo::Box(const __Blend2DInfoInterop& value)
	{
		return MonoUtil::Box(metaData.ScriptClass->GetInternalClass(), (void*)&value);
	}

	__Blend2DInfoInterop ScriptBlend2DInfo::Unbox(MonoObject* value)
	{
		return *(__Blend2DInfoInterop*)MonoUtil::Unbox(value);
	}

	Blend2DInfo ScriptBlend2DInfo::FromInterop(const __Blend2DInfoInterop& value)
	{
		Blend2DInfo output;
		TResourceHandle<AnimationClip> tmpTopLeftClip;
		ScriptRRefBase* scriptWrapperObjectTopLeftClip;
		scriptWrapperObjectTopLeftClip = ScriptRRefBase::ToNative(value.TopLeftClip);
		if(scriptWrapperObjectTopLeftClip != nullptr)
			tmpTopLeftClip = B3DStaticResourceCast<AnimationClip>(scriptWrapperObjectTopLeftClip->GetHandle());
		output.TopLeftClip = tmpTopLeftClip;
		TResourceHandle<AnimationClip> tmpTopRightClip;
		ScriptRRefBase* scriptWrapperObjectTopRightClip;
		scriptWrapperObjectTopRightClip = ScriptRRefBase::ToNative(value.TopRightClip);
		if(scriptWrapperObjectTopRightClip != nullptr)
			tmpTopRightClip = B3DStaticResourceCast<AnimationClip>(scriptWrapperObjectTopRightClip->GetHandle());
		output.TopRightClip = tmpTopRightClip;
		TResourceHandle<AnimationClip> tmpBotLeftClip;
		ScriptRRefBase* scriptWrapperObjectBotLeftClip;
		scriptWrapperObjectBotLeftClip = ScriptRRefBase::ToNative(value.BotLeftClip);
		if(scriptWrapperObjectBotLeftClip != nullptr)
			tmpBotLeftClip = B3DStaticResourceCast<AnimationClip>(scriptWrapperObjectBotLeftClip->GetHandle());
		output.BotLeftClip = tmpBotLeftClip;
		TResourceHandle<AnimationClip> tmpBotRightClip;
		ScriptRRefBase* scriptWrapperObjectBotRightClip;
		scriptWrapperObjectBotRightClip = ScriptRRefBase::ToNative(value.BotRightClip);
		if(scriptWrapperObjectBotRightClip != nullptr)
			tmpBotRightClip = B3DStaticResourceCast<AnimationClip>(scriptWrapperObjectBotRightClip->GetHandle());
		output.BotRightClip = tmpBotRightClip;

		return output;
	}

	__Blend2DInfoInterop ScriptBlend2DInfo::ToInterop(const Blend2DInfo& value)
	{
		__Blend2DInfoInterop output;
		MonoObject* tmpTopLeftClip;
		ScriptRRefBase* scriptWrapperObjectTopLeftClip;
		scriptWrapperObjectTopLeftClip = ScriptResourceManager::Instance().GetScriptRRef(value.TopLeftClip);
		if(scriptWrapperObjectTopLeftClip != nullptr)
			tmpTopLeftClip = scriptWrapperObjectTopLeftClip->GetManagedInstance();
		else
			tmpTopLeftClip = nullptr;
		output.TopLeftClip = tmpTopLeftClip;
		MonoObject* tmpTopRightClip;
		ScriptRRefBase* scriptWrapperObjectTopRightClip;
		scriptWrapperObjectTopRightClip = ScriptResourceManager::Instance().GetScriptRRef(value.TopRightClip);
		if(scriptWrapperObjectTopRightClip != nullptr)
			tmpTopRightClip = scriptWrapperObjectTopRightClip->GetManagedInstance();
		else
			tmpTopRightClip = nullptr;
		output.TopRightClip = tmpTopRightClip;
		MonoObject* tmpBotLeftClip;
		ScriptRRefBase* scriptWrapperObjectBotLeftClip;
		scriptWrapperObjectBotLeftClip = ScriptResourceManager::Instance().GetScriptRRef(value.BotLeftClip);
		if(scriptWrapperObjectBotLeftClip != nullptr)
			tmpBotLeftClip = scriptWrapperObjectBotLeftClip->GetManagedInstance();
		else
			tmpBotLeftClip = nullptr;
		output.BotLeftClip = tmpBotLeftClip;
		MonoObject* tmpBotRightClip;
		ScriptRRefBase* scriptWrapperObjectBotRightClip;
		scriptWrapperObjectBotRightClip = ScriptResourceManager::Instance().GetScriptRRef(value.BotRightClip);
		if(scriptWrapperObjectBotRightClip != nullptr)
			tmpBotRightClip = scriptWrapperObjectBotRightClip->GetManagedInstance();
		else
			tmpBotRightClip = nullptr;
		output.BotRightClip = tmpBotRightClip;

		return output;
	}

}
