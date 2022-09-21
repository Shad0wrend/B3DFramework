//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptMultiResource.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../Foundation/bsfCore/Importer/BsImporter.h"
#include "BsScriptSubResource.generated.h"

namespace bs
{
#if !BS_IS_BANSHEE3D
	ScriptMultiResource::ScriptMultiResource(MonoObject* managedInstance, const SPtr<MultiResource>& value)
		:ScriptObject(managedInstance), mInternal(value)
	{
	}

	void ScriptMultiResource::InitRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_MultiResource", (void*)&ScriptMultiResource::InternalMultiResource);
		metaData.scriptClass->AddInternalCall("Internal_MultiResource0", (void*)&ScriptMultiResource::InternalMultiResource0);
		metaData.scriptClass->AddInternalCall("Internal_Getentries", (void*)&ScriptMultiResource::InternalGetentries);
		metaData.scriptClass->AddInternalCall("Internal_Setentries", (void*)&ScriptMultiResource::InternalSetentries);

	}

	MonoObject* ScriptMultiResource::Create(const SPtr<MultiResource>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->CreateInstance("bool", ctorParams);
		new (bs_alloc<ScriptMultiResource>()) ScriptMultiResource(managedInstance, value);
		return managedInstance;
	}
	void ScriptMultiResource::InternalMultiResource(MonoObject* managedInstance)
	{
		SPtr<MultiResource> instance = bs_shared_ptr_new<MultiResource>();
		new (bs_alloc<ScriptMultiResource>())ScriptMultiResource(managedInstance, instance);
	}

	void ScriptMultiResource::InternalMultiResource0(MonoObject* managedInstance, MonoArray* entries)
	{
		Vector<SubResource> vecentries;
		if(entries != nullptr)
		{
			ScriptArray arrayentries(entries);
			vecentries.resize(arrayentries.Size());
			for(int i = 0; i < (int)arrayentries.Size(); i++)
			{
				vecentries[i] = ScriptSubResource::FromInterop(arrayentries.Get<__SubResourceInterop>(i));
			}
		}
		SPtr<MultiResource> instance = bs_shared_ptr_new<MultiResource>(vecentries);
		new (bs_alloc<ScriptMultiResource>())ScriptMultiResource(managedInstance, instance);
	}

	MonoArray* ScriptMultiResource::InternalGetentries(ScriptMultiResource* thisPtr)
	{
		Vector<SubResource> vec__output;
		vec__output = thisPtr->GetInternal()->entries;

		MonoArray* __output;
		int arraySize__output = (int)vec__output.size();
		ScriptArray array__output = ScriptArray::Create<ScriptSubResource>(arraySize__output);
		for(int i = 0; i < arraySize__output; i++)
		{
			array__output.Set(i, ScriptSubResource::ToInterop(vec__output[i]));
		}
		__output = array__output.GetInternal();

		return __output;
	}

	void ScriptMultiResource::InternalSetentries(ScriptMultiResource* thisPtr, MonoArray* value)
	{
		Vector<SubResource> vecvalue;
		if(value != nullptr)
		{
			ScriptArray arrayvalue(value);
			vecvalue.resize(arrayvalue.Size());
			for(int i = 0; i < (int)arrayvalue.Size(); i++)
			{
				vecvalue[i] = ScriptSubResource::FromInterop(arrayvalue.Get<__SubResourceInterop>(i));
			}

		}
		thisPtr->GetInternal()->entries = vecvalue;
	}
#endif
}
