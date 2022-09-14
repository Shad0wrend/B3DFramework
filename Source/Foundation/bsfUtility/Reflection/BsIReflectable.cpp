//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Reflection/BsIReflectable.h"
#include "Reflection/BsRTTIType.h"
#include "Error/BsException.h"
#include "RTTI/BsIReflectableRTTI.h"

namespace bs
{
	void IReflectable::RegisterRTTITypeInternal(RTTITypeBase* rttiType)
	{
		if(IsTypeIdDuplicateInternal(rttiType->getRTTIId()))
		{
			BS_EXCEPT(InternalErrorException, "RTTI type \"" + rttiType->getRTTIName() +
				"\" has a duplicate ID: " + toString(rttiType->getRTTIId()));
		}

		GetAllRttiTypes()[rttiType->getRTTIId()] = rttiType;
	}

	SPtr<IReflectable> IReflectable::CreateInstanceFromTypeId(UINT32 rttiTypeId)
	{
		RTTITypeBase* type = GetRTTIfromTypeIdInternal(rttiTypeId);

		SPtr<IReflectable> output;
		if(type != nullptr)
			output = type->NewRttiObject();
		
		return output;
	}

	RTTITypeBase* IReflectable::GetRTTIfromTypeIdInternal(UINT32 rttiTypeId)
	{
		const auto iterFind = GetAllRttiTypes().find(rttiTypeId);
		if(iterFind != getAllRTTITypes().end())
			return iterFind->second;

		return nullptr;
	}

	bool IReflectable::IsTypeIdDuplicateInternal(UINT32 typeId)
	{
		if(typeId == TID_Abstract)
			return false;

		return IReflectable::GetRTTIfromTypeIdInternal(typeId) != nullptr;
	}

	bool IReflectable::IsDerivedFrom(RTTITypeBase* base)
	{
		return getRTTI()->isDerivedFrom(base);
	}

	void IReflectable::CheckForCircularReferencesInternal()
	{
		Stack<RTTITypeBase*> todo;

		const UnorderedMap<UINT32, RTTITypeBase*>& allTypes = getAllRTTITypes();
		for(auto& entry : allTypes)
		{
			RTTITypeBase* myType = entry.second;

			UINT32 myNumFields = myType->getNumFields();
			for (UINT32 i = 0; i < myNumFields; i++)
			{
				RTTIField* myField = myType->getField(i);

				if (myField->schema.type != SerializableFT_ReflectablePtr)
					continue;

				auto* myReflectablePtrField = static_cast<RTTIReflectablePtrFieldBase*>(myField);
				
				RTTITypeBase* otherType = myReflectablePtrField->getType();
				UINT32 otherNumFields = otherType->getNumFields();
				for (UINT32 j = 0; j < otherNumFields; j++)
				{
					RTTIField* otherField = otherType->getField(j);

					if (otherField->schema.type != SerializableFT_ReflectablePtr)
						continue;

					auto* otherReflectablePtrField = static_cast<RTTIReflectablePtrFieldBase*>(otherField);

					if (myType->getRTTIId() == otherReflectablePtrField->getType()->getRTTIId() &&
						(!myReflectablePtrField->schema.info.flags.isSet(RTTIFieldFlag::WeakRef) &&
						!otherReflectablePtrField->schema.info.flags.isSet(RTTIFieldFlag::WeakRef)))
					{
						BS_EXCEPT(InternalErrorException, "Found circular reference on RTTI type: " + myType->getRTTIName()
							+ " to type: " + otherType->getRTTIName() + ". Either remove one of the references or mark it"
							+ " as a weak reference when defining the RTTI field.");
					}
				}
			}
		}
	}

	UINT32 IReflectable::GetTypeId() const
	{
		return getRTTI()->getRTTIId();
	}

	const String& IReflectable::GetTypeName() const
	{
		return getRTTI()->getRTTIName();
	}

	RTTITypeBase* IReflectable::GetRttiStatic()
	{
		return IReflectableRTTI::Instance();
	}
}
