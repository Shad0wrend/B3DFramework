//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptBlendClipInfo.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptResourceManager.h"
#include "Wrappers/BsScriptRRefBase.h"
#include "../../../Foundation/bsfCore/Animation/BsAnimationClip.h"
#include "BsScriptAnimationClip.generated.h"

namespace bs
{
	ScriptBlendClipInfo::ScriptBlendClipInfo(MonoObject* managedInstance)
		:ScriptObject(managedInstance)
	{ }

	void ScriptBlendClipInfo::InitRuntimeData()
	{ }

	MonoObject*ScriptBlendClipInfo::Box(const __BlendClipInfoInterop& value)
	{
		return MonoUtil::Box(metaData.ScriptClass->GetInternalClassInternal(), (void*)&value);
	}

	__BlendClipInfoInterop ScriptBlendClipInfo::Unbox(MonoObject* value)
	{
		return *(__BlendClipInfoInterop*)MonoUtil::Unbox(value);
	}

	BlendClipInfo ScriptBlendClipInfo::FromInterop(const __BlendClipInfoInterop& value)
	{
		BlendClipInfo output;
		ResourceHandle<AnimationClip> tmpclip;
		ScriptRRefBase* scriptclip;
		scriptclip = ScriptRRefBase::ToNative(value.Clip);
		if(scriptclip != nullptr)
			tmpclip = static_resource_cast<AnimationClip>(scriptclip->GetHandle());
		output.Clip = tmpclip;
		output.Position = value.Position;

		return output;
	}

	__BlendClipInfoInterop ScriptBlendClipInfo::ToInterop(const BlendClipInfo& value)
	{
		__BlendClipInfoInterop output;
		ScriptRRefBase* scriptclip;
		scriptclip = ScriptResourceManager::Instance().GetScriptRRef(value.Clip);
		MonoObject* tmpclip;
		if(scriptclip != nullptr)
			tmpclip = scriptclip->GetManagedInstance();
		else
			tmpclip = nullptr;
		output.Clip = tmpclip;
		output.Position = value.Position;

		return output;
	}

}
