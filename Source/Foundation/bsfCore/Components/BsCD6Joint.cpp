//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Components/BsCD6Joint.h"

#include "Physics/BsPhysics.h"
#include "Scene/BsSceneObject.h"
#include "Private/RTTI/BsCD6JointRTTI.h"
#include "Scene/BsSceneInstance.h"

using namespace b3d;

RTTIType* D6JointDrive::GetRttiStatic()
{
	return D6JointDriveRTTI::Instance();
}

RTTIType* D6JointDrive::GetRtti() const
{
	return GetRttiStatic();
}

CD6Joint::CD6Joint(const HSceneObject& parent)
	: CJoint(parent, mInformation)
{
	SetName("D6Joint");
}

CD6Joint::CD6Joint()
	: CD6Joint(nullptr)
{ }

void CD6Joint::SetMotion(D6JointAxis axis, D6JointMotion motion)
{
	if(mInformation.Motion[(int)axis] == motion)
		return;

	mInformation.Motion[(int)axis] = motion;

	if(mImplementation != nullptr)
		GetImplementation().SetMotion(axis, motion);
}

Radian CD6Joint::GetTwist() const
{
	if(mImplementation == nullptr)
		return Radian(0.0f);

	return GetImplementation().GetTwist();
}

Radian CD6Joint::GetSwingY() const
{
	if(mImplementation == nullptr)
		return Radian(0.0f);

	return GetImplementation().GetSwingY();
}

Radian CD6Joint::GetSwingZ() const
{
	if(mImplementation == nullptr)
		return Radian(0.0f);

	return GetImplementation().GetSwingZ();
}

void CD6Joint::SetLimitLinear(const LimitLinear& limit)
{
	if(mInformation.LimitLinear == limit)
		return;

	mInformation.LimitLinear = limit;

	if(mImplementation != nullptr)
		GetImplementation().SetLimitLinear(limit);
}

void CD6Joint::SetLimitTwist(const LimitAngularRange& limit)
{
	if(mInformation.LimitTwist == limit)
		return;

	mInformation.LimitTwist = limit;

	if(mImplementation != nullptr)
		GetImplementation().SetLimitTwist(limit);
}

void CD6Joint::SetLimitSwing(const LimitConeRange& limit)
{
	if(mInformation.LimitSwing == limit)
		return;

	mInformation.LimitSwing = limit;

	if(mImplementation != nullptr)
		GetImplementation().SetLimitSwing(limit);
}

D6JointDrive CD6Joint::GetDrive(D6JointDriveType type) const
{
	return mInformation.Drive[(int)type];
}

void CD6Joint::SetDrive(D6JointDriveType type, const D6JointDrive& drive)
{
	if(mInformation.Drive[(int)type] == drive)
		return;

	mInformation.Drive[(int)type] = drive;

	if(mImplementation != nullptr)
		GetImplementation().SetDrive(type, drive);
}

void CD6Joint::SetDriveTransform(const Vector3& position, const Quaternion& rotation)
{
	if(mInformation.DrivePosition == position && mInformation.DriveRotation == rotation)
		return;

	mInformation.DrivePosition = position;
	mInformation.DriveRotation = rotation;

	if(mImplementation != nullptr)
		GetImplementation().SetDriveTransform(position, rotation);
}

void CD6Joint::SetDriveVelocity(const Vector3& linear, const Vector3& angular)
{
	if(mInformation.DriveLinearVelocity == linear && mInformation.DriveAngularVelocity == angular)
		return;

	mInformation.DriveLinearVelocity = linear;
	mInformation.DriveAngularVelocity = angular;

	if(mImplementation != nullptr)
		GetImplementation().SetDriveVelocity(linear, angular);
}

SPtr<IJointImplementation> CD6Joint::CreateImplementation()
{
	const SPtr<SceneInstance>& scene = SO()->GetScene();
	return scene->GetPhysicsScene()->CreateD6Joint(*this, mInformation);
}

ID6JointImplementation& CD6Joint::GetImplementation() const
{
	return static_cast<ID6JointImplementation&>(*mImplementation);
}

RTTIType* CD6Joint::GetRttiStatic()
{
	return CD6JointRTTI::Instance();
}

RTTIType* CD6Joint::GetRtti() const
{
	return CD6Joint::GetRttiStatic();
}
