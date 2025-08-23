//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Components/BsCSphericalJoint.h"

#include "Physics/BsPhysics.h"
#include "Scene/BsSceneObject.h"
#include "Private/RTTI/BsCSphericalJointRTTI.h"
#include "Scene/BsSceneInstance.h"

using namespace b3d;

CSphericalJoint::CSphericalJoint(const HSceneObject& parent)
	: CJoint(parent, mInformation)
{
	SetName("SphericalJoint");
}

CSphericalJoint::CSphericalJoint()
	: CSphericalJoint(nullptr)
{ }

void CSphericalJoint::SetLimit(const LimitConeRange& limit)
{
	if(limit == mInformation.Limit)
		return;

	mInformation.Limit = limit;

	if(mImplementation != nullptr)
		GetImplementation().SetLimit(limit);
}

void CSphericalJoint::SetFlag(SphericalJointFlag flag, bool enabled)
{
	bool isEnabled = ((u32)mInformation.Flag & (u32)flag) != 0;
	if(isEnabled == enabled)
		return;

	if(enabled)
		mInformation.Flag = (SphericalJointFlag)((u32)mInformation.Flag | (u32)flag);
	else
		mInformation.Flag = (SphericalJointFlag)((u32)mInformation.Flag & ~(u32)flag);

	if(mImplementation != nullptr)
		GetImplementation().SetFlag(flag, enabled);
}

bool CSphericalJoint::HasFlag(SphericalJointFlag flag) const
{
	return ((u32)mInformation.Flag & (u32)flag) != 0;
}

SPtr<IJointImplementation> CSphericalJoint::CreateImplementation()
{
	const SPtr<SceneInstance>& scene = SO()->GetScene();
	return scene->GetPhysicsScene()->CreateSphericalJoint(*this, mInformation);
}

ISphericalJointImplementation& CSphericalJoint::GetImplementation() const
{
	return static_cast<ISphericalJointImplementation&>(*mImplementation);
}

RTTIType* CSphericalJoint::GetRttiStatic()
{
	return CSphericalJointRTTI::Instance();
}

RTTIType* CSphericalJoint::GetRtti() const
{
	return CSphericalJoint::GetRttiStatic();
}
