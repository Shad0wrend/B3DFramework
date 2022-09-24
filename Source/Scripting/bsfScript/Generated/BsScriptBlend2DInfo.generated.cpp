//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
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
		return MonoUtil::Box(metaData.ScriptClass->GetInternalClassInternal(), (void*)&value);
	}

	__Blend2DInfoInterop ScriptBlend2DInfo::Unbox(MonoObject* value)
	{
		return *(__Blend2DInfoInterop*)MonoUtil::Unbox(value);
	}

	Blend2DInfo ScriptBlend2DInfo::FromInterop(const __Blend2DInfoInterop& value)
	{
		Blend2DInfo output;
		ResourceHandle<AnimationClip> tmptopLeftClip;
		ScriptRRefBase* scripttopLeftClip;
		scripttopLeftClip = ScriptRRefBase::ToNative(value.TopLeftClip);
		if(scripttopLeftClip != nullptr)
			tmptopLeftClip = static_resource_cast<AnimationClip>(scripttopLeftClip->GetHandle());
		output.TopLeftClip = tmptopLeftClip;
		ResourceHandle<AnimationClip> tmptopRightClip;
		ScriptRRefBase* scripttopRightClip;
		scripttopRightClip = ScriptRRefBase::ToNative(value.TopRightClip);
		if(scripttopRightClip != nullptr)
			tmptopRightClip = static_resource_cast<AnimationClip>(scripttopRightClip->GetHandle());
		output.TopRightClip = tmptopRightClip;
		ResourceHandle<AnimationClip> tmpbotLeftClip;
		ScriptRRefBase* scriptbotLeftClip;
		scriptbotLeftClip = ScriptRRefBase::ToNative(value.BotLeftClip);
		if(scriptbotLeftClip != nullptr)
			tmpbotLeftClip = static_resource_cast<AnimationClip>(scriptbotLeftClip->GetHandle());
		output.BotLeftClip = tmpbotLeftClip;
		ResourceHandle<AnimationClip> tmpbotRightClip;
		ScriptRRefBase* scriptbotRightClip;
		scriptbotRightClip = ScriptRRefBase::ToNative(value.BotRightClip);
		if(scriptbotRightClip != nullptr)
			tmpbotRightClip = static_resource_cast<AnimationClip>(scriptbotRightClip->GetHandle());
		output.BotRightClip = tmpbotRightClip;

		return output;
	}

	__Blend2DInfoInterop ScriptBlend2DInfo::ToInterop(const Blend2DInfo& value)
	{
		__Blend2DInfoInterop output;
		ScriptRRefBase* scripttopLeftClip;
		scripttopLeftClip = ScriptResourceManager::Instance().GetScriptRRef(value.TopLeftClip);
		MonoObject* tmptopLeftClip;
		if(scripttopLeftClip != nullptr)
			tmptopLeftClip = scripttopLeftClip->GetManagedInstance();
		else
			tmptopLeftClip = nullptr;
		output.TopLeftClip = tmptopLeftClip;
		ScriptRRefBase* scripttopRightClip;
		scripttopRightClip = ScriptResourceManager::Instance().GetScriptRRef(value.TopRightClip);
		MonoObject* tmptopRightClip;
		if(scripttopRightClip != nullptr)
			tmptopRightClip = scripttopRightClip->GetManagedInstance();
		else
			tmptopRightClip = nullptr;
		output.TopRightClip = tmptopRightClip;
		ScriptRRefBase* scriptbotLeftClip;
		scriptbotLeftClip = ScriptResourceManager::Instance().GetScriptRRef(value.BotLeftClip);
		MonoObject* tmpbotLeftClip;
		if(scriptbotLeftClip != nullptr)
			tmpbotLeftClip = scriptbotLeftClip->GetManagedInstance();
		else
			tmpbotLeftClip = nullptr;
		output.BotLeftClip = tmpbotLeftClip;
		ScriptRRefBase* scriptbotRightClip;
		scriptbotRightClip = ScriptResourceManager::Instance().GetScriptRRef(value.BotRightClip);
		MonoObject* tmpbotRightClip;
		if(scriptbotRightClip != nullptr)
			tmpbotRightClip = scriptbotRightClip->GetManagedInstance();
		else
			tmpbotRightClip = nullptr;
		output.BotRightClip = tmpbotRightClip;

		return output;
	}

}
