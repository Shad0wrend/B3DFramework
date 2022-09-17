//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptMeshImportOptions.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "BsScriptAnimationSplitInfo.generated.h"
#include "BsScriptImportedAnimationEvents.generated.h"
#include "BsScriptMeshImportOptions.generated.h"

namespace bs
{
#if !BS_IS_BANSHEE3D
	ScriptMeshImportOptions::ScriptMeshImportOptions(MonoObject* managedInstance, const SPtr<MeshImportOptions>& value)
		:TScriptReflectable(managedInstance, value)
	{
		mInternal = value;
	}

	void ScriptMeshImportOptions::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_getcpuCached", (void*)&ScriptMeshImportOptions::InternalGetcpuCached);
		metaData.scriptClass->AddInternalCall("Internal_setcpuCached", (void*)&ScriptMeshImportOptions::InternalSetcpuCached);
		metaData.scriptClass->AddInternalCall("Internal_getimportNormals", (void*)&ScriptMeshImportOptions::InternalGetimportNormals);
		metaData.scriptClass->AddInternalCall("Internal_setimportNormals", (void*)&ScriptMeshImportOptions::InternalSetimportNormals);
		metaData.scriptClass->AddInternalCall("Internal_getimportTangents", (void*)&ScriptMeshImportOptions::InternalGetimportTangents);
		metaData.scriptClass->AddInternalCall("Internal_setimportTangents", (void*)&ScriptMeshImportOptions::InternalSetimportTangents);
		metaData.scriptClass->AddInternalCall("Internal_getimportBlendShapes", (void*)&ScriptMeshImportOptions::InternalGetimportBlendShapes);
		metaData.scriptClass->AddInternalCall("Internal_setimportBlendShapes", (void*)&ScriptMeshImportOptions::InternalSetimportBlendShapes);
		metaData.scriptClass->AddInternalCall("Internal_getimportSkin", (void*)&ScriptMeshImportOptions::InternalGetimportSkin);
		metaData.scriptClass->AddInternalCall("Internal_setimportSkin", (void*)&ScriptMeshImportOptions::InternalSetimportSkin);
		metaData.scriptClass->AddInternalCall("Internal_getimportAnimation", (void*)&ScriptMeshImportOptions::InternalGetimportAnimation);
		metaData.scriptClass->AddInternalCall("Internal_setimportAnimation", (void*)&ScriptMeshImportOptions::InternalSetimportAnimation);
		metaData.scriptClass->AddInternalCall("Internal_getreduceKeyFrames", (void*)&ScriptMeshImportOptions::InternalGetreduceKeyFrames);
		metaData.scriptClass->AddInternalCall("Internal_setreduceKeyFrames", (void*)&ScriptMeshImportOptions::InternalSetreduceKeyFrames);
		metaData.scriptClass->AddInternalCall("Internal_getimportRootMotion", (void*)&ScriptMeshImportOptions::InternalGetimportRootMotion);
		metaData.scriptClass->AddInternalCall("Internal_setimportRootMotion", (void*)&ScriptMeshImportOptions::InternalSetimportRootMotion);
		metaData.scriptClass->AddInternalCall("Internal_getimportScale", (void*)&ScriptMeshImportOptions::InternalGetimportScale);
		metaData.scriptClass->AddInternalCall("Internal_setimportScale", (void*)&ScriptMeshImportOptions::InternalSetimportScale);
		metaData.scriptClass->AddInternalCall("Internal_getcollisionMeshType", (void*)&ScriptMeshImportOptions::InternalGetcollisionMeshType);
		metaData.scriptClass->AddInternalCall("Internal_setcollisionMeshType", (void*)&ScriptMeshImportOptions::InternalSetcollisionMeshType);
		metaData.scriptClass->AddInternalCall("Internal_getanimationSplits", (void*)&ScriptMeshImportOptions::InternalGetanimationSplits);
		metaData.scriptClass->AddInternalCall("Internal_setanimationSplits", (void*)&ScriptMeshImportOptions::InternalSetanimationSplits);
		metaData.scriptClass->AddInternalCall("Internal_getanimationEvents", (void*)&ScriptMeshImportOptions::InternalGetanimationEvents);
		metaData.scriptClass->AddInternalCall("Internal_setanimationEvents", (void*)&ScriptMeshImportOptions::InternalSetanimationEvents);
		metaData.scriptClass->AddInternalCall("Internal_create", (void*)&ScriptMeshImportOptions::InternalCreate);

	}

	MonoObject* ScriptMeshImportOptions::Create(const SPtr<MeshImportOptions>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptMeshImportOptions>()) ScriptMeshImportOptions(managedInstance, value);
		return managedInstance;
	}
	void ScriptMeshImportOptions::InternalCreate(MonoObject* managedInstance)
	{
		SPtr<MeshImportOptions> instance = MeshImportOptions::Create();
		new (bs_alloc<ScriptMeshImportOptions>())ScriptMeshImportOptions(managedInstance, instance);
	}
	bool ScriptMeshImportOptions::InternalGetcpuCached(ScriptMeshImportOptions* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->cpuCached;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptMeshImportOptions::InternalSetcpuCached(ScriptMeshImportOptions* thisPtr, bool value)
	{
		thisPtr->GetInternal()->cpuCached = value;
	}

	bool ScriptMeshImportOptions::InternalGetimportNormals(ScriptMeshImportOptions* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->importNormals;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptMeshImportOptions::InternalSetimportNormals(ScriptMeshImportOptions* thisPtr, bool value)
	{
		thisPtr->GetInternal()->importNormals = value;
	}

	bool ScriptMeshImportOptions::InternalGetimportTangents(ScriptMeshImportOptions* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->importTangents;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptMeshImportOptions::InternalSetimportTangents(ScriptMeshImportOptions* thisPtr, bool value)
	{
		thisPtr->GetInternal()->importTangents = value;
	}

	bool ScriptMeshImportOptions::InternalGetimportBlendShapes(ScriptMeshImportOptions* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->importBlendShapes;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptMeshImportOptions::InternalSetimportBlendShapes(ScriptMeshImportOptions* thisPtr, bool value)
	{
		thisPtr->GetInternal()->importBlendShapes = value;
	}

	bool ScriptMeshImportOptions::InternalGetimportSkin(ScriptMeshImportOptions* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->importSkin;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptMeshImportOptions::InternalSetimportSkin(ScriptMeshImportOptions* thisPtr, bool value)
	{
		thisPtr->GetInternal()->importSkin = value;
	}

	bool ScriptMeshImportOptions::InternalGetimportAnimation(ScriptMeshImportOptions* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->importAnimation;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptMeshImportOptions::InternalSetimportAnimation(ScriptMeshImportOptions* thisPtr, bool value)
	{
		thisPtr->GetInternal()->importAnimation = value;
	}

	bool ScriptMeshImportOptions::InternalGetreduceKeyFrames(ScriptMeshImportOptions* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->reduceKeyFrames;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptMeshImportOptions::InternalSetreduceKeyFrames(ScriptMeshImportOptions* thisPtr, bool value)
	{
		thisPtr->GetInternal()->reduceKeyFrames = value;
	}

	bool ScriptMeshImportOptions::InternalGetimportRootMotion(ScriptMeshImportOptions* thisPtr)
	{
		bool tmp__output;
		tmp__output = thisPtr->GetInternal()->importRootMotion;

		bool __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptMeshImportOptions::InternalSetimportRootMotion(ScriptMeshImportOptions* thisPtr, bool value)
	{
		thisPtr->GetInternal()->importRootMotion = value;
	}

	float ScriptMeshImportOptions::InternalGetimportScale(ScriptMeshImportOptions* thisPtr)
	{
		float tmp__output;
		tmp__output = thisPtr->GetInternal()->importScale;

		float __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptMeshImportOptions::InternalSetimportScale(ScriptMeshImportOptions* thisPtr, float value)
	{
		thisPtr->GetInternal()->importScale = value;
	}

	CollisionMeshType ScriptMeshImportOptions::InternalGetcollisionMeshType(ScriptMeshImportOptions* thisPtr)
	{
		CollisionMeshType tmp__output;
		tmp__output = thisPtr->GetInternal()->collisionMeshType;

		CollisionMeshType __output;
		__output = tmp__output;

		return __output;
	}

	void ScriptMeshImportOptions::InternalSetcollisionMeshType(ScriptMeshImportOptions* thisPtr, CollisionMeshType value)
	{
		thisPtr->GetInternal()->collisionMeshType = value;
	}

	MonoArray* ScriptMeshImportOptions::InternalGetanimationSplits(ScriptMeshImportOptions* thisPtr)
	{
		Vector<AnimationSplitInfo> vec__output;
		vec__output = thisPtr->GetInternal()->animationSplits;

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptAnimationSplitInfo>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			SPtr<AnimationSplitInfo> arrayElemPtr__output = bs_shared_ptr_new<AnimationSplitInfo>();
			*arrayElemPtr__output = vec__output[i];
			MonoObject* arrayElem__output;
			arrayElem__output = ScriptAnimationSplitInfo::Create(arrayElemPtr__output);
			array__output.Set(i, arrayElem__output);
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptMeshImportOptions::InternalSetanimationSplits(ScriptMeshImportOptions* thisPtr, MonoArray* value)
	{
		Vector<AnimationSplitInfo> vecvalue;
		if(value != nullptr)
		{
			ScriptArray arrayvalue(value);
			vecvalue.resize(arrayvalue.size());
			for(int i = 0; i < (int)arrayvalue.size(); i++)
			{
				ScriptAnimationSplitInfo* scriptvalue;
				scriptvalue = ScriptAnimationSplitInfo::toNative(arrayvalue.get<MonoObject*>(i));
				if(scriptvalue != nullptr)
				{
					SPtr<AnimationSplitInfo> arrayElemPtrvalue = scriptvalue->GetInternal();
					if(arrayElemPtrvalue)
						vecvalue[i] = *arrayElemPtrvalue;
				}
			}

		}
		thisPtr->GetInternal()->animationSplits = vecvalue;
	}

	MonoArray* ScriptMeshImportOptions::InternalGetanimationEvents(ScriptMeshImportOptions* thisPtr)
	{
		Vector<ImportedAnimationEvents> vec__output;
		vec__output = thisPtr->GetInternal()->animationEvents;

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::create<ScriptImportedAnimationEvents>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			SPtr<ImportedAnimationEvents> arrayElemPtr__output = bs_shared_ptr_new<ImportedAnimationEvents>();
			*arrayElemPtr__output = vec__output[i];
			MonoObject* arrayElem__output;
			arrayElem__output = ScriptImportedAnimationEvents::Create(arrayElemPtr__output);
			array__output.Set(i, arrayElem__output);
		}
		__output = array__output.getInternal();

		return __output;
	}

	void ScriptMeshImportOptions::InternalSetanimationEvents(ScriptMeshImportOptions* thisPtr, MonoArray* value)
	{
		Vector<ImportedAnimationEvents> vecvalue;
		if(value != nullptr)
		{
			ScriptArray arrayvalue(value);
			vecvalue.resize(arrayvalue.size());
			for(int i = 0; i < (int)arrayvalue.size(); i++)
			{
				ScriptImportedAnimationEvents* scriptvalue;
				scriptvalue = ScriptImportedAnimationEvents::toNative(arrayvalue.get<MonoObject*>(i));
				if(scriptvalue != nullptr)
				{
					SPtr<ImportedAnimationEvents> arrayElemPtrvalue = scriptvalue->GetInternal();
					if(arrayElemPtrvalue)
						vecvalue[i] = *arrayElemPtrvalue;
				}
			}

		}
		thisPtr->GetInternal()->animationEvents = vecvalue;
	}
#endif
}
