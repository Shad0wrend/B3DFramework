//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Serialization/BsManagedSerializableDictionary.h"
#include "RTTI/BsManagedSerializableDictionaryRTTI.h"
#include "BsMonoManager.h"
#include "Serialization/BsScriptAssemblyManager.h"
#include "Serialization/BsManagedSerializableField.h"
#include "BsMonoClass.h"
#include "BsMonoMethod.h"
#include "BsMonoProperty.h"
#include "BsMonoUtil.h"

namespace bs
{
	ManagedSerializableDictionaryKeyValue::ManagedSerializableDictionaryKeyValue(const SPtr<ManagedSerializableFieldData>& key,
		const SPtr<ManagedSerializableFieldData>& value)
		:key(key), value(value)
	{
		
	}

	RTTITypeBase* ManagedSerializableDictionaryKeyValue::GetRttiStatic()
	{
		return ManagedSerializableDictionaryKeyValueRTTI::Instance();
	}

	RTTITypeBase* ManagedSerializableDictionaryKeyValue::GetRtti() const
	{
		return ManagedSerializableDictionaryKeyValue::GetRttiStatic();
	}

	ManagedSerializableDictionary::Enumerator::Enumerator(const ManagedSerializableDictionary* parent)
		: mIteratorInitialized(false), mParent(parent)
	{
		MonoArray* keysArray = nullptr;
		MonoArray* valuesArray = nullptr;
		if(parent->mGCHandle != 0)
		{
			MonoObject* managedInstance = MonoUtil::getObjectFromGCHandle(parent->mGCHandle);

			mNumEntries = *(UINT32*)MonoUtil::unbox(parent->mCountProp->get(managedInstance));
			MonoObject* keyCollection = parent->mKeysProp->get(managedInstance);
			MonoObject* valueCollection = parent->mValuesProp->get(managedInstance);

			mKeyType = parent->mDictionaryTypeInfo->mKeyType->GetMonoClass();
			mValueType = parent->mDictionaryTypeInfo->mValueType->GetMonoClass();

			ScriptArray keys(mKeyType, mNumEntries);
			ScriptArray values(mValueType, mNumEntries);

			UINT32 offset = 0;
			void* keyParams[2] = { keys.getInternal(), &offset };
			parent->mKeysCopyTo->invoke(keyCollection, keyParams);

			void* valueParams[2] = { values.getInternal(), &offset };
			parent->mValuesCopyTo->invoke(valueCollection, valueParams);

			keysArray = keys.getInternal();
			valuesArray = values.getInternal();
		}
		else
			mNumEntries = (UINT32)parent->mCachedEntries.size();

		// Note: Handle needed since Enumerator will be on the stack? meaning the GC should be able to find the references.
		if(keysArray && valuesArray)
		{
			mKeysArrayHandle = MonoUtil::newGCHandle((MonoObject*)keysArray, false);
			mValuesArrayHandle = MonoUtil::newGCHandle((MonoObject*)valuesArray, false);
		}
	}

	ManagedSerializableDictionary::Enumerator::Enumerator(const Enumerator& other)
		: mNumEntries(other.mNumEntries), mIteratorInitialized(false), mParent(other.mParent)
	{
		if(other.mKeysArrayHandle != 0 && other.mValuesArrayHandle != 0)
		{
			MonoObject* keysArray = MonoUtil::getObjectFromGCHandle(other.mKeysArrayHandle);
			mKeysArrayHandle = MonoUtil::newGCHandle(keysArray, false);

			MonoObject* valuesArray = MonoUtil::getObjectFromGCHandle(other.mValuesArrayHandle);
			mValuesArrayHandle = MonoUtil::newGCHandle(valuesArray, false);

			mKeyType = other.mKeyType;
			mValueType = other.mValueType;
		}
	}

	ManagedSerializableDictionary::Enumerator::~Enumerator()
	{
		if(mKeysArrayHandle != 0)
			MonoUtil::freeGCHandle(mKeysArrayHandle);

		if(mValuesArrayHandle != 0)
			MonoUtil::freeGCHandle(mValuesArrayHandle);
	}

	ManagedSerializableDictionary::Enumerator&
		ManagedSerializableDictionary::Enumerator::operator=(const Enumerator& other)
	{
		mNumEntries = other.mNumEntries;
		mIteratorInitialized = false;
		mParent = other.mParent;
		mKeyType = nullptr;
		mValueType = nullptr;

		if(mKeysArrayHandle != 0)
		{
			MonoUtil::freeGCHandle(mKeysArrayHandle);
			mKeysArrayHandle = 0;
		}

		if(mValuesArrayHandle != 0)
		{
			MonoUtil::freeGCHandle(mValuesArrayHandle);
			mValuesArrayHandle = 0;
		}

		if(other.mKeysArrayHandle != 0 && other.mValuesArrayHandle != 0)
		{
			MonoObject* keysArray = MonoUtil::getObjectFromGCHandle(other.mKeysArrayHandle);
			mKeysArrayHandle = MonoUtil::newGCHandle(keysArray, false);

			MonoObject* valuesArray = MonoUtil::getObjectFromGCHandle(other.mValuesArrayHandle);
			mValuesArrayHandle = MonoUtil::newGCHandle(valuesArray, false);

			mKeyType = other.mKeyType;
			mValueType = other.mValueType;
		}

		return *this;
	}

	SPtr<ManagedSerializableFieldData> ManagedSerializableDictionary::Enumerator::GetKey() const
	{
		if (mKeysArrayHandle != 0)
		{
			MonoArray* keysArray = (MonoArray*)MonoUtil::getObjectFromGCHandle(mKeysArrayHandle);
			ScriptArray keys(keysArray);

			if(mCurrentIdx != (UINT32)-1)
			{
				void* val = (void*)keys.getRaw(mCurrentIdx, keys.elementSize());

				MonoObject* obj = nullptr;
				if (MonoUtil::isValueType(mKeyType))
				{
					if (val != nullptr)
						obj = MonoUtil::box(mKeyType, val);
				}
				else
					obj = *(MonoObject**)val;

				return ManagedSerializableFieldData::Create(mParent->mDictionaryTypeInfo->mKeyType, obj);
			}
			else
				return nullptr;
		}
		else
		{
			return mCachedIter->first;
		}
	}

	SPtr<ManagedSerializableFieldData> ManagedSerializableDictionary::Enumerator::GetValue() const
	{
		if (mValuesArrayHandle != 0)
		{
			MonoArray* valuesArray = (MonoArray*)MonoUtil::getObjectFromGCHandle(mValuesArrayHandle);
			ScriptArray values(valuesArray);

			if(mCurrentIdx != (UINT32)-1)
			{
				void* val = (void*)values.getRaw(mCurrentIdx, values.elementSize());

				MonoObject* obj = nullptr;
				if (MonoUtil::isValueType(mValueType))
				{
					if (val != nullptr)
						obj = MonoUtil::box(mValueType, val);
				}
				else
					obj = *(MonoObject**)val;

				return ManagedSerializableFieldData::Create(mParent->mDictionaryTypeInfo->mValueType, obj);
			}
			else
				return nullptr;
		}
		else
		{
			return mCachedIter->second;
		}
	}

	bool ManagedSerializableDictionary::Enumerator::MoveNext()
	{
		if (mKeysArrayHandle != 0 && mValuesArrayHandle != 0)
		{
			if((mCurrentIdx + 1) < mNumEntries)
			{
				mCurrentIdx++;
				return true;
			}

			return false;
		}
		else
		{
			if (!mIteratorInitialized)
			{
				mCachedIter = mParent->mCachedEntries.begin();
				mIteratorInitialized = true;
			}
			else
				++mCachedIter;

			return mCachedIter != mParent->mCachedEntries.end();
		}
	}

	ManagedSerializableDictionary::ManagedSerializableDictionary(const ConstructPrivately& dummy)
	{ }

	ManagedSerializableDictionary::ManagedSerializableDictionary(const ConstructPrivately& dummy, const SPtr<ManagedSerializableTypeInfoDictionary>& typeInfo, MonoObject* managedInstance)
		: mDictionaryTypeInfo(typeInfo)
	{
		mGCHandle = MonoUtil::newGCHandle(managedInstance, false);

		MonoClass* dictClass = MonoManager::Instance().findClass(MonoUtil::getClass(managedInstance));
		if (dictClass == nullptr)
			return;

		InitMonoObjects(dictClass);
	}

	ManagedSerializableDictionary::~ManagedSerializableDictionary()
	{
		if(mGCHandle != 0)
		{
			MonoUtil::freeGCHandle(mGCHandle);
			mGCHandle = 0;
		}
	}

	SPtr<ManagedSerializableDictionary> ManagedSerializableDictionary::CreateFromExisting(MonoObject* managedInstance,
		const SPtr<ManagedSerializableTypeInfoDictionary>& typeInfo)
	{
		if(managedInstance == nullptr)
			return nullptr;

		String elementNs;
		String elementTypeName;
		MonoUtil::getClassName(managedInstance, elementNs, elementTypeName);

		String fullName = elementNs + "." + elementTypeName;

		if(ScriptAssemblyManager::Instance().getBuiltinClasses().systemGenericDictionaryClass->GetFullName() != fullName)
			return nullptr;

		return bs_shared_ptr_new<ManagedSerializableDictionary>(ConstructPrivately(), typeInfo, managedInstance);
	}

	SPtr<ManagedSerializableDictionary> ManagedSerializableDictionary::CreateNew(const SPtr<ManagedSerializableTypeInfoDictionary>& typeInfo)
	{
		return bs_shared_ptr_new<ManagedSerializableDictionary>(ConstructPrivately(), typeInfo, createManagedInstance(typeInfo));
	}

	MonoObject* ManagedSerializableDictionary::CreateManagedInstance(const SPtr<ManagedSerializableTypeInfoDictionary>& typeInfo)
	{
		if (!typeInfo->isTypeLoaded())
			return nullptr;

		::MonoClass* dictionaryMonoClass = typeInfo->GetMonoClass();
		MonoClass* dictionaryClass = MonoManager::Instance().findClass(dictionaryMonoClass);
		if (dictionaryClass == nullptr)
			return nullptr;

		return dictionaryClass->createInstance();
	}

	SPtr<ManagedSerializableDictionary> ManagedSerializableDictionary::CreateEmpty()
	{
		return bs_shared_ptr_new<ManagedSerializableDictionary>(ConstructPrivately());
	}

	MonoObject* ManagedSerializableDictionary::GetManagedInstance() const
	{
		if(mGCHandle != 0)
			return MonoUtil::getObjectFromGCHandle(mGCHandle);

		return nullptr;
	}

	void ManagedSerializableDictionary::Serialize()
	{
		if (mGCHandle == 0)
			return;

		MonoObject* managedInstance = MonoUtil::getObjectFromGCHandle(mGCHandle);
		MonoClass* dictionaryClass = MonoManager::Instance().findClass(MonoUtil::getClass(managedInstance));
		if (dictionaryClass == nullptr)
			return;

		InitMonoObjects(dictionaryClass);
		mCachedEntries.clear();

		Enumerator enumerator = GetEnumerator();

		while (enumerator.MoveNext())
		{
			SPtr<ManagedSerializableFieldData> key = enumerator.getKey();
			mCachedEntries.insert(std::make_pair(key, enumerator.getValue()));
		}

		// Serialize children
		for (auto& fieldEntry : mCachedEntries)
		{
			fieldEntry.first->serialize();
			fieldEntry.second->serialize();
		}

		MonoUtil::freeGCHandle(mGCHandle);
		mGCHandle = 0;
	}

	MonoObject* ManagedSerializableDictionary::Deserialize()
	{
		MonoObject* managedInstance = CreateManagedInstance(mDictionaryTypeInfo);
		if (managedInstance == nullptr)
			return nullptr;

		::MonoClass* dictionaryMonoClass = mDictionaryTypeInfo->GetMonoClass();
		MonoClass* dictionaryClass = MonoManager::Instance().findClass(dictionaryMonoClass);
		if (dictionaryClass == nullptr)
			return nullptr;

		InitMonoObjects(dictionaryClass);

		// Deserialize children
		for (auto& fieldEntry : mCachedEntries)
		{
			fieldEntry.first->deserialize();
			fieldEntry.second->deserialize();
		}

		UINT32 idx = 0;
		for (auto& entry : mCachedEntries)
		{
			setFieldData(managedInstance, entry.first, entry.second);
			idx++;
		}

		return managedInstance;
	}

	SPtr<ManagedSerializableFieldData> ManagedSerializableDictionary::GetFieldData(const SPtr<ManagedSerializableFieldData>& key)
	{
		if (mGCHandle != 0)
		{
			MonoObject* value = nullptr;

			void* params[2];
			params[0] = key->GetValue(mDictionaryTypeInfo->mKeyType);
			params[1] = &value;

			MonoObject* managedInstance = MonoUtil::getObjectFromGCHandle(mGCHandle);
			mTryGetValueMethod->invoke(managedInstance, params);

			MonoObject* boxedValue = value;
			::MonoClass* valueTypeClass = mDictionaryTypeInfo->mValueType->GetMonoClass();
			if (MonoUtil::isValueType(valueTypeClass))
			{
				if (value != nullptr)
					boxedValue = MonoUtil::box(valueTypeClass, &value);
			}

			return ManagedSerializableFieldData::Create(mDictionaryTypeInfo->mValueType, boxedValue);
		}
		else
		{
			return mCachedEntries[key];
		}
	}

	void ManagedSerializableDictionary::SetFieldData(const SPtr<ManagedSerializableFieldData>& key, const SPtr<ManagedSerializableFieldData>& val)
	{
		if (mGCHandle != 0)
		{
			MonoObject* managedInstance = MonoUtil::getObjectFromGCHandle(mGCHandle);
			setFieldData(managedInstance, key, val);
		}
		else
		{
			mCachedEntries[key] = val;
		}
	}

	void ManagedSerializableDictionary::SetFieldData(MonoObject* obj, const SPtr<ManagedSerializableFieldData>& key, const SPtr<ManagedSerializableFieldData>& val)
	{
		void* params[2];
		params[0] = key->GetValue(mDictionaryTypeInfo->mKeyType);
		params[1] = val->GetValue(mDictionaryTypeInfo->mValueType);

		mAddMethod->invoke(obj, params);
	}

	void ManagedSerializableDictionary::RemoveFieldData(const SPtr<ManagedSerializableFieldData>& key)
	{
		if (mGCHandle != 0)
		{
			void* params[1];
			params[0] = key->GetValue(mDictionaryTypeInfo->mKeyType);

			MonoObject* managedInstance = MonoUtil::getObjectFromGCHandle(mGCHandle);
			mRemoveMethod->invoke(managedInstance, params);
		}
		else
		{
			auto findIter = mCachedEntries.find(key);
			if (findIter != mCachedEntries.end())
				mCachedEntries.erase(findIter);
		}
	}

	bool ManagedSerializableDictionary::Contains(const SPtr<ManagedSerializableFieldData>& key) const
	{
		if (mGCHandle != 0)
		{
			void* params[1];
			params[0] = key->GetValue(mDictionaryTypeInfo->mKeyType);

			MonoObject* managedInstance = MonoUtil::getObjectFromGCHandle(mGCHandle);
			MonoObject* returnVal = mContainsKeyMethod->invoke(managedInstance, params);
			return *(bool*)MonoUtil::unbox(returnVal);
		}
		else
			return mCachedEntries.find(key) != mCachedEntries.end();
	}

	ManagedSerializableDictionary::Enumerator ManagedSerializableDictionary::GetEnumerator() const
	{
		return Enumerator(this);
	}

	void ManagedSerializableDictionary::InitMonoObjects(MonoClass* dictionaryClass)
	{
		mAddMethod = dictionaryClass->GetMethod("Add", 2);
		mRemoveMethod = dictionaryClass->GetMethod("Remove", 1);
		mTryGetValueMethod = dictionaryClass->GetMethod("TryGetValue", 2);
		mContainsKeyMethod = dictionaryClass->GetMethod("ContainsKey", 1);
		mCountProp = dictionaryClass->GetProperty("Count");
		mKeysProp = dictionaryClass->GetProperty("Keys");
		mValuesProp = dictionaryClass->GetProperty("Values");

		MonoClass* keyCollectionClass = mKeysProp->GetReturnType();
		mKeysCopyTo = keyCollectionClass->GetMethod("CopyTo", 2);

		MonoClass* valueCollectionClass = mValuesProp->GetReturnType();
		mValuesCopyTo = valueCollectionClass->GetMethod("CopyTo", 2);
	}

	RTTITypeBase* ManagedSerializableDictionary::GetRttiStatic()
	{
		return ManagedSerializableDictionaryRTTI::Instance();
	}

	RTTITypeBase* ManagedSerializableDictionary::GetRtti() const
	{
		return ManagedSerializableDictionary::GetRttiStatic();
	}
}
