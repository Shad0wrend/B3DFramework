//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Components/BsCRigidbody.h"
#include "Scene/BsSceneObject.h"
#include "Components/BsCollider.h"
#include "Components/BsCJoint.h"
#include "Private/RTTI/BsCRigidbodyRTTI.h"
#include "Physics/BsPhysics.h"
#include "Scene/BsSceneInstance.h"

using namespace b3d;

CRigidbody::CRigidbody(const HSceneObject& parent)
	: Component(parent)
{
	SetName("Rigidbody");

	mNotifyFlags = (TransformChangedFlags)(TCF_Parent | TCF_Transform);
}

CRigidbody::CRigidbody()
	: CRigidbody(nullptr)
{ }

void CRigidbody::Move(const Vector3& position)
{
	if(!GetIsKinematic())
	{
		mNotifyFlags = (TransformChangedFlags)0;
		SO()->SetWorldPosition(position);
		mNotifyFlags = (TransformChangedFlags)(TCF_Parent | TCF_Transform);
	}
}

void CRigidbody::Rotate(const Quaternion& rotation)
{
	if(!GetIsKinematic())
	{
		mNotifyFlags = (TransformChangedFlags)0;
		SO()->SetWorldRotation(rotation);
		mNotifyFlags = (TransformChangedFlags)(TCF_Parent | TCF_Transform);
	}
}

void CRigidbody::SetIsKinematic(bool kinematic)
{
	if(mIsKinematic == kinematic)
		return;

	mIsKinematic = kinematic;

	ClearColliders();
	UpdateColliders();
}

void CRigidbody::SetCenterOfMassPosition(const Vector3& position)
{
	mCenterOfMassPosition = position;

	SetCenterOfMass(position, mCenterOfMassRotation);
}

void CRigidbody::SetCenterOfMassRotation(const Quaternion& rotation)
{
	mCenterOfMassRotation = rotation;

	SetCenterOfMass(mCenterOfMassPosition, rotation);
}

void CRigidbody::SetCollisionReportMode(CollisionReportMode mode)
{
	if(mCollisionReportMode == mode)
		return;

	mCollisionReportMode = mode;

	for(auto& entry : mChildColliders)
		entry->UpdateCollisionReportMode();
}

void CRigidbody::SetFlags(RigidbodyFlag flags)
{
	mFlags = flags;

	UpdateMassDistribution();
}

void CRigidbody::UpdateColliders()
{
	// Note: Instead of clearing all, then re-registering below, detect changes between arrays and add/remove only changed
	ClearColliders();

	Stack<HSceneObject> todo;
	todo.push(SO());

	while(!todo.empty())
	{
		HSceneObject currentSO = todo.top();
		todo.pop();

		if(currentSO->HasComponent<Collider>())
		{
			Vector<HCollider> colliders = currentSO->GetComponents<Collider>();

			for(auto& entry : colliders)
			{
				if(!entry->IsValidParent(B3DStaticGameObjectCast<CRigidbody>(mThisHandle)))
					continue;

				entry->SetRigidbody(B3DStaticGameObjectCast<CRigidbody>(mThisHandle));
				mChildColliders.push_back(entry);
			}
		}

		u32 childCount = currentSO->GetChildCount();
		for(u32 i = 0; i < childCount; i++)
		{
			HSceneObject child = currentSO->GetChild(i);

			if(child->HasComponent<CRigidbody>())
				continue;

			todo.push(child);
		}
	}
}

void CRigidbody::ClearColliders()
{
	Vector<HCollider> children = std::move(mChildColliders);
	mChildColliders.clear();

	for(auto& collider : children)
		collider->SetRigidbody(HRigidbody());
}

void CRigidbody::AddCollider(const HCollider& collider)
{
	mChildColliders.push_back(collider);
}

void CRigidbody::RemoveCollider(const HCollider& collider)
{
	auto iterFind = std::find(mChildColliders.begin(), mChildColliders.end(), collider);

	if(iterFind != mChildColliders.end())
		mChildColliders.erase(iterFind);
}

void CRigidbody::CheckForNestedRigibody()
{
	HSceneObject currentSO = SO()->GetParent();

	while(currentSO != nullptr)
	{
		if(currentSO->HasComponent<CRigidbody>())
		{
			B3D_LOG(Warning, Physics, "Nested Rigidbodies detected. This will result in inconsistent transformations. "
									 "To parent one Rigidbody to another move its colliders to the new parent, but remove the Rigidbody "
									 "component.");
			return;
		}

		currentSO = currentSO->GetParent();
	}
}

void CRigidbody::ProcessCollisionData(const CollisionDataRaw& data, CollisionData& output)
{
	output.ContactPoints = std::move(data.ContactPoints);

	ColliderShape* const myColliderShape = data.ColliderShapes[0];
	if(myColliderShape != nullptr)
	{
		Collider* const myCollider = myColliderShape->GetParentCollider();
		output.Collider[0] = B3DStaticGameObjectCast<Collider>(myCollider->GetHandle());
		output.ColliderShapes[0] = myCollider->GetShapes()[myColliderShape->GetShapeIndexInParent()];
	}

	ColliderShape* const otherColliderShape = data.ColliderShapes[1];
	if(otherColliderShape != nullptr)
	{
		Collider* const otherCollider = otherColliderShape->GetParentCollider();
		output.Collider[1] = B3DStaticGameObjectCast<Collider>(otherCollider->GetHandle());
		output.ColliderShapes[1] = otherCollider->GetShapes()[otherColliderShape->GetShapeIndexInParent()];
	}
}

void CRigidbody::DestroyInternal()
{
	ClearColliders();

	if(mInternal)
	{
		mInternal->SetOwnerInternal(PhysicsOwnerType::None, nullptr);
		mInternal = nullptr;
	}
}

void CRigidbody::OnDestroyed()
{
	// TODO:
	// - Clear colliders (this should also detach shapes)
	// - Unregister low-level rigidbody from scene
	// - Destroy low-level rigidbody

	// TODO - Ideally, we should not clear colliders as long as rigidbody is relevant to the collider. This way we can avoid this potentially expensive step when
	// starting/stopping scene (but will this even matter in a standalone build?)

	DestroyInternal();
}

void CRigidbody::OnDisabled()
{
	// TODO:
	// - Clear colliders (this should also detach shapes)
	// - Unregister low-level rigidbody from scene

	DestroyInternal();
}

void CRigidbody::OnCreated()
{
	// TODO:
	// - Create low-level rigidbody
	// - Apply all deserialized properties to low-level rigidbody (e.g. mLinearDrag, etc.)
	// - Find child colliders (this should attach shapes)
	// - Register low-level rigidbody in the scene
	// - Set current transform from scene object
	// - Update mass distribution
	
}

void CRigidbody::OnEnabled()
{
	// TODO:
	// - Find child colliders (this should attach shapes)
	// - Register low-level rigidbody in the scene
	// - Set current transform from scene object

	mInternal = Rigidbody::Create(SO());
	mInternal->SetOwnerInternal(PhysicsOwnerType::Component, this);

	UpdateColliders();

#if B3D_DEBUG
	CheckForNestedRigibody();
#endif

	mInternal->OnCollisionBegin.Connect(std::bind(&CRigidbody::TriggerOnCollisionBegin, this, _1));
	mInternal->OnCollisionStay.Connect(std::bind(&CRigidbody::TriggerOnCollisionStay, this, _1));
	mInternal->OnCollisionEnd.Connect(std::bind(&CRigidbody::TriggerOnCollisionEnd, this, _1));

	const Transform& tfrm = SO()->GetTransform();
	mInternal->SetTransform(tfrm.GetPosition(), tfrm.GetRotation());

	// Note: Merge into one call to avoid many virtual function calls
	mInternal->SetPositionSolverCount(mPositionSolverCount);
	mInternal->SetVelocitySolverCount(mVelocitySolverCount);
	mInternal->SetMaxAngularVelocity(mMaxAngularVelocity);
	mInternal->SetDrag(mLinearDrag);
	mInternal->SetAngularDrag(mAngularDrag);
	mInternal->SetSleepThreshold(mSleepThreshold);
	mInternal->SetUseGravity(mUseGravity);
	mInternal->SetIsKinematic(mIsKinematic);
	mInternal->SetFlags(mFlags);

	if(((u32)mFlags & (u32)RigidbodyFlag::AutoTensors) == 0)
	{
		mInternal->SetCenterOfMass(mCenterOfMassPosition, mCenterOfMassRotation);
		mInternal->SetInertiaTensor(mInertiaTensor);
		mInternal->SetMass(mMass);
	}
	else
	{
		if(((u32)mFlags & (u32)RigidbodyFlag::AutoMass) == 0)
			mInternal->SetMass(mMass);

		mInternal->UpdateMassDistribution();
	}
}

void CRigidbody::OnTransformChanged(TransformChangedFlags flags)
{
	if((flags & TCF_Parent) != 0)
	{
		ClearColliders();
		UpdateColliders();

		if(((u32)mFlags & (u32)RigidbodyFlag::AutoTensors) != 0)
			UpdateMassDistribution();

#if B3D_DEBUG
		CheckForNestedRigibody();
#endif
	}

	const SPtr<SceneInstance>& scene = SceneObject()->GetScene();
	const SPtr<PhysicsScene>& physicsScene = scene->GetPhysicsScene();

	if(physicsScene->IsUpdateInProgress())
		return;

	const Transform& transform = SO()->GetTransform();
	SetTransform(transform.GetPosition(), transform.GetRotation());

	if(mParentJoint != nullptr)
		mParentJoint->NotifyRigidbodyMoved(B3DStaticGameObjectCast<CRigidbody>(mThisHandle));
}

RTTIType* CRigidbody::GetRttiStatic()
{
	return CRigidbodyRTTI::Instance();
}

RTTIType* CRigidbody::GetRtti() const
{
	return CRigidbody::GetRttiStatic();
}
