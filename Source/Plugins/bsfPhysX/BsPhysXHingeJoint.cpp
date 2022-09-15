//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsPhysXHingeJoint.h"
#include "BsFPhysXJoint.h"
#include "BsPhysXRigidbody.h"
#include "PxRigidDynamic.h"

using namespace physx;

namespace bs
{
	PxRevoluteJointFlag::Enum toPxFlag(HingeJointFlag flag)
	{
		switch (flag)
		{
		case HingeJointFlag::Limit:
			return PxRevoluteJointFlag::eLIMIT_ENABLED;
		default:
		case HingeJointFlag::Drive:
			return PxRevoluteJointFlag::eDRIVE_ENABLED;
		}
	}

	PhysXHingeJoint::PhysXHingeJoint(PxPhysics* physx, const HINGE_JOINT_DESC& desc)
		:HingeJoint(desc)
	{
		PxRigidActor* actor0 = nullptr;
		if (desc.bodies[0].body != nullptr)
			actor0 = static_cast<PhysXRigidbody*>(desc.bodies[0].body)->GetInternalInternal();

		PxRigidActor* actor1 = nullptr;
		if (desc.bodies[1].body != nullptr)
			actor1 = static_cast<PhysXRigidbody*>(desc.bodies[1].body)->GetInternalInternal();

		PxTransform tfrm0 = toPxTransform(desc.bodies[0].position, desc.bodies[0].rotation);
		PxTransform tfrm1 = toPxTransform(desc.bodies[1].position, desc.bodies[1].rotation);

		PxRevoluteJoint* joint = PxRevoluteJointCreate(*physx, actor0, tfrm0, actor1, tfrm1);
		joint->userData = this;

		mInternal = bs_new<FPhysXJoint>(joint, desc);

		PxRevoluteJointFlags flags;

		if (((UINT32)desc.flag & (UINT32)HingeJointFlag::Limit) != 0)
			flags |= PxRevoluteJointFlag::eLIMIT_ENABLED;

		if (((UINT32)desc.flag & (UINT32)HingeJointFlag::Drive) != 0)
			flags |= PxRevoluteJointFlag::eDRIVE_ENABLED;

		joint->SetRevoluteJointFlags(flags);

		// Must be set after global flags, as it will append to them.
		// Calls to virtual methods are okay here.
		SetLimit(desc.limit);
		SetDrive(desc.drive);
	}

	PhysXHingeJoint::~PhysXHingeJoint()
	{
		bs_delete(mInternal);
	}

	Radian PhysXHingeJoint::GetAngle() const
	{
		return Radian(GetInternal()->GetAngle());
	}

	float PhysXHingeJoint::GetSpeed() const
	{
		return GetInternal()->GetVelocity();
	}

	LimitAngularRange PhysXHingeJoint::GetLimit() const
	{
		PxJointAngularLimitPair pxLimit = GetInternal()->GetLimit();

		LimitAngularRange limit;
		limit.lower = pxLimit.lower;
		limit.upper = pxLimit.upper;
		limit.contactDist = pxLimit.contactDistance;
		limit.restitution = pxLimit.restitution;
		limit.spring.stiffness = pxLimit.stiffness;
		limit.spring.damping = pxLimit.damping;

		return limit;
	}

	void PhysXHingeJoint::SetLimit(const LimitAngularRange& limit)
	{
		PxJointAngularLimitPair pxLimit(limit.lower.valueRadians(), limit.upper.valueRadians(), limit.contactDist);
		pxLimit.stiffness = limit.spring.stiffness;
		pxLimit.damping = limit.spring.damping;
		pxLimit.restitution = limit.restitution;

		GetInternal()->SetLimit(pxLimit);
	}

	HingeJointDrive PhysXHingeJoint::GetDrive() const
	{
		HingeJointDrive drive;
		drive.speed = GetInternal()->GetDriveVelocity();
		drive.forceLimit = GetInternal()->GetDriveForceLimit();
		drive.gearRatio = GetInternal()->GetDriveGearRatio();
		drive.freeSpin = GetInternal()->GetRevoluteJointFlags() & PxRevoluteJointFlag::eDRIVE_FREESPIN;

		return drive;
	}

	void PhysXHingeJoint::SetDrive(const HingeJointDrive& drive)
	{
		GetInternal()->SetDriveVelocity(drive.speed);
		GetInternal()->SetDriveForceLimit(drive.forceLimit);
		GetInternal()->SetDriveGearRatio(drive.gearRatio);
		GetInternal()->SetRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_FREESPIN, drive.freeSpin);
	}

	void PhysXHingeJoint::SetFlag(HingeJointFlag flag, bool enabled)
	{
		GetInternal()->SetRevoluteJointFlag(toPxFlag(flag), enabled);
	}

	bool PhysXHingeJoint::HasFlag(HingeJointFlag flag) const
	{
		return GetInternal()->GetRevoluteJointFlags() & toPxFlag(flag);
	}

	PxRevoluteJoint* PhysXHingeJoint::GetInternal() const
	{
		FPhysXJoint* internal = static_cast<FPhysXJoint*>(mInternal);

		return static_cast<PxRevoluteJoint*>(internal->GetInternalInternal());
	}
}
