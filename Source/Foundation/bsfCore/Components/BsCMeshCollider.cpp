//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Components/BsCMeshCollider.h"
#include "Scene/BsSceneObject.h"
#include "Components/BsCRigidbody.h"
#include "Physics/BsPhysicsMesh.h"
#include "Private/RTTI/BsCMeshColliderRTTI.h"
#include "Scene/BsSceneInstance.h"

using namespace b3d;

CMeshCollider::CMeshCollider(const HSceneObject& parent)
	: CCollider(parent)
{
	SetName("MeshCollider");
}

CMeshCollider::CMeshCollider()
	: CMeshCollider(nullptr)
{ }

void CMeshCollider::OnCreated()
{
	SPtr<ColliderShape> colliderShape = ColliderShape::CreateMesh(mMesh);

	mShapes = { colliderShape };

	CCollider::OnCreated();
}

void CMeshCollider::SetMesh(const HPhysicsMesh& mesh)
{
	if(mMesh == mesh)
		return;

	if(GetIsTrigger() && mesh->GetType() == PhysicsMeshType::Triangle)
	{
		B3D_LOG(Warning, Physics, "Triangle meshes are not supported on Trigger colliders.");
		return;
	}

	mMesh = mesh;

	if(B3D_ENSURE(mShapes.Size() == 1))
		mShapes[0]->SetShape(MeshColliderShapeInformation(mesh));

	if(mParentDynamicRigidbody != nullptr)
	{
		// If triangle mesh its possible the parent can no longer use this collider (they're not supported for
		// non-kinematic rigidbodies)
		if(mMesh.IsLoaded() && mMesh->GetType() == PhysicsMeshType::Triangle)
			RefreshParentRigidbody();
		else
			mParentDynamicRigidbody->UpdateMassDistribution();
	}
}

bool CMeshCollider::IsValidParent(const HRigidbody& parent) const
{
	// Triangle mesh colliders cannot be used for non-kinematic rigidbodies
	return !mMesh.IsLoaded() || mMesh->GetType() == PhysicsMeshType::Convex || parent->GetIsKinematic();
}

RTTIType* CMeshCollider::GetRttiStatic()
{
	return CMeshColliderRTTI::Instance();
}

RTTIType* CMeshCollider::GetRtti() const
{
	return CMeshCollider::GetRttiStatic();
}
