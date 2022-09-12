//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsPhysXPrerequisites.h"
#include "Physics/BsHingeJoint.h"
#include "PxPhysics.h"
#include "extensions/PxRevoluteJoint.h"

namespace bs
{
	/** @addtogroup PhysX
	 *  @{
	 */

	/** PhysX implementation of a HingeJoint. */
	class PhysXHingeJoint : public HingeJoint
	{
	public:
		PhysXHingeJoint(physx::PxPhysics* physx, const HINGE_JOINT_DESC& desc);
		~PhysXHingeJoint();

		/** @copydoc HingeJoint::getAngle */
		Radian GetAngle() const override;

		/** @copydoc HingeJoint::getSpeed */
		float GetSpeed() const override;

		/** @copydoc HingeJoint::getLimit */
		LimitAngularRange GetLimit() const override;

		/** @copydoc HingeJoint::setLimit */
		void SetLimit(const LimitAngularRange& limit) override;

		/** @copydoc HingeJoint::getDrive */
		HingeJointDrive GetDrive() const override;

		/** @copydoc HingeJoint::setDrive */
		void SetDrive(const HingeJointDrive& drive) override;

		/** @copydoc HingeJoint::setFlag */
		void SetFlag(HingeJointFlag flag, bool enabled) override;

		/** @copydoc HingeJoint::hasFlag */
		bool HasFlag(HingeJointFlag flag) const override;

	private:
		/** Returns the internal PhysX representation of the hinge (revolute) joint. */
		inline physx::PxRevoluteJoint* getInternal() const;
	};

	/** @} */
}
