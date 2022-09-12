//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsNullPhysicsPrerequisites.h"
#include "Physics/BsJoint.h"
#include "Physics/BsFJoint.h"
#include "Physics/BsD6Joint.h"
#include "Physics/BsDistanceJoint.h"
#include "Physics/BsFixedJoint.h"
#include "Physics/BsHingeJoint.h"
#include "Physics/BsSliderJoint.h"
#include "Physics/BsSphericalJoint.h"

namespace bs
{
	/** @addtogroup NullPhysics
	 *  @{
	 */

	/** Null implementation of an FJoint. */
	class FNullPhysicsJoint : public FJoint
	{
	public:
		FNullPhysicsJoint(const JOINT_DESC& desc);
		~FNullPhysicsJoint() = default;

		/** @copydoc FJoint::getBody */
		Rigidbody* getBody(JointBody body) const override { return mDesc.bodies[(int)body].body; }


		/** @copydoc FJoint::setBody */
		void SetBody(JointBody body, Rigidbody* value) override { mDesc.bodies[(int)body].body = value; }

		/** @copydoc FJoint::getPosition */
		Vector3 GetPosition(JointBody body) const override { return mDesc.bodies[(int)body].position; }

		/** @copydoc FJoint::getRotation */
		Quaternion GetRotation(JointBody body) const override { return mDesc.bodies[(int)body].rotation; }

		/** @copydoc FJoint::setTransform */
		void SetTransform(JointBody body, const Vector3& position, const Quaternion& rotation) override;

		/** @copydoc FJoint::getBreakForce */
		float GetBreakForce() const override { return mDesc.breakForce; }

		/** @copydoc FJoint::setBreakForce */
		void SetBreakForce(float force) override { mDesc.breakForce = force; }

		/** @copydoc FJoint::getBreakTorque */
		float GetBreakTorque() const override { return mDesc.breakTorque; }

		/** @copydoc FJoint::setBreakTorque */
		void SetBreakTorque(float torque) override { mDesc.breakTorque = torque; }

		/** @copydoc FJoint::getEnableCollision */
		bool GetEnableCollision() const override { return mDesc.enableCollision; }

		/** @copydoc FJoint::setEnableCollision */
		void SetEnableCollision(bool value) override { mDesc.enableCollision = value; }

	private:
		JOINT_DESC mDesc;
	};

	/** NullPhysics implementation of a D6 joint. */
	class NullPhysicsD6Joint : public D6Joint
	{
	public:
		NullPhysicsD6Joint(const D6_JOINT_DESC& desc);
		~NullPhysicsD6Joint();

		/** @copydoc D6Joint::getMotion */
		D6JointMotion GetMotion(D6JointAxis axis) const override { return mDesc.motion[(int)axis]; }

		/** @copydoc D6Joint::setMotion */
		void SetMotion(D6JointAxis axis, D6JointMotion motion) override { mDesc.motion[(int)axis] = motion; }

		/** @copydoc D6Joint::getTwist */
		Radian GetTwist() const override { return Radian(0.0f); }

		/** @copydoc D6Joint::getSwingY */
		Radian GetSwingY() const override { return Radian(0.0f); }

		/** @copydoc D6Joint::getSwingZ */
		Radian GetSwingZ() const override { return Radian(0.0f); }

		/** @copydoc D6Joint::getLimitLinear */
		LimitLinear GetLimitLinear() const override { return mDesc.limitLinear; }

		/** @copydoc D6Joint::setLimitLinear */
		void SetLimitLinear(const LimitLinear& limit) override { mDesc.limitLinear = limit; }

		/** @copydoc D6Joint::getLimitTwist */
		LimitAngularRange GetLimitTwist() const override { return mDesc.limitTwist; }

		/** @copydoc D6Joint::setLimitTwist */
		void SetLimitTwist(const LimitAngularRange& limit) override { mDesc.limitTwist = limit; }

		/** @copydoc D6Joint::getLimitSwing */
		LimitConeRange GetLimitSwing() const override { return mDesc.limitSwing; }

		/** @copydoc D6Joint::setLimitSwing */
		void SetLimitSwing(const LimitConeRange& limit) override { mDesc.limitSwing = limit; }

		/** @copydoc D6Joint::getDrive */
		D6JointDrive GetDrive(D6JointDriveType type) const override { return mDesc.drive[(int)type]; }

		/** @copydoc D6Joint::setDrive */
		void SetDrive(D6JointDriveType type, const D6JointDrive& drive) override { mDesc.drive[(int)type] = drive; }

		/** @copydoc D6Joint::getDrivePosition */
		Vector3 GetDrivePosition() const override { return mDesc.drivePosition; }

		/** @copydoc D6Joint::getDriveRotation */
		Quaternion GetDriveRotation() const override { return mDesc.driveRotation; }

		/** @copydoc D6Joint::setDriveTransform */
		void SetDriveTransform(const Vector3& position, const Quaternion& rotation) override;

		/** @copydoc D6Joint::getDriveLinearVelocity */
		Vector3 GetDriveLinearVelocity() const override { return mDesc.driveLinearVelocity; }

		/** @copydoc D6Joint::getDriveAngularVelocity */
		Vector3 GetDriveAngularVelocity() const override { return mDesc.driveAngularVelocity; }

		/** @copydoc D6Joint::setDriveVelocity */
		void SetDriveVelocity(const Vector3& linear, const Vector3& angular) override;

	private:
		D6_JOINT_DESC mDesc;
	};

	/** Null implementation of a DistanceJoint */
	class NullPhysicsDistanceJoint : public DistanceJoint
	{
	public:
		NullPhysicsDistanceJoint(const DISTANCE_JOINT_DESC& desc);
		~NullPhysicsDistanceJoint();

		/** @copydoc DistanceJoint::getDistance */
		float GetDistance() const override { return 0.0f; }

		/** @copydoc DistanceJoint::getMinDistance */
		float GetMinDistance() const override { return mDesc.minDistance; }

		/** @copydoc DistanceJoint::setMinDistance */
		void SetMinDistance(float value) override { mDesc.minDistance = value; }

		/** @copydoc DistanceJoint::getMaxDistance */
		float GetMaxDistance() const override { return mDesc.maxDistance; }

		/** @copydoc DistanceJoint::setMaxDistance */
		void SetMaxDistance(float value) override { mDesc.maxDistance = value; }

		/** @copydoc DistanceJoint::getTolerance */
		float GetTolerance() const override { return mDesc.tolerance; }

		/** @copydoc DistanceJoint::setTolerance */
		void SetTolerance(float value) override { mDesc.tolerance = value; }

		/** @copydoc DistanceJoint::getSpring */
		Spring GetSpring() const override { return mDesc.spring; }

		/** @copydoc DistanceJoint::setSpring */
		void SetSpring(const Spring& value) override { mDesc.spring = value; }

		/** @copydoc DistanceJoint::setFlag */
		void SetFlag(DistanceJointFlag flag, bool enabled) override;

		/** @copydoc DistanceJoint::hasFlag */
		bool HasFlag(DistanceJointFlag flag) const override;

	private:
		DISTANCE_JOINT_DESC mDesc;
	};

	/** Null implementation of a FixedJoint. */
	class NullPhysicsFixedJoint : public FixedJoint
	{
	public:
		NullPhysicsFixedJoint(const FIXED_JOINT_DESC& desc);
		~NullPhysicsFixedJoint();
	};

	/** Null implementation of a HingeJoint. */
	class NullPhysicsHingeJoint : public HingeJoint
	{
	public:
		NullPhysicsHingeJoint(const HINGE_JOINT_DESC& desc);
		~NullPhysicsHingeJoint();

		/** @copydoc HingeJoint::getAngle */
		Radian GetAngle() const override { return Radian(0.0f); }

		/** @copydoc HingeJoint::getSpeed */
		float GetSpeed() const override { return 0.0f; }

		/** @copydoc HingeJoint::getLimit */
		LimitAngularRange GetLimit() const override { return mDesc.limit; }

		/** @copydoc HingeJoint::setLimit */
		void SetLimit(const LimitAngularRange& limit) override { mDesc.limit = limit; }

		/** @copydoc HingeJoint::getDrive */
		HingeJointDrive GetDrive() const override { return mDesc.drive; }

		/** @copydoc HingeJoint::setDrive */
		void SetDrive(const HingeJointDrive& drive) override { mDesc.drive = drive; }

		/** @copydoc HingeJoint::setFlag */
		void SetFlag(HingeJointFlag flag, bool enabled) override;

		/** @copydoc HingeJoint::hasFlag */
		bool HasFlag(HingeJointFlag flag) const override;

	private:
		HINGE_JOINT_DESC mDesc;
	};

	/** Null implementation of a SliderJoint. */
	class NullPhysicsSliderJoint : public SliderJoint
	{
	public:
		NullPhysicsSliderJoint(const SLIDER_JOINT_DESC& desc);
		~NullPhysicsSliderJoint();

		/** @copydoc SliderJoint::getPosition */
		float GetPosition() const override { return 0.0f; }

		/** @copydoc SliderJoint::getSpeed */
		float GetSpeed() const override { return 0.0f; }

		/** @copydoc SliderJoint::getLimit */
		LimitLinearRange GetLimit() const override { return mDesc.limit; }

		/** @copydoc SliderJoint::setLimit */
		void SetLimit(const LimitLinearRange& limit) override { mDesc.limit = limit; }

		/** @copydoc SliderJoint::setFlag */
		void SetFlag(SliderJointFlag flag, bool enabled) override;

		/** @copydoc SliderJoint::hasFlag */
		bool HasFlag(SliderJointFlag flag) const override;

	private:
		SLIDER_JOINT_DESC mDesc;
	};

	/** Null implementation of a SphericalJoint. */
	class NullPhysicsSphericalJoint : public SphericalJoint
	{
	public:
		NullPhysicsSphericalJoint(const SPHERICAL_JOINT_DESC& desc);
		~NullPhysicsSphericalJoint();

		/** @copydoc SphericalJoint::getLimit */
		LimitConeRange GetLimit() const override { return mDesc.limit; }

		/** @copydoc SphericalJoint::setLimit */
		void SetLimit(const LimitConeRange& limit) override { mDesc.limit = limit; }

		/** @copydoc SphericalJoint::setFlag */
		void SetFlag(SphericalJointFlag flag, bool enabled) override;

		/** @copydoc SphericalJoint::hasFlag */
		bool HasFlag(SphericalJointFlag flag) const override;

	private:
		SPHERICAL_JOINT_DESC mDesc;
	};

	/** @} */
}
