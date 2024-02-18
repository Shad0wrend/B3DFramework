//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsIReflectable.h"
#include "Scene/BsGameObject.h"
#include "Math/BsVector3.h"
#include "Math/BsQuaternion.h"

namespace bs
{
	struct SerializationContext;

	/** @addtogroup Scene-Internal
	 *  @{
	 */

	/**
	 * Contains differences between two components of the same type.
	 *
	 * @see		PrefabDiff
	 */
	struct B3D_CORE_EXPORT PrefabComponentDiff : public IReflectable
	{
		UUID Id = UUID::kEmpty;
		SPtr<SerializedObject> Data;

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/

	public:
		friend class PrefabComponentDiffRTTI;
		static RTTITypeBase* GetRttiStatic();
		RTTITypeBase* GetRtti() const override;
	};

	/** Flags that mark which portion of a scene-object is modified. */
	enum class B3D_SCRIPT_EXPORT(API(Editor), DocumentationGroup(Utility - Editor)) SceneObjectDiffFlags
	{
		Name = 0x01,
		Position = 0x02,
		Rotation = 0x04,
		Scale = 0x08,
		Active = 0x10
	};

	/**
	 * Contains a set of prefab differences for a single scene object.
	 *
	 * @see		PrefabDiff
	 */
	struct B3D_CORE_EXPORT PrefabObjectDiff : public IReflectable
	{
		PrefabObjectDiff() {}

		UUID Id = UUID::kEmpty;

		String Name;
		Vector3 Position = Vector3::kZero;
		Quaternion Rotation = Quaternion::kIdentity;
		Vector3 Scale = Vector3::kZero;
		bool IsActive = false;
		u32 SoFlags = 0;

		Vector<SPtr<PrefabComponentDiff>> ComponentDeltas;
		Vector<UUID> RemovedComponents;
		Vector<SPtr<SerializedObject>> AddedComponents;

		Vector<SPtr<PrefabObjectDiff>> ChildDeltas;
		Vector<UUID> RemovedChildren;
		Vector<SPtr<SerializedObject>> AddedChildren;

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/

	public:
		friend class PrefabObjectDiffRTTI;
		static RTTITypeBase* GetRttiStatic();
		RTTITypeBase* GetRtti() const override;
	};

	/**
	 * Contains modifications between an prefab and its instance. The modifications are a set of added/removed children or
	 * components and per-field "diffs" of their components.
	 */
	class B3D_CORE_EXPORT PrefabDiff : public IReflectable
	{
	public:
		/**
		 * Creates a new prefab diff by comparing the provided instanced scene object hierarchy with the prefab scene
		 * object hierarchy.
		 */
		static SPtr<PrefabDiff> Create(const HSceneObject& prefab, const HSceneObject& instance);

		/**
		 * Applies the internal prefab diff to the provided object. The object should have similar hierarchy as the prefab
		 * the diff was created for, otherwise the results are undefined.
		 *
		 * @note	Be aware that this method will not instantiate newly added components or scene objects. It's expected
		 *			that this method will be called on a fresh copy of a scene object hierarchy, and everything to be
		 *			instantiated at once after diff is applied.
		 */
		void Apply(const HSceneObject& object);

	private:
		/**
		 * Recurses over every scene object in the prefab a generates differences between itself and the instanced version.
		 *
		 * @see		Create
		 */
		static SPtr<PrefabObjectDiff> GenerateDelta(const HSceneObject& prefab, const HSceneObject& instance);

		/**
		 * Recursively applies a per-object set of prefab differences to a specific object.
		 *
		 * @see		apply
		 */
		static void ApplyDiff(const SPtr<PrefabObjectDiff>& diff, const HSceneObject& object, SerializationContext* context);

		SPtr<PrefabObjectDiff> mRoot;

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/

		Any mRTTIData;

	public:
		friend class PrefabDiffRTTI;
		static RTTITypeBase* GetRttiStatic();
		RTTITypeBase* GetRtti() const override;
	};

	/** @} */
} // namespace bs
