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
	class B3D_UTILITY_EXPORT BinaryCloner
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
		static void GatherReferences(IReflectable* object, FrameAllocator& alloc, ObjectReferenceData& referenceData);

		/**
		 * Restores a set of references retrieved by gatherReferences() and applies them to a specific object. Type of the
		 * object must be the same as the type that was used when calling gatherReferences().
		 */
		static void RestoreReferences(IReflectable* object, FrameAllocator& alloc, const ObjectReferenceData& referenceData);
	};

	/**
	 * Clones the provided object.
	 *
	 * @param	object		Object to clone.
	 * @param	shallow		Determines how are fields containing reflectable pointers. If true, then those pointers will keep pointing to the original
	 *						object (both the clone and original referencing the same object by the pointer). If false, then the pointer object to will be cloned as well.
	 */
	template <class T>
	SPtr<T> B3DRTTIClone(const T* const object, bool shallow = false)
	{
		static_assert((std::is_base_of_v<IReflectable, T>), "Cannot clone object. It needs to derive from bs::IReflectable.");

		if(object == nullptr)
			return nullptr;

		BinaryCloner cloner;
		return std::static_pointer_cast<T>(cloner.Clone(const_cast<T*>(object), shallow));
	}

	/** @copydoc B3DRTTIClone(const IReflectable* const, bool) */
	template <class T>
	SPtr<T> B3DRTTIClone(const SPtr<T>& object, bool shallow = false)
	{
		static_assert((std::is_base_of_v<IReflectable, T>), "Cannot clone object. It needs to derive from bs::IReflectable.");

		if(object == nullptr)
			return nullptr;

		BinaryCloner cloner;
		return std::static_pointer_cast<T>(cloner.Clone(const_cast<T*>(object.get()), shallow));
	}

	/** @} */
} // namespace bs
