//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/BsScriptSerializableObject.h"
#include "Wrappers/BsScriptSerializableField.h"
#include "Wrappers/BsScriptSerializableProperty.h"
#include "Serialization/BsScriptAssemblyManager.h"
#include "BsScriptMeta.h"
#include "BsMonoField.h"
#include "BsMonoClass.h"
#include "BsMonoManager.h"
#include "BsMonoUtil.h"

namespace bs
{
	MonoField* ScriptSerializableObject::FieldsField = nullptr;

	ScriptSerializableObject::ScriptSerializableObject(MonoObject* instance, const SPtr<ManagedSerializableObjectInfo>& objInfo)
		:ScriptObject(instance), mObjInfo(objInfo)
	{

	}

	void ScriptSerializableObject::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_CreateInstance", (void*)&ScriptSerializableObject::InternalCreateInstance);
		metaData.scriptClass->AddInternalCall("Internal_GetBaseClass", (void*)&ScriptSerializableObject::InternalGetBaseClass);

		FieldsField = metaData.scriptClass->GetField("_fields");
	}

	MonoObject* ScriptSerializableObject::Create(const ScriptSerializableProperty* native, MonoObject* managed,
		MonoReflectionType* reflType)
	{
		void* params[2] = { reflType, managed };
		MonoObject* managedInstance = metaData.scriptClass->CreateInstance("Type,SerializableProperty", params);

		return managedInstance;
	}

	MonoObject* ScriptSerializableObject::Create(MonoObject* managed, MonoReflectionType* reflType)
	{
		void* params[2] = { reflType, managed };
		MonoObject* managedInstance = metaData.scriptClass->CreateInstance("Type,object", params);

		return managedInstance;
	}

	void ScriptSerializableObject::InternalCreateInstance(MonoObject* instance, MonoReflectionType* type)
	{
		::MonoClass* monoClass = MonoUtil::getClass(type);

		String elementNs;
		String elementTypeName;
		MonoUtil::getClassName(monoClass, elementNs, elementTypeName);

		SPtr<ManagedSerializableObjectInfo> objInfo;
		ScriptAssemblyManager::Instance().GetSerializableObjectInfo(elementNs, elementTypeName, objInfo);

		CreateInternal(instance, objInfo);
	}

	MonoObject* ScriptSerializableObject::InternalGetBaseClass(ScriptSerializableObject* thisPtr, MonoObject* owningObject)
	{
		if(!thisPtr->mObjInfo->mBaseClass)
			return nullptr;

		MonoReflectionType* reflType = MonoUtil::getType(thisPtr->mObjInfo->mBaseClass->mMonoClass->GetInternalClassInternal());
		return Create(owningObject, reflType);
	}

	ScriptSerializableObject* ScriptSerializableObject::CreateInternal(MonoObject* instance, const SPtr<ManagedSerializableObjectInfo>& objInfo)
	{
		ScriptSerializableObject* nativeInstance = new (bs_alloc<ScriptSerializableObject>()) ScriptSerializableObject(instance, objInfo);

		Vector<SPtr<ManagedSerializableMemberInfo>> sortedFields;
		
		if(objInfo != nullptr)
		{
			sortedFields.resize(objInfo->mFields.size());
			UINT32 i = 0;
			for (auto& fieldPair : objInfo->mFields)
			{
				sortedFields[i] = fieldPair.second;
				i++;
			}
		}

		std::sort(sortedFields.begin(), sortedFields.end(),
			[&](const SPtr<ManagedSerializableMemberInfo>& x, const SPtr<ManagedSerializableMemberInfo>& y)
		{
			return x->mFieldId < y->mFieldId;
		});

		::MonoClass* serializableFieldClass = ScriptSerializableField::getMetaData()->scriptClass->GetInternalClassInternal();
		ScriptArray scriptArray(serializableFieldClass, (UINT32)sortedFields.size());

		UINT32 i = 0;
		for (auto& field : sortedFields)
		{
			MonoObject* fieldManagedInstance = ScriptSerializableField::Create(instance, field);

			scriptArray.set(i, fieldManagedInstance);
			i++;
		}

		FieldsField->set(instance, scriptArray.getInternal());

		return nativeInstance;
	}
}
