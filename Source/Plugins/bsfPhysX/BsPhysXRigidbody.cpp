//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsPhysXRigidbody.h"
#include "Scene/BsSceneObject.h"
#include "Physics/BsPhysics.h"
#include "PxRigidDynamic.h"
#include "PxScene.h"
#include "extensions/PxRigidBodyExt.h"
#include "BsPhysX.h"
#include "BsPhysXColliderShape.h"

using namespace physx;

using namespace b3d;

PxForceMode::Enum ToPxForceMode(ForceMode mode)
{
	switch(mode)
	{
	case ForceMode::Force:
		return PxForceMode::eFORCE;
	case ForceMode::Impulse:
		return PxForceMode::eIMPULSE;
	case ForceMode::Velocity:
		return PxForceMode::eVELOCITY_CHANGE;
	case ForceMode::Acceleration:
		return PxForceMode::eACCELERATION;
	}

	return PxForceMode::eFORCE;
}

PxForceMode::Enum ToPxForceMode(PointForceMode mode)
{
	switch(mode)
	{
	case PointForceMode::Force:
		return PxForceMode::eFORCE;
	case PointForceMode::Impulse:
		return PxForceMode::eIMPULSE;
	}

	return PxForceMode::eFORCE;
}

PhysXRigidbody::PhysXRigidbody(PxScene* scene)
{
	const Transform& tfrm = linkedSO->GetTransform();
	PxTransform pxTfrm = ToPxTransform(tfrm.GetPosition(), tfrm.GetRotation());

	//mPxRigidStatic = GetPhysX().GetPhysX()->createRigidStatic(PxTransform(PxIdentity));
	mPxRigidDynamic = GetPhysX().GetPhysX()->createRigidDynamic(pxTfrm);
	mPxRigidDynamic->userData = this;

	scene->addActor(*mPxRigidDynamic);
}

PhysXRigidbody::~PhysXRigidbody()
{
	mPxRigidDynamic->userData = nullptr;
	mPxRigidDynamic->release();
}

void PhysXRigidbody::Move(const Vector3& position)
{
	if(GetIsKinematic())
	{
		PxTransform target;
		if(!mPxRigidDynamic->getKinematicTarget(target))
			target = PxTransform(PxIdentity);

		target.p = ToPxVector(position);

		mPxRigidDynamic->setKinematicTarget(target);
	}
	else
	{
		const PxTransform& pxTransform = mPxRigidDynamic->getGlobalPose();
		SetTransform(position, FromPxQuaternion(pxTransform.q));
	}

	CRigidbody::Move(position);
}

void PhysXRigidbody::Rotate(const Quaternion& rotation)
{
	if(GetIsKinematic())
	{
		PxTransform target;
		if(!mPxRigidDynamic->getKinematicTarget(target))
			target = PxTransform(PxIdentity);

		target.q = ToPxQuaternion(rotation);

		mPxRigidDynamic->setKinematicTarget(target);
	}
	else
	{
		const PxTransform& pxTransform = mPxRigidDynamic->getGlobalPose();
		SetTransform(FromPxVector(pxTransform.p), rotation);
	}

	CRigidbody::Rotate(rotation);
}

void PhysXRigidbody::SetTransform(const Vector3& position, const Quaternion& rotation)
{
	mPxRigidDynamic->setGlobalPose(ToPxTransform(position, rotation));
}

void PhysXRigidbody::GetTransform(Vector3& outPosition, Quaternion& outRotation)
{
	const PxTransform& pxTransform = mPxRigidDynamic->getGlobalPose();

	outPosition = FromPxVector(pxTransform.p);
	outRotation = FromPxQuaternion(pxTransform.q);
}

void PhysXRigidbody::SetMass(float mass)
{
	CRigidbody::SetMass(mass);

	if(((u32)mFlags & (u32)RigidbodyFlag::AutoMass) != 0)
	{
		B3D_LOG(Warning, Physics, "Attempting to set Rigidbody mass, but it has automatic mass calculation turned on.");
		return;
	}

	mPxRigidDynamic->setMass(mass);
}

void PhysXRigidbody::SetIsKinematic(bool kinematic)
{
	CRigidbody::SetIsKinematic(kinematic);

	const bool oldIsKinematic = mPxRigidDynamic->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC;
	if(oldIsKinematic == kinematic)
		return;

	mPxRigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, kinematic);
}

bool PhysXRigidbody::IsSleeping() const
{
	return mPxRigidDynamic->isSleeping();
}

void PhysXRigidbody::Sleep()
{
	mPxRigidDynamic->putToSleep();
}

void PhysXRigidbody::WakeUp()
{
	mPxRigidDynamic->wakeUp();
}

void PhysXRigidbody::SetSleepThreshold(float threshold)
{
	CRigidbody::SetSleepThreshold(threshold);

	mPxRigidDynamic->setSleepThreshold(threshold);
}

void PhysXRigidbody::SetUseGravity(bool gravity)
{
	CRigidbody::SetUseGravity(gravity);

	mPxRigidDynamic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !gravity);
}

void PhysXRigidbody::SetVelocity(const Vector3& velocity)
{
	mPxRigidDynamic->setLinearVelocity(ToPxVector(velocity));
}

Vector3 PhysXRigidbody::GetVelocity() const
{
	return FromPxVector(mPxRigidDynamic->getLinearVelocity());
}

void PhysXRigidbody::SetAngularVelocity(const Vector3& velocity)
{
	mPxRigidDynamic->setAngularVelocity(ToPxVector(velocity));
}

Vector3 PhysXRigidbody::GetAngularVelocity() const
{
	return FromPxVector(mPxRigidDynamic->getAngularVelocity());
}

void PhysXRigidbody::SetDrag(float drag)
{
	CRigidbody::SetDrag(drag);

	mPxRigidDynamic->setLinearDamping(drag);
}

void PhysXRigidbody::SetAngularDrag(float drag)
{
	CRigidbody::SetAngularDrag(drag);

	mPxRigidDynamic->setAngularDamping(drag);
}

void PhysXRigidbody::SetInertiaTensor(const Vector3& tensor)
{
	if(((u32)mFlags & (u32)RigidbodyFlag::AutoTensors) != 0)
	{
		B3D_LOG(Warning, Physics, "Attempting to set Rigidbody inertia tensor, but it has automatic tensor calculation turned on.");
		return;
	}

	CRigidbody::SetInertiaTensor(tensor);

	mPxRigidDynamic->setMassSpaceInertiaTensor(ToPxVector(tensor));
}

Vector3 PhysXRigidbody::GetInertiaTensor() const
{
	return FromPxVector(mPxRigidDynamic->getMassSpaceInertiaTensor());
}

void PhysXRigidbody::SetMaxAngularVelocity(float maxVelocity)
{
	CRigidbody::SetMaxAngularVelocity(maxVelocity);

	mPxRigidDynamic->setMaxAngularVelocity(maxVelocity);
}

void PhysXRigidbody::SetCenterOfMass(const Vector3& position, const Quaternion& rotation)
{
	if(((u32)mFlags & (u32)RigidbodyFlag::AutoTensors) != 0)
	{
		B3D_LOG(Warning, Physics, "Attempting to set Rigidbody center of mass, but it has automatic tensor calculation turned on.");
		return;
	}

	mPxRigidDynamic->setCMassLocalPose(ToPxTransform(position, rotation));
}

Vector3 PhysXRigidbody::GetCenterOfMassPosition() const
{
	const PxTransform& centerOfMassTransform = mPxRigidDynamic->getCMassLocalPose();
	return FromPxVector(centerOfMassTransform.p);
}

Quaternion PhysXRigidbody::GetCenterOfMassRotation() const
{
	const PxTransform& centerOfMassTransform = mPxRigidDynamic->getCMassLocalPose();
	return FromPxQuaternion(centerOfMassTransform.q);
}

void PhysXRigidbody::SetPositionSolverCount(u32 count)
{
	CRigidbody::SetPositionSolverCount(count);

	mPxRigidDynamic->setSolverIterationCounts(std::max(1U, count), GetVelocitySolverCount());
}

void PhysXRigidbody::SetVelocitySolverCount(u32 count)
{
	CRigidbody::SetVelocitySolverCount(count);

	mPxRigidDynamic->setSolverIterationCounts(GetPositionSolverCount(), std::max(1U, count));
}

void PhysXRigidbody::SetFlags(RigidbodyFlag flags)
{
	bool ccdEnabledOld = mPxRigidDynamic->getRigidBodyFlags() & PxRigidBodyFlag::eENABLE_CCD;
	bool ccdEnabledNew = ((u32)flags & (u32)RigidbodyFlag::CCD) != 0;

	if(ccdEnabledOld != ccdEnabledNew)
	{
		mPxRigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, ccdEnabledNew);

		// Enable/disable CCD on shapes so the filter can handle them properly
		u32 numShapes = mPxRigidDynamic->getNbShapes();
		StackMemory<PxShape*> shapes = B3DManagedStackAllocate<PxShape*>(numShapes);

		mPxRigidDynamic->getShapes(shapes, sizeof(PxShape*) * numShapes);

		for(u32 i = 0; i < numShapes; i++)
		{
			ColliderShape* const colliderShape = (ColliderShape*)shapes[i]->userData;
			colliderShape->SetContinuousCollisionDetection(ccdEnabledNew);
		}
	}

	CRigidbody::SetFlags(flags);
}

void PhysXRigidbody::AddForce(const Vector3& force, ForceMode mode)
{
	mPxRigidDynamic->addForce(ToPxVector(force), ToPxForceMode(mode));
}

void PhysXRigidbody::AddTorque(const Vector3& force, ForceMode mode)
{
	mPxRigidDynamic->addTorque(ToPxVector(force), ToPxForceMode(mode));
}

void PhysXRigidbody::AddForceAtPoint(const Vector3& force, const Vector3& position, PointForceMode mode)
{
	const PxVec3& pxForce = ToPxVector(force);
	const PxVec3& pxPos = ToPxVector(position);

	const PxTransform globalPose = mPxRigidDynamic->getGlobalPose();
	PxVec3 centerOfMass = globalPose.transform(mPxRigidDynamic->getCMassLocalPose().p);

	PxForceMode::Enum pxMode = ToPxForceMode(mode);

	PxVec3 torque = (pxPos - centerOfMass).cross(pxForce);
	mPxRigidDynamic->addForce(pxForce, pxMode);
	mPxRigidDynamic->addTorque(torque, pxMode);
}

Vector3 PhysXRigidbody::GetVelocityAtPoint(const Vector3& point) const
{
	const PxVec3& pxPoint = ToPxVector(point);

	const PxTransform globalPose = mPxRigidDynamic->getGlobalPose();
	const PxVec3 centerOfMass = globalPose.transform(mPxRigidDynamic->getCMassLocalPose().p);
	const PxVec3 rpoint = pxPoint - centerOfMass;

	PxVec3 velocity = mPxRigidDynamic->getLinearVelocity();
	velocity += mPxRigidDynamic->getAngularVelocity().cross(rpoint);

	return FromPxVector(velocity);
}

void PhysXRigidbody::UpdateMassDistribution()
{
	if(((u32)mFlags & (u32)RigidbodyFlag::AutoTensors) == 0)
		return;

	if(((u32)mFlags & (u32)RigidbodyFlag::AutoMass) == 0)
	{
		PxRigidBodyExt::setMassAndUpdateInertia(*mPxRigidDynamic, mPxRigidDynamic->getMass());
	}
	else
	{
		const u32 shapeCount = mPxRigidDynamic->getNbShapes();
		if(shapeCount == 0)
		{
			PxRigidBodyExt::setMassAndUpdateInertia(*mPxRigidDynamic, mPxRigidDynamic->getMass());
			return;
		}

		StackMemory<PxShape*> shapes = B3DManagedStackAllocate<PxShape*>(shapeCount);
		mPxRigidDynamic->getShapes(shapes, shapeCount);

		StackMemory<float> masses = B3DManagedStackAllocate<float>(shapeCount);
		for(u32 shapeIndex = 0; shapeIndex < shapeCount; shapeIndex++)
			masses[shapeIndex] = ((ColliderShape*)shapes[shapeIndex]->userData)->GetMass();

		PxRigidBodyExt::setMassAndUpdateInertia(*mPxRigidDynamic, masses, shapeCount);
	}
}

void PhysXRigidbody::AttachShape(const SPtr<ColliderShape>& shape)
{
	if(!B3D_ENSURE(shape != nullptr))
		return;

	const u32 rigidbodyFlags = (u32)GetFlags();
	shape->SetContinuousCollisionDetection((rigidbodyFlags & (u32)RigidbodyFlag::CCD) != 0);

	const PhysXColliderShape& physxShape = static_cast<const PhysXColliderShape&>(*shape);
	mPxRigidDynamic->attachShape(*physxShape.GetPxShape());
}

void PhysXRigidbody::DetachShape(const SPtr<ColliderShape>& shape)
{
	if(!B3D_ENSURE(shape != nullptr))
		return;

	shape->SetContinuousCollisionDetection(false);

	const PhysXColliderShape& physxShape = static_cast<const PhysXColliderShape&>(*shape);
	mPxRigidDynamic->detachShape(*physxShape.GetPxShape());
}
