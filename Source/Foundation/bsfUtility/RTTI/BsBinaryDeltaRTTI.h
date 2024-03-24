//************************************ bs::framework - Copyright 2024 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once
#include "Prerequisites/BsPrerequisitesUtil.h"
#include "Reflection/BsRTTIType.h"
#include "Serialization/BsBinaryDelta.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Utility
	 *  @{
	 */

	class B3D_UTILITY_EXPORT SerializedTupleDeltaRTTI : public RTTIType<SerializedTupleDelta, ISerialized, SerializedTupleDeltaRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER_PLAIN(Key, 0)
			B3D_RTTI_MEMBER_CONTAINER(Values, 1)
		B3D_RTTI_END_MEMBERS

	public:
		const String& GetRttiName() override
		{
			static String name = "SerializedTupleDelta";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_SerializedTupleDelta;
		}

		SPtr<IReflectable> NewRttiObject() override
		{
			return B3DMakeShared<SerializedTupleDelta>();
		}
	};

	class B3D_UTILITY_EXPORT SerializedTupleEntryDeltaRTTI : public RTTIType<SerializedTupleEntryDelta, IReflectable, SerializedTupleEntryDeltaRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER_PLAIN(Index, 0)
			B3D_RTTI_MEMBER_REFLPTR(Value, 1)
		B3D_RTTI_END_MEMBERS

	public:
		const String& GetRttiName() override
		{
			static String name = "SerializedTupleEntryDelta";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_SerializedTupleEntryDelta;
		}

		SPtr<IReflectable> NewRttiObject() override
		{
			return B3DMakeShared<SerializedTupleEntryDelta>();
		}
	};

	class B3D_UTILITY_EXPORT SerializedArrayDeltaRTTI : public RTTIType<SerializedArrayDelta, ISerialized, SerializedArrayDeltaRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER_PLAIN(ElementCount, 0)
			B3D_RTTI_MEMBER_CONTAINER(Entries, 1)
		B3D_RTTI_END_MEMBERS

	public:
		const String& GetRttiName() override
		{
			static String name = "SerializedArrayDelta";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_SerializedArrayDelta;
		}

		SPtr<IReflectable> NewRttiObject() override
		{
			return B3DMakeShared<SerializedArrayDelta>();
		}
	};

	class B3D_UTILITY_EXPORT SerializedArrayEntryDeltaRTTI : public RTTIType<SerializedArrayEntryDelta, IReflectable, SerializedArrayEntryDeltaRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER_PLAIN(Index, 0)
			B3D_RTTI_MEMBER_REFLPTR(Value, 1)
		B3D_RTTI_END_MEMBERS

	public:
		const String& GetRttiName() override
		{
			static String name = "SerializedArrayEntryDelta";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_SerializedArrayEntryDelta;
		}

		SPtr<IReflectable> NewRttiObject() override
		{
			return B3DMakeShared<SerializedArrayEntryDelta>();
		}
	};

	/** @} */
	/** @endcond */
} // namespace bs
