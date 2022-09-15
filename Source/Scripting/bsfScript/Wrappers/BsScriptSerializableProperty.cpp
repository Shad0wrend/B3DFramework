//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Wrappers/BsScriptSerializableProperty.h"
#include "BsScriptMeta.h"
#include "BsMonoField.h"
#include "BsMonoClass.h"
#include "BsMonoManager.h"
#include "BsMonoUtil.h"
#include "Serialization/BsManagedSerializableObjectInfo.h"
#include "Wrappers/BsScriptSerializableObject.h"
#include "Wrappers/BsScriptSerializableArray.h"
#include "Wrappers/BsScriptSerializableList.h"
#include "Wrappers/BsScriptSerializableDictionary.h"
#include "Serialization/BsScriptAssemblyManager.h"
#include "Serialization/BsManagedSerializableObject.h"
#include "Serialization/BsManagedSerializableArray.h"
#include "Serialization/BsManagedSerializableList.h"
#include "Serialization/BsManagedSerializableDictionary.h"
#include "Serialization/BsManagedSerializableField.h"

namespace bs
{
	ScriptSerializableProperty::ScriptSerializableProperty(MonoObject* instance, const SPtr<ManagedSerializableTypeInfo>& typeInfo)
		:ScriptObject(instance), mTypeInfo(typeInfo)
	{

	}

	void ScriptSerializableProperty::initRuntimeData()
	{
		metaData.scriptClass->AddInternalCall("Internal_CreateInstance", (void*)&ScriptSerializableProperty::InternalCreateInstance);
		metaData.scriptClass->AddInternalCall("Internal_CreateObject", (void*)&ScriptSerializableProperty::InternalCreateObject);
		metaData.scriptClass->AddInternalCall("Internal_CreateArray", (void*)&ScriptSerializableProperty::InternalCreateArray);
		metaData.scriptClass->AddInternalCall("Internal_CreateList", (void*)&ScriptSerializableProperty::InternalCreateList);
		metaData.scriptClass->AddInternalCall("Internal_CreateDictionary", (void*)&ScriptSerializableProperty::InternalCreateDictionary);
		metaData.scriptClass->AddInternalCall("Internal_CreateManagedObjectInstance", (void*)&ScriptSerializableProperty::InternalCreateManagedObjectInstance);
		metaData.scriptClass->AddInternalCall("Internal_CreateManagedArrayInstance", (void*)&ScriptSerializableProperty::InternalCreateManagedArrayInstance);
		metaData.scriptClass->AddInternalCall("Internal_CreateManagedListInstance", (void*)&ScriptSerializableProperty::InternalCreateManagedListInstance);
		metaData.scriptClass->AddInternalCall("Internal_CreateManagedDictionaryInstance", (void*)&ScriptSerializableProperty::InternalCreateManagedDictionaryInstance);
	}

	MonoObject* ScriptSerializableProperty::Create(const SPtr<ManagedSerializableTypeInfo>& typeInfo)
	{
		MonoObject* managedInstance = metaData.scriptClass->createInstance();
		new (bs_alloc<ScriptSerializableProperty>()) ScriptSerializableProperty(managedInstance, typeInfo);

		return managedInstance;
	}

	void ScriptSerializableProperty::InternalCreateInstance(MonoObject* instance, MonoReflectionType* reflType)
	{
		if (reflType == nullptr)
			return;

		::MonoClass* monoClass = MonoUtil::getClass(reflType);
		MonoClass* engineClass = MonoManager::Instance().findClass(monoClass);

		SPtr<ManagedSerializableTypeInfo> typeInfo = ScriptAssemblyManager::Instance().getTypeInfo(engineClass);
		if (typeInfo == nullptr)
		{
			BS_LOG(Warning, Script, "Cannot create an instance of type \"{0}\", it is not marked as serializable.",
				engineClass->GetFullName());
			return;
		}

		new (bs_alloc<ScriptSerializableProperty>()) ScriptSerializableProperty(instance, typeInfo);
	}

	MonoObject* ScriptSerializableProperty::InternalCreateObject(ScriptSerializableProperty* nativeInstance,
		MonoObject* managedInstance, MonoReflectionType* reflType)
	{
		return ScriptSerializableObject::Create(nativeInstance, managedInstance, reflType);
	}

	MonoObject* ScriptSerializableProperty::InternalCreateArray(ScriptSerializableProperty* nativeInstance, MonoObject* managedInstance)
	{
		return ScriptSerializableArray::Create(nativeInstance, managedInstance);
	}

	MonoObject* ScriptSerializableProperty::InternalCreateList(ScriptSerializableProperty* nativeInstance, MonoObject* managedInstance)
	{
		return ScriptSerializableList::Create(nativeInstance, managedInstance);
	}

	MonoObject* ScriptSerializableProperty::InternalCreateDictionary(ScriptSerializableProperty* nativeInstance, MonoObject* managedInstance)
	{
		return ScriptSerializableDictionary::Create(nativeInstance, managedInstance);
	}

	MonoObject* ScriptSerializableProperty::InternalCreateManagedObjectInstance(ScriptSerializableProperty* nativeInstance)
	{
		SPtr<ManagedSerializableTypeInfoObject> objectTypeInfo = std::static_pointer_cast<ManagedSerializableTypeInfoObject>(nativeInstance->mTypeInfo);
		return ManagedSerializableObject::createManagedInstance(objectTypeInfo);
	}

	MonoObject* ScriptSerializableProperty::InternalCreateManagedArrayInstance(ScriptSerializableProperty* nativeInstance, MonoArray* sizes)
	{
		ScriptArray scriptArray(sizes);

		Vector<UINT32> nativeSizes;
		UINT32 arrayLen = scriptArray.size();
		for (UINT32 i = 0; i < arrayLen; i++)
			nativeSizes.push_back(scriptArray.get<UINT32>(i));

		SPtr<ManagedSerializableTypeInfoArray> arrayTypeInfo = std::static_pointer_cast<ManagedSerializableTypeInfoArray>(nativeInstance->mTypeInfo);
		return ManagedSerializableArray::createManagedInstance(arrayTypeInfo, nativeSizes);
	}

	MonoObject* ScriptSerializableProperty::InternalCreateManagedListInstance(ScriptSerializableProperty* nativeInstance, int size)
	{
		SPtr<ManagedSerializableTypeInfoList> listTypeInfo = std::static_pointer_cast<ManagedSerializableTypeInfoList>(nativeInstance->mTypeInfo);
		return ManagedSerializableList::createManagedInstance(listTypeInfo, size);
	}

	MonoObject* ScriptSerializableProperty::InternalCreateManagedDictionaryInstance(ScriptSerializableProperty* nativeInstance)
	{
		SPtr<ManagedSerializableTypeInfoDictionary> dictTypeInfo = std::static_pointer_cast<ManagedSerializableTypeInfoDictionary>(nativeInstance->mTypeInfo);
		return ManagedSerializableDictionary::createManagedInstance(dictTypeInfo);
	}
}
