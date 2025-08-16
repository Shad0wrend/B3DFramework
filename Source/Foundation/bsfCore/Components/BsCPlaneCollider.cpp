//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Components/BsCPlaneCollider.h"
#include "Scene/BsSceneObject.h"
#include "Components/BsCRigidbody.h"
#include "Private/RTTI/BsCPlaneColliderRTTI.h"
#include "Scene/BsSceneInstance.h"

using namespace b3d;

CPlaneCollider::CPlaneCollider(const HSceneObject& parent)
	: CCollider(parent)
{
	SetName("PlaneCollider");

	mShapeLocalRotation = Quaternion::GetRotationFromTo(Vector3::kUnitX, mNormal);
}

CPlaneCollider::CPlaneCollider()
	: CPlaneCollider(nullptr)
{ }

void CPlaneCollider::OnCreated()
{
	SPtr<ColliderShape> colliderShape = ColliderShape::CreatePlane(PlaneColliderShapeInformation());
	colliderShape->SetPosition(mShapeLocalPosition);
	colliderShape->SetRotation(mShapeLocalRotation);

	mShapes = { colliderShape };

	CCollider::OnCreated();
}

void CPlaneCollider::SetNormal(const Vector3& normal)
{
	if(mNormal == normal)
		return;

	mNormal = normal;
	mNormal.Normalize();

	mShapeLocalRotation = Quaternion::GetRotationFromTo(Vector3::kUnitX, normal);
	mShapeLocalPosition = mNormal * mDistance;

	if(B3D_ENSURE(mShapes.Size() == 1))
	{
		mShapes[0]->SetPosition(mShapeLocalPosition);
		mShapes[0]->SetRotation(mShapeLocalRotation);
	}
}

void CPlaneCollider::SetDistance(float distance)
{
	if(mDistance == distance)
		return;

	mDistance = distance;
	mShapeLocalPosition = mNormal * distance;

	if(B3D_ENSURE(mShapes.Size() == 1))
		mShapes[0]->SetPosition(mShapeLocalPosition);
}

bool CPlaneCollider::IsValidParent(const HRigidbody& parent) const
{
	// Planes cannot be added to non-kinematic rigidbodies
	return parent->GetIsKinematic();
}

RTTIType* CPlaneCollider::GetRttiStatic()
{
	return CPlaneColliderRTTI::Instance();
}

RTTIType* CPlaneCollider::GetRtti() const
{
	return CPlaneCollider::GetRttiStatic();
}
