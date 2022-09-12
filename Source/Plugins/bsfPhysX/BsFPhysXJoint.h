//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsPhysXPrerequisites.h"
#include "Physics/BsFJoint.h"
#include "extensions/PxJoint.h"

namespace bs
{
	/** @addtogroup PhysX
	 *  @{
	 */

	/** PhysX implementation of an FJoint. */
	class FPhysXJoint : public FJoint
	{
	public:
		FPhysXJoint(physx::PxJoint* joint, const JOINT_DESC& desc);
		~FPhysXJoint();

		/** @copydoc FJoint::getBody */
		Rigidbody* getBody(JointBody body) const override;

		/** @copydoc FJoint::setBody */
		void SetBody(JointBody body, Rigidbody* value) override;

		/** @copydoc FJoint::getPosition */
		Vector3 GetPosition(JointBody body) const override;

		/** @copydoc FJoint::getRotation */
		Quaternion GetRotation(JointBody body) const override;

		/** @copydoc FJoint::setTransform */
		void SetTransform(JointBody body, const Vector3& position, const Quaternion& rotation) override;

		/** @copydoc FJoint::getBreakForce */
		float GetBreakForce() const override;

		/** @copydoc FJoint::setBreakForce */
		void SetBreakForce(float force) override;

		/** @copydoc FJoint::getBreakTorque */
		float GetBreakTorque() const override;

		/** @copydoc FJoint::setBreakTorque */
		void SetBreakTorque(float torque) override;

		/** @copydoc FJoint::getEnableCollision */
		bool GetEnableCollision() const override;

		/** @copydoc FJoint::setEnableCollision */
		void SetEnableCollision(bool value) override;

		/** Gets the internal PhysX joint object. */
		physx::PxJoint* _getInternal() const { return mJoint; }
	protected:
		physx::PxJoint* mJoint;
	};

	/** @} */
}
