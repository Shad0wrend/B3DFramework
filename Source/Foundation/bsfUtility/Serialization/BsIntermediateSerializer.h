//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Prerequisites/BsPrerequisitesUtil.h"
#include "Reflection/BsIReflectable.h"
#include "BsSerializedObject.h"

namespace bs
{
	class IRTTIIterator;
	struct RTTIIteratorField;
	struct SerializationContext;

	/** @addtogroup Serialization
	 *  @{
	 */

	/** Helper class for performing SerializedObject <-> IReflectable encoding & decoding. */
	class B3D_UTILITY_EXPORT IntermediateSerializer
	{
	public:
		IntermediateSerializer();

		/** Encodes an IReflectable object into an intermediate representation. */
		SPtr<SerializedObject> Encode(IReflectable* object, SerializedObjectEncodeFlags flags, SerializationContext* context = nullptr);

		/** Decodes an intermediate representation of a serialized object into the actual object. */
		SPtr<IReflectable> Decode(const SerializedObject* serializedObject, SerializationContext* context = nullptr);

		/**
		 * @name Internal
		 * @{
		 */

		/**
		 * Serializes a single field entry from a reflectable object into a SerializedInstance. If a field is an array and
		 * @p arrayIdx is -1 then the entire array will be encoded, otherwise just a single array field will. If the
		 * field is not array the value of @p arrayIdx is not relevant.
		 */
		static SPtr<SerializedInstance> SerializeField(IReflectable* object, RTTITypeBase* rtti, RTTIField* field, u32 arrayIdx, SerializedObjectEncodeFlags flags, SerializationContext* context, FrameAllocator* alloc);

		/**
		 * Serializes a single field from the provided reflectable object into a SerializedInstance.
		 *
		 * @param	object		Reflectable object that stores the data to serialize.
		 * @param	rttiType	Type information for the provided reflectable object.
		 * @param	field		Field from which to retrieve the data.
		 *
		 * TODO - DOc
		 *
		 */
		static SPtr<SerializedInstance> SerializeField(IReflectable& object, RTTITypeBase& rttiType, RTTIIteratorField& field, SerializedObjectEncodeFlags flags, SerializationContext* context, FrameAllocator& allocator);

		/** @} */
	private:
		friend class BinaryDiff;

		struct ObjectDeserializationData
		{
			ObjectDeserializationData(const SPtr<IReflectable>& object, const SerializedObject* serializedObject)
				: Object(object), SerializedObject(serializedObject)
			{}

			SPtr<IReflectable> Object;
			const SerializedObject* SerializedObject;
			bool IsDeserialized = false;
			bool DeserializationInProgress = false; // Used for error reporting circular references
		};

		/**	Deserializes a single IReflectable object. */
		void DeserializeReflectableObject(const SPtr<IReflectable>& object, const SerializedObject* serializableObject);

		/** Serializes a single IReflectable object. */
		static SPtr<SerializedObject> SerializeReflectableObject(const IReflectable& object, SerializedObjectEncodeFlags flags, SerializationContext* context, FrameAllocator& allocator);

		UnorderedMap<const SerializedObject*, ObjectDeserializationData> mObjectMap;
		SerializationContext* mContext = nullptr;
		FrameAllocator* mAlloc = nullptr;
	};

	/** @} */
} // namespace bs
