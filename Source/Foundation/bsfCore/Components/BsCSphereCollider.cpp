//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Components/BsCSphereCollider.h"
#include "Scene/BsSceneObject.h"
#include "Components/BsCRigidbody.h"
#include "Private/RTTI/BsCSphereColliderRTTI.h"
#include "Scene/BsSceneInstance.h"

using namespace b3d;

CSphereCollider::CSphereCollider(const HSceneObject& parent, float radius)
	: CCollider(parent), mRadius(radius)
{
	SetName("SphereCollider");
}

CSphereCollider::CSphereCollider()
	: CSphereCollider(nullptr)
{ }

void CSphereCollider::OnCreated()
{
	SPtr<ColliderShape> colliderShape = ColliderShape::CreateSphere(mRadius);
	colliderShape->SetPosition(mShapeLocalPosition);

	mShapes = { colliderShape };

	CCollider::OnCreated();
}

void CSphereCollider::SetRadius(float radius)
{
	float clampedRadius = std::max(radius, 0.01f);
	if(mRadius == clampedRadius)
		return;

	mRadius = clampedRadius;

	if(B3D_ENSURE(mShapes.Size() == 1))
		mShapes[0]->SetShape(SphereColliderShapeInformation(clampedRadius));

	if(mParentDynamicRigidbody != nullptr)
		mParentDynamicRigidbody->UpdateMassDistribution();
}

void CSphereCollider::SetCenter(const Vector3& center)
{
	if(mShapeLocalPosition == center)
		return;

	mShapeLocalPosition = center;

	if(B3D_ENSURE(mShapes.Size() == 1))
		mShapes[0]->SetPosition(mShapeLocalPosition);
}

RTTIType* CSphereCollider::GetRttiStatic()
{
	return CSphereColliderRTTI::Instance();
}

RTTIType* CSphereCollider::GetRtti() const
{
	return CSphereCollider::GetRttiStatic();
}
