//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsPhysXRigidbody.h"
#include "Physics/BsCollider.h"
#include "Scene/BsSceneObject.h"
#include "Physics/BsPhysics.h"
#include "PxRigidDynamic.h"
#include "PxScene.h"
#include "extensions/PxRigidBodyExt.h"
#include "BsPhysX.h"

using namespace physx;

using namespace bs;

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

PhysXRigidbody::PhysXRigidbody(PxPhysics* physx, PxScene* scene, const HSceneObject& linkedSO)
	: Rigidbody(linkedSO)
{
	const Transform& tfrm = linkedSO->GetTransform();
	PxTransform pxTfrm = ToPxTransform(tfrm.GetPosition(), tfrm.GetRotation());

	mInternal = physx->createRigidDynamic(pxTfrm);
	mInternal->userData = this;

	scene->addActor(*mInternal);
}

PhysXRigidbody::~PhysXRigidbody()
{
	mInternal->userData = nullptr;
	mInternal->release();
}

void PhysXRigidbody::Move(const Vector3& position)
{
	if(GetIsKinematic())
	{
		PxTransform target;
		if(!mInternal->getKinematicTarget(target))
			target = PxTransform(PxIdentity);

		target.p = ToPxVector(position);

		mInternal->setKinematicTarget(target);
	}
	else
	{
		SetTransform(position, GetRotation());
	}
}

void PhysXRigidbody::Rotate(const Quaternion& rotation)
{
	if(GetIsKinematic())
	{
		PxTransform target;
		if(!mInternal->getKinematicTarget(target))
			target = PxTransform(PxIdentity);

		target.q = ToPxQuaternion(rotation);

		mInternal->setKinematicTarget(target);
	}
	else
	{
		SetTransform(GetPosition(), rotation);
	}
}

Vector3 PhysXRigidbody::GetPosition() const
{
	return FromPxVector(mInternal->getGlobalPose().p);
}

Quaternion PhysXRigidbody::GetRotation() const
{
	return FromPxQuaternion(mInternal->getGlobalPose().q);
}

void PhysXRigidbody::SetTransform(const Vector3& pos, const Quaternion& rot)
{
	mInternal->setGlobalPose(ToPxTransform(pos, rot));
}

void PhysXRigidbody::SetMass(float mass)
{
	if(((u32)mFlags & (u32)RigidbodyFlag::AutoMass) != 0)
	{
		B3D_LOG(Warning, Physics, "Attempting to set Rigidbody mass, but it has automatic mass calculation turned on.");
		return;
	}

	mInternal->setMass(mass);
}

float PhysXRigidbody::GetMass() const
{
	return mInternal->getMass();
}

void PhysXRigidbody::SetIsKinematic(bool kinematic)
{
	mInternal->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, kinematic);
}

bool PhysXRigidbody::GetIsKinematic() const
{
	return ((u32)mInternal->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC) != 0;
}

bool PhysXRigidbody::IsSleeping() const
{
	return mInternal->isSleeping();
}

void PhysXRigidbody::Sleep()
{
	mInternal->putToSleep();
}

void PhysXRigidbody::WakeUp()
{
	mInternal->wakeUp();
}

void PhysXRigidbody::SetSleepThreshold(float threshold)
{
	mInternal->setSleepThreshold(threshold);
}

float PhysXRigidbody::GetSleepThreshold() const
{
	return mInternal->getSleepThreshold();
}

void PhysXRigidbody::SetUseGravity(bool gravity)
{
	mInternal->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !gravity);
}

bool PhysXRigidbody::GetUseGravity() const
{
	return ((u32)mInternal->getActorFlags() & PxActorFlag::eDISABLE_GRAVITY) == 0;
}

void PhysXRigidbody::SetVelocity(const Vector3& velocity)
{
	mInternal->setLinearVelocity(ToPxVector(velocity));
}

Vector3 PhysXRigidbody::GetVelocity() const
{
	return FromPxVector(mInternal->getLinearVelocity());
}

void PhysXRigidbody::SetAngularVelocity(const Vector3& velocity)
{
	mInternal->setAngularVelocity(ToPxVector(velocity));
}

Vector3 PhysXRigidbody::GetAngularVelocity() const
{
	return FromPxVector(mInternal->getAngularVelocity());
}

void PhysXRigidbody::SetDrag(float drag)
{
	mInternal->setLinearDamping(drag);
}

float PhysXRigidbody::GetDrag() const
{
	return mInternal->getLinearDamping();
}

void PhysXRigidbody::SetAngularDrag(float drag)
{
	mInternal->setAngularDamping(drag);
}

float PhysXRigidbody::GetAngularDrag() const
{
	return mInternal->getAngularDamping();
}

void PhysXRigidbody::SetInertiaTensor(const Vector3& tensor)
{
	if(((u32)mFlags & (u32)RigidbodyFlag::AutoTensors) != 0)
	{
		B3D_LOG(Warning, Physics, "Attempting to set Rigidbody inertia tensor, but it has automatic tensor calculation turned on.");
		return;
	}

	mInternal->setMassSpaceInertiaTensor(ToPxVector(tensor));
}

Vector3 PhysXRigidbody::GetInertiaTensor() const
{
	return FromPxVector(mInternal->getMassSpaceInertiaTensor());
}

void PhysXRigidbody::SetMaxAngularVelocity(float maxVelocity)
{
	mInternal->setMaxAngularVelocity(maxVelocity);
}

float PhysXRigidbody::GetMaxAngularVelocity() const
{
	return mInternal->getMaxAngularVelocity();
}

void PhysXRigidbody::SetCenterOfMass(const Vector3& position, const Quaternion& rotation)
{
	if(((u32)mFlags & (u32)RigidbodyFlag::AutoTensors) != 0)
	{
		B3D_LOG(Warning, Physics, "Attempting to set Rigidbody center of mass, but it has automatic tensor calculation turned on.");
		return;
	}

	mInternal->setCMassLocalPose(ToPxTransform(position, rotation));
}

Vector3 PhysXRigidbody::GetCenterOfMassPosition() const
{
	PxTransform cMassTfrm = mInternal->getCMassLocalPose();
	return FromPxVector(cMassTfrm.p);
}

Quaternion PhysXRigidbody::GetCenterOfMassRotation() const
{
	PxTransform cMassTfrm = mInternal->getCMassLocalPose();
	return FromPxQuaternion(cMassTfrm.q);
}

void PhysXRigidbody::SetPositionSolverCount(u32 count)
{
	mInternal->setSolverIterationCounts(std::max(1U, count), GetVelocitySolverCount());
}

u32 PhysXRigidbody::GetPositionSolverCount() const
{
	u32 posCount = 1;
	u32 velCount = 1;

	mInternal->getSolverIterationCounts(posCount, velCount);
	return posCount;
}

void PhysXRigidbody::SetVelocitySolverCount(u32 count)
{
	mInternal->setSolverIterationCounts(GetPositionSolverCount(), std::max(1U, count));
}

u32 PhysXRigidbody::GetVelocitySolverCount() const
{
	u32 posCount = 1;
	u32 velCount = 1;

	mInternal->getSolverIterationCounts(posCount, velCount);
	return velCount;
}

void PhysXRigidbody::SetFlags(RigidbodyFlag flags)
{
	bool ccdEnabledOld = mInternal->getRigidBodyFlags() & PxRigidBodyFlag::eENABLE_CCD;
	bool ccdEnabledNew = ((u32)flags & (u32)RigidbodyFlag::CCD) != 0;

	if(ccdEnabledOld != ccdEnabledNew)
	{
		mInternal->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, ccdEnabledNew);

		// Enable/disable CCD on shapes so the filter can handle them properly
		u32 numShapes = mInternal->getNbShapes();
		StackMemory<PxShape*> shapes = B3DManagedStackAllocate<PxShape*>(numShapes);

		mInternal->getShapes(shapes, sizeof(PxShape*) * numShapes);

		for(u32 i = 0; i < numShapes; i++)
		{
			ColliderShape* const colliderShape = (ColliderShape*)shapes[i]->userData;
			colliderShape->SetContinuousCollisionDetection(ccdEnabledNew);
		}
	}

	Rigidbody::SetFlags(flags);
}

void PhysXRigidbody::AddForce(const Vector3& force, ForceMode mode)
{
	mInternal->addForce(ToPxVector(force), ToPxForceMode(mode));
}

void PhysXRigidbody::AddTorque(const Vector3& force, ForceMode mode)
{
	mInternal->addTorque(ToPxVector(force), ToPxForceMode(mode));
}

void PhysXRigidbody::AddForceAtPoint(const Vector3& force, const Vector3& position, PointForceMode mode)
{
	const PxVec3& pxForce = ToPxVector(force);
	const PxVec3& pxPos = ToPxVector(position);

	const PxTransform globalPose = mInternal->getGlobalPose();
	PxVec3 centerOfMass = globalPose.transform(mInternal->getCMassLocalPose().p);

	PxForceMode::Enum pxMode = ToPxForceMode(mode);

	PxVec3 torque = (pxPos - centerOfMass).cross(pxForce);
	mInternal->addForce(pxForce, pxMode);
	mInternal->addTorque(torque, pxMode);
}

Vector3 PhysXRigidbody::GetVelocityAtPoint(const Vector3& point) const
{
	const PxVec3& pxPoint = ToPxVector(point);

	const PxTransform globalPose = mInternal->getGlobalPose();
	const PxVec3 centerOfMass = globalPose.transform(mInternal->getCMassLocalPose().p);
	const PxVec3 rpoint = pxPoint - centerOfMass;

	PxVec3 velocity = mInternal->getLinearVelocity();
	velocity += mInternal->getAngularVelocity().cross(rpoint);

	return FromPxVector(velocity);
}

void PhysXRigidbody::UpdateMassDistribution()
{
	if(((u32)mFlags & (u32)RigidbodyFlag::AutoTensors) == 0)
		return;

	if(((u32)mFlags & (u32)RigidbodyFlag::AutoMass) == 0)
	{
		PxRigidBodyExt::setMassAndUpdateInertia(*mInternal, mInternal->getMass());
	}
	else
	{
		const u32 shapeCount = mInternal->getNbShapes();
		if(shapeCount == 0)
		{
			PxRigidBodyExt::setMassAndUpdateInertia(*mInternal, mInternal->getMass());
			return;
		}

		StackMemory<PxShape*> shapes = B3DManagedStackAllocate<PxShape*>(shapeCount);
		mInternal->getShapes(shapes, shapeCount);

		StackMemory<float> masses = B3DManagedStackAllocate<float>(shapeCount);
		for(u32 shapeIndex = 0; shapeIndex < shapeCount; shapeIndex++)
			masses[shapeIndex] = ((ColliderShape*)shapes[shapeIndex]->userData)->GetMass();

		PxRigidBodyExt::setMassAndUpdateInertia(*mInternal, masses, shapeCount);
	}
}
