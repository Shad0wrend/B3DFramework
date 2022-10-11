//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Prerequisites/BsPrerequisitesUtil.h"

namespace bs
{
	/** @addtogroup Serialization
	 *  @{
	 */

	/** Helper class that performs cloning of an object that implements RTTI. */
	class BS_UTILITY_EXPORT BinaryCloner
	{
	public:

		/**
		 * Returns a copy of the provided object with identical data.
		 *
		 * @param[in]	object		Object to clone.
		 * @param[in]	shallow		If false then all referenced objects will be cloned as well, otherwise the references
		 *							to the original objects will be kept.
		 */
		static SPtr<IReflectable> Clone(IReflectable* object, bool shallow = false);

	private:
		struct ObjectReferenceData;

		/** Identifier representing a single field or an array entry in an object. */
		struct FieldId
		{
			RTTIField* Field;
			i32 ArrayIdx;
		};

		/** A saved reference to an object with a field identifier that owns it. */
		struct ObjectReference
		{
			FieldId FieldId;
			SPtr<IReflectable> Object;
		};

		/**
		 * Contains all object references in a portion of an object belonging to a specific class (base and derived
		 * classes count as separate sub-objects).
		 */
		struct SubObjectReferenceData
		{
			RTTITypeBase* Rtti;
			Vector<ObjectReference> References;
			Vector<ObjectReferenceData> Children;
		};

		/**
		 * Contains all object references in an entire object, as well as the identifier of the field owning this object.
		 */
		struct ObjectReferenceData
		{
			FieldId FieldId;
			Vector<SubObjectReferenceData> SubObjectData;
		};

		/**
		 * Iterates over the provided object hierarchy and retrieves all object references which are returned in
		 * @p referenceData output parameter, also in a hierarchical format for easier parsing.
		 */
		static void GatherReferences(IReflectable* object, FrameAlloc& alloc, ObjectReferenceData& referenceData);

		/**
		 * Restores a set of references retrieved by gatherReferences() and applies them to a specific object. Type of the
		 * object must be the same as the type that was used when calling gatherReferences().
		 */
		static void RestoreReferences(IReflectable* object, FrameAlloc& alloc, const ObjectReferenceData& referenceData);
	};

	/** @} */
}
