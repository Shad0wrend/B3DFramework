//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Components/BsCHingeJoint.h"
#include "Scene/BsSceneObject.h"
#include "Private/RTTI/BsCHingeJointRTTI.h"
#include "Scene/BsSceneManager.h"

namespace bs
{
	CHingeJoint::CHingeJoint()
		: CJoint(mDesc)
	{
		setName("HingeJoint");
	}

	CHingeJoint::CHingeJoint(const HSceneObject& parent)
		: CJoint(parent, mDesc)
	{
		setName("HingeJoint");
	}

	Radian CHingeJoint::GetAngle() const
	{
		if (mInternal == nullptr)
			return Radian(0.0f);

		return GetInternalInternal()->GetAngle();
	}

	float CHingeJoint::GetSpeed() const
	{
		if (mInternal == nullptr)
			return 0.0f;

		return GetInternalInternal()->GetSpeed();
	}

	LimitAngularRange CHingeJoint::GetLimit() const
	{
		return mDesc.limit;
	}

	void CHingeJoint::SetLimit(const LimitAngularRange& limit)
	{
		if (limit == mDesc.limit)
			return;

		mDesc.limit = limit;

		if (mInternal != nullptr)
			GetInternalInternal()->SetLimit(limit);
	}

	HingeJointDrive CHingeJoint::GetDrive() const
	{
		return mDesc.drive;
	}

	void CHingeJoint::SetDrive(const HingeJointDrive& drive)
	{
		if (drive == mDesc.drive)
			return;

		mDesc.drive = drive;

		if (mInternal != nullptr)
			GetInternalInternal()->SetDrive(drive);
	}

	void CHingeJoint::SetFlag(HingeJointFlag flag, bool enabled)
	{
		bool isEnabled = ((UINT32)mDesc.flag & (UINT32)flag) != 0;
		if (isEnabled == enabled)
			return;

		if (enabled)
			mDesc.flag = (HingeJointFlag)((UINT32)mDesc.flag | (UINT32)flag);
		else
			mDesc.flag = (HingeJointFlag)((UINT32)mDesc.flag & ~(UINT32)flag);

		if (mInternal != nullptr)
			GetInternalInternal()->SetFlag(flag, enabled);
	}

	bool CHingeJoint::HasFlag(HingeJointFlag flag) const
	{
		return ((UINT32)mDesc.flag & (UINT32)flag) != 0;
	}

	SPtr<Joint> CHingeJoint::CreateInternal()
	{
		const SPtr<SceneInstance>& scene = SO()->getScene();
		SPtr<Joint> joint = HingeJoint::Create(*scene->getPhysicsScene(), mDesc);

		joint->SetOwnerInternal(PhysicsOwnerType::Component, this);
		return joint;
	}

	RTTITypeBase* CHingeJoint::GetRttiStatic()
	{
		return CHingeJointRTTI::Instance();
	}

	RTTITypeBase* CHingeJoint::GetRtti() const
	{
		return CHingeJoint::GetRttiStatic();
	}
}
