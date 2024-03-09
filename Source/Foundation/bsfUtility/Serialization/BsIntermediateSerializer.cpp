//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Serialization/BsIntermediateSerializer.h"
#include "FileSystem/BsDataStream.h"
#include "Reflection/BsRTTIReflectablePtrField.h"
#include "Reflection/BsRTTIType.h"

using namespace bs;

IntermediateSerializer::IntermediateSerializer()
	: mAlloc(&GetFrameAllocator())
{}

SPtr<IReflectable> IntermediateSerializer::Decode(const SerializedObject* serializedObject, SerializationContext* context)
{
	mContext = context;

	mAlloc->MarkFrame();
	mObjectMap.clear();

	SPtr<IReflectable> output;
	RTTITypeBase* type = IReflectable::GetRTTITypeFromTypeId(serializedObject->GetRootTypeId());
	if(type != nullptr)
	{
		output = type->NewRttiObject();
		auto iterNewObj = mObjectMap.insert(std::make_pair(serializedObject, ObjectDeserializationData(output, serializedObject)));

		iterNewObj.first->second.DeserializationInProgress = true;
		DeserializeReflectableObject(output, serializedObject);
		iterNewObj.first->second.DeserializationInProgress = false;
		iterNewObj.first->second.IsDeserialized = true;
	}

	// Go through the remaining objects (should be only ones with weak refs)
	for(auto iter = mObjectMap.begin(); iter != mObjectMap.end(); ++iter)
	{
		ObjectDeserializationData& objToDecode = iter->second;

		if(objToDecode.IsDeserialized)
			continue;

		objToDecode.DeserializationInProgress = true;
		DeserializeReflectableObject(objToDecode.Object, objToDecode.SerializedObject);
		objToDecode.DeserializationInProgress = false;
		objToDecode.IsDeserialized = true;
	}

	mObjectMap.clear();
	mAlloc->Clear();

	return output;
}

SPtr<SerializedObject> IntermediateSerializer::Encode(IReflectable* object, SerializedObjectEncodeFlags flags, SerializationContext* context)
{
	mContext = context;

	return SerializeReflectableObject(object, flags, context, mAlloc);
}

void IntermediateSerializer::DeserializeReflectableObject(const SPtr<IReflectable>& object, const SerializedObject* serializableObject)
{
	u32 numSubObjects = (u32)serializableObject->SubObjects.size();
	if(numSubObjects == 0)
		return;

	FrameStack<RTTITypeBase*> rttiInstances;
	for(i32 subObjectIdx = numSubObjects - 1; subObjectIdx >= 0; subObjectIdx--)
	{
		const SerializedSubObject& subObject = serializableObject->SubObjects[subObjectIdx];

		RTTITypeBase* rtti = IReflectable::GetRTTITypeFromTypeId(subObject.TypeId);
		if(rtti == nullptr)
			continue;

		RTTITypeBase* rttiInstance = rtti->CloneInternal(*mAlloc);
		rttiInstance->OnDeserializationStarted(object.get(), mContext);
		rttiInstances.push(rttiInstance);

		u32 numFields = rtti->GetNumFields();
		for(u32 fieldIdx = 0; fieldIdx < numFields; fieldIdx++)
		{
			RTTIField* curGenericField = rtti->GetField(fieldIdx);

			auto iterFindFieldData = subObject.Entries.find(curGenericField->Schema.Id);
			if(iterFindFieldData == subObject.Entries.end())
				continue;

			SPtr<SerializedInstance> entryData = iterFindFieldData->second.Serialized;
			if(curGenericField->Schema.IsArray)
			{
				SPtr<SerializedArray> arrayData = std::static_pointer_cast<SerializedArray>(entryData);

				u32 arrayNumElems = (u32)arrayData->ElementCount;
				curGenericField->SetArraySize(rttiInstance, object.get(), arrayNumElems);

				switch(curGenericField->Schema.Type)
				{
				case SerializableFT_ReflectablePtr:
					{
						auto* curField = static_cast<RTTIReflectablePtrFieldBase*>(curGenericField);

						for(auto& arrayElem : arrayData->Entries)
						{
							SPtr<SerializedObject> arrayElemData = std::static_pointer_cast<SerializedObject>(arrayElem.second.Serialized);
							RTTITypeBase* childRtti = nullptr;

							if(arrayElemData != nullptr)
								childRtti = IReflectable::GetRTTITypeFromTypeId(arrayElemData->GetRootTypeId());

							if(childRtti != nullptr)
							{
								auto findObj = mObjectMap.find(arrayElemData.get());
								if(findObj == mObjectMap.end())
								{
									SPtr<IReflectable> newObject = childRtti->NewRttiObject();
									findObj = mObjectMap.insert(std::make_pair(arrayElemData.get(), ObjectDeserializationData(newObject, arrayElemData.get()))).first;
								}

								ObjectDeserializationData& objToDecode = findObj->second;

								bool needsDecoding = !curField->Schema.Info.Flags.IsSet(RTTIFieldFlag::WeakRef) && !objToDecode.IsDeserialized;
								if(needsDecoding)
								{
									if(objToDecode.DeserializationInProgress)
									{
										B3D_LOG(Warning, Generic, "Detected a circular reference when decoding. "
																 "Referenced object's fields will be resolved in an undefined order "
																 "(i.e. one of the objects will not be fully deserialized when assigned "
																 "to its field). Use RTTI_Flag_WeakRef to get rid of this warning and tell "
																 "the system which of the objects is allowed to be deserialized after it "
																 "is assigned to its field.");
									}
									else
									{
										objToDecode.DeserializationInProgress = true;
										DeserializeReflectableObject(objToDecode.Object, objToDecode.SerializedObject);
										objToDecode.DeserializationInProgress = false;
										objToDecode.IsDeserialized = true;
									}
								}

								curField->SetArrayValue(rttiInstance, object.get(), arrayElem.first, objToDecode.Object);
							}
							else
							{
								curField->SetArrayValue(rttiInstance, object.get(), arrayElem.first, nullptr);
							}
						}
					}
					break;
				case SerializableFT_Reflectable:
					{
						auto* curField = static_cast<RTTIReflectableFieldBase*>(curGenericField);

						for(auto& arrayElem : arrayData->Entries)
						{
							SPtr<SerializedObject> arrayElemData = std::static_pointer_cast<SerializedObject>(arrayElem.second.Serialized);
							RTTITypeBase* childRtti = nullptr;

							if(arrayElemData != nullptr)
								childRtti = IReflectable::GetRTTITypeFromTypeId(arrayElemData->GetRootTypeId());

							if(childRtti != nullptr)
							{
								SPtr<IReflectable> newObject = childRtti->NewRttiObject();
								DeserializeReflectableObject(newObject, arrayElemData.get());
								curField->SetArrayValue(rttiInstance, object.get(), arrayElem.first, *newObject);
							}
						}
						break;
					}
				case SerializableFT_Plain:
					{
						auto* curField = static_cast<RTTIPlainFieldBase*>(curGenericField);

						for(auto& arrayElem : arrayData->Entries)
						{
							SPtr<SerializedField> fieldData = std::static_pointer_cast<SerializedField>(arrayElem.second.Serialized);
							if(fieldData != nullptr)
							{
								Bitstream tempStream(fieldData->Value, fieldData->Size);
								curField->ArrayElemFromBuffer(rttiInstance, object.get(), arrayElem.first, tempStream);
							}
						}
					}
					break;
				default:
					break;
				}
			}
			else
			{
				switch(curGenericField->Schema.Type)
				{
				case SerializableFT_ReflectablePtr:
					{
						auto* curField = static_cast<RTTIReflectablePtrFieldBase*>(curGenericField);

						SPtr<SerializedObject> fieldObjectData = std::static_pointer_cast<SerializedObject>(entryData);
						RTTITypeBase* childRtti = nullptr;

						if(fieldObjectData != nullptr)
							childRtti = IReflectable::GetRTTITypeFromTypeId(fieldObjectData->GetRootTypeId());

						if(childRtti != nullptr)
						{
							auto findObj = mObjectMap.find(fieldObjectData.get());
							if(findObj == mObjectMap.end())
							{
								SPtr<IReflectable> newObject = childRtti->NewRttiObject();
								findObj = mObjectMap.insert(std::make_pair(fieldObjectData.get(), ObjectDeserializationData(newObject, fieldObjectData.get()))).first;
							}

							ObjectDeserializationData& objToDecode = findObj->second;

							bool needsDecoding = !curField->Schema.Info.Flags.IsSet(RTTIFieldFlag::WeakRef) && !objToDecode.IsDeserialized;
							if(needsDecoding)
							{
								if(objToDecode.DeserializationInProgress)
								{
									B3D_LOG(Warning, Generic, "Detected a circular reference when decoding. Referenced "
															 "object's fields will be resolved in an undefined order (i.e. one of the "
															 "objects will not be fully deserialized when assigned to its field). "
															 "Use RTTI_Flag_WeakRef to get rid of this warning and tell the system which of"
															 "the objects is allowed to be deserialized after it is assigned to its field.");
								}
								else
								{
									objToDecode.DeserializationInProgress = true;
									DeserializeReflectableObject(objToDecode.Object, objToDecode.SerializedObject);
									objToDecode.DeserializationInProgress = false;
									objToDecode.IsDeserialized = true;
								}
							}

							curField->SetValue(rttiInstance, object.get(), objToDecode.Object);
						}
						else
						{
							curField->SetValue(rttiInstance, object.get(), nullptr);
						}
					}
					break;
				case SerializableFT_Reflectable:
					{
						auto* curField = static_cast<RTTIReflectableFieldBase*>(curGenericField);

						SPtr<SerializedObject> fieldObjectData = std::static_pointer_cast<SerializedObject>(entryData);
						RTTITypeBase* childRtti = nullptr;

						if(fieldObjectData != nullptr)
							childRtti = IReflectable::GetRTTITypeFromTypeId(fieldObjectData->GetRootTypeId());

						if(childRtti != nullptr)
						{
							SPtr<IReflectable> newObject = childRtti->NewRttiObject();
							DeserializeReflectableObject(newObject, fieldObjectData.get());
							curField->SetValue(rttiInstance, object.get(), *newObject);
						}
						break;
					}
				case SerializableFT_Plain:
					{
						auto* curField = static_cast<RTTIPlainFieldBase*>(curGenericField);

						SPtr<SerializedField> fieldData = std::static_pointer_cast<SerializedField>(entryData);
						if(fieldData != nullptr)
						{
							Bitstream tempStream(fieldData->Value, fieldData->Size);
							curField->FromBuffer(rttiInstance, object.get(), tempStream);
						}
					}
					break;
				case SerializableFT_DataBlock:
					{
						auto* curField = static_cast<RTTIManagedDataBlockFieldBase*>(curGenericField);

						SPtr<SerializedDataBlock> fieldData = std::static_pointer_cast<SerializedDataBlock>(entryData);
						if(fieldData != nullptr)
						{
							fieldData->Stream->Seek(fieldData->Offset);
							curField->SetValue(rttiInstance, object.get(), fieldData->Stream, fieldData->Size);
						}

						break;
					}
				}
			}
		}
	}

	while(!rttiInstances.empty())
	{
		RTTITypeBase* rttiInstance = rttiInstances.top();
		rttiInstance->OnDeserializationEnded(object.get(), mContext);
		mAlloc->Destruct(rttiInstance);

		rttiInstances.pop();
	}
}

SPtr<SerializedObject> IntermediateSerializer::SerializeReflectableObject(const IReflectable& object, SerializedObjectEncodeFlags flags, SerializationContext* context, FrameAllocator& allocator)
{
	FrameStack<RTTITypeBase*> rttiInstances;
	RTTITypeBase* rtti = object.GetRtti();

	const auto cleanup = [&]()
	{
		while(!rttiInstances.empty())
		{
			RTTITypeBase* rttiInstance = rttiInstances.top();
			rttiInstance->OnSerializationEnded(const_cast<IReflectable*>(&object), context);
			allocator.Destruct(rttiInstance);

			rttiInstances.pop();
		}
	};

	bool replicableOnly = flags.IsSet(SerializedObjectEncodeFlag::ReplicableOnly);
	SPtr<SerializedObject> output = B3DMakeShared<SerializedObject>();

	// If an object has base classes, we need to iterate through all of them
	do
	{
		RTTITypeBase* rttiInstance = rtti->CloneInternal(allocator);
		rttiInstances.push(rttiInstance);

		rttiInstance->OnSerializationStarted(const_cast<IReflectable*>(&object), context);

		output->SubObjects.push_back(SerializedSubObject());
		SerializedSubObject& subObject = output->SubObjects.back();
		subObject.TypeId = rtti->GetRttiId();

		const u32 numFields = rtti->GetNumFields();
		for(u32 i = 0; i < numFields; i++)
		{
			RTTIField* curGenericField = rtti->GetField(i);

			if(replicableOnly)
			{
				if(!curGenericField->Schema.Info.Flags.IsSet(RTTIFieldFlag::Replicate))
					continue;
			}

			SPtr<SerializedInstance> serializedEntry = SerializeField(const_cast<IReflectable*>(&object), rttiInstance, curGenericField, (u32)-1, flags, context, &allocator);

			SerializedEntry entry;
			entry.FieldId = curGenericField->Schema.Id;
			entry.Serialized = serializedEntry;

			subObject.Entries.insert(std::make_pair(curGenericField->Schema.Id, entry));
		}

		rtti = rtti->GetBaseClass();
	}
	while(rtti != nullptr); // Repeat until we reach the top of the inheritance hierarchy

	cleanup();

	return output;
}

SPtr<SerializedInstance> IntermediateSerializer::SerializeField(IReflectable* object, RTTITypeBase* rtti, RTTIField* field, u32 arrayIdx, SerializedObjectEncodeFlags flags, SerializationContext* context, FrameAllocator* alloc)
{
	bool shallow = flags.IsSet(SerializedObjectEncodeFlag::Shallow);

	SPtr<SerializedInstance> output;
	if(field->Schema.IsArray)
	{
		const u32 arrayNumElems = field->GetArraySize(rtti, object);
		bool wholeArray = arrayIdx == (u32)-1;

		u32 arrayEndIdx;
		if(wholeArray)
		{
			arrayIdx = 0;
			arrayEndIdx = arrayNumElems;
		}
		else
			arrayEndIdx = arrayIdx + 1;

		SPtr<SerializedArray> serializedArray;
		if(wholeArray)
		{
			serializedArray = B3DMakeShared<SerializedArray>();
			serializedArray->ElementCount = arrayNumElems;

			output = serializedArray;
		}

		switch(field->Schema.Type)
		{
		case SerializableFT_ReflectablePtr:
			{
				auto curField = static_cast<RTTIReflectablePtrFieldBase*>(field);

				for(u32 arrIdx = arrayIdx; arrIdx < arrayEndIdx; arrIdx++)
				{
					SPtr<SerializedObject> serializedChildObj = nullptr;

					if(!shallow)
					{
						SPtr<IReflectable> childObject = curField->GetArrayValue(rtti, object, arrIdx);

						if(childObject)
							serializedChildObj = SerializeReflectableObject(*childObject, flags, context, *alloc);
					}

					if(wholeArray)
					{
						SerializedArrayEntry arrayEntry;
						arrayEntry.Serialized = serializedChildObj;
						arrayEntry.Index = arrIdx;

						serializedArray->Entries[arrIdx] = arrayEntry;
					}
					else
						output = serializedChildObj;
				}

				break;
			}
		case SerializableFT_Reflectable:
			{
				auto curField = static_cast<RTTIReflectableFieldBase*>(field);

				for(u32 arrIdx = 0; arrIdx < arrayNumElems; arrIdx++)
				{
					IReflectable& childObject = curField->GetArrayValue(rtti, object, arrIdx);

					const SPtr<SerializedObject> serializedChildObj = SerializeReflectableObject(childObject, flags, context, *alloc);

					if(wholeArray)
					{
						SerializedArrayEntry arrayEntry;
						arrayEntry.Serialized = serializedChildObj;
						arrayEntry.Index = arrIdx;

						serializedArray->Entries[arrIdx] = arrayEntry;
					}
					else
						output = serializedChildObj;
				}

				break;
			}
		case SerializableFT_Plain:
			{
				auto curField = static_cast<RTTIPlainFieldBase*>(field);

				for(u32 arrIdx = 0; arrIdx < arrayNumElems; arrIdx++)
				{
					u32 typeSize = 0;
					if(curField->Schema.HasDynamicSize)
						typeSize = curField->GetArrayElemDynamicSize(rtti, object, arrIdx, false).Bytes;
					else
						typeSize = curField->Schema.Size.Bytes;

					const auto serializedField = B3DMakeShared<SerializedField>();
					serializedField->Value = (u8*)B3DAllocate(typeSize);
					serializedField->OwnsMemory = true;
					serializedField->Size = typeSize;

					Bitstream tempStream(serializedField->Value, typeSize);
					curField->ArrayElemToStream(rtti, object, arrIdx, tempStream);

					if(wholeArray)
					{
						SerializedArrayEntry arrayEntry;
						arrayEntry.Serialized = serializedField;
						arrayEntry.Index = arrIdx;

						serializedArray->Entries[arrIdx] = arrayEntry;
					}
					else
						output = serializedField;
				}

				break;
			}
		default:
			B3D_EXCEPT(InternalErrorException, "Error encoding data. Encountered a type I don't know how to encode. Type: " + ToString(u32(field->Schema.Type)) + ", Is array: " + ToString(field->Schema.IsArray));
		}
	}
	else
	{
		switch(field->Schema.Type)
		{
		case SerializableFT_ReflectablePtr:
			{
				auto curField = static_cast<RTTIReflectablePtrFieldBase*>(field);

				if(!shallow)
				{
					SPtr<IReflectable> childObject = curField->GetValue(rtti, object);

					if(childObject)
						output = SerializeReflectableObject(*childObject, flags, context, *alloc);
				}

				break;
			}
		case SerializableFT_Reflectable:
			{
				auto curField = static_cast<RTTIReflectableFieldBase*>(field);
				IReflectable& childObject = curField->GetValue(rtti, object);

				output = SerializeReflectableObject(childObject, flags, context, *alloc);

				break;
			}
		case SerializableFT_Plain:
			{
				auto curField = static_cast<RTTIPlainFieldBase*>(field);

				u32 typeSize = 0;
				if(curField->Schema.HasDynamicSize)
					typeSize = curField->GetDynamicSize(rtti, object, false).Bytes;
				else
					typeSize = curField->Schema.Size.Bytes;

				const auto serializedField = B3DMakeShared<SerializedField>();
				serializedField->Value = (u8*)B3DAllocate(typeSize);
				serializedField->OwnsMemory = true;
				serializedField->Size = typeSize;

				Bitstream tempStream(serializedField->Value, typeSize);
				curField->ToStream(rtti, object, tempStream);

				output = serializedField;

				break;
			}
		case SerializableFT_DataBlock:
			{
				auto curField = static_cast<RTTIManagedDataBlockFieldBase*>(field);

				u32 dataBlockSize = 0;
				SPtr<DataStream> blockStream = curField->GetValue(rtti, object, dataBlockSize);

				SPtr<MemoryDataStream> stream = B3DMakeShared<MemoryDataStream>(dataBlockSize);
				blockStream->Read(stream->Data(), dataBlockSize);

				SPtr<SerializedDataBlock> serializedDataBlock = B3DMakeShared<SerializedDataBlock>();
				serializedDataBlock->Stream = stream;
				serializedDataBlock->Offset = 0;

				serializedDataBlock->Size = dataBlockSize;
				output = serializedDataBlock;

				break;
			}
		default:
			B3D_EXCEPT(InternalErrorException, "Error encoding data. Encountered a type I don't know how to encode. Type: " + ToString(u32(field->Schema.Type)) + ", Is array: " + ToString(field->Schema.IsArray));
		}
	}

	return output;
}

SPtr<SerializedInstance> IntermediateSerializer::SerializeField(IReflectable& object, RTTITypeBase& rttiType, RTTIIteratorField& field, SerializedObjectEncodeFlags flags, SerializationContext* context, FrameAllocator& allocator)
{
	const bool shallow = flags.IsSet(SerializedObjectEncodeFlag::Shallow);
	const bool wholeArray = true;

	SPtr<SerializedInstance> output;
	if(field.Schema.IsArray)
	{
		if(!B3D_ENSURE(field.Schema.IsIterator))
			return nullptr;

		const SPtr<IRTTIIterator> iterator = field.GetIterator(&rttiType, &object, allocator);

		SPtr<SerializedArray> serializedArray;
		if(wholeArray && iterator != nullptr)
		{
			serializedArray = B3DMakeShared<SerializedArray>();
			serializedArray->ElementCount = (u32)iterator->GetElementCount();

			output = serializedArray;
		}

		if(iterator != nullptr)
		{
			for(u32 arrayIndex = 0; iterator->IsValid(); iterator->Increment(), arrayIndex++)
			{
				const void* fieldValue = iterator->GetValue();
				for(u32 typeIndex = 0; typeIndex < (u32)field.Schema.FieldTypes.Size(); ++typeIndex)
				{
					// TODO - Ignoring type index


					const RTTIFieldTypeSchema& typeSchema = field.Schema.FieldTypes[typeIndex];
					switch(typeSchema.Type)
					{
					case SerializableFT_ReflectablePtr:
						{
							SPtr<SerializedObject> serializedReferencedObject = nullptr;

							if(!shallow)
							{
								const SPtr<IReflectable> referencedObject = field.GetReflectablePointer(fieldValue, typeIndex);

								if(referencedObject)
									serializedReferencedObject = SerializeReflectableObject(*referencedObject, flags, context, allocator);
							}

							if(wholeArray)
							{
								SerializedArrayEntry arrayEntry;
								arrayEntry.Serialized = serializedReferencedObject;
								arrayEntry.Index = arrayIndex;

								serializedArray->Entries[arrayIndex] = arrayEntry;
							}
							else
								output = serializedReferencedObject;

							break;
						}
					case SerializableFT_Reflectable:
						{
							const IReflectable& inlineObject = field.GetReflectable(fieldValue, typeIndex);
							const SPtr<SerializedObject> serializedInlineObject = SerializeReflectableObject(inlineObject, flags, context, allocator);

							if(wholeArray)
							{
								SerializedArrayEntry arrayEntry;
								arrayEntry.Serialized = serializedInlineObject;
								arrayEntry.Index = arrayIndex;

								serializedArray->Entries[arrayIndex] = arrayEntry;
							}
							else
								output = serializedInlineObject;

							break;
						}
					case SerializableFT_Plain:
						{
							u32 typeSize = 0;
							if(typeSchema.HasDynamicSize)
								typeSize = curField->GetArrayElemDynamicSize(rtti, object, arrIdx, false).Bytes;
							else
								typeSize = typeSchema.FixedSize.Bytes;

							const auto serializedField = B3DMakeShared<SerializedField>();
							serializedField->Value = (u8*)B3DAllocate(typeSize);
							serializedField->OwnsMemory = true;
							serializedField->Size = typeSize;

							Bitstream tempStream(serializedField->Value, typeSize);
							field.WritePlainTypeTupleToStream(fieldValue, typeIndex, tempStream, false);

							if(wholeArray)
							{
								SerializedArrayEntry arrayEntry;
								arrayEntry.Serialized = serializedField;
								arrayEntry.Index = arrayIndex;

								serializedArray->Entries[arrayIndex] = arrayEntry;
							}
							else
								output = serializedField;

							break;
						}
					default:
						B3D_LOG(Error, Serialization, "Error serializing data. Encountered a type I don't know how to encode. Type: {0}, Is array: {1}", typeSchema.Type, field.Schema.IsArray);
					}
				}

				if(!wholeArray)
					break;
			}
		}
	}
	else
	{
		B3D_ENSURE(false); // Non-container fields not supported yet. Use the other SerializeField overload.
	}

	return output;
}
