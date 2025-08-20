//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Physics/BsJoint.h"
#include "Scene/BsComponent.h"

namespace b3d
{
	/** @addtogroup Components-Core
	 *  @{
	 */

	/**
	 * Base class for all Joint types. Joints constrain how two rigidbodies move relative to one another (for example a door
	 * hinge). One of the bodies in the joint must always be movable (non-kinematic).
	 */
	class B3D_CORE_EXPORT B3D_SCRIPT_EXPORT(DocumentationGroup(Physics), ExportName(Joint)) CJoint : public Component
	{
	public:
		CJoint(const HSceneObject& parent, JointCreateInformation& createInformation);
		virtual ~CJoint() = default;

		/** Determines a body managed by the joint. One of the bodies must be movable (non-kinematic). */
		B3D_SCRIPT_EXPORT(ExportName(SetBody))
		void SetBody(JointBody body, const HRigidbody& value);

		/** @copydoc SetBody */
		B3D_SCRIPT_EXPORT(ExportName(GetBody))
		HRigidbody GetBody(JointBody body) const;

		/** Returns the position relative to the body, at which the body is anchored to the joint. */
		B3D_SCRIPT_EXPORT(ExportName(GetPosition))
		Vector3 GetPosition(JointBody body) const;

		/** Returns the rotation relative to the body, at which the body is anchored to the joint. */
		B3D_SCRIPT_EXPORT(ExportName(GetRotation))
		Quaternion GetRotation(JointBody body) const;

		/** Sets the position and rotation relative to the body, at which the body is anchored to the joint.  */
		B3D_SCRIPT_EXPORT(ExportName(SetTransform))
		void SetTransform(JointBody body, const Vector3& position, const Quaternion& rotation);

		/** Determines the maximum force the joint can apply before breaking. Broken joints no longer participate in physics simulation. */
		B3D_SCRIPT_EXPORT(ExportName(BreakForce), Property(Setter))
		void SetBreakForce(float force);

		/** @copydoc SetBreakForce */
		B3D_SCRIPT_EXPORT(ExportName(BreakForce), Property(Getter))
		float GetBreakForce() const;

		/** Determines the maximum torque the joint can apply before breaking. Broken joints no longer participate in physics simulation. */
		B3D_SCRIPT_EXPORT(ExportName(BreakTorque), Property(Setter))
		void SetBreakTorque(float torque);

		/** @copydoc SetBreakTorque */
		B3D_SCRIPT_EXPORT(ExportName(BreakTorque), Property(Getter))
		float GetBreakTorque() const;

		/** Determines whether collision between the two bodies managed by the joint are enabled. */
		B3D_SCRIPT_EXPORT(ExportName(EnableCollision), Property(Setter))
		void SetEnableCollision(bool value);

		/** @copydoc SetEnableCollision */
		B3D_SCRIPT_EXPORT(ExportName(EnableCollision), Property(Getter))
		bool GetEnableCollision() const;

		/** Triggered when the joint's break force or torque is exceeded. */
		B3D_SCRIPT_EXPORT(ExportName(OnJointBreak))
		Event<void()> OnJointBreak;

		/************************************************************************/
		/* 						COMPONENT OVERRIDES                      		*/
		/************************************************************************/
	protected:
		friend class SceneObject;

		void OnBeginPlay() override;
		void OnDestroyed() override;
		void OnDisabled() override;
		void OnEnabled() override;
		void OnTransformChanged(TransformChangedFlags flags) override;

	protected:
		friend class Rigidbody;

		/** Creates the internal representation of the Joint for use by the component. */
		virtual SPtr<Joint> CreateInternal() = 0;

		/** Creates the internal representation of the Joint and restores the values saved by the Component. */
		virtual void RestoreInternal();

		/** Calculates the local position/rotation that needs to be applied to the particular joint body. */
		virtual void GetLocalTransform(JointBody body, Vector3& position, Quaternion& rotation);

		/** Destroys the internal joint representation. */
		void DestroyInternal();

		/** Notifies the joint that one of the attached rigidbodies moved and that its transform needs updating. */
		void NotifyRigidbodyMoved(const HRigidbody& body);

		/** Updates the local transform for the specified body attached to the joint. */
		void UpdateTransform(JointBody body);

		/** Triggered when the joint constraint gets broken. */
		void TriggerOnJointBroken();

		SPtr<Joint> mInternal;

		HRigidbody mBodies[2];
		Vector3 mPositions[2];
		Quaternion mRotations[2];

	private:
		JointCreateInformation& mInformation; // References the information in the derived class

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/
	public:
		friend class CJointRTTI;
		static RTTIType* GetRttiStatic();
		RTTIType* GetRtti() const;

		CJoint(JointCreateInformation& createInformation); // Serialization only
	};

	/** Low-level interface for a joint used by the Joint component. Should be implemented by the physics plugin to provide joint functionality. */
	class B3D_CORE_EXPORT IJointImplementation
	{
	public:
		virtual ~IJointImplementation() = default;

		/** @copydoc Joint::SetBody */
		virtual void SetBody(JointBody body, Rigidbody* value) = 0;

		/** @copydoc SetBody() */
		virtual Rigidbody* GetBody(JointBody body) const = 0;

		/** @copydoc Joint::GetPosition */
		virtual Vector3 GetPosition(JointBody body) const = 0; // TODO - Needed?

		/** @copydoc Joint::GetRotation */
		virtual Quaternion GetRotation(JointBody body) const = 0; // TODO - Needed?

		/** @copydoc Joint::SetTransform */
		virtual void SetTransform(JointBody body, const Vector3& position, const Quaternion& rotation) = 0; // TODO - Needed?

		/** @copydoc Joint::SetBreakForce */
		virtual void SetBreakForce(float force) = 0;

		/** @copydoc SetBreakForce() */
		virtual float GetBreakForce() const = 0;

		/** @copydoc Joint::SetBreakTorque */
		virtual void SetBreakTorque(float torque) = 0;

		/** @copydoc SetBreakTorque() */
		virtual float GetBreakTorque() const = 0;

		/** @copydoc Joint::SetEnableCollision() */
		virtual void SetEnableCollision(bool value) = 0;

		/** @copydoc SetEnableCollision() */
		virtual bool GetEnableCollision() const = 0;
	};

	/** @} */
} // namespace b3d
