//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Components/BsCBoxCollider.h"
#include "Scene/BsSceneObject.h"
#include "Components/BsCRigidbody.h"
#include "Private/RTTI/BsCBoxColliderRTTI.h"
#include "Scene/BsSceneInstance.h"

using namespace b3d;

CBoxCollider::CBoxCollider(const HSceneObject& parent, const Vector3& extents)
	: CCollider(parent), mExtents(extents)
{
	SetName("BoxCollider");
}

CBoxCollider::CBoxCollider()
	: CBoxCollider(nullptr)
{ }

void CBoxCollider::OnCreated()
{
	SPtr<ColliderShape> colliderShape = ColliderShape::CreateBox(mExtents);
	colliderShape->SetPosition(mShapeLocalPosition);

	mShapes = { colliderShape };

	CCollider::OnCreated();
}

void CBoxCollider::SetExtents(const Vector3& extents)
{
	Vector3 clampedExtents = Vector3::Max(extents, Vector3(0.01f, 0.01f, 0.01f));

	if(mExtents == clampedExtents)
		return;

	mExtents = clampedExtents;

	if(B3D_ENSURE(mShapes.Size() == 1))
		mShapes[0]->SetShape(BoxColliderShapeInformation(clampedExtents));

	if(mParentDynamicRigidbody != nullptr)
		mParentDynamicRigidbody->UpdateMassDistribution();
}

void CBoxCollider::SetCenter(const Vector3& center)
{
	if(mShapeLocalPosition == center)
		return;

	mShapeLocalPosition = center;

	if(B3D_ENSURE(mShapes.Size() == 1))
		mShapes[0]->SetPosition(mShapeLocalPosition);
}

RTTIType* CBoxCollider::GetRttiStatic()
{
	return CBoxColliderRTTI::Instance();
}

RTTIType* CBoxCollider::GetRtti() const
{
	return CBoxCollider::GetRttiStatic();
}
