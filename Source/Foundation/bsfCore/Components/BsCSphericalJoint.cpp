//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Components/BsCSphericalJoint.h"
#include "Scene/BsSceneObject.h"
#include "Private/RTTI/BsCSphericalJointRTTI.h"
#include "Scene/BsSceneManager.h"

namespace bs
{
	CSphericalJoint::CSphericalJoint()
		: CJoint(mDesc)
	{
		setName("SphericalJoint");
	}

	CSphericalJoint::CSphericalJoint(const HSceneObject& parent)
		: CJoint(parent, mDesc)
	{
		setName("SphericalJoint");
	}

	LimitConeRange CSphericalJoint::GetLimit() const
	{
		return mDesc.limit;
	}

	void CSphericalJoint::SetLimit(const LimitConeRange& limit)
	{
		if (limit == mDesc.limit)
			return;

		mDesc.limit = limit;

		if (mInternal != nullptr)
			GetInternalInternal()->setLimit(limit);
	}
	
	void CSphericalJoint::SetFlag(SphericalJointFlag flag, bool enabled)
	{
		bool isEnabled = ((UINT32)mDesc.flag & (UINT32)flag) != 0;
		if (isEnabled == enabled)
			return;

		if (enabled)
			mDesc.flag = (SphericalJointFlag)((UINT32)mDesc.flag | (UINT32)flag);
		else
			mDesc.flag = (SphericalJointFlag)((UINT32)mDesc.flag & ~(UINT32)flag);

		if (mInternal != nullptr)
			GetInternalInternal()->setFlag(flag, enabled);
	}

	bool CSphericalJoint::HasFlag(SphericalJointFlag flag) const
	{
		return ((UINT32)mDesc.flag & (UINT32)flag) != 0;
	}

	SPtr<Joint> CSphericalJoint::CreateInternal()
	{
		const SPtr<SceneInstance>& scene = SO()->getScene();
		SPtr<Joint> joint = SphericalJoint::Create(*scene->getPhysicsScene(), mDesc);

		joint->SetOwnerInternal(PhysicsOwnerType::Component, this);
		return joint;
	}

	RTTITypeBase* CSphericalJoint::GetRttiStatic()
	{
		return CSphericalJointRTTI::Instance();
	}

	RTTITypeBase* CSphericalJoint::GetRtti() const
	{
		return CSphericalJoint::GetRttiStatic();
	}
}
