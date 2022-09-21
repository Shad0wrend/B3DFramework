//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptAnimationSplitInfo.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
#if !BS_IS_BANSHEE3D
	ScriptAnimationSplitInfo::ScriptAnimationSplitInfo(MonoObject* managedInstance, const SPtr<AnimationSplitInfo>& value)
		:TScriptReflectable(managedInstance, value)
	{
	}

	void ScriptAnimationSplitInfo::InitRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_AnimationSplitInfo", (void*)&ScriptAnimationSplitInfo::InternalAnimationSplitInfo);
		metaData.scriptClass->AddInternalCall("Internal_AnimationSplitInfo0", (void*)&ScriptAnimationSplitInfo::InternalAnimationSplitInfo0);
		metaData.scriptClass->AddInternalCall("Internal_Getname", (void*)&ScriptAnimationSplitInfo::InternalGetname);
		metaData.scriptClass->AddInternalCall("Internal_Setname", (void*)&ScriptAnimationSplitInfo::InternalSetname);
		metaData.scriptClass->AddInternalCall("Internal_GetstartFrame", (void*)&ScriptAnimationSplitInfo::InternalGetstartFrame);
		metaData.scriptClass->AddInternalCall("Internal_SetstartFrame", (void*)&ScriptAnimationSplitInfo::InternalSetstartFrame);
		metaData.scriptClass->AddInternalCall("Internal_GetendFrame", (void*)&ScriptAnimationSplitInfo::InternalGetendFrame);
		metaData.scriptClass->AddInternalCall("Internal_SetendFrame", (void*)&ScriptAnimationSplitInfo::InternalSetendFrame);
		metaData.scriptClass->AddInternalCall("Internal_GetisAdditive", (void*)&ScriptAnimationSplitInfo::InternalGetisAdditive);
		metaData.scriptClass->AddInternalCall("Internal_SetisAdditive", (void*)&ScriptAnimationSplitInfo::InternalSetisAdditive);

	}

	MonoObject* ScriptAnimationSplitInfo::Create(const SPtr<AnimationSplitInfo>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptAnimationSplitInfo>()) ScriptAnimationSplitInfo(managedInstance, value);
		return managedInstance;
	}
	void ScriptAnimationSplitInfo::InternalAnimationSplitInfo(MonoObject* managedInstance)
	{
		SPtr<AnimationSplitInfo> instance = bs_shared_ptr_new<AnimationSplitInfo>();
		new (bs_alloc<ScriptAnimationSplitInfo>())ScriptAnimationSplitInfo(managedInstance, instance);
	}

	void ScriptAnimationSplitInfo::InternalAnimationSplitInfo0(MonoObject* managedInstance, MonoString* name, uint32_t startFrame, uint32_t endFrame, bool isAdditive)
	{
		String tmpname;
		tmpname = MonoUtil::MonoToString(name);
		SPtr<AnimationSplitInfo> instance = bs_shared_ptr_new<AnimationSplitInfo>(tmpname, startFrame, endFrame, isAdditive);
		new (bs_alloc<ScriptAnimationSplitInfo>())ScriptAnimationSplitInfo(managedInstance, instance);
	}

	MonoString* ScriptAnimationSplitInfo::InternalGetname(ScriptAnimationSplitInfo* thisPtr)
	{
		String tmp__output;
		tmp__output = thisPtr->GetInternal()->name;

		MonoString* __output;
		__output = MonoUtil::StringToMono(tmp__output);

		return __output;
	}

	void ScriptAnimationSplitInfo::InternalSetname(ScriptAnimationSplitInfo* thisPtr, MonoString* value)
	{
		String tmpvalue;
		tmpvalue = MonoUtil::MonoToString(value);
		thisPtr->GetInternal()->name = tmpvalue;
	}

	uint32_t ScriptAnimationSplitInfo::InternalGetstartFrame(ScriptAnimationSplitInfo* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->startFrame;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAnimationSplitInfo::InternalSetstartFrame(ScriptAnimationSplitInfo* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->startFrame = value;
	}

	uint32_t ScriptAnimationSplitInfo::InternalGetendFrame(ScriptAnimationSplitInfo* thisPtr)
	{
		uint32_t tmp__output;
		tmp__output = thisPtr->GetInternal()->endFrame;

		uint32_t __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAnimationSplitInfo::InternalSetendFrame(ScriptAnimationSplitInfo* thisPtr, uint32_t value)
	{
		thisPtr->GetInternal()->endFrame = value;
	}

	bool ScriptAnimationSplitInfo::InternalGetisAdditive(ScriptAnimationSplitInfo* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->isAdditive;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptAnimationSplitInfo::InternalSetisAdditive(ScriptAnimationSplitInfo* thisPtr, bool value)
	{
		thisPtr->GetInternal()->isAdditive = value;
	}
#endif
}
