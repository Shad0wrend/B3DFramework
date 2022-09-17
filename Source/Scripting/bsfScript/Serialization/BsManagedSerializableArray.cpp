//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Serialization/BsManagedSerializableArray.h"
#include "RTTI/BsManagedSerializableArrayRTTI.h"
#include "BsMonoManager.h"
#include "Serialization/BsScriptAssemblyManager.h"
#include "Serialization/BsManagedSerializableField.h"
#include "BsMonoClass.h"
#include "BsMonoMethod.h"
#include "BsMonoArray.h"

namespace bs
{
	ManagedSerializableArray::ManagedSerializableArray(const ConstructPrivately& dummy)
	{

	}

	ManagedSerializableArray::ManagedSerializableArray(const ConstructPrivately& dummy,
		const SPtr<ManagedSerializableTypeInfoArray>& typeInfo, MonoObject* managedInstance)
		: mArrayTypeInfo(typeInfo)
		
	{
		mGCHandle = MonoUtil::NewGcHandle(managedInstance, false);

		ScriptArray scriptArray((MonoArray*)managedInstance);
		mElemSize = scriptArray.ElementSize();

		InitMonoObjects();

		mNumElements.resize(typeInfo->mRank);
		for(UINT32 i = 0; i < typeInfo->mRank; i++)
			mNumElements[i] = GetLengthInternal(i);
	}

	ManagedSerializableArray::~ManagedSerializableArray()
	{
		if(mGCHandle != 0)
		{
			MonoUtil::FreeGcHandle(mGCHandle);
			mGCHandle = 0;
		}
	}

	SPtr<ManagedSerializableArray> ManagedSerializableArray::CreateFromExisting(MonoObject* managedInstance,
		const SPtr<ManagedSerializableTypeInfoArray>& typeInfo)
	{
		if(managedInstance == nullptr)
			return nullptr;

		if(!ScriptAssemblyManager::Instance().GetBuiltinClasses().systemArrayClass->IsInstanceOfType(managedInstance))
			return nullptr;

		return bs_shared_ptr_new<ManagedSerializableArray>(ConstructPrivately(), typeInfo, managedInstance);
	}

	SPtr<ManagedSerializableArray> ManagedSerializableArray::CreateNew(const SPtr<ManagedSerializableTypeInfoArray>& typeInfo, const Vector<UINT32>& sizes)
	{
		return bs_shared_ptr_new<ManagedSerializableArray>(ConstructPrivately(), typeInfo, CreateManagedInstance(typeInfo, sizes));
	}

	SPtr<ManagedSerializableArray> ManagedSerializableArray::CreateNew()
	{
		return bs_shared_ptr_new<ManagedSerializableArray>(ConstructPrivately());
	}

	MonoObject* ManagedSerializableArray::CreateManagedInstance(const SPtr<ManagedSerializableTypeInfoArray>& typeInfo, const Vector<UINT32>& sizes)
	{
		if (!typeInfo->IsTypeLoaded())
			return nullptr;

		MonoClass* arrayClass = ScriptAssemblyManager::Instance().GetBuiltinClasses().systemArrayClass;

		MonoMethod* createInstance = arrayClass->GetMethodExact("CreateInstance", "Type,int[]");

		ScriptArray lengthArray(MonoUtil::GetInT32Class(), (UINT32)sizes.size());
		for (UINT32 i = 0; i < (UINT32)sizes.size(); i++)
			lengthArray.Set(i, sizes[i]);

		void* params[2] = { MonoUtil::GetType(typeInfo->mElementType->GetMonoClass()), lengthArray.GetInternal() };
		return createInstance->Invoke(nullptr, params);
	}

	MonoObject* ManagedSerializableArray::GetManagedInstance() const
	{
		if(mGCHandle != 0)
			return MonoUtil::GetObjectFromGcHandle(mGCHandle);

		return nullptr;
	}

	void ManagedSerializableArray::SetFieldData(UINT32 arrayIdx, const SPtr<ManagedSerializableFieldData>& val)
	{
		if (mGCHandle != 0)
		{
			MonoArray* array = (MonoArray*)MonoUtil::GetObjectFromGcHandle(mGCHandle);
			SetFieldData(array, arrayIdx, val);
		}
		else
		{
			mCachedEntries[arrayIdx] = val;
		}
	}

	void ManagedSerializableArray::SetFieldData(MonoArray* obj, UINT32 arrayIdx, const SPtr<ManagedSerializableFieldData>& val)
	{
		if (MonoUtil::IsValueType(mElementMonoClass))
			SetValueInternal(obj, arrayIdx, val->GetValue(mArrayTypeInfo->mElementType));
		else
		{
			MonoObject* ptrToObj = (MonoObject*)val->GetValue(mArrayTypeInfo->mElementType);
			SetValueInternal(obj, arrayIdx, &ptrToObj);
		}
	}

	SPtr<ManagedSerializableFieldData> ManagedSerializableArray::GetFieldData(UINT32 arrayIdx)
	{
		if (mGCHandle != 0)
		{
			MonoArray* array = (MonoArray*)MonoUtil::GetObjectFromGcHandle(mGCHandle);
			ScriptArray scriptArray(array);

			UINT32 numElems = scriptArray.Size();
			assert(arrayIdx < numElems);

			void* arrayValue = scriptArray.GetRaw(arrayIdx, mElemSize);

			if (MonoUtil::IsValueType(mElementMonoClass))
			{
				MonoObject* boxedObj = nullptr;

				if (arrayValue != nullptr)
					boxedObj = MonoUtil::Box(mElementMonoClass, arrayValue);

				return ManagedSerializableFieldData::Create(mArrayTypeInfo->mElementType, boxedObj);
			}
			else
				return ManagedSerializableFieldData::Create(mArrayTypeInfo->mElementType, *(MonoObject**)arrayValue);
		}
		else
			return mCachedEntries[arrayIdx];
	}

	void ManagedSerializableArray::Serialize()
	{
		if(mGCHandle == 0)
			return;

		mNumElements.resize(mArrayTypeInfo->mRank);
		for (UINT32 i = 0; i < mArrayTypeInfo->mRank; i++)
			mNumElements[i] = GetLengthInternal(i);

		UINT32 numElements = GetTotalLength();
		mCachedEntries = Vector<SPtr<ManagedSerializableFieldData>>(numElements);

		for (UINT32 i = 0; i < numElements; i++)
			mCachedEntries[i] = GetFieldData(i);

		// Serialize children
		for (auto& fieldEntry : mCachedEntries)
			fieldEntry->Serialize();

		MonoUtil::FreeGcHandle(mGCHandle);
		mGCHandle = 0;
	}

	MonoObject* ManagedSerializableArray::Deserialize()
	{
		MonoObject* managedInstance = CreateManagedInstance(mArrayTypeInfo, mNumElements);

		if (managedInstance == nullptr)
			return nullptr;

		ScriptArray scriptArray((MonoArray*)managedInstance);
		mElemSize = scriptArray.ElementSize();

		InitMonoObjects();

		// Deserialize children
		for (auto& fieldEntry : mCachedEntries)
			fieldEntry->Deserialize();

		UINT32 idx = 0;
		for (auto& arrayEntry : mCachedEntries)
		{
			SetFieldData((MonoArray*)managedInstance, idx, arrayEntry);
			idx++;
		}

		return managedInstance;
	}
	
	void ManagedSerializableArray::SetValueInternal(MonoArray* obj, UINT32 arrayIdx, void* val)
	{
		ScriptArray scriptArray(obj);
		UINT32 numElems = (UINT32)scriptArray.Size();
		assert(arrayIdx < numElems);
	
		scriptArray.SetRaw(arrayIdx, (UINT8*)val, mElemSize);
	}

	void ManagedSerializableArray::InitMonoObjects()
	{
		mElementMonoClass = mArrayTypeInfo->mElementType->GetMonoClass();

		MonoClass* arrayClass = ScriptAssemblyManager::Instance().GetBuiltinClasses().systemArrayClass;
		mCopyMethod = arrayClass->GetMethodExact("Copy", "Array,Array,int");
	}

	UINT32 ManagedSerializableArray::ToSequentialIdx(const Vector<UINT32>& idx) const
	{
		UINT32 mNumDims = (UINT32)mNumElements.size();

		if(idx.size() != mNumDims)
			BS_EXCEPT(InvalidParametersException, "Provided index doesn't have the correct number of dimensions");

		if(mNumElements.size() == 0)
			return 0;

		UINT32 curIdx = 0;
		UINT32 prevDimensionSize = 1;
		
		for(INT32 i = mNumDims - 1; i >= 0; i--)
		{
			curIdx += idx[i] * prevDimensionSize;

			prevDimensionSize *= mNumElements[i];
		}

		return curIdx;
	}

	void ManagedSerializableArray::Resize(const Vector<UINT32>& newSizes)
	{
		if (mGCHandle != 0)
		{
			assert(mArrayTypeInfo->mRank == (UINT32)newSizes.size());

			UINT32 srcCount = 1;
			for (auto& numElems : mNumElements)
				srcCount *= numElems;

			UINT32 dstCount = 1;
			for (auto& numElems : newSizes)
				dstCount *= numElems;

			UINT32 copyCount = std::min(srcCount, dstCount);

			MonoObject* newArray = CreateManagedInstance(mArrayTypeInfo, newSizes);

			void* params[3];
			params[0] = GetManagedInstance();
			params[1] = newArray;
			params[2] = &copyCount;

			mCopyMethod->Invoke(nullptr, params);

			MonoUtil::FreeGcHandle(mGCHandle);
			mGCHandle = MonoUtil::NewGcHandle(newArray, false);

			mNumElements = newSizes;
		}
		else
		{
			mNumElements = newSizes;
			mCachedEntries.resize(GetTotalLength());
		}
	}

	UINT32 ManagedSerializableArray::GetLengthInternal(UINT32 dimension) const
	{
		MonoObject* managedInstace = MonoUtil::GetObjectFromGcHandle(mGCHandle);

		MonoClass* systemArray = ScriptAssemblyManager::Instance().GetBuiltinClasses().systemArrayClass;
		MonoMethod* getLength = systemArray->GetMethod("GetLength", 1);

		void* params[1] = { &dimension };
		MonoObject* returnObj = getLength->Invoke(managedInstace, params);

		return *(UINT32*)MonoUtil::Unbox(returnObj);
	}

	UINT32 ManagedSerializableArray::GetTotalLength() const
	{
		UINT32 totalNumElements = 1;
		for (auto& numElems : mNumElements)
			totalNumElements *= numElems;

		return totalNumElements;
	}

	RTTITypeBase* ManagedSerializableArray::GetRttiStatic()
	{
		return ManagedSerializableArrayRTTI::Instance();
	}

	RTTITypeBase* ManagedSerializableArray::GetRtti() const
	{
		return ManagedSerializableArray::GetRttiStatic();
	}
}
