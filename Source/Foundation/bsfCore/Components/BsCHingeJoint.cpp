//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Components/BsCHingeJoint.h"

#include "Physics/BsPhysics.h"
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
	if(mImplementation == nullptr)
		return Radian(0.0f);

	return GetImplementation().GetAngle();
}

float CHingeJoint::GetSpeed() const
{
	if(mImplementation == nullptr)
		return 0.0f;

	return GetImplementation().GetSpeed();
}

void CHingeJoint::SetLimit(const LimitAngularRange& limit)
{
	if(limit == mInformation.Limit)
		return;

	mInformation.Limit = limit;

	if(mImplementation != nullptr)
		GetImplementation().SetLimit(limit);
}

void CHingeJoint::SetDrive(const HingeJointDrive& drive)
{
	if(drive == mInformation.Drive)
		return;

	mInformation.Drive = drive;

	if(mImplementation != nullptr)
		GetImplementation().SetDrive(drive);
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

	if(mImplementation != nullptr)
		GetImplementation().SetFlag(flag, enabled);
}

bool CHingeJoint::HasFlag(HingeJointFlag flag) const
{
	return ((u32)mInformation.Flag & (u32)flag) != 0;
}

SPtr<IJointImplementation> CHingeJoint::CreateImplementation()
{
	const SPtr<SceneInstance>& scene = SO()->GetScene();
	return scene->GetPhysicsScene()->CreateHingeJoint(*this, mInformation);
}

IHingeJointImplementation& CHingeJoint::GetImplementation() const
{
	return static_cast<IHingeJointImplementation&>(*mImplementation);
}

RTTIType* CHingeJoint::GetRttiStatic()
{
	return CHingeJointRTTI::Instance();
}

RTTIType* CHingeJoint::GetRtti() const
{
	return CHingeJoint::GetRttiStatic();
}
