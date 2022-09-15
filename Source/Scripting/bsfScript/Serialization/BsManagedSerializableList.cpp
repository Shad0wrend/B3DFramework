//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Serialization/BsManagedSerializableList.h"
#include "RTTI/BsManagedSerializableListRTTI.h"
#include "BsMonoManager.h"
#include "Serialization/BsScriptAssemblyManager.h"
#include "Serialization/BsManagedSerializableField.h"
#include "BsMonoClass.h"
#include "BsMonoMethod.h"
#include "BsMonoProperty.h"
#include "BsMonoUtil.h"

namespace bs
{
	ManagedSerializableList::ManagedSerializableList(const ConstructPrivately& dummy)
	{

	}

	ManagedSerializableList::ManagedSerializableList(const ConstructPrivately& dummy,
		const SPtr<ManagedSerializableTypeInfoList>& typeInfo, MonoObject* managedInstance)
		:mListTypeInfo(typeInfo)
	{
		mGCHandle = MonoUtil::NewGcHandle(managedInstance, false);

		MonoClass* listClass = MonoManager::Instance().findClass(MonoUtil::getClass(managedInstance));
		if(listClass == nullptr)
			return;

		initMonoObjects(listClass);

		mNumElements = getLengthInternal();
	}

	ManagedSerializableList::~ManagedSerializableList()
	{
		if(mGCHandle != 0)
		{
			MonoUtil::freeGCHandle(mGCHandle);
			mGCHandle = 0;
		}
	}

	SPtr<ManagedSerializableList> ManagedSerializableList::CreateFromExisting(MonoObject* managedInstance,
		const SPtr<ManagedSerializableTypeInfoList>& typeInfo)
	{
		if(managedInstance == nullptr)
			return nullptr;

		String elementNs;
		String elementTypeName;
		MonoUtil::getClassName(managedInstance, elementNs, elementTypeName);

		String fullName = elementNs + "." + elementTypeName;

		if(ScriptAssemblyManager::Instance().getBuiltinClasses().systemGenericListClass->GetFullName() != fullName)
			return nullptr;

		return bs_shared_ptr_new<ManagedSerializableList>(ConstructPrivately(), typeInfo, managedInstance);
	}

	SPtr<ManagedSerializableList> ManagedSerializableList::CreateNew(const SPtr<ManagedSerializableTypeInfoList>& typeInfo, UINT32 size)
	{
		return bs_shared_ptr_new<ManagedSerializableList>(ConstructPrivately(), typeInfo, createManagedInstance(typeInfo, size));
	}

	MonoObject* ManagedSerializableList::CreateManagedInstance(const SPtr<ManagedSerializableTypeInfoList>& typeInfo, UINT32 size)
	{
		if (!typeInfo->isTypeLoaded())
			return nullptr;

		::MonoClass* listMonoClass = typeInfo->GetMonoClass();
		MonoClass* listClass = MonoManager::Instance().findClass(listMonoClass);
		if (listClass == nullptr)
			return nullptr;

		void* params[1] = { &size };
		MonoObject* instance = listClass->createInstance("int", params);
		
		ScriptArray tempArray(typeInfo->mElementType->GetMonoClass(), size);
		params[0] = tempArray.getInternal();

		MonoMethod* addRangeMethod = listClass->GetMethod("AddRange", 1);
		addRangeMethod->invoke(instance, params);

		return instance;
	}

	SPtr<ManagedSerializableList> ManagedSerializableList::CreateEmpty()
	{
		return bs_shared_ptr_new<ManagedSerializableList>(ConstructPrivately());
	}

	MonoObject* ManagedSerializableList::GetManagedInstance() const
	{
		if(mGCHandle != 0)
			return MonoUtil::getObjectFromGCHandle(mGCHandle);

		return nullptr;
	}

	void ManagedSerializableList::SetFieldData(UINT32 arrayIdx, const SPtr<ManagedSerializableFieldData>& val)
	{
		if (mGCHandle != 0)
		{
			MonoObject* managedInstance = MonoUtil::getObjectFromGCHandle(mGCHandle);
			setFieldData(managedInstance, arrayIdx, val);
		}
		else
			mCachedEntries[arrayIdx] = val;
	}

	void ManagedSerializableList::SetFieldData(MonoObject* obj, UINT32 arrayIdx, const SPtr<ManagedSerializableFieldData>& val)
	{
		mItemProp->SetIndexed(obj, arrayIdx, val->GetValue(mListTypeInfo->mElementType));
	}

	void ManagedSerializableList::AddFieldDataInternal(const SPtr<ManagedSerializableFieldData>& val)
	{
		MonoObject* managedInstance = MonoUtil::getObjectFromGCHandle(mGCHandle);

		void* params[1];
		params[0] = val->GetValue(mListTypeInfo->mElementType);
		mAddMethod->invoke(managedInstance, params);
	}

	SPtr<ManagedSerializableFieldData> ManagedSerializableList::GetFieldData(UINT32 arrayIdx)
	{
		if (mGCHandle != 0)
		{
			MonoObject* managedInstance = MonoUtil::getObjectFromGCHandle(mGCHandle);
			MonoObject* obj = mItemProp->GetIndexed(managedInstance, arrayIdx);

			return ManagedSerializableFieldData::Create(mListTypeInfo->mElementType, obj);
		}
		else
			return mCachedEntries[arrayIdx];
	}

	void ManagedSerializableList::Resize(UINT32 newSize)
	{
		if (mGCHandle != 0)
		{
			ScriptArray tempArray(mListTypeInfo->mElementType->GetMonoClass(), newSize);

			UINT32 minSize = std::min(mNumElements, newSize);
			UINT32 dummy = 0;

			void* params[4];
			params[0] = &dummy;;
			params[1] = tempArray.getInternal();
			params[2] = &dummy;
			params[3] = &minSize;

			mCopyToMethod->invoke(getManagedInstance(), params);
			mClearMethod->invoke(getManagedInstance(), nullptr);

			params[0] = tempArray.getInternal();
			mAddRangeMethod->invoke(getManagedInstance(), params);
		}
		else
		{
			mCachedEntries.resize(newSize);
		}

		mNumElements = newSize;
	}

	void ManagedSerializableList::Serialize()
	{
		if (mGCHandle == 0)
			return;

		mNumElements = getLengthInternal();
		mCachedEntries = Vector<SPtr<ManagedSerializableFieldData>>(mNumElements);

		for (UINT32 i = 0; i < mNumElements; i++)
			mCachedEntries[i] = getFieldData(i);

		// Serialize children
		for (auto& fieldEntry : mCachedEntries)
			fieldEntry->serialize();

		MonoUtil::freeGCHandle(mGCHandle);
		mGCHandle = 0;
	}

	MonoObject* ManagedSerializableList::Deserialize()
	{
		MonoObject* managedInstance = createManagedInstance(mListTypeInfo, mNumElements);

		if (managedInstance == nullptr)
			return nullptr;

		MonoClass* listClass = MonoManager::Instance().findClass(mListTypeInfo->GetMonoClass());
		initMonoObjects(listClass);

		// Deserialize children
		for (auto& fieldEntry : mCachedEntries)
			fieldEntry->deserialize();

		UINT32 idx = 0;
		for (auto& entry : mCachedEntries)
		{
			setFieldData(managedInstance, idx, entry);
			idx++;
		}

		return managedInstance;
	}

	UINT32 ManagedSerializableList::GetLengthInternal() const
	{
		MonoObject* managedInstance = MonoUtil::getObjectFromGCHandle(mGCHandle);
		MonoObject* length = mCountProp->get(managedInstance);

		if(length == nullptr)
			return 0;

		return *(UINT32*)MonoUtil::unbox(length);
	}

	void ManagedSerializableList::InitMonoObjects(MonoClass* listClass)
	{
		mItemProp = listClass->GetProperty("Item");
		mCountProp = listClass->GetProperty("Count");
		mAddMethod = listClass->GetMethod("Add", 1);
		mAddRangeMethod = listClass->GetMethod("AddRange", 1);
		mClearMethod = listClass->GetMethod("Clear");
		mCopyToMethod = listClass->GetMethod("CopyTo", 4);
	}

	RTTITypeBase* ManagedSerializableList::GetRttiStatic()
	{
		return ManagedSerializableListRTTI::Instance();
	}

	RTTITypeBase* ManagedSerializableList::GetRtti() const
	{
		return ManagedSerializableList::GetRttiStatic();
	}
}
