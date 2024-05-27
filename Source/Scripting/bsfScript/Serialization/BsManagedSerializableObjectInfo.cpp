//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Serialization/BsManagedSerializableObjectInfo.h"
#include "RTTI/BsManagedSerializableObjectInfoRTTI.h"
#include "Wrappers/GUI/BsScriptRange.h"
#include "Wrappers/GUI/BsScriptStep.h"
#include "BsMonoUtil.h"
#include "BsMonoClass.h"
#include "BsMonoManager.h"
#include "BsMonoField.h"
#include "BsMonoProperty.h"
#include "Serialization/BsScriptAssemblyManager.h"
#include "Wrappers/BsScriptManagedResource.h"
#include "Wrappers/BsScriptRRefBase.h"

using namespace bs;
RTTIType* ManagedSerializableAssemblyInfo::GetRttiStatic()
{
	return ManagedSerializableAssemblyInfoRTTI::Instance();
}

RTTIType* ManagedSerializableAssemblyInfo::GetRtti() const
{
	return ManagedSerializableAssemblyInfo::GetRttiStatic();
}

SPtr<ManagedSerializableMemberInfo> ManagedSerializableObjectInfo::FindMatchingField(const SPtr<ManagedSerializableMemberInfo>& fieldInfo, const SPtr<ManagedSerializableTypeInfo>& fieldTypeInfo) const
{
	const ManagedSerializableObjectInfo* objInfo = this;
	while(objInfo != nullptr)
	{
		if(objInfo->MTypeInfo->Matches(fieldTypeInfo))
		{
			auto iterFind = objInfo->MFieldNameToId.find(fieldInfo->MName);
			if(iterFind != objInfo->MFieldNameToId.end())
			{
				auto iterFind2 = objInfo->MFields.find(iterFind->second);
				if(iterFind2 != objInfo->MFields.end())
				{
					SPtr<ManagedSerializableMemberInfo> foundField = iterFind2->second;
					if(foundField->IsSerializable())
					{
						if(fieldInfo->MTypeInfo->Matches(foundField->MTypeInfo))
							return foundField;
					}
				}
			}

			return nullptr;
		}

		if(objInfo->MBaseClass != nullptr)
			objInfo = objInfo->MBaseClass.get();
		else
			objInfo = nullptr;
	}

	return nullptr;
}

RTTIType* ManagedSerializableObjectInfo::GetRttiStatic()
{
	return ManagedSerializableObjectInfoRTTI::Instance();
}

RTTIType* ManagedSerializableObjectInfo::GetRtti() const
{
	return ManagedSerializableObjectInfo::GetRttiStatic();
}

RTTIType* ManagedSerializableMemberInfo::GetRttiStatic()
{
	return ManagedSerializableMemberInfoRTTI::Instance();
}

RTTIType* ManagedSerializableMemberInfo::GetRtti() const
{
	return ManagedSerializableMemberInfo::GetRttiStatic();
}

::MonoObject* ManagedSerializableFieldInfo::GetAttribute(MonoClass* monoClass)
{
	return MMonoField->GetAttribute(monoClass);
}

MonoObject* ManagedSerializableFieldInfo::GetValue(MonoObject* instance) const
{
	return MMonoField->GetBoxed(instance);
}

void ManagedSerializableFieldInfo::SetValue(MonoObject* instance, void* value) const
{
	MMonoField->Set(instance, value);
}

RTTIType* ManagedSerializableFieldInfo::GetRttiStatic()
{
	return ManagedSerializableFieldInfoRTTI::Instance();
}

RTTIType* ManagedSerializableFieldInfo::GetRtti() const
{
	return ManagedSerializableFieldInfo::GetRttiStatic();
}

::MonoObject* ManagedSerializablePropertyInfo::GetAttribute(MonoClass* monoClass)
{
	return MMonoProperty->GetAttribute(monoClass);
}

MonoObject* ManagedSerializablePropertyInfo::GetValue(MonoObject* instance) const
{
	return MMonoProperty->Get(instance);
}

void ManagedSerializablePropertyInfo::SetValue(MonoObject* instance, void* value) const
{
	MMonoProperty->Set(instance, value);
}

RTTIType* ManagedSerializablePropertyInfo::GetRttiStatic()
{
	return ManagedSerializablePropertyInfoRTTI::Instance();
}

RTTIType* ManagedSerializablePropertyInfo::GetRtti() const
{
	return ManagedSerializablePropertyInfo::GetRttiStatic();
}

RTTIType* ManagedSerializableTypeInfo::GetRttiStatic()
{
	return ManagedSerializableTypeInfoRTTI::Instance();
}

RTTIType* ManagedSerializableTypeInfo::GetRtti() const
{
	return ManagedSerializableTypeInfo::GetRttiStatic();
}

bool ManagedSerializableTypeInfoPrimitive::Matches(const SPtr<ManagedSerializableTypeInfo>& typeInfo) const
{
	if(!B3DRTTIIsOfType<ManagedSerializableTypeInfoPrimitive>(typeInfo))
		return false;

	auto primTypeInfo = std::static_pointer_cast<ManagedSerializableTypeInfoPrimitive>(typeInfo);

	return primTypeInfo->MType == MType;
}

bool ManagedSerializableTypeInfoPrimitive::IsTypeLoaded() const
{
	return MType < ScriptPrimitiveType::Count; // Ignoring some removed types
}

::MonoClass* ManagedSerializableTypeInfoPrimitive::GetMonoClass() const
{
	switch(MType)
	{
	case ScriptPrimitiveType::Bool:
		return MonoUtil::GetBoolClass();
	case ScriptPrimitiveType::Char:
		return MonoUtil::GetCharClass();
	case ScriptPrimitiveType::I8:
		return MonoUtil::GetSByteClass();
	case ScriptPrimitiveType::U8:
		return MonoUtil::GetByteClass();
	case ScriptPrimitiveType::I16:
		return MonoUtil::GetInT16Class();
	case ScriptPrimitiveType::U16:
		return MonoUtil::GetUinT16Class();
	case ScriptPrimitiveType::I32:
		return MonoUtil::GetInT32Class();
	case ScriptPrimitiveType::U32:
		return MonoUtil::GetUinT32Class();
	case ScriptPrimitiveType::I64:
		return MonoUtil::GetInT64Class();
	case ScriptPrimitiveType::U64:
		return MonoUtil::GetUinT64Class();
	case ScriptPrimitiveType::Float:
		return MonoUtil::GetFloatClass();
	case ScriptPrimitiveType::Double:
		return MonoUtil::GetDoubleClass();
	case ScriptPrimitiveType::String:
		return MonoUtil::GetStringClass();
	default:
		break;
	}

	return nullptr;
}

RTTIType* ManagedSerializableTypeInfoPrimitive::GetRttiStatic()
{
	return ManagedSerializableTypeInfoPrimitiveRTTI::Instance();
}

RTTIType* ManagedSerializableTypeInfoPrimitive::GetRtti() const
{
	return ManagedSerializableTypeInfoPrimitive::GetRttiStatic();
}

bool ManagedSerializableTypeInfoEnum::Matches(const SPtr<ManagedSerializableTypeInfo>& typeInfo) const
{
	if(const auto enumTypeInfo = B3DRTTICast<ManagedSerializableTypeInfoEnum>(typeInfo.get()))
	{
		return enumTypeInfo->MTypeNamespace == MTypeNamespace &&
			enumTypeInfo->MTypeName == MTypeName &&
			enumTypeInfo->MUnderlyingType == MUnderlyingType;
	}

	return false;
}

bool ManagedSerializableTypeInfoEnum::IsTypeLoaded() const
{
	MonoClass* klass = MonoManager::Instance().FindClass(MTypeNamespace, MTypeName);
	return klass != nullptr;
}

::MonoClass* ManagedSerializableTypeInfoEnum::GetMonoClass() const
{
	MonoClass* klass = MonoManager::Instance().FindClass(MTypeNamespace, MTypeName);

	if(klass)
		return klass->GetInternalClassInternal();

	return nullptr;
}

RTTIType* ManagedSerializableTypeInfoEnum::GetRttiStatic()
{
	return ManagedSerializableTypeInfoEnumRTTI::Instance();
}

RTTIType* ManagedSerializableTypeInfoEnum::GetRtti() const
{
	return ManagedSerializableTypeInfoEnum::GetRttiStatic();
}

bool ManagedSerializableTypeInfoRef::Matches(const SPtr<ManagedSerializableTypeInfo>& typeInfo) const
{
	if(!B3DRTTIIsOfType<ManagedSerializableTypeInfoRef>(typeInfo))
		return false;

	auto objTypeInfo = std::static_pointer_cast<ManagedSerializableTypeInfoRef>(typeInfo);

	return objTypeInfo->MTypeNamespace == MTypeNamespace && objTypeInfo->MTypeName == MTypeName;
}

bool ManagedSerializableTypeInfoRef::IsTypeLoaded() const
{
	switch(MType)
	{
	case ScriptReferenceType::BuiltinResourceBase:
	case ScriptReferenceType::ManagedResourceBase:
	case ScriptReferenceType::BuiltinResource:
	case ScriptReferenceType::BuiltinComponentBase:
	case ScriptReferenceType::ManagedComponentBase:
	case ScriptReferenceType::BuiltinComponent:
	case ScriptReferenceType::SceneObject:
	case ScriptReferenceType::ReflectableObject:
		return true;
	default:
		break;
	}

	return ScriptAssemblyManager::Instance().HasSerializableObjectInfo(MTypeNamespace, MTypeName);
}

::MonoClass* ManagedSerializableTypeInfoRef::GetMonoClass() const
{
	switch(MType)
	{
	case ScriptReferenceType::BuiltinResourceBase:
		return ScriptResource::GetMetaData()->ScriptClass->GetInternalClassInternal();
	case ScriptReferenceType::ManagedResourceBase:
		return ScriptManagedResource::GetMetaData()->ScriptClass->GetInternalClassInternal();
	case ScriptReferenceType::SceneObject:
		return ScriptAssemblyManager::Instance().GetBuiltinClasses().SceneObjectClass->GetInternalClassInternal();
	case ScriptReferenceType::BuiltinComponentBase:
		return ScriptAssemblyManager::Instance().GetBuiltinClasses().ComponentClass->GetInternalClassInternal();
	case ScriptReferenceType::ManagedComponentBase:
		return ScriptAssemblyManager::Instance().GetBuiltinClasses().ManagedComponentClass->GetInternalClassInternal();
	default:
		break;
	}

	// Specific component or resource (either builtin or custom)
	SPtr<ManagedSerializableObjectInfo> objInfo;
	if(!ScriptAssemblyManager::Instance().GetSerializableObjectInfo(MTypeNamespace, MTypeName, objInfo))
		return nullptr;

	return objInfo->MMonoClass->GetInternalClassInternal();
}

RTTIType* ManagedSerializableTypeInfoRef::GetRttiStatic()
{
	return ManagedSerializableTypeInfoRefRTTI::Instance();
}

RTTIType* ManagedSerializableTypeInfoRef::GetRtti() const
{
	return ManagedSerializableTypeInfoRef::GetRttiStatic();
}

bool ManagedSerializableTypeInfoRRef::Matches(const SPtr<ManagedSerializableTypeInfo>& typeInfo) const
{
	if(!B3DRTTIIsOfType<ManagedSerializableTypeInfoRRef>(typeInfo))
		return false;

	auto resourceTypeInfo = std::static_pointer_cast<ManagedSerializableTypeInfoRRef>(typeInfo);

	if(MResourceType == nullptr)
		return resourceTypeInfo->MResourceType == nullptr;

	return MResourceType->Matches(resourceTypeInfo->MResourceType);
}

bool ManagedSerializableTypeInfoRRef::IsTypeLoaded() const
{
	return MResourceType == nullptr || MResourceType->IsTypeLoaded();
}

::MonoClass* ManagedSerializableTypeInfoRRef::GetMonoClass() const
{
	// If non-null, this is a templated (i.e. C# generic) RRef type
	if(MResourceType)
	{
		::MonoClass* resourceTypeClass = MResourceType->GetMonoClass();
		if(resourceTypeClass == nullptr)
			return nullptr;

		return ScriptRRefBase::BindGenericParam(resourceTypeClass);
	}
	// RRefBase
	else
		return ScriptAssemblyManager::Instance().GetBuiltinClasses().RrefBaseClass->GetInternalClassInternal();
}

RTTIType* ManagedSerializableTypeInfoRRef::GetRttiStatic()
{
	return ManagedSerializableTypeInfoRRefRTTI::Instance();
}

RTTIType* ManagedSerializableTypeInfoRRef::GetRtti() const
{
	return ManagedSerializableTypeInfoRRef::GetRttiStatic();
}

bool ManagedSerializableTypeInfoObject::Matches(const SPtr<ManagedSerializableTypeInfo>& typeInfo) const
{
	if(!B3DRTTIIsOfType<ManagedSerializableTypeInfoObject>(typeInfo))
		return false;

	auto objTypeInfo = std::static_pointer_cast<ManagedSerializableTypeInfoObject>(typeInfo);

	return objTypeInfo->MTypeNamespace == MTypeNamespace && objTypeInfo->MTypeName == MTypeName &&
		objTypeInfo->MValueType == MValueType && objTypeInfo->MRtiiTypeId == MRtiiTypeId;
}

bool ManagedSerializableTypeInfoObject::IsTypeLoaded() const
{
	return ScriptAssemblyManager::Instance().HasSerializableObjectInfo(MTypeNamespace, MTypeName);
}

::MonoClass* ManagedSerializableTypeInfoObject::GetMonoClass() const
{
	SPtr<ManagedSerializableObjectInfo> objInfo;
	if(!ScriptAssemblyManager::Instance().GetSerializableObjectInfo(MTypeNamespace, MTypeName, objInfo))
		return nullptr;

	return objInfo->MMonoClass->GetInternalClassInternal();
}

RTTIType* ManagedSerializableTypeInfoObject::GetRttiStatic()
{
	return ManagedSerializableTypeInfoObjectRTTI::Instance();
}

RTTIType* ManagedSerializableTypeInfoObject::GetRtti() const
{
	return ManagedSerializableTypeInfoObject::GetRttiStatic();
}

bool ManagedSerializableTypeInfoArray::Matches(const SPtr<ManagedSerializableTypeInfo>& typeInfo) const
{
	if(!B3DRTTIIsOfType<ManagedSerializableTypeInfoArray>(typeInfo))
		return false;

	auto arrayTypeInfo = std::static_pointer_cast<ManagedSerializableTypeInfoArray>(typeInfo);

	return arrayTypeInfo->MRank == MRank && arrayTypeInfo->MElementType->Matches(MElementType);
}

bool ManagedSerializableTypeInfoArray::IsTypeLoaded() const
{
	return MElementType->IsTypeLoaded();
}

::MonoClass* ManagedSerializableTypeInfoArray::GetMonoClass() const
{
	::MonoClass* elementClass = MElementType->GetMonoClass();
	if(elementClass == nullptr)
		return nullptr;

	return ScriptArray::BuildArrayClass(MElementType->GetMonoClass(), MRank);
}

RTTIType* ManagedSerializableTypeInfoArray::GetRttiStatic()
{
	return ManagedSerializableTypeInfoArrayRTTI::Instance();
}

RTTIType* ManagedSerializableTypeInfoArray::GetRtti() const
{
	return ManagedSerializableTypeInfoArray::GetRttiStatic();
}

bool ManagedSerializableTypeInfoList::Matches(const SPtr<ManagedSerializableTypeInfo>& typeInfo) const
{
	if(!B3DRTTIIsOfType<ManagedSerializableTypeInfoList>(typeInfo))
		return false;

	auto listTypeInfo = std::static_pointer_cast<ManagedSerializableTypeInfoList>(typeInfo);

	return listTypeInfo->MElementType->Matches(MElementType);
}

bool ManagedSerializableTypeInfoList::IsTypeLoaded() const
{
	return MElementType->IsTypeLoaded();
}

::MonoClass* ManagedSerializableTypeInfoList::GetMonoClass() const
{
	::MonoClass* elementClass = MElementType->GetMonoClass();
	if(elementClass == nullptr)
		return nullptr;

	MonoClass* genericListClass = ScriptAssemblyManager::Instance().GetBuiltinClasses().SystemGenericListClass;
	::MonoClass* genParams[1] = { elementClass };

	return MonoUtil::BindGenericParameters(genericListClass->GetInternalClassInternal(), genParams, 1);
}

RTTIType* ManagedSerializableTypeInfoList::GetRttiStatic()
{
	return ManagedSerializableTypeInfoListRTTI::Instance();
}

RTTIType* ManagedSerializableTypeInfoList::GetRtti() const
{
	return ManagedSerializableTypeInfoList::GetRttiStatic();
}

bool ManagedSerializableTypeInfoDictionary::Matches(const SPtr<ManagedSerializableTypeInfo>& typeInfo) const
{
	if(!B3DRTTIIsOfType<ManagedSerializableTypeInfoDictionary>(typeInfo))
		return false;

	auto dictTypeInfo = std::static_pointer_cast<ManagedSerializableTypeInfoDictionary>(typeInfo);

	return dictTypeInfo->MKeyType->Matches(MKeyType) && dictTypeInfo->MValueType->Matches(MValueType);
}

bool ManagedSerializableTypeInfoDictionary::IsTypeLoaded() const
{
	return MKeyType->IsTypeLoaded() && MValueType->IsTypeLoaded();
}

::MonoClass* ManagedSerializableTypeInfoDictionary::GetMonoClass() const
{
	::MonoClass* keyClass = MKeyType->GetMonoClass();
	::MonoClass* valueClass = MValueType->GetMonoClass();
	if(keyClass == nullptr || valueClass == nullptr)
		return nullptr;

	MonoClass* genericDictionaryClass =
		ScriptAssemblyManager::Instance().GetBuiltinClasses().SystemGenericDictionaryClass;

	::MonoClass* params[2] = { keyClass, valueClass };
	return MonoUtil::BindGenericParameters(genericDictionaryClass->GetInternalClassInternal(), params, 2);
}

RTTIType* ManagedSerializableTypeInfoDictionary::GetRttiStatic()
{
	return ManagedSerializableTypeInfoDictionaryRTTI::Instance();
}

RTTIType* ManagedSerializableTypeInfoDictionary::GetRtti() const
{
	return ManagedSerializableTypeInfoDictionary::GetRttiStatic();
}
