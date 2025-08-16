//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Components/BsCCapsuleCollider.h"
#include "Scene/BsSceneObject.h"
#include "Components/BsCRigidbody.h"
#include "Private/RTTI/BsCCapsuleColliderRTTI.h"
#include "Scene/BsSceneInstance.h"

using namespace b3d;

CCapsuleCollider::CCapsuleCollider(const HSceneObject& parent, float radius, float halfHeight)
	: CCollider(parent), mRadius(radius), mHalfHeight(halfHeight)
{
	SetName("CapsuleCollider");

	mShapeLocalRotation = Quaternion::GetRotationFromTo(Vector3::kUnitX, mNormal);
}

CCapsuleCollider::CCapsuleCollider()
	: CCapsuleCollider(nullptr)
{ }

void CCapsuleCollider::OnCreated()
{
	SPtr<ColliderShape> colliderShape = ColliderShape::CreateCapsule(CapsuleColliderShapeInformation(mRadius, mHalfHeight));
	colliderShape->SetPosition(mShapeLocalPosition);
	colliderShape->SetRotation(mShapeLocalRotation);

	mShapes = { colliderShape };

	CCollider::OnCreated();
}

void CCapsuleCollider::SetNormal(const Vector3& normal)
{
	if(mNormal == normal)
		return;

	mNormal = b3d::Vector3::Normalize(normal);
	mShapeLocalRotation = Quaternion::GetRotationFromTo(Vector3::kUnitX, mNormal);

	if(B3D_ENSURE(mShapes.Size() == 1))
		mShapes[0]->SetRotation(mShapeLocalRotation);
		
}

void CCapsuleCollider::SetCenter(const Vector3& center)
{
	if(mShapeLocalPosition == center)
		return;

	mShapeLocalPosition = center;

	if(B3D_ENSURE(mShapes.Size() == 1))
		mShapes[0]->SetPosition(mShapeLocalPosition);
}

void CCapsuleCollider::SetHalfHeight(float halfHeight)
{
	float clampedHalfHeight = std::max(halfHeight, 0.01f);
	if(mHalfHeight == clampedHalfHeight)
		return;

	mHalfHeight = clampedHalfHeight;

	if(B3D_ENSURE(mShapes.Size() == 1))
		mShapes[0]->SetShape(CapsuleColliderShapeInformation(mRadius, clampedHalfHeight));

	if(mParentDynamicRigidbody != nullptr)
		mParentDynamicRigidbody->UpdateMassDistribution();
}

void CCapsuleCollider::SetRadius(float radius)
{
	float clampedRadius = std::max(radius, 0.01f);
	if(mRadius == clampedRadius)
		return;

	mRadius = clampedRadius;

	if(B3D_ENSURE(mShapes.Size() == 1))
		mShapes[0]->SetShape(CapsuleColliderShapeInformation(clampedRadius, mHalfHeight));

	if(mParentDynamicRigidbody != nullptr)
		mParentDynamicRigidbody->UpdateMassDistribution();
}

RTTIType* CCapsuleCollider::GetRttiStatic()
{
	return CCapsuleColliderRTTI::Instance();
}

RTTIType* CCapsuleCollider::GetRtti() const
{
	return CCapsuleCollider::GetRttiStatic();
}
