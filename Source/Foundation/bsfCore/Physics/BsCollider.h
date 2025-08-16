//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Physics/BsPhysicsCommon.h"

namespace b3d
{
	class PhysicsScene;
	class ColliderShape;

	/** @addtogroup Physics
	 *  @{
	 */

	/**
	 * Low-level physics object that can be interacted with in the physics world. It can either block other physics objects, or act as a trigger. Trigger will report
	 * collision events, but won't actually prevent other physical objects from going through them.
	 *
	 * One or multiple shapes can be attached to the rigidbody, that determine its geometry.
	 *
	 * This type of rigidbody is intended to remain static in the world. You /can/ move it, but it will not interact with the physics world correctly when moved. For that case
	 * use DynamicRigidbody instead.
	 */
	class B3D_CORE_EXPORT StaticRigidbody
	{
	public:
		StaticRigidbody() = default;
		virtual ~StaticRigidbody() = default;

		/** Adds the collider to the physics scene. */
		virtual void AddToScene(PhysicsScene& scene) = 0;

		/** Removes the collider from the currently assigned physics scene. */
		virtual void RemoveFromScene() = 0;

		/** Assigns a new child shape to the collider. */
		virtual void AttachShape(const SPtr<ColliderShape>& shape) = 0;

		/** Removes a shape that was previously attached to the collider. */
		virtual void DetachShape(const SPtr<ColliderShape>& shape) = 0;

		/** Changes the position and rotation of the collider. All child shapes will maintain relative position and rotation to the collider. */
		virtual void SetTransform(const Vector3& position, const Quaternion& rotation) = 0;
	};

	/** @} */
} // namespace b3d
