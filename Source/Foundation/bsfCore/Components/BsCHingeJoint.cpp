//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Components/BsCHingeJoint.h"
#include "Scene/BsSceneObject.h"
#include "Private/RTTI/BsCHingeJointRTTI.h"
#include "Scene/BsSceneInstance.h"

using namespace b3d;

CHingeJoint::CHingeJoint(const HSceneObject& parent)
	: CJoint(parent, mInformation)
{
	SetName("HingeJoint");
}

CHingeJoint::CHingeJoint()
	: CHingeJoint(nullptr)
{ }

Radian CHingeJoint::GetAngle() const
{
	if(mInternal == nullptr)
		return Radian(0.0f);

	return GetInternalInternal()->GetAngle();
}

float CHingeJoint::GetSpeed() const
{
	if(mInternal == nullptr)
		return 0.0f;

	return GetInternalInternal()->GetSpeed();
}

LimitAngularRange CHingeJoint::GetLimit() const
{
	return mInformation.Limit;
}

void CHingeJoint::SetLimit(const LimitAngularRange& limit)
{
	if(limit == mInformation.Limit)
		return;

	mInformation.Limit = limit;

	if(mInternal != nullptr)
		GetInternalInternal()->SetLimit(limit);
}

HingeJointDrive CHingeJoint::GetDrive() const
{
	return mInformation.Drive;
}

void CHingeJoint::SetDrive(const HingeJointDrive& drive)
{
	if(drive == mInformation.Drive)
		return;

	mInformation.Drive = drive;

	if(mInternal != nullptr)
		GetInternalInternal()->SetDrive(drive);
}

void CHingeJoint::SetFlag(HingeJointFlag flag, bool enabled)
{
	bool isEnabled = ((u32)mInformation.Flag & (u32)flag) != 0;
	if(isEnabled == enabled)
		return;

	if(enabled)
		mInformation.Flag = (HingeJointFlag)((u32)mInformation.Flag | (u32)flag);
	else
		mInformation.Flag = (HingeJointFlag)((u32)mInformation.Flag & ~(u32)flag);

	if(mInternal != nullptr)
		GetInternalInternal()->SetFlag(flag, enabled);
}

bool CHingeJoint::HasFlag(HingeJointFlag flag) const
{
	return ((u32)mInformation.Flag & (u32)flag) != 0;
}

SPtr<Joint> CHingeJoint::CreateInternal()
{
	const SPtr<SceneInstance>& scene = SO()->GetScene();
	SPtr<Joint> joint = HingeJoint::Create(*scene->GetPhysicsScene(), mInformation);

	joint->SetOwnerInternal(PhysicsOwnerType::Component, this);
	return joint;
}

RTTIType* CHingeJoint::GetRttiStatic()
{
	return CHingeJointRTTI::Instance();
}

RTTIType* CHingeJoint::GetRtti() const
{
	return CHingeJoint::GetRttiStatic();
}
