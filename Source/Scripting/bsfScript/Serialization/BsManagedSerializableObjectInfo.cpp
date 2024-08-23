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
		if(objInfo->TypeInfo->Matches(fieldTypeInfo))
		{
			auto iterFind = objInfo->FieldNameToId.find(fieldInfo->Name);
			if(iterFind != objInfo->FieldNameToId.end())
			{
				auto iterFind2 = objInfo->Fields.find(iterFind->second);
				if(iterFind2 != objInfo->Fields.end())
				{
					SPtr<ManagedSerializableMemberInfo> foundField = iterFind2->second;
					if(foundField->IsSerializable())
					{
						if(fieldInfo->TypeInfo->Matches(foundField->TypeInfo))
							return foundField;
					}
				}
			}

			return nullptr;
		}

		if(objInfo->BaseClass != nullptr)
			objInfo = objInfo->BaseClass.get();
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
	return ScriptField->GetAttribute(monoClass);
}

MonoObject* ManagedSerializableFieldInfo::GetValue(MonoObject* instance) const
{
	return ScriptField->GetBoxed(instance);
}

void ManagedSerializableFieldInfo::SetValue(MonoObject* instance, void* value) const
{
	ScriptField->Set(instance, value);
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
	return ScriptProperty->GetAttribute(monoClass);
}

MonoObject* ManagedSerializablePropertyInfo::GetValue(MonoObject* instance) const
{
	return ScriptProperty->Get(instance);
}

void ManagedSerializablePropertyInfo::SetValue(MonoObject* instance, void* value) const
{
	ScriptProperty->Set(instance, value);
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

	return primTypeInfo->Type == Type;
}

bool ManagedSerializableTypeInfoPrimitive::IsTypeLoaded() const
{
	return Type < ScriptPrimitiveType::Count; // Ignoring some removed types
}

::MonoClass* ManagedSerializableTypeInfoPrimitive::GetMonoClass() const
{
	switch(Type)
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
		return enumTypeInfo->TypeNamespace == TypeNamespace &&
			enumTypeInfo->TypeName == TypeName &&
			enumTypeInfo->UnderlyingType == UnderlyingType;
	}

	return false;
}

bool ManagedSerializableTypeInfoEnum::IsTypeLoaded() const
{
	MonoClass* klass = MonoManager::Instance().FindClass(TypeNamespace, TypeName);
	return klass != nullptr;
}

::MonoClass* ManagedSerializableTypeInfoEnum::GetMonoClass() const
{
	MonoClass* klass = MonoManager::Instance().FindClass(TypeNamespace, TypeName);

	if(klass)
		return klass->GetInternalClass();

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

	return objTypeInfo->TypeNamespace == TypeNamespace && objTypeInfo->TypeName == TypeName;
}

bool ManagedSerializableTypeInfoRef::IsTypeLoaded() const
{
	switch(Type)
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

	return ScriptAssemblyManager::Instance().HasSerializableObjectInfo(TypeNamespace, TypeName);
}

::MonoClass* ManagedSerializableTypeInfoRef::GetMonoClass() const
{
	switch(Type)
	{
	case ScriptReferenceType::BuiltinResourceBase:
		return ScriptResource::GetMetaData()->ScriptClass->GetInternalClass();
	case ScriptReferenceType::ManagedResourceBase:
		return ScriptManagedResource::GetMetaData()->ScriptClass->GetInternalClass();
	case ScriptReferenceType::SceneObject:
		return ScriptAssemblyManager::Instance().GetBuiltinClasses().SceneObjectClass->GetInternalClass();
	case ScriptReferenceType::BuiltinComponentBase:
		return ScriptAssemblyManager::Instance().GetBuiltinClasses().ComponentClass->GetInternalClass();
	case ScriptReferenceType::ManagedComponentBase:
		return ScriptAssemblyManager::Instance().GetBuiltinClasses().ManagedComponentClass->GetInternalClass();
	default:
		break;
	}

	// Specific component or resource (either builtin or custom)
	SPtr<ManagedSerializableObjectInfo> objInfo;
	if(!ScriptAssemblyManager::Instance().GetSerializableObjectInfo(TypeNamespace, TypeName, objInfo))
		return nullptr;

	return objInfo->ScriptClass->GetInternalClass();
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

	if(ResourceType == nullptr)
		return resourceTypeInfo->ResourceType == nullptr;

	return ResourceType->Matches(resourceTypeInfo->ResourceType);
}

bool ManagedSerializableTypeInfoRRef::IsTypeLoaded() const
{
	return ResourceType == nullptr || ResourceType->IsTypeLoaded();
}

::MonoClass* ManagedSerializableTypeInfoRRef::GetMonoClass() const
{
	// If non-null, this is a templated (i.e. C# generic) RRef type
	if(ResourceType)
	{
		::MonoClass* resourceTypeClass = ResourceType->GetMonoClass();
		if(resourceTypeClass == nullptr)
			return nullptr;

		return ScriptRRefBase::BindGenericParam(resourceTypeClass);
	}
	// RRefBase
	else
		return ScriptAssemblyManager::Instance().GetBuiltinClasses().RrefBaseClass->GetInternalClass();
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

	return objTypeInfo->TypeNamespace == TypeNamespace && objTypeInfo->TypeName == TypeName &&
		objTypeInfo->ValueType == ValueType && objTypeInfo->RttiTypeId == RttiTypeId;
}

bool ManagedSerializableTypeInfoObject::IsTypeLoaded() const
{
	return ScriptAssemblyManager::Instance().HasSerializableObjectInfo(TypeNamespace, TypeName);
}

::MonoClass* ManagedSerializableTypeInfoObject::GetMonoClass() const
{
	SPtr<ManagedSerializableObjectInfo> objInfo;
	if(!ScriptAssemblyManager::Instance().GetSerializableObjectInfo(TypeNamespace, TypeName, objInfo))
		return nullptr;

	return objInfo->ScriptClass->GetInternalClass();
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

	return arrayTypeInfo->Rank == Rank && arrayTypeInfo->ElementType->Matches(ElementType);
}

bool ManagedSerializableTypeInfoArray::IsTypeLoaded() const
{
	return ElementType->IsTypeLoaded();
}

::MonoClass* ManagedSerializableTypeInfoArray::GetMonoClass() const
{
	::MonoClass* elementClass = ElementType->GetMonoClass();
	if(elementClass == nullptr)
		return nullptr;

	return ScriptArray::BuildArrayClass(ElementType->GetMonoClass(), Rank);
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

	return listTypeInfo->ElementType->Matches(ElementType);
}

bool ManagedSerializableTypeInfoList::IsTypeLoaded() const
{
	return ElementType->IsTypeLoaded();
}

::MonoClass* ManagedSerializableTypeInfoList::GetMonoClass() const
{
	::MonoClass* elementClass = ElementType->GetMonoClass();
	if(elementClass == nullptr)
		return nullptr;

	MonoClass* genericListClass = ScriptAssemblyManager::Instance().GetBuiltinClasses().SystemGenericListClass;
	::MonoClass* genParams[1] = { elementClass };

	return MonoUtil::BindGenericParameters(genericListClass->GetInternalClass(), genParams, 1);
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

	return dictTypeInfo->KeyType->Matches(KeyType) && dictTypeInfo->ValueType->Matches(ValueType);
}

bool ManagedSerializableTypeInfoDictionary::IsTypeLoaded() const
{
	return KeyType->IsTypeLoaded() && ValueType->IsTypeLoaded();
}

::MonoClass* ManagedSerializableTypeInfoDictionary::GetMonoClass() const
{
	::MonoClass* keyClass = KeyType->GetMonoClass();
	::MonoClass* valueClass = ValueType->GetMonoClass();
	if(keyClass == nullptr || valueClass == nullptr)
		return nullptr;

	MonoClass* genericDictionaryClass =
		ScriptAssemblyManager::Instance().GetBuiltinClasses().SystemGenericDictionaryClass;

	::MonoClass* params[2] = { keyClass, valueClass };
	return MonoUtil::BindGenericParameters(genericDictionaryClass->GetInternalClass(), params, 2);
}

RTTIType* ManagedSerializableTypeInfoDictionary::GetRttiStatic()
{
	return ManagedSerializableTypeInfoDictionaryRTTI::Instance();
}

RTTIType* ManagedSerializableTypeInfoDictionary::GetRtti() const
{
	return ManagedSerializableTypeInfoDictionary::GetRttiStatic();
}
