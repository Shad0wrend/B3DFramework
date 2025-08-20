//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Components/BsCDistanceJoint.h"
#include "Scene/BsSceneObject.h"
#include "Private/RTTI/BsCDistanceJointRTTI.h"
#include "Scene/BsSceneInstance.h"

using namespace b3d;

CDistanceJoint::CDistanceJoint(const HSceneObject& parent)
	: CJoint(parent, mInformation)
{
	SetName("DistanceJoint");
}

CDistanceJoint::CDistanceJoint()
	: CDistanceJoint(nullptr)
{ }

float CDistanceJoint::GetDistance() const
{
	if(mInternal == nullptr)
		return 0.0f;

	return GetInternalInternal()->GetDistance();
}

float CDistanceJoint::GetMinDistance() const
{
	return mInformation.MinDistance;
}

void CDistanceJoint::SetMinDistance(float value)
{
	if(mInformation.MinDistance == value)
		return;

	mInformation.MinDistance = value;

	if(mInternal != nullptr)
		GetInternalInternal()->SetMinDistance(value);
}

float CDistanceJoint::GetMaxDistance() const
{
	return mInformation.MaxDistance;
}

void CDistanceJoint::SetMaxDistance(float value)
{
	if(mInformation.MaxDistance == value)
		return;

	mInformation.MaxDistance = value;

	if(mInternal != nullptr)
		GetInternalInternal()->SetMaxDistance(value);
}

float CDistanceJoint::GetTolerance() const
{
	return mInformation.Tolerance;
}

void CDistanceJoint::SetTolerance(float value)
{
	if(mInformation.Tolerance == value)
		return;

	mInformation.Tolerance = value;

	if(mInternal != nullptr)
		GetInternalInternal()->SetTolerance(value);
}

Spring CDistanceJoint::GetSpring() const
{
	return mInformation.Spring;
}

void CDistanceJoint::SetSpring(const Spring& value)
{
	if(mInformation.Spring == value)
		return;

	mInformation.Spring = value;

	if(mInternal != nullptr)
		GetInternalInternal()->SetSpring(value);
}

void CDistanceJoint::SetFlag(DistanceJointFlag flag, bool enabled)
{
	bool isEnabled = ((u32)mInformation.Flag & (u32)flag) != 0;
	if(isEnabled == enabled)
		return;

	if(enabled)
		mInformation.Flag = (DistanceJointFlag)((u32)mInformation.Flag | (u32)flag);
	else
		mInformation.Flag = (DistanceJointFlag)((u32)mInformation.Flag & ~(u32)flag);

	if(mInternal != nullptr)
		GetInternalInternal()->SetFlag(flag, enabled);
}

bool CDistanceJoint::HasFlag(DistanceJointFlag flag) const
{
	return ((u32)mInformation.Flag & (u32)flag) != 0;
}

SPtr<Joint> CDistanceJoint::CreateInternal()
{
	const SPtr<SceneInstance>& scene = SO()->GetScene();
	SPtr<Joint> joint = DistanceJoint::Create(*scene->GetPhysicsScene(), mInformation);

	joint->SetOwnerInternal(PhysicsOwnerType::Component, this);
	return joint;
}

RTTIType* CDistanceJoint::GetRttiStatic()
{
	return CDistanceJointRTTI::Instance();
}

RTTIType* CDistanceJoint::GetRtti() const
{
	return CDistanceJoint::GetRttiStatic();
}
