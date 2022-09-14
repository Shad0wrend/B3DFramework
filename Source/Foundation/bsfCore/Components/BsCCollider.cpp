//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Components/BsCCollider.h"
#include "Scene/BsSceneObject.h"
#include "Components/BsCRigidbody.h"
#include "Physics/BsPhysics.h"
#include "Private/RTTI/BsCColliderRTTI.h"

using namespace std::placeholders;

namespace bs
{
	CCollider::CCollider()
	{
		SetName("Collider");

		mNotifyFlags = (TransformChangedFlags)(TCF_Parent | TCF_Transform);
	}

	CCollider::CCollider(const HSceneObject& parent)
		: Component(parent)
	{
		SetName("Collider");

		mNotifyFlags = (TransformChangedFlags)(TCF_Parent | TCF_Transform);
	}

	void CCollider::SetIsTrigger(bool value)
	{
		if (mIsTrigger == value)
			return;

		mIsTrigger = value;

		if (mInternal != nullptr)
		{
			mInternal->SetIsTrigger(value);

			UpdateParentRigidbody();
			UpdateTransform();
		}
	}

	void CCollider::SetMass(float mass)
	{
		if (mMass == mass)
			return;

		mMass = mass;

		if (mInternal != nullptr)
		{
			mInternal->SetMass(mass);

			if (mParent != nullptr)
				mParent->UpdateMassDistributionInternal();
		}
	}

	void CCollider::SetMaterial(const HPhysicsMaterial& material)
	{
		mMaterial = material;

		if (mInternal != nullptr)
			mInternal->SetMaterial(material);
	}

	void CCollider::SetContactOffset(float value)
	{
		value = std::max(0.0f, std::max(value, GetRestOffset()));

		mContactOffset = value;

		if (mInternal != nullptr)
			mInternal->SetContactOffset(value);
	}

	void CCollider::SetRestOffset(float value)
	{
		value = std::min(value, GetContactOffset());

		mRestOffset = value;

		if (mInternal != nullptr)
			mInternal->SetRestOffset(value);
	}

	void CCollider::SetLayer(UINT64 layer)
	{
		mLayer = layer;

		if (mInternal != nullptr)
			mInternal->SetLayer(layer);
	}

	void CCollider::SetCollisionReportMode(CollisionReportMode mode)
	{
		mCollisionReportMode = mode;

		if (mInternal != nullptr)
			UpdateCollisionReportMode();
	}

	void CCollider::OnInitialized()
	{

	}

	void CCollider::OnDestroyed()
	{
		DestroyInternal();
	}

	void CCollider::OnDisabled()
	{
		DestroyInternal();
	}

	void CCollider::OnEnabled()
	{
		RestoreInternal();
	}

	void CCollider::OnTransformChanged(TransformChangedFlags flags)
	{
		if (!SO()->GetActive())
			return;

		if ((flags & TCF_Parent) != 0)
			UpdateParentRigidbody();

		// Don't update the transform if it's due to Physics update since then we can guarantee it will remain at the same
		// relative transform to its parent
		if (gPhysics().IsUpdateInProgressInternal())
			return;

		if ((flags & (TCF_Parent | TCF_Transform)) != 0)
			UpdateTransform();
	}

	void CCollider::SetRigidbody(const HRigidbody& rigidbody, bool internal)
	{
		if (rigidbody == mParent)
			return;

		if (mInternal != nullptr && !internal)
		{
			if (mParent != nullptr)
				mParent->RemoveCollider(static_object_cast<CCollider>(mThisHandle));

			Rigidbody* rigidBodyPtr = nullptr;

			if (rigidbody != nullptr)
				rigidBodyPtr = rigidbody->GetInternalInternal();

			mInternal->SetRigidbody(rigidBodyPtr);

			if (rigidbody != nullptr)
				rigidbody->AddCollider(static_object_cast<CCollider>(mThisHandle));
		}

		mParent = rigidbody;
		UpdateCollisionReportMode();
		UpdateTransform();
	}

	bool CCollider::RayCast(const Ray& ray, PhysicsQueryHit& hit, float maxDist) const
	{
		if (mInternal == nullptr)
			return false;

		return mInternal->RayCast(ray, hit, maxDist);
	}

	bool CCollider::RayCast(const Vector3& origin, const Vector3& unitDir, PhysicsQueryHit& hit,
		float maxDist) const
	{
		if (mInternal == nullptr)
			return false;

		return mInternal->RayCast(origin, unitDir, hit, maxDist);
	}

	void CCollider::RestoreInternal()
	{
		if (mInternal == nullptr)
		{
			mInternal = CreateInternal();

			mInternal->onCollisionBegin.Connect(std::bind(&CCollider::TriggerOnCollisionBegin, this, _1));
			mInternal->onCollisionStay.Connect(std::bind(&CCollider::TriggerOnCollisionStay, this, _1));
			mInternal->onCollisionEnd.Connect(std::bind(&CCollider::TriggerOnCollisionEnd, this, _1));
		}

		// Note: Merge into one call to avoid many virtual function calls
		mInternal->setIsTrigger(mIsTrigger);
		mInternal->setMass(mMass);
		mInternal->setMaterial(mMaterial);
		mInternal->setContactOffset(mContactOffset);
		mInternal->setRestOffset(mRestOffset);
		mInternal->setLayer(mLayer);

		updateParentRigidbody();
		updateTransform();
		updateCollisionReportMode();
	}

	void CCollider::DestroyInternal()
	{
		if (mParent != nullptr)
			mParent->removeCollider(static_object_cast<CCollider>(mThisHandle));

		mParent = nullptr;

		// This should release the last reference and destroy the internal collider
		if(mInternal)
		{
			mInternal->SetOwnerInternal(PhysicsOwnerType::None, nullptr);
			mInternal = nullptr;
		}
	}

	void CCollider::UpdateParentRigidbody()
	{
		if (mIsTrigger)
		{
			setRigidbody(HRigidbody());
			return;
		}

		HSceneObject currentSO = SO();
		while (currentSO != nullptr)
		{
			HRigidbody parent = currentSO->getComponent<CRigidbody>();
			if (parent != nullptr)
			{
				if(currentSO->getActive() && isValidParent(parent))
					setRigidbody(parent);
				else
					setRigidbody(HRigidbody());

				return;
			}

			currentSO = currentSO->getParent();
		}

		// Not found
		setRigidbody(HRigidbody());
	}

	void CCollider::UpdateTransform()
	{
		const Transform& tfrm = SO()->getTransform();
		Vector3 myScale = tfrm.getScale();

		if (mParent != nullptr)
		{
			const Transform& parentTfrm = mParent->SO()->getTransform();
			Vector3 parentPos = parentTfrm.getPosition();
			Quaternion parentRot = parentTfrm.getRotation();

			Vector3 myPos = tfrm.getPosition();
			Quaternion myRot = tfrm.getRotation();

			Vector3 scale = parentTfrm.getScale();
			Vector3 invScale = scale;
			if (invScale.x != 0) invScale.x = 1.0f / invScale.x;
			if (invScale.y != 0) invScale.y = 1.0f / invScale.y;
			if (invScale.z != 0) invScale.z = 1.0f / invScale.z;

			Quaternion invRotation = parentRot.inverse();

			Vector3 relativePos = invRotation.rotate(myPos - parentPos) *  invScale;
			Quaternion relativeRot = invRotation * myRot;

			relativePos = relativePos + relativeRot.rotate(mLocalPosition * scale);
			relativeRot = relativeRot * mLocalRotation;

			if(mInternal)
				mInternal->setTransform(relativePos, relativeRot);

			mParent->UpdateMassDistributionInternal();
		}
		else
		{
			Quaternion myRot = tfrm.getRotation();
			Vector3 myPos = tfrm.getPosition() + myRot.rotate(mLocalPosition * myScale);
			myRot = myRot * mLocalRotation;

			if(mInternal)
				mInternal->setTransform(myPos, myRot);
		}

		if (mInternal)
			mInternal->setScale(myScale);
	}

	void CCollider::UpdateCollisionReportMode()
	{
		CollisionReportMode mode = mCollisionReportMode;

		if (mParent != nullptr)
			mode = mParent->getCollisionReportMode();

		if(mInternal != nullptr)
			mInternal->setCollisionReportMode(mode);
	}

	void CCollider::TriggerOnCollisionBegin(const CollisionDataRaw& data)
	{
		CollisionData hit;
		hit.contactPoints = data.contactPoints;
		hit.collider[0] = static_object_cast<CCollider>(mThisHandle);

		if(data.colliders[1] != nullptr)
		{
			CCollider* other = (CCollider*)data.colliders[1]->GetOwnerInternal(PhysicsOwnerType::Component);
			hit.collider[1] = static_object_cast<CCollider>(other->getHandle());
		}

		onCollisionBegin(hit);
	}

	void CCollider::TriggerOnCollisionStay(const CollisionDataRaw& data)
	{
		CollisionData hit;
		hit.contactPoints = data.contactPoints;
		hit.collider[0] = static_object_cast<CCollider>(mThisHandle);

		if (data.colliders[1] != nullptr)
		{
			CCollider* other = (CCollider*)data.colliders[1]->GetOwnerInternal(PhysicsOwnerType::Component);
			hit.collider[1] = static_object_cast<CCollider>(other->getHandle());
		}

		onCollisionStay(hit);
	}

	void CCollider::TriggerOnCollisionEnd(const CollisionDataRaw& data)
	{
		CollisionData hit;
		hit.contactPoints = data.contactPoints;
		hit.collider[0] = static_object_cast<CCollider>(mThisHandle);

		if (data.colliders[1] != nullptr)
		{
			CCollider* other = (CCollider*)data.colliders[1]->GetOwnerInternal(PhysicsOwnerType::Component);
			hit.collider[1] = static_object_cast<CCollider>(other->getHandle());
		}

		onCollisionEnd(hit);
	}

	RTTITypeBase* CCollider::GetRttiStatic()
	{
		return CColliderRTTI::Instance();
	}

	RTTITypeBase* CCollider::GetRtti() const
	{
		return CCollider::GetRttiStatic();
	}
}
