//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "RTTI/BsStringRTTI.h"
#include "RTTI/BsFlagsRTTI.h"
#include "Serialization/BsManagedSerializableObjectInfo.h"
#include "Error/BsException.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-SEngine
	 *  @{
	 */

	class B3D_SCRIPT_INTEROP_EXPORT ManagedSerializableAssemblyInfoRTTI : public TRTTIType<ManagedSerializableAssemblyInfo, IReflectable, ManagedSerializableAssemblyInfoRTTI>
	{
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER(Name, 0)
			B3D_RTTI_MEMBER_CONTAINER(ObjectInfos, 1)
		B3D_RTTI_END_MEMBERS

	public:
		void OnOperationEnded(ManagedSerializableAssemblyInfo& object, RTTIOperationTypeFlags operationType, RTTIOperationContext& context) override
		{
			if(operationType.IsSet(RTTIOperationType::WriteBit))
			{
				object.TypeNameToId.clear();
				for(const auto& pair : object.ObjectInfos)
				{
					if(!B3D_ENSURE(pair.second != nullptr))
						continue;

					B3D_ENSURE(pair.first == pair.second->TypeInfo->TypeId);
					object.TypeNameToId[pair.second->GetFullTypeName()] = pair.second->TypeInfo->TypeId;
				}
			}
		}

		const String& GetRttiName()
		{
			static String name = "ScriptSerializableAssemblyInfo";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_SerializableAssemblyInfo;
		}

		SPtr<IReflectable> NewRttiObject()
		{
			return B3DMakeShared<ManagedSerializableAssemblyInfo>();
		}
	};

	class B3D_SCRIPT_INTEROP_EXPORT ManagedSerializableObjectInfoRTTI : public TRTTIType<ManagedSerializableObjectInfo, IReflectable, ManagedSerializableObjectInfoRTTI>
	{
	private:
		using TRTTIType<ManagedSerializableObjectInfo, IReflectable, ManagedSerializableObjectInfoRTTI>::GetBaseClass;

		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER(TypeInfo, 0)
			B3D_RTTI_MEMBER(BaseClass, 2)
			B3D_RTTI_MEMBER_CONTAINER(Fields, 3)
		B3D_RTTI_END_MEMBERS

	public:
		void OnOperationEnded(ManagedSerializableObjectInfo& object, RTTIOperationTypeFlags operationType, RTTIOperationContext& context) override
		{
			if(operationType.IsSet(RTTIOperationType::WriteBit))
			{
				object.FieldNameToId.clear();
				for(const auto& pair : object.Fields)
				{
					if(!B3D_ENSURE(pair.second != nullptr))
						continue;

					B3D_ENSURE(pair.first == pair.second->FieldId);
					object.FieldNameToId[pair.second->Name] = pair.second->FieldId;
				}
			}
		}

		const String& GetRttiName() override
		{
			static String name = "ScriptSerializableObjectInfo";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_SerializableObjectInfo;
		}

		SPtr<IReflectable> NewRttiObject() override
		{
			return B3DMakeShared<ManagedSerializableObjectInfo>();
		}
	};

	class B3D_SCRIPT_INTEROP_EXPORT ManagedSerializableMemberInfoRTTI : public TRTTIType<ManagedSerializableMemberInfo, IReflectable, ManagedSerializableMemberInfoRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER(Name, 0)
			B3D_RTTI_MEMBER(TypeInfo, 1)
			B3D_RTTI_MEMBER(FieldId, 2)
			B3D_RTTI_MEMBER(Flags, 3)
			B3D_RTTI_MEMBER(ParentTypeId, 4)
		B3D_RTTI_END_MEMBERS

	public:
		ManagedSerializableMemberInfoRTTI()
			: mInitMembers(this)
		{}

		const String& GetRttiName() override
		{
			static String name = "ScriptSerializableMemberInfo";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_SerializableMemberInfo;
		}

		SPtr<IReflectable> NewRttiObject() override
		{
			// This is an abstract class, but it wasn't always. For compatibility sake we return an object instance so old
			// data can still be properly read.
			return B3DMakeShared<ManagedSerializableFieldInfo>();
		}
	};

	class B3D_SCRIPT_INTEROP_EXPORT ManagedSerializableFieldInfoRTTI : public TRTTIType<ManagedSerializableFieldInfo, ManagedSerializableMemberInfo, ManagedSerializableFieldInfoRTTI>
	{
	public:
		const String& GetRttiName() override
		{
			static String name = "ScriptSerializableFieldInfo";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_SerializableFieldInfo;
		}

		SPtr<IReflectable> NewRttiObject() override
		{
			return B3DMakeShared<ManagedSerializableFieldInfo>();
		}
	};

	class B3D_SCRIPT_INTEROP_EXPORT ManagedSerializablePropertyInfoRTTI : public TRTTIType<ManagedSerializablePropertyInfo, ManagedSerializableMemberInfo, ManagedSerializablePropertyInfoRTTI>
	{
	public:
		const String& GetRttiName() override
		{
			static String name = "ScriptSerializablePropertyInfo";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_SerializablePropertyInfo;
		}

		SPtr<IReflectable> NewRttiObject() override
		{
			return B3DMakeShared<ManagedSerializablePropertyInfo>();
		}
	};

	class B3D_SCRIPT_INTEROP_EXPORT ManagedSerializableTypeInfoRTTI : public TRTTIType<ManagedSerializableTypeInfo, IReflectable, ManagedSerializableTypeInfoRTTI>
	{
	public:
		const String& GetRttiName() override
		{
			static String name = "ScriptSerializableTypeInfo";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_SerializableTypeInfo;
		}

		SPtr<IReflectable> NewRttiObject() override
		{
			B3D_EXCEPT(InvalidStateException, "Cannot instantiate an abstract class");
			return nullptr;
		}
	};

	class B3D_SCRIPT_INTEROP_EXPORT ManagedSerializableTypeInfoPrimitiveRTTI : public TRTTIType<ManagedSerializableTypeInfoPrimitive, ManagedSerializableTypeInfo, ManagedSerializableTypeInfoPrimitiveRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER(Type, 0)
		B3D_RTTI_END_MEMBERS

	public:
		ManagedSerializableTypeInfoPrimitiveRTTI()
			: mInitMembers(this)
		{}

		const String& GetRttiName() override
		{
			static String name = "ScriptSerializableTypeInfoPrimitive";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_SerializableTypeInfoPrimitive;
		}

		SPtr<IReflectable> NewRttiObject() override
		{
			return B3DMakeShared<ManagedSerializableTypeInfoPrimitive>();
		}
	};

	class B3D_SCRIPT_INTEROP_EXPORT ManagedSerializableTypeInfoEnumRTTI : public TRTTIType<ManagedSerializableTypeInfoEnum, ManagedSerializableTypeInfo, ManagedSerializableTypeInfoEnumRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER(UnderlyingType, 0)
			B3D_RTTI_MEMBER(TypeNamespace, 1)
			B3D_RTTI_MEMBER(TypeName, 2)
		B3D_RTTI_END_MEMBERS

	public:
		ManagedSerializableTypeInfoEnumRTTI()
			: mInitMembers(this)
		{}

		const String& GetRttiName() override
		{
			static String name = "ScriptSerializableTypeInfoEnum";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_SerializableTypeInfoEnum;
		}

		SPtr<IReflectable> NewRttiObject() override
		{
			return B3DMakeShared<ManagedSerializableTypeInfoEnum>();
		}
	};

	class B3D_SCRIPT_INTEROP_EXPORT ManagedSerializableTypeInfoRefRTTI : public TRTTIType<ManagedSerializableTypeInfoRef, ManagedSerializableTypeInfo, ManagedSerializableTypeInfoRefRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER(Type, 0)
			B3D_RTTI_MEMBER(TypeName, 1)
			B3D_RTTI_MEMBER(TypeNamespace, 2)
			B3D_RTTI_MEMBER(RtiiTypeId, 3)
		B3D_RTTI_END_MEMBERS

	public:
		ManagedSerializableTypeInfoRefRTTI()
			: mInitMembers(this)
		{}

		const String& GetRttiName() override
		{
			static String name = "ScriptSerializableTypeInfoRef";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_SerializableTypeInfoRef;
		}

		SPtr<IReflectable> NewRttiObject() override
		{
			return B3DMakeShared<ManagedSerializableTypeInfoRef>();
		}
	};

	class B3D_SCRIPT_INTEROP_EXPORT ManagedSerializableTypeInfoRRefRTTI : public TRTTIType<ManagedSerializableTypeInfoRRef, ManagedSerializableTypeInfo, ManagedSerializableTypeInfoRRefRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER(ResourceType, 0)
		B3D_RTTI_END_MEMBERS

	public:
		ManagedSerializableTypeInfoRRefRTTI()
			: mInitMembers(this)
		{}

		const String& GetRttiName() override
		{
			static String name = "ScriptSerializableTypeInfoRRef";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_SerializableTypeInfoRRef;
		}

		SPtr<IReflectable> NewRttiObject() override
		{
			return B3DMakeShared<ManagedSerializableTypeInfoRRef>();
		}
	};

	class B3D_SCRIPT_INTEROP_EXPORT ManagedSerializableTypeInfoObjectRTTI : public TRTTIType<ManagedSerializableTypeInfoObject, ManagedSerializableTypeInfo, ManagedSerializableTypeInfoObjectRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER(TypeName, 0)
			B3D_RTTI_MEMBER(TypeNamespace, 1)
			B3D_RTTI_MEMBER(ValueType, 2)
			B3D_RTTI_MEMBER(TypeId, 4)
			B3D_RTTI_MEMBER(Flags, 5)
			B3D_RTTI_MEMBER(RttiTypeId, 6)
		B3D_RTTI_END_MEMBERS

	public:
		ManagedSerializableTypeInfoObjectRTTI()
			: mInitMembers(this)
		{}

		const String& GetRttiName() override
		{
			static String name = "ScriptSerializableTypeInfoObject";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_SerializableTypeInfoObject;
		}

		SPtr<IReflectable> NewRttiObject() override
		{
			return B3DMakeShared<ManagedSerializableTypeInfoObject>();
		}
	};

	class B3D_SCRIPT_INTEROP_EXPORT ManagedSerializableTypeInfoArrayRTTI : public TRTTIType<ManagedSerializableTypeInfoArray, ManagedSerializableTypeInfo, ManagedSerializableTypeInfoArrayRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER(ElementType, 0)
			B3D_RTTI_MEMBER(Rank, 1)
		B3D_RTTI_END_MEMBERS

	public:
		ManagedSerializableTypeInfoArrayRTTI()
			: mInitMembers(this)
		{}

		const String& GetRttiName() override
		{
			static String name = "ScriptSerializableTypeInfoArray";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_SerializableTypeInfoArray;
		}

		SPtr<IReflectable> NewRttiObject() override
		{
			return B3DMakeShared<ManagedSerializableTypeInfoArray>();
		}
	};

	class B3D_SCRIPT_INTEROP_EXPORT ManagedSerializableTypeInfoListRTTI : public TRTTIType<ManagedSerializableTypeInfoList, ManagedSerializableTypeInfo, ManagedSerializableTypeInfoListRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER(ElementType, 0)
		B3D_RTTI_END_MEMBERS

	public:
		ManagedSerializableTypeInfoListRTTI()
			: mInitMembers(this)
		{}

		const String& GetRttiName() override
		{
			static String name = "ScriptSerializableTypeInfoList";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_SerializableTypeInfoList;
		}

		SPtr<IReflectable> NewRttiObject() override
		{
			return B3DMakeShared<ManagedSerializableTypeInfoList>();
		}
	};

	class B3D_SCRIPT_INTEROP_EXPORT ManagedSerializableTypeInfoDictionaryRTTI : public TRTTIType<ManagedSerializableTypeInfoDictionary, ManagedSerializableTypeInfo, ManagedSerializableTypeInfoDictionaryRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER(KeyType, 0)
			B3D_RTTI_MEMBER(ValueType, 1)
		B3D_RTTI_END_MEMBERS

	public:
		ManagedSerializableTypeInfoDictionaryRTTI()
			: mInitMembers(this)
		{}

		const String& GetRttiName() override
		{
			static String name = "ScriptSerializableTypeInfoDictionary";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_SerializableTypeInfoDictionary;
		}

		SPtr<IReflectable> NewRttiObject() override
		{
			return B3DMakeShared<ManagedSerializableTypeInfoDictionary>();
		}
	};

	/** @} */
	/** @endcond */
} // namespace bs
