//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Components/BsCFixedJoint.h"
#include "Scene/BsSceneObject.h"
#include "Components/BsRigidbody.h"
#include "Physics/BsPhysics.h"
#include "Private/RTTI/BsCFixedJointRTTI.h"
#include "Scene/BsSceneInstance.h"

using namespace b3d;

CFixedJoint::CFixedJoint(const HSceneObject& parent)
	: CJoint(parent, mInformation)
{
	SetName("FixedJoint");
}

CFixedJoint::CFixedJoint()
	: CFixedJoint(nullptr)
{ }

SPtr<IJointImplementation> CFixedJoint::CreateImplementation()
{
	const SPtr<SceneInstance>& scene = SO()->GetScene();
	return scene->GetPhysicsScene()->CreateFixedJoint(*this, mInformation);
}

void CFixedJoint::CalculateLocalBodyTransform(JointBody body, Vector3& position, Quaternion& rotation)
{
	position = mInformation.Bodies[(u32)body].Position;
	rotation = mInformation.Bodies[(u32)body].Rotation;

	HRigidbody rigidbody = mInformation.Bodies[(u32)body].Body;
	const Transform& tfrm = SO()->GetTransform();
	if(rigidbody == nullptr) // Get world space transform if no relative to any body
	{
		Quaternion worldRot = tfrm.GetRotation();

		rotation = worldRot * rotation;
		position = worldRot.Rotate(position) + tfrm.GetPosition();
	}
	else
	{
		const Transform& rigidbodyTfrm = rigidbody->SO()->GetTransform();

		// Find world space transform
		Quaternion worldRot = rigidbodyTfrm.GetRotation();

		rotation = worldRot * rotation;
		position = worldRot.Rotate(position) + rigidbodyTfrm.GetPosition();

		// Get transform of the joint local to the object
		Quaternion invRotation = rotation.Inverse();

		position = invRotation.Rotate(tfrm.GetPosition() - position);
		rotation = invRotation * tfrm.GetRotation();
	}
}

IFixedJointImplementation& CFixedJoint::GetImplementation() const
{
	return static_cast<IFixedJointImplementation&>(*mImplementation);
}

RTTIType* CFixedJoint::GetRttiStatic()
{
	return CFixedJointRTTI::Instance();
}

RTTIType* CFixedJoint::GetRtti() const
{
	return CFixedJoint::GetRttiStatic();
}
