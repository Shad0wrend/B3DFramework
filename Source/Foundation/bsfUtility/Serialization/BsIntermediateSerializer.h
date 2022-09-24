//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Prerequisites/BsPrerequisitesUtil.h"
#include "Reflection/BsIReflectable.h"
#include "BsSerializedObject.h"

namespace bs
{
	struct SerializationContext;

	/** @addtogroup Serialization
	 *  @{
	 */

	/** Helper class for performing SerializedObject <-> IReflectable encoding & decoding. */
	class BS_UTILITY_EXPORT IntermediateSerializer
	{
	public:
		IntermediateSerializer();

		/** Encodes an IReflectable object into an intermediate representation. */
		SPtr<SerializedObject> Encode(IReflectable* object, SerializedObjectEncodeFlags flags,
			SerializationContext* context = nullptr);

		/** Decodes an intermediate representation of a serialized object into the actual object. */
		SPtr<IReflectable> Decode(const SerializedObject* serializedObject,
			SerializationContext* context = nullptr);

		/**
		 * @name Internal
		 * @{
		 */

		/**
		 * Encodes a single field from a reflectable object into a SerializedInstance. If a field is an array and
		 * @p arrayIdx is -1 then the entire array will be encoded, otherwise just a single array field will. If the
		 * field is not array the value of @p arrayIdx is not relevant.
		 */
		static SPtr<SerializedInstance> EncodeFieldInternal(IReflectable* object, RTTITypeBase* rtti, RTTIField* field, UINT32 arrayIdx,
			SerializedObjectEncodeFlags flags, SerializationContext* context, FrameAlloc* alloc);

		/** @} */
	private:
		friend class BinaryDiff;
		
		struct ObjectToDecode
		{
			ObjectToDecode(const SPtr<IReflectable>& _object, const SerializedObject* serializedObject)
				:Object(_object), SerializedObject(serializedObject)
			{ }

			SPtr<IReflectable> Object;
			const SerializedObject* SerializedObject;
			bool IsDecoded = false;
			bool DecodeInProgress = false; // Used for error reporting circular references
		};

		/**	Decodes a single IReflectable object. */
		void DecodeEntry(const SPtr<IReflectable>& object, const SerializedObject* serializableObject);

		/** Encodes a single IReflectable object. */
		static SPtr<SerializedObject> EncodeEntry(IReflectable* object, SerializedObjectEncodeFlags flags,
			SerializationContext* context, FrameAlloc* alloc);

		UnorderedMap<const SerializedObject*, ObjectToDecode> mObjectMap;
		SerializationContext* mContext = nullptr;
		FrameAlloc* mAlloc = nullptr;
	};

	/** @} */
}
