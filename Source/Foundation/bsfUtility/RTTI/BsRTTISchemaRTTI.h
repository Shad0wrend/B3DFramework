//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Prerequisites/BsPrerequisitesUtil.h"
#include "RTTI/BsFlagsRTTI.h"
#include "RTTI/BsBitLengthRTTI.h"
#include "Reflection/BsRTTIType.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Utility
	 *  @{
	 */

	template <>
	struct RTTIPlainType<RTTIFieldInfo>
	{
		enum
		{
			id = TID_RTTIFieldInfo
		};

		enum
		{
			hasDynamicSize = 0
		};

		static BitLength ToMemory(const RTTIFieldInfo& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength size;
			size += B3DRTTIWrite(data.Flags, stream);

			return size;
		}

		static BitLength FromMemory(RTTIFieldInfo& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength size;
			size += B3DRTTIRead(data.Flags, stream);

			return size;
		}

		static BitLength GetSize(const RTTIFieldInfo& data, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			return B3DRTTISize(data.Flags);
		}
	};

	class RTTIFieldTypeSchemaRTTI : public RTTIType<RTTIFieldTypeSchema, IReflectable, RTTIFieldTypeSchemaRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER(Type, 0)
			B3D_RTTI_MEMBER(HasDynamicSize, 1)
			B3D_RTTI_MEMBER(FixedSize, 2)
			B3D_RTTI_MEMBER(FieldTypeId, 3)
			B3D_RTTI_MEMBER(FieldTypeSchema, 4)
		B3D_RTTI_END_MEMBERS

	public:
		const String& GetRttiName()
		{
			static String name = "RTTIFieldTupleSchema";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_RTTIFieldTypeSchema;
		}

		SPtr<IReflectable> NewRttiObject()
		{
			return B3DMakeShared<RTTIFieldTypeSchema>();
		}
	};

	class RTTIFieldSchemaRTTI : public RTTIType<RTTIFieldSchema, IReflectable, RTTIFieldSchemaRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER(Id, 0)
			B3D_RTTI_MEMBER(Type, 1)
			B3D_RTTI_MEMBER(IsArray, 2)
			B3D_RTTI_MEMBER(HasDynamicSize, 3)
			B3D_RTTI_MEMBER(Size, 4)
			B3D_RTTI_MEMBER(FieldTypeId, 5)
			B3D_RTTI_MEMBER(FieldTypeSchema, 6)
			B3D_RTTI_MEMBER(Info, 7)
			B3D_RTTI_MEMBER_CONTAINER(FieldTypes, 8)
			B3D_RTTI_MEMBER(IsIterator, 9)
		B3D_RTTI_END_MEMBERS

	public:
		const String& GetRttiName()
		{
			static String name = "RTTIFieldSchema";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_RTTIFieldSchema;
		}

		SPtr<IReflectable> NewRttiObject()
		{
			return B3DMakeShared<RTTIFieldSchema>();
		}
	};

	class RTTISchemaRTTI : public RTTIType<RTTISchema, IReflectable, RTTISchemaRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER(TypeId, 0)
			B3D_RTTI_MEMBER(BaseTypeSchema, 1)
			B3D_RTTI_MEMBER_CONTAINER(FieldSchemas, 2)
		B3D_RTTI_END_MEMBERS

	public:
		const String& GetRttiName() override
		{
			static String name = "RTTISchema";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_RTTISchema;
		}

		SPtr<IReflectable> NewRttiObject()
		{
			return B3DMakeShared<RTTISchema>();
		}
	};

	/** @} */
	/** @endcond */
} // namespace bs
