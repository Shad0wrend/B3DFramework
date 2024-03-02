//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Serialization/BsBinarySerializer.h"

#include "Error/BsException.h"
#include "Debug/BsDebug.h"
#include "Reflection/BsIReflectable.h"
#include "Reflection/BsRTTIType.h"
#include "Reflection/BsRTTIField.h"
#include "Reflection/BsRTTIPlainField.h"
#include "Reflection/BsRTTIReflectableField.h"
#include "Reflection/BsRTTIReflectablePtrField.h"
#include "Reflection/BsRTTIManagedDataBlockField.h"
#include "FileSystem/BsDataStream.h"
#include "Reflection/BsRTTIIteratorField.h"
#include "Utility/BsBufferedBitstream.h"

using namespace bs;

constexpr u32 BinarySerializer::kReportAfterBytes;
constexpr u32 BinarySerializer::kWriteBufferSize;
constexpr u32 BinarySerializer::kFlushAfterBytes;
constexpr u32 BinarySerializer::kPreloadChunkBytes;

/** Encoding used for storing either fixed field size, or additional field information. */
union FieldTypeSizeOrExtendedMetaData
{
	u8 FixedSize;

	struct
	{
		u8 Unused : 4;
		u8 BuiltinTypeId : 4;
	};
};

/** Encoding used for storing information about a type in a RTTI field. */
union FieldTypeMetaData
{
	/** Creates the field meta-data from the type schema. */
	static FieldTypeMetaData Create(const RTTIFieldSchema& fieldSchema, const RTTIFieldTypeSchema& fieldTypeSchema, bool isLastFieldInType, bool isAnotherFieldTypeFollowing);

	/** Converts the internal data to RTTIFieldTypeSchema. */
	RTTIFieldTypeSchema ToFieldTypeSchema(bool& hasMoreTypesFollowing) const;

	u16 PackedData;
	struct
	{
		u8 IsObjectDescriptor : 1; /**< If 1, meta-data represents an object rather than the field. Rest of the data is invalid in such case. */
		u8 IsArray : 1; /**< Field contains multiple entries that are individually serialized. */
		u8 IsDataBlockOrTypeFollowing : 1; /**< If IsArray = 0, this signifies the field holds the DataBlock type. If IsArray = 1, the signifies that another field type meta-data structure follows this one. */
		u8 IsReflectable : 1; /**< Field contains a Reflectable type. */
		u8 IsReflectablePointer : 1; /**< Field contains a Reflectable pointer type. */
		u8 HasDynamicSize : 1; /**< Field has size encoded right after the field entry, rather as part of the field meta-data. */
		u8 IsLastFieldInType : 1; /**< Field is the last field in the type. Only set for the first provided field type, unused in rest. */
		u8 IsExtended : 1; /**< Fields with this flag contain additional information rather than fixed size, in the AdditionalData field. */
		FieldTypeSizeOrExtendedMetaData FixedSizeOrAdditionalData; /** Contains fixed field size if IsExtended = 0 && HasDynamicSize == 0. Otherwise contains additional information about the field. */
	};
};


FieldTypeMetaData FieldTypeMetaData::Create(const RTTIFieldSchema& fieldSchema, const RTTIFieldTypeSchema& fieldTypeSchema, bool isLastFieldInType, bool isAnotherFieldTypeFollowing)
{
	FieldTypeMetaData output;
	output.IsObjectDescriptor = 0;

	B3D_ASSERT(fieldTypeSchema.Type != SerializableFT_DataBlock || !fieldSchema.IsArray);
	output.IsArray = fieldSchema.IsArray;
	output.IsReflectable = fieldTypeSchema.Type == SerializableFT_Reflectable;

	B3D_ASSERT(fieldTypeSchema.Type != SerializableFT_DataBlock || !isAnotherFieldTypeFollowing);
	output.IsDataBlockOrTypeFollowing = fieldTypeSchema.Type == SerializableFT_DataBlock || isAnotherFieldTypeFollowing;
	output.IsReflectablePointer = fieldTypeSchema.Type == SerializableFT_ReflectablePtr;
	output.HasDynamicSize = fieldTypeSchema.HasDynamicSize;
	output.IsLastFieldInType = isLastFieldInType;

	const u32 fieldTypeId = fieldTypeSchema.FieldTypeId;
	const bool isBuiltin = false; // Not supported at the moment. This was never properly implemented. fieldTypeId < 16;
	if(isBuiltin)
	{
		output.IsExtended = 1;
		output.FixedSizeOrAdditionalData.Unused = 0;
		output.FixedSizeOrAdditionalData.BuiltinTypeId = fieldTypeId & 0xF;
	}
	else
	{
		output.IsExtended = 0;
		output.FixedSizeOrAdditionalData.FixedSize = fieldTypeSchema.HasDynamicSize ? 0 : fieldTypeSchema.FixedSize.Bytes;
	}

	return output;
}

RTTIFieldTypeSchema FieldTypeMetaData::ToFieldTypeSchema(bool& hasMoreTypesFollowing) const
{
	RTTIFieldTypeSchema schema;
	schema.HasDynamicSize = HasDynamicSize;

	if(IsReflectablePointer)
		schema.Type = SerializableFT_ReflectablePtr;
	else if(IsReflectable)
		schema.Type = SerializableFT_Reflectable;
	else if(IsDataBlockOrTypeFollowing && !IsArray)
		schema.Type = SerializableFT_DataBlock;
	else
		schema.Type = SerializableFT_Plain;

	hasMoreTypesFollowing = IsDataBlockOrTypeFollowing && IsArray;

	if(!IsExtended)
		schema.FixedSize = FixedSizeOrAdditionalData.FixedSize;
	else
		schema.FieldTypeId = FixedSizeOrAdditionalData.BuiltinTypeId;

	return schema;
}

BinarySerializer::BinarySerializer()
	: mAlloc(&GetFrameAllocator())
{}

void BinarySerializer::Encode(IReflectable* object, const SPtr<DataStream>& stream, BinarySerializerFlags flags, SerializationContext* context)
{
	mReflectableObjectsToSerialize.clear();
	mReflectableObjectToID.clear();
	mLastUsedObjectId = 1;
	mContext = context;
	mBuffer.Seek(0);

	mAlloc->MarkFrame();

	BufferedBitstreamWriter bufferedStream(&mBuffer, stream, kWriteBufferSize, kFlushAfterBytes);

	Vector<SPtr<IReflectable>> encodedObjects;
	u32 objectId = FindOrCreateReflectableObjectId(object);

	// Encode primary object and its value types
	if(!SerializeReflectableObject(object, objectId, bufferedStream, flags))
	{
		B3D_LOG(Error, Serialization, "Destination buffer is null or not large enough.");
		return;
	}

	// Encode pointed to objects and their value types
	UnorderedSet<u32> serializedObjects;
	while(true)
	{
		auto iter = mReflectableObjectsToSerialize.begin();
		bool foundObjectToProcess = false;
		for(; iter != mReflectableObjectsToSerialize.end(); ++iter)
		{
			auto foundExisting = serializedObjects.find(iter->ObjectId);
			if(foundExisting != serializedObjects.end())
				continue; // Already processed

			SPtr<IReflectable> curObject = iter->Object;
			u32 curObjectid = iter->ObjectId;
			serializedObjects.insert(curObjectid);
			mReflectableObjectsToSerialize.erase(iter); // TODO - Should update iter to returned value from erase()

			if(!SerializeReflectableObject(curObject.get(), curObjectid, bufferedStream, flags))
			{
				B3D_LOG(Error, Serialization, "Destination buffer is null or not large enough.");
				return;
			}

			foundObjectToProcess = true;

			// Ensure we keep a reference to the object so it isn't released.
			// The system assigns unique IDs to IReflectable objects based on pointer
			// addresses but if objects get released then same address could be assigned twice.
			// Note: To get around this I could assign unique IDs to IReflectable objects
			encodedObjects.push_back(curObject);

			break; // Need to start over as mObjectsToSerialize was possibly modified
		}

		if(!foundObjectToProcess) // We're done
			break;
	}

	bufferedStream.Flush(true);

	encodedObjects.clear();
	mReflectableObjectsToSerialize.clear();
	mReflectableObjectToID.clear();

	mAlloc->Clear();
}

SPtr<IReflectable> BinarySerializer::Decode(const SPtr<DataStream>& stream, u32 dataLength, BinarySerializerFlags flags, SerializationContext* context, std::function<void(float)> progress, SPtr<RTTISchema> schema)
{
	mContext = context;
	mReportProgress = nullptr;
	mTotalBytesToRead = dataLength;
	mBuffer.Seek(0);

	if(dataLength == 0)
	{
		if(mReportProgress)
			mReportProgress(1.0f);

		return nullptr;
	}

	const size_t start = stream->Tell();
	const size_t end = start + dataLength;
	const size_t endBits = end * 8;
	mDecodeObjectMap.clear();

	bool hasMeta = !flags.IsSet(BinarySerializerFlag::NoMeta);
	bool compress = flags.IsSet(BinarySerializerFlag::Compress);

	// Don't need a schema if we have meta-data
	if(hasMeta)
		schema = nullptr;
	else
	{
		if(!schema)
		{
			B3D_LOG(Error, Serialization, "Cannot decode an object without meta-data nor schema.");
			return nullptr;
		}
	}

	BufferedBitstreamReader bufferedStream(&mBuffer, stream, kPreloadChunkBytes, kFlushAfterBytes);

	// Note: Ideally we can avoid iterating twice over the stream data
	// We need to find offsets at which all objects start at so we can map object id to offset
	u32 rootObjectId = (u32)-1;
	SPtr<RTTISchema> curSchema = schema;
	do
	{
		bool isRoot = rootObjectId == (u32)-1;

		u32 objectId = 0;
		u32 objectTypeId = 0;
		bool objectIsBaseClass = false;

		u32 bitsRead = ReadObjectMetaData(bufferedStream, flags, objectId, objectTypeId, objectIsBaseClass);
		bufferedStream.Skip(-(int64_t)bitsRead);

		if(objectIsBaseClass)
		{
			B3D_EXCEPT(InternalErrorException, "Encountered a base-class object while looking for a new object. "
											  "Base class objects are only supposed to be parts of a larger object.");
		}

		if(curSchema)
			objectTypeId = curSchema->TypeId;

		if(isRoot)
		{
			SPtr<IReflectable> object = IReflectable::CreateInstanceFromTypeId(objectTypeId);
			mDecodeObjectMap.insert(std::make_pair(objectId, ObjectToDecode(object, bufferedStream.Tell(), curSchema)));
		}
		else
		{
			if(hasMeta)
			{
				SPtr<IReflectable> object = IReflectable::CreateInstanceFromTypeId(objectTypeId);
				mDecodeObjectMap.insert(std::make_pair(objectId, ObjectToDecode(object, bufferedStream.Tell(), curSchema)));
			}
			else
			{
				// If no meta, it's expected the pass over the root object has populated mDecodeObjectMap with object instances
				// as well as references to the schema
				auto iterFind = mDecodeObjectMap.find(objectId);
				B3D_ASSERT(iterFind != mDecodeObjectMap.end());

				ObjectToDecode& objectToDecode = iterFind->second;
				objectToDecode.Offset = bufferedStream.Tell();

				curSchema = objectToDecode.Schema;
				objectTypeId = curSchema->TypeId;
			}
		}

		if(isRoot)
			rootObjectId = objectId;
	}
	while(DecodeEntry(bufferedStream, endBits, flags, nullptr, curSchema));

	B3D_ASSERT(bufferedStream.Tell() == endBits);

	// Don't set report callback until we actually do the reads
	mReportProgress = std::move(progress);
	bufferedStream.Seek((uint64_t)start * 8);

	// Now actually decode the objects
	ObjectToDecode& rootObjectToDecode = mDecodeObjectMap[rootObjectId];
	SPtr<IReflectable> rootObject = rootObjectToDecode.Object;

	rootObjectToDecode.DecodeInProgress = true;
	DecodeEntry(bufferedStream, endBits, flags, rootObject, schema);
	rootObjectToDecode.DecodeInProgress = false;
	rootObjectToDecode.IsDecoded = true;

	mDecodeObjectMap.clear();
	bufferedStream.Seek((uint64_t)endBits);
	stream->Seek(end);

	B3D_ASSERT(bufferedStream.Tell() == endBits);

	if(mReportProgress)
		mReportProgress(1.0f);

	return rootObject;
}

bool BinarySerializer::SerializeReflectableObject(IReflectable* object, u32 objectId, BufferedBitstreamWriter& stream, BinarySerializerFlags flags)
{
	const bool writeMeta = !flags.IsSet(BinarySerializerFlag::NoMeta);
	const bool compress = flags.IsSet(BinarySerializerFlag::Compress);

	RTTITypeBase* rtti = object->GetRtti();
	bool isBaseClass = false;

	FrameStack<RTTITypeBase*> rttiInstances;

	const auto cleanup = [&]() // TODO - Use scope guard
	{
		while(!rttiInstances.empty())
		{
			RTTITypeBase* rttiInstance = rttiInstances.top();
			rttiInstance->OnSerializationEnded(object, mContext);
			mAlloc->Destruct(rttiInstance);

			rttiInstances.pop();
		}
	};

	// If an object has base classes, we need to iterate through all of them
	do
	{
		RTTITypeBase* rttiInstance = rtti->CloneInternal(*mAlloc);
		rttiInstances.push(rttiInstance);

		rttiInstance->OnSerializationStarted(object, mContext);

		if(writeMeta)
		{
			// Encode object ID & type
			ObjectMetaData objectMetaData = EncodeObjectMetaData(objectId, rtti->GetRttiId(), isBaseClass);
			stream.WriteBytes(objectMetaData);
		}
		else
		{
			// Encode object ID
			u32 objectMetaData = EncodeObjectMetaData(objectId, isBaseClass);

			if(compress)
				stream.WriteVarInt(objectMetaData);
			else
				stream.WriteBytes(objectMetaData);
		}

		const u32 numFields = rtti->GetNumFields();
		for(u32 i = 0; i < numFields; i++)
		{
			RTTIField* curGenericField = rtti->GetField(i);

			if(writeMeta)
			{
				// Copy field ID & other meta-data like field size and type
				WriteFieldMetaData(curGenericField->Schema, false, stream);
			}

			const bool isIteratorField = false; // TODO - Debug only, this should be grabbed from the schema
			if(isIteratorField)
			{
				FrameAllocator frameAllocator; // TODO - Provide one as a parameter
				RTTIIteratorField* const field = static_cast<RTTIIteratorField*>(curGenericField);
				UPtr<IRTTIIterator> iterator = field->GetIterator(rttiInstance, object, frameAllocator);

				if(field->Schema.IsArray)
				{
					u32 elementCount = 0;
					if(iterator != nullptr)
						elementCount = (u32)iterator->GetElementCount();

					// Copy num vector elements
					if(compress)
						stream.WriteVarInt(elementCount);
					else
						stream.WriteBytes(elementCount);
				}

				for(; iterator->IsValid(); iterator->Increment())
				{
					const void* fieldValue = iterator->GetValue();
					for(u32 typeIndex = 0; typeIndex < (u32)field->Schema.FieldTypes.Size(); ++typeIndex)
					{
						const RTTIFieldTypeSchema& typeSchema = field->Schema.FieldTypes[typeIndex];
						switch(typeSchema.Type)
						{
						case SerializableFT_ReflectablePtr:
						{
							SPtr<IReflectable> childObject;

							if(!flags.IsSet(BinarySerializerFlag::Shallow))
								childObject = field->GetReflectablePointer(fieldValue, typeIndex);

							const u32 objectId = RegisterReflectableObjectForSerialization(childObject);
							if(compress)
								stream.WriteVarInt(objectId);
							else
								stream.WriteBytes(objectId);

							break;
						}
						case SerializableFT_Reflectable:
						{
							const IReflectable& childObject = field->GetReflectable(fieldValue, typeIndex);
							if(!SerializeReflectableObjectInline(const_cast<IReflectable*>(&childObject), stream, flags)) // TODO - Get rid of const cast
							{
								cleanup();
								return false;
							}
							
							break;
						}
						case SerializableFT_Plain:
						{
							field->WritePlainTypeTupleToStream(fieldValue, typeIndex, stream.GetBitstream(), compress);
							break;
						}
						default:
						B3D_LOG(Error, Serialization, "Error serializing data. Encountered a type I don't know how to encode. Type: {0}, Is array: {1}", typeSchema.Type, field->Schema.IsArray);
						}
					}
				}
			}
			else if(curGenericField->Schema.IsArray)
			{
				u32 arrayNumElems = curGenericField->GetArraySize(rttiInstance, object);

				// Copy num vector elements
				if(compress)
					stream.WriteVarInt(arrayNumElems);
				else
					stream.WriteBytes(arrayNumElems);

				switch(curGenericField->Schema.Type)
				{
				case SerializableFT_ReflectablePtr:
					{
						auto* curField = static_cast<RTTIReflectablePtrFieldBase*>(curGenericField);

						for(u32 arrIdx = 0; arrIdx < arrayNumElems; arrIdx++)
						{
							SPtr<IReflectable> childObject;

							if(!flags.IsSet(BinarySerializerFlag::Shallow))
								childObject = curField->GetArrayValue(rttiInstance, object, arrIdx);

							u32 objId = RegisterReflectableObjectForSerialization(childObject);
							if(compress)
								stream.WriteVarInt(objId);
							else
								stream.WriteBytes(objId);
						}

						break;
					}
				case SerializableFT_Reflectable:
					{
						auto* curField = static_cast<RTTIReflectableFieldBase*>(curGenericField);

						for(u32 arrIdx = 0; arrIdx < arrayNumElems; arrIdx++)
						{
							IReflectable& childObject = curField->GetArrayValue(rttiInstance, object, arrIdx);

							if(!SerializeReflectableObjectInline(&childObject, stream, flags))
							{
								cleanup();
								return false;
							}
						}

						break;
					}
				case SerializableFT_Plain:
					{
						auto* curField = static_cast<RTTIPlainFieldBase*>(curGenericField);

						for(u32 arrIdx = 0; arrIdx < arrayNumElems; arrIdx++)
							curField->ArrayElemToStream(rttiInstance, object, arrIdx, stream.GetBitstream(), compress);

						break;
					}
				default:
					B3D_LOG(Error, Serialization, "Error encoding data. Encountered a type I don't know how to encode. Type: {0}, Is array: {1}", curGenericField->Schema.Type, curGenericField->Schema.IsArray);
				}
			}
			else
			{
				switch(curGenericField->Schema.Type)
				{
				case SerializableFT_ReflectablePtr:
					{
						auto* curField = static_cast<RTTIReflectablePtrFieldBase*>(curGenericField);
						SPtr<IReflectable> childObject;

						if(!flags.IsSet(BinarySerializerFlag::Shallow))
							childObject = curField->GetValue(rttiInstance, object);

						u32 objId = RegisterReflectableObjectForSerialization(childObject);
						if(compress)
							stream.WriteVarInt(objId);
						else
							stream.WriteBytes(objId);

						break;
					}
				case SerializableFT_Reflectable:
					{
						auto* curField = static_cast<RTTIReflectableFieldBase*>(curGenericField);
						IReflectable& childObject = curField->GetValue(rttiInstance, object);

						if(!SerializeReflectableObjectInline(&childObject, stream, flags))
						{
							cleanup();
							return false;
						}

						break;
					}
				case SerializableFT_Plain:
					{
						auto* curField = static_cast<RTTIPlainFieldBase*>(curGenericField);
						curField->ToStream(rttiInstance, object, stream.GetBitstream(), compress);

						break;
					}
				case SerializableFT_DataBlock:
					{
						auto* curField = static_cast<RTTIManagedDataBlockFieldBase*>(curGenericField);

						u32 dataBlockSize = 0;
						SPtr<DataStream> blockStream = curField->GetValue(rttiInstance, object, dataBlockSize);

						// Data block size
						if(compress)
							stream.WriteVarInt(dataBlockSize);
						else
							stream.WriteBytes(dataBlockSize);

						// Data block data
						auto dataToStore = (u8*)B3DStackAllocate(dataBlockSize);
						blockStream->Read(dataToStore, dataBlockSize);

						stream.Align();
						stream.WriteBytes(dataToStore, dataBlockSize);
						B3DStackFree(dataToStore);

						break;
					}
				default:
					B3D_LOG(Error, Serialization, "Error encoding data. Encountered a type I don't know how to encode. Type: {0}, Is array: {1}", curGenericField->Schema.Type, curGenericField->Schema.IsArray);
				}
			}

			stream.Flush(false);
		}

		rtti = rtti->GetBaseClass();
		isBaseClass = true;
	}
	while(rtti != nullptr); // Repeat until we reach the top of the inheritance hierarchy

	cleanup();

	return true;
}

bool BinarySerializer::DecodeEntry(BufferedBitstreamReader& stream, size_t dataEnd, BinarySerializerFlags flags, const SPtr<IReflectable>& output, SPtr<RTTISchema> schema)
{
	const bool hasMeta = !flags.IsSet(BinarySerializerFlag::NoMeta);
	const bool compressed = flags.IsSet(BinarySerializerFlag::Compress);

	u32 objectId = 0;
	u32 objectTypeId = 0;
	bool objectIsBaseClass = false;

	ReadObjectMetaData(stream, flags, objectId, objectTypeId, objectIsBaseClass);

	if(schema)
		objectTypeId = schema->TypeId;

	if(objectIsBaseClass)
	{
		B3D_EXCEPT(InternalErrorException, "Encountered a base-class object while looking for a new object. "
										  "Base class objects are only supposed to be parts of a larger object.");
	}

	RTTITypeBase* rtti = nullptr;
	if(output)
		rtti = output->GetRtti();

	FrameVector<RTTITypeBase*> rttiInstances;

	auto finalizeObject = [&rttiInstances, this](IReflectable* object)
	{
		// Note: It would make sense to finish deserializing derived classes before base classes, but some code
		// depends on the old functionality, so we'll keep it this way
		for(auto iter = rttiInstances.rbegin(); iter != rttiInstances.rend(); ++iter)
		{
			RTTITypeBase* curRTTI = *iter;

			curRTTI->OnDeserializationEnded(object, mContext);
			mAlloc->Destruct(curRTTI);
		}

		rttiInstances.clear();
	};

	RTTITypeBase* curRTTI = rtti;
	while(curRTTI)
	{
		RTTITypeBase* rttiInstance = curRTTI->CloneInternal(*mAlloc);
		rttiInstances.push_back(rttiInstance);

		curRTTI = curRTTI->GetBaseClass();
	}

	// Iterate in reverse to notify base classes before derived classes
	for(auto iter = rttiInstances.rbegin(); iter != rttiInstances.rend(); ++iter)
		(*iter)->OnDeserializationStarted(output.get(), mContext);

	RTTITypeBase* rttiInstance = nullptr;
	u32 rttiInstanceIdx = 0;
	if(!rttiInstances.empty())
		rttiInstance = rttiInstances[0];

	Vector<RTTIFieldSchema>::iterator fieldSchemaIter;
	if(schema)
		fieldSchemaIter = schema->FieldSchemas.begin();

	while(stream.Tell() < dataEnd)
	{
		RTTIFieldSchema fieldSchema;
		SPtr<RTTISchema> fieldTypeSchema;
		bool terminator = false;

		u32 baseObjId = 0;
		u32 baseObjTypeId = 0;
		bool objIsBaseClass = false;

		if(hasMeta)
		{
			u8 metaDataHeader = 0;
			stream.ReadBytes(metaDataHeader);
			stream.SkipBytes(-(int32_t)sizeof(metaDataHeader));

			if(IsObjectMetaData(metaDataHeader)) // We've reached a new object or a base class of the current one
			{
				u32 readBits = ReadObjectMetaData(stream, flags, baseObjId, baseObjTypeId, objIsBaseClass);

				if(!objIsBaseClass)
				{
					// Found new object, we're done
					stream.Skip(-(int64_t)readBits);

					finalizeObject(output.get());
					return true;
				}
			}
			else
			{
				if(compressed)
					terminator = IsFieldTerminator(metaDataHeader);

				if(!terminator)
					fieldSchema = ReadFieldMetaData(stream, terminator);
			}
		}
		else
		{
			if(fieldSchemaIter == schema->FieldSchemas.end())
			{
				// No more fields, move to base type if one exists
				if(schema->BaseTypeSchema)
				{
					schema = schema->BaseTypeSchema;
					fieldSchemaIter = schema->FieldSchemas.begin();

					baseObjTypeId = schema->TypeId;
					objIsBaseClass = true;
				}
				else // Otherwise we're done here
					terminator = true;
			}
			else
			{
				fieldSchema = *fieldSchemaIter;
				fieldTypeSchema = fieldSchema.FieldTypeSchema;

				++fieldSchemaIter;
			}
		}

		if(objIsBaseClass)
		{
			if(rtti != nullptr)
				rtti = rtti->GetBaseClass();

			// Saved and current base classes don't match, so just skip over all that data
			if(rtti == nullptr || rtti->GetRttiId() != baseObjTypeId)
				rtti = nullptr;

			rttiInstance = nullptr;

			if(rtti)
			{
				rttiInstance = rttiInstances[rttiInstanceIdx + 1];
				rttiInstanceIdx++;
			}

			continue;
		}

		if(terminator)
		{
			// We've processed the last field in this object, so return. Although we return false we don't actually know
			// if there is an object following this one. However it doesn't matter since terminator fields are only used
			// for embedded objects that are all processed within this method so we can compensate.
			finalizeObject(output.get());
			return false;
		}

		RTTIField* curGenericField = nullptr;

		if(rtti != nullptr)
			curGenericField = rtti->FindField(fieldSchema.Id);

		if(curGenericField != nullptr)
		{
			if(!fieldSchema.HasDynamicSize && curGenericField->Schema.Size != fieldSchema.Size)
			{
				B3D_EXCEPT(InternalErrorException, "Data type mismatch. Type size stored in file and actual type size don't match. (" + ToString(curGenericField->Schema.Size.Bytes) + " vs. " + ToString(fieldSchema.Size.Bytes) + ")");
			}

			if(curGenericField->Schema.IsArray != fieldSchema.IsArray)
			{
				B3D_EXCEPT(InternalErrorException, "Data type mismatch. One is array, other is a single type.");
			}

			if(curGenericField->Schema.Type != fieldSchema.Type)
			{
				B3D_EXCEPT(InternalErrorException, "Data type mismatch. Field types don't match. " + ToString(u32(curGenericField->Schema.Type)) + " vs. " + ToString(u32(fieldSchema.Type)));
			}
		}

		u32 arrayNumElems = 1;
		if(fieldSchema.IsArray)
		{
			if(compressed)
				stream.ReadVarInt(arrayNumElems);
			else
				stream.ReadBytes(arrayNumElems);

			if(curGenericField != nullptr)
				curGenericField->SetArraySize(rttiInstance, output.get(), arrayNumElems);

			switch(fieldSchema.Type)
			{
			case SerializableFT_ReflectablePtr:
				{
					auto* curField = static_cast<RTTIReflectablePtrFieldBase*>(curGenericField);

					for(u32 i = 0; i < arrayNumElems; i++)
					{
						u32 childObjectId = 0;
						if(compressed)
							stream.ReadVarInt(childObjectId);
						else
							stream.ReadBytes(childObjectId);

						auto findObj = mDecodeObjectMap.find(childObjectId);

						// If reading from schema we need to create object here as we don't know its type during the normal pass
						if(schema)
						{
							if(findObj == mDecodeObjectMap.end())
							{
								SPtr<IReflectable> childObj = IReflectable::CreateInstanceFromTypeId(fieldTypeSchema->TypeId);
								mDecodeObjectMap.insert(std::make_pair(objectId, ObjectToDecode(childObj, (u32)-1, fieldTypeSchema)));
							}
						}

						if(curField != nullptr)
						{
							if(findObj == mDecodeObjectMap.end())
							{
								if(childObjectId != 0)
								{
									B3D_LOG(Warning, Generic, "When deserializing, object ID: {0} was found but no such "
															 "object was contained in the file.",
										   childObjectId);
								}

								curField->SetArrayValue(rttiInstance, output.get(), i, nullptr);
							}
							else
							{
								ObjectToDecode& objToDecode = findObj->second;

								const bool needsDecoding = !curField->Schema.Info.Flags.IsSet(RTTIFieldFlag::WeakRef) && !objToDecode.IsDecoded;
								if(needsDecoding)
								{
									if(objToDecode.DecodeInProgress)
									{
										B3D_LOG(Warning, Generic, "Detected a circular reference when decoding. Referenced "
																 "object's fields will be resolved in an undefined order (i.e. one of the "
																 "objects will not be fully deserialized when assigned to its field). "
																 "Use RTTI_Flag_WeakRef to get rid of this warning and tell the system which of"
																 "the objects is allowed to be deserialized after it is assigned to its field.");
									}
									else
									{
										objToDecode.DecodeInProgress = true;

										const uint64_t curOffset = stream.Tell();
										stream.Seek(objToDecode.Offset);
										DecodeEntry(stream, dataEnd, flags, objToDecode.Object, fieldTypeSchema);
										stream.Seek(curOffset);

										objToDecode.DecodeInProgress = false;
										objToDecode.IsDecoded = true;
									}
								}

								curField->SetArrayValue(rttiInstance, output.get(), i, objToDecode.Object);
							}
						}
					}

					break;
				}
			case SerializableFT_Reflectable:
				{
					auto* curField = static_cast<RTTIReflectableFieldBase*>(curGenericField);

					for(u32 i = 0; i < arrayNumElems; i++)
					{
						SPtr<IReflectable> childObj;
						if(curField)
							childObj = curField->NewObject();

						DecodeEntry(stream, dataEnd, flags, childObj, fieldTypeSchema);

						if(curField != nullptr)
						{
							// Note: Would be nice to avoid this copy by value and decode directly into the field
							curField->SetArrayValue(rttiInstance, output.get(), i, *childObj);
						}
					}
					break;
				}
			case SerializableFT_Plain:
				{
					auto* curField = static_cast<RTTIPlainFieldBase*>(curGenericField);

					for(u32 i = 0; i < arrayNumElems; i++)
					{
						uint64_t typeSizeBits = fieldSchema.Size.GetBits();
						if(fieldSchema.HasDynamicSize)
						{
							if(compressed)
							{
								BitLength typeSize;
								BitLength headerSize = B3DRTTIReadSizeHeader(stream, true, typeSize);
								stream.Skip(-(int64_t)headerSize.GetBits());

								typeSizeBits = typeSize.GetBits();
							}
							else
							{
								uint32_t typeSize;
								stream.ReadBytes(typeSize);
								stream.SkipBytes(-(int32_t)sizeof(uint32_t));

								typeSizeBits = (uint64_t)typeSize * 8;
							}
						}

						if(curField != nullptr)
						{
							stream.Preload((uint32_t)Math::DivideAndRoundUp(typeSizeBits, (uint64_t)8));
							curField->ArrayElemFromBuffer(rttiInstance, output.get(), i, stream.GetBitstream(), compressed);

							stream.Skip(typeSizeBits);
						}
						else
						{
							bool builtin = fieldSchema.FieldTypeId < 16;
							if(compressed && builtin)
								SkipBuiltinType(fieldSchema.FieldTypeId, stream, compressed);
							else
								stream.Skip(typeSizeBits);
						}
					}
					break;
				}
			default:
				B3D_EXCEPT(InternalErrorException, "Error decoding data. Encountered a type I don't know how to decode. Type: " + ToString(u32(fieldSchema.Type)) + ", Is array: " + ToString(fieldSchema.IsArray));
			}
		}
		else
		{
			switch(fieldSchema.Type)
			{
			case SerializableFT_ReflectablePtr:
				{
					auto* curField = static_cast<RTTIReflectablePtrFieldBase*>(curGenericField);

					u32 childObjectId = 0;
					if(compressed)
						stream.ReadVarInt(childObjectId);
					else
						stream.ReadBytes(childObjectId);

					auto findObj = mDecodeObjectMap.find(childObjectId);

					// If reading from schema we need to create object here as we don't know its type during the normal pass
					if(schema)
					{
						if(findObj == mDecodeObjectMap.end())
						{
							SPtr<IReflectable> childObj = IReflectable::CreateInstanceFromTypeId(fieldTypeSchema->TypeId);
							mDecodeObjectMap.insert(std::make_pair(objectId, ObjectToDecode(childObj, (u32)-1, fieldTypeSchema)));
						}
					}

					if(curField != nullptr)
					{
						if(findObj == mDecodeObjectMap.end())
						{
							if(childObjectId != 0)
							{
								B3D_LOG(Warning, Generic, "When deserializing, object ID: {0} was found but no such object "
														 "was contained in the file.",
									   childObjectId);
							}

							curField->SetValue(rttiInstance, output.get(), nullptr);
						}
						else
						{
							ObjectToDecode& objToDecode = findObj->second;

							const bool needsDecoding = !curField->Schema.Info.Flags.IsSet(RTTIFieldFlag::WeakRef) && !objToDecode.IsDecoded;
							if(needsDecoding)
							{
								if(objToDecode.DecodeInProgress)
								{
									B3D_LOG(Warning, Generic, "Detected a circular reference when decoding. Referenced "
															 "object's fields will be resolved in an undefined order (i.e. one of the objects "
															 "will not be fully deserialized when assigned to its field). Use "
															 "RTTI_Flag_WeakRef to get rid of this warning and tell the system which of the "
															 "objects is allowed to be deserialized after it is assigned to its field.");
								}
								else
								{
									objToDecode.DecodeInProgress = true;

									const uint64_t curOffset = stream.Tell();
									stream.Seek(objToDecode.Offset);
									DecodeEntry(stream, dataEnd, flags, objToDecode.Object, fieldTypeSchema);
									stream.Seek(curOffset);

									objToDecode.DecodeInProgress = false;
									objToDecode.IsDecoded = true;
								}
							}

							curField->SetValue(rttiInstance, output.get(), objToDecode.Object);
						}
					}

					break;
				}
			case SerializableFT_Reflectable:
				{
					auto* curField = static_cast<RTTIReflectableFieldBase*>(curGenericField);

					// Note: Ideally we can skip decoding the entry if the field no longer exists
					SPtr<IReflectable> childObj;
					if(curField)
						childObj = curField->NewObject();

					DecodeEntry(stream, dataEnd, flags, childObj, fieldTypeSchema);

					if(curField != nullptr)
					{
						// Note: Would be nice to avoid this copy by value and decode directly into the field
						curField->SetValue(rttiInstance, output.get(), *childObj);
					}

					break;
				}
			case SerializableFT_Plain:
				{
					auto* curField = static_cast<RTTIPlainFieldBase*>(curGenericField);

					uint64_t typeSizeBits = fieldSchema.Size.GetBits();
					if(fieldSchema.HasDynamicSize)
					{
						if(compressed)
						{
							BitLength typeSize;
							BitLength headerSize = B3DRTTIReadSizeHeader(stream, true, typeSize);
							stream.Skip(-(int64_t)headerSize.GetBits());

							typeSizeBits = typeSize.GetBits();
						}
						else
						{
							uint32_t typeSize;
							stream.ReadBytes(typeSize);
							stream.SkipBytes(-(int32_t)sizeof(uint32_t));

							typeSizeBits = (uint64_t)typeSize * 8;
						}
					}

					if(curField != nullptr)
					{
						stream.Preload((uint32_t)Math::DivideAndRoundUp(typeSizeBits, (uint64_t)8));
						curField->FromBuffer(rttiInstance, output.get(), stream.GetBitstream(), compressed);

						stream.Skip(typeSizeBits);
					}
					else
					{
						bool builtin = fieldSchema.FieldTypeId < 16;
						if(compressed && builtin)
							SkipBuiltinType(fieldSchema.FieldTypeId, stream, compressed);
						else
							stream.Skip(typeSizeBits);
					}

					break;
				}
			case SerializableFT_DataBlock:
				{
					auto* curField = static_cast<RTTIManagedDataBlockFieldBase*>(curGenericField);

					// Data block size
					u32 dataBlockSize = 0;
					if(compressed)
						stream.ReadVarInt(dataBlockSize);
					else
						stream.ReadBytes(dataBlockSize);

					stream.Align();

					// Data block data
					if(curField != nullptr)
					{
						const SPtr<DataStream>& dataStream = stream.GetDataStream();
						if(dataStream->IsFile()) // Allow streaming
						{
							uint64_t curOffset = stream.Tell();

							// Data blocks don't support data that isn't byte aligned, but encoder should take care of that
							B3D_ASSERT((curOffset % 8) == 0);
							curOffset /= 8;

							dataStream->Seek(curOffset);
							curField->SetValue(rttiInstance, output.get(), dataStream, dataBlockSize);

							stream.SkipBytes(dataBlockSize);
						}
						else
						{
							SPtr<MemoryDataStream> dataBlockStream = B3DMakeShared<MemoryDataStream>(dataBlockSize);
							stream.ReadBytes(dataBlockStream->Data(), dataBlockSize);

							curField->SetValue(rttiInstance, output.get(), dataBlockStream, dataBlockSize);
						}
					}
					else
						stream.SkipBytes(dataBlockSize);

					break;
				}
			default:
				B3D_EXCEPT(InternalErrorException, "Error decoding data. Encountered a type I don't know how to decode. Type: " + ToString(u32(fieldSchema.Type)) + ", Is array: " + ToString(fieldSchema.IsArray));
			}

			stream.ClearBuffered(false);
		}

		u32 bytesRead = (u32)Math::DivideAndRoundUp(stream.Tell(), (uint64_t)8);
		if(mReportProgress && (bytesRead >= mNextProgressReport))
		{
			u32 lastReport = (bytesRead / kReportAfterBytes) * kReportAfterBytes;
			mNextProgressReport = lastReport + kReportAfterBytes;

			mReportProgress(bytesRead / (float)mTotalBytesToRead);
		}
	}

	finalizeObject(output.get());
	return false;
}

bool BinarySerializer::SerializeReflectableObjectInline(IReflectable* object, BufferedBitstreamWriter& stream, BinarySerializerFlags flags)
{
	if(object != nullptr)
	{
		if(!SerializeReflectableObject(object, 0, stream, flags))
			return false;

		if(!flags.IsSet(BinarySerializerFlag::NoMeta))
		{
			// Encode terminator field
			// Complex types require terminator fields because they can be embedded within other complex types and we need
			// to know when their fields end and parent's resume
			if(flags.IsSet(BinarySerializerFlag::Compress))
			{
				u8 metaData = EncodeFieldTerminator();
				stream.WriteBytes(metaData);
			}
			else
				WriteFieldMetaData(RTTIFieldSchema(), true, stream);
		}
	}

	return true;
}

void BinarySerializer::WriteFieldMetaData(const RTTIFieldSchema& fieldSchema, bool isLastFieldInType, BufferedBitstreamWriter& stream)
{
	// If O == 0 - Meta contains field information (Encoded using this method)
	//// Encoding if E = 0: IIII IIII IIII IIII SSSS SSSS ETYP CDAO
	//// Encoding if E = 1: IIII IIII IIII IIII BBBB xxxx ETYP CDAO
	//// I - Id
	//// S - Size
	//// C - Complex
	//// A - Array
	//// D - Data block
	//// P - Complex ptr
	//// O - Object descriptor
	//// Y - Plain field has dynamic size
	//// T - Terminator (last field in an object)
	//// E - Extended (size is replaced with additional meta-data)
	//// B - Built-in type ID
    //// If both D & A bits are set, signifies that additional field types follow in encoding using the last two bytes from the encoding above

	FieldTypeMetaData firstFieldTypeMetaData;
	if(fieldSchema.FieldTypes.Empty()) // Old approach, single field type
	{
		const RTTIFieldTypeSchema fieldTypeSchema(fieldSchema.HasDynamicSize, fieldSchema.Size, fieldSchema.Type, fieldSchema.FieldTypeId, fieldSchema.FieldTypeSchema);
		firstFieldTypeMetaData = FieldTypeMetaData::Create(fieldSchema, fieldTypeSchema, isLastFieldInType, false);
	}
	else
	{
		firstFieldTypeMetaData = FieldTypeMetaData::Create(fieldSchema, fieldSchema.FieldTypes[0], isLastFieldInType, fieldSchema.FieldTypes.Size() > 1);
	}

	// Encodes field ID and meta-data for the first type
	const u32 fieldMetaData = fieldSchema.Id << 16 | (u32)firstFieldTypeMetaData.PackedData;
	stream.WriteBytes(fieldMetaData);

	const u32 fieldTypeCount = (u32)fieldSchema.FieldTypes.Size();
	for(u32 fieldTypeIndex = 1; fieldTypeIndex < fieldTypeCount; fieldTypeIndex++)
	{
		const bool isLastFieldType = (fieldTypeIndex + 1) == fieldTypeCount;
		FieldTypeMetaData additionalFieldTypeMetaData = FieldTypeMetaData::Create(fieldSchema, fieldSchema.FieldTypes[fieldTypeIndex], false, isLastFieldType);

		stream.WriteBytes(additionalFieldTypeMetaData.PackedData);
	}
}

RTTIFieldSchema BinarySerializer::ReadFieldMetaData(BufferedBitstreamReader& stream, bool& terminator)
{
	u32 fieldMetaData;
	stream.ReadBytes(fieldMetaData);

	if(IsObjectMetaData(fieldMetaData))
	{
		B3D_EXCEPT(InternalErrorException, "Meta data represents an object description but is trying to be decoded as a field descriptor.");
	}

	RTTIFieldSchema fieldSchema;
	fieldSchema.Id = (u16)((fieldMetaData >> 16) & 0xFFFF);

	FieldTypeMetaData firstFieldTypeMetaData;
	firstFieldTypeMetaData.PackedData = (u16)(fieldMetaData & 0xFFFF);

	bool hasMoreFieldTypes;
	const RTTIFieldTypeSchema firstFieldTypeSchema = firstFieldTypeMetaData.ToFieldTypeSchema(hasMoreFieldTypes);

	terminator = firstFieldTypeMetaData.IsLastFieldInType;
	fieldSchema.IsArray = firstFieldTypeMetaData.IsArray;
	fieldSchema.FieldTypes.Add(firstFieldTypeSchema);

	// For now, duplicate these fields. But ultimately we'll remove them in favor data stored of FieldTypes
	fieldSchema.HasDynamicSize = firstFieldTypeSchema.HasDynamicSize;
	fieldSchema.Type = firstFieldTypeSchema.Type;
	fieldSchema.Size = firstFieldTypeSchema.FixedSize;
	fieldSchema.FieldTypeId = firstFieldTypeSchema.FieldTypeId;

	while(hasMoreFieldTypes)
	{
		u16 fieldTypeMetaData;
		stream.ReadBytes(fieldTypeMetaData);

		FieldTypeMetaData additionalFieldTypeMetaData;
		additionalFieldTypeMetaData.PackedData = fieldTypeMetaData;

		const RTTIFieldTypeSchema additionalFieldTypeSchema = additionalFieldTypeMetaData.ToFieldTypeSchema(hasMoreFieldTypes);
		fieldSchema.FieldTypes.Add(firstFieldTypeSchema);
	}

	return fieldSchema;
}

u8 BinarySerializer::EncodeFieldTerminator()
{
	// See the documentation for WriteFieldMetaData() on why we're using this format
	return 0x40;
}

void BinarySerializer::SkipBuiltinType(u32 fieldType, BufferedBitstreamReader& stream, bool compressed)
{
	switch(fieldType)
	{
	case TID_Bool:
		{
			bool data;
			stream.Preload(sizeof(data));
			RTTIPlainType<bool>::FromMemory(data, stream.GetBitstream(), RTTIFieldInfo(), compressed);
			break;
		}
	case TID_Int32:
		{
			int32_t data;
			stream.Preload(sizeof(data));
			RTTIPlainType<int32_t>::FromMemory(data, stream.GetBitstream(), RTTIFieldInfo(), compressed);
			break;
		}
	case TID_UInt32:
		{
			uint32_t data;
			stream.Preload(sizeof(data));
			RTTIPlainType<uint32_t>::FromMemory(data, stream.GetBitstream(), RTTIFieldInfo(), compressed);
			break;
		}
	default:
		B3D_ASSERT(false);
		break;
	}
}

bool BinarySerializer::IsFieldTerminator(u8 data)
{
	if(IsObjectMetaData(data))
	{
		B3D_EXCEPT(InternalErrorException, "Meta data represents an object description but is trying to be decoded as a field descriptor.");
	}

	return (data & 0x40) != 0;
}

BinarySerializer::ObjectMetaData BinarySerializer::EncodeObjectMetaData(u32 objId, u32 objTypeId, bool isBaseClass)
{
	// If O == 1 - Meta contains object instance information (Encoded using encodeObjectMetaData)
	//// Encoding: SSSS SSSS SSSS SSSS xxxx xxxx xxxx xxBO
	//// S - Size of the object identifier
	//// O - Object descriptor
	//// B - Base class indicator

	if(objId > 1073741823)
	{
		B3D_EXCEPT(InvalidParametersException, "Object ID is larger than we can store (max 30 bits): " + ToString(objId));
	}

	ObjectMetaData metaData;
	metaData.ObjectMeta = EncodeObjectMetaData(objId, isBaseClass);
	metaData.TypeId = objTypeId;
	return metaData;
}

void BinarySerializer::DecodeObjectMetaData(ObjectMetaData encodedData, u32& objId, u32& objTypeId, bool& isBaseClass)
{
	if(!IsObjectMetaData(encodedData.ObjectMeta))
	{
		B3D_EXCEPT(InternalErrorException, "Meta data represents a field description but is trying to be decoded as an object descriptor.");
	}

	DecodeObjectMetaData(encodedData.ObjectMeta, objId, isBaseClass);
	objTypeId = encodedData.TypeId;
}

u32 BinarySerializer::EncodeObjectMetaData(u32 objId, bool isBaseClass)
{
	// If O == 1 - Meta contains object instance information (Encoded using encodeObjectMetaData)
	//// Encoding: SSSS SSSS SSSS SSSS xxxx xxxx xxxx xxBO
	//// S - Size of the object identifier
	//// O - Object descriptor
	//// B - Base class indicator

	if(objId > 1073741823)
	{
		B3D_EXCEPT(InvalidParametersException, "Object ID is larger than we can store (max 30 bits): " + ToString(objId));
	}

	return (objId << 2) | (isBaseClass ? 0x02 : 0) | 0x01;
}

void BinarySerializer::DecodeObjectMetaData(u32 encodedData, u32& objId, bool& isBaseClass)
{
	if(!IsObjectMetaData(encodedData))
	{
		B3D_EXCEPT(InternalErrorException, "Meta data represents a field description but is trying to be decoded as an object descriptor.");
	}

	objId = (encodedData >> 2) & 0x3FFFFFFF;
	isBaseClass = (encodedData & 0x02) != 0;
}

bool BinarySerializer::IsObjectMetaData(u32 encodedData)
{
	return ((encodedData & 0x01) != 0);
}

u32 BinarySerializer::ReadObjectMetaData(BufferedBitstreamReader& stream, BinarySerializerFlags flags, uint32_t& objId, uint32_t& objTypeId, bool& isBaseType)
{
	if(!flags.IsSet(BinarySerializerFlag::NoMeta))
	{
		ObjectMetaData objectMetaData;
		objectMetaData.ObjectMeta = 0;
		objectMetaData.TypeId = 0;

		if(stream.ReadBytes(objectMetaData) != sizeof(ObjectMetaData))
			B3D_EXCEPT(InternalErrorException, "Error decoding data.");

		DecodeObjectMetaData(objectMetaData, objId, objTypeId, isBaseType);
		return sizeof(ObjectMetaData) * 8;
	}
	else
	{
		u32 objectMetaData = 0;

		u32 bitsRead = 0;
		if(flags.IsSet(BinarySerializerFlag::Compress))
			bitsRead = stream.ReadVarInt(objectMetaData);
		else
		{
			if(stream.ReadBytes(objectMetaData) != sizeof(objectMetaData))
				B3D_EXCEPT(InternalErrorException, "Error decoding data.");

			bitsRead = sizeof(objectMetaData) * 8;
		}

		DecodeObjectMetaData(objectMetaData, objId, isBaseType);
		objTypeId = 0; // Unavailable, needs to be read from a schema

		return bitsRead;
	}
}

u32 BinarySerializer::FindOrCreateReflectableObjectId(IReflectable* object)
{
	void* const objectMemoryAddress = object;

	auto found = mReflectableObjectToID.find(objectMemoryAddress);
	if(found != mReflectableObjectToID.end())
		return found->second;

	u32 objId = mLastUsedObjectId++;
	mReflectableObjectToID.insert(std::make_pair(objectMemoryAddress, objId));

	return objId;
}

u32 BinarySerializer::RegisterReflectableObjectForSerialization(SPtr<IReflectable> object)
{
	if(object == nullptr)
		return 0;

	void* const objectMemoryAddress = object.get();

	auto found = mReflectableObjectToID.find(objectMemoryAddress);
	if(found == mReflectableObjectToID.end())
	{
		const u32 objectId = FindOrCreateReflectableObjectId(object.get());

		mReflectableObjectsToSerialize.push_back(ObjectToSerialize(objectId, object));
		mReflectableObjectToID.insert(std::make_pair(objectMemoryAddress, objectId));

		return objectId;
	}

	return found->second;
}

#undef COPY_TO_BUFFER
