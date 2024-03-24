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

	class B3D_UTILITY_EXPORT SerializedTupleDeltaEntryRTTI : public RTTIType<SerializedTupleDeltaEntry, IReflectable, SerializedTupleDeltaEntryRTTI>
	{
	private:
		B3D_RTTI_BEGIN_MEMBERS
			B3D_RTTI_MEMBER_PLAIN(Index, 0)
			B3D_RTTI_MEMBER_REFLPTR(Value, 1)
		B3D_RTTI_END_MEMBERS

	public:
		const String& GetRttiName() override
		{
			static String name = "SerializedTupleDeltaEntry";
			return name;
		}

		u32 GetRttiId() const override
		{
			return TID_SerializedTupleDeltaEntry;
		}

		SPtr<IReflectable> NewRttiObject() override
		{
			return B3DMakeShared<SerializedTupleDeltaEntry>();
		}
	};

	/** @} */
	/** @endcond */
} // namespace bs
