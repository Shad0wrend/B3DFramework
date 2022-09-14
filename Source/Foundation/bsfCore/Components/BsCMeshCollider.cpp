//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Components/BsCMeshCollider.h"
#include "Scene/BsSceneObject.h"
#include "Components/BsCRigidbody.h"
#include "Physics/BsPhysicsMesh.h"
#include "Private/RTTI/BsCMeshColliderRTTI.h"
#include "Scene/BsSceneManager.h"

namespace bs
{
	CMeshCollider::CMeshCollider()
	{
		setName("MeshCollider");
	}

	CMeshCollider::CMeshCollider(const HSceneObject& parent)
		: CCollider(parent)
	{
		setName("MeshCollider");
	}

	void CMeshCollider::SetMesh(const HPhysicsMesh& mesh)
	{
		if (mMesh == mesh)
			return;

		if(getIsTrigger() && mesh->GetType() == PhysicsMeshType::Triangle)
		{
			BS_LOG(Warning, Physics, "Triangle meshes are not supported on Trigger colliders.");
			return;
		}

		mMesh = mesh;

		if (mInternal != nullptr)
		{
			GetInternalInternal()->SetMesh(mesh);

			if (mParent != nullptr)
			{
				// If triangle mesh its possible the parent can no longer use this collider (they're not supported for
				// non-kinematic rigidbodies)
				if (mMesh.isLoaded() && mMesh->GetType() == PhysicsMeshType::Triangle)
					updateParentRigidbody();
				else
					mParent->UpdateMassDistributionInternal();
			}
		}
	}

	SPtr<Collider> CMeshCollider::CreateInternal()
	{
		const SPtr<SceneInstance>& scene = SO()->getScene();
		const Transform& tfrm = SO()->getTransform();

		SPtr<MeshCollider> collider = MeshCollider::Create(*scene->getPhysicsScene(), tfrm.getPosition(),
			tfrm.getRotation());
		collider->setMesh(mMesh);
		collider->SetOwnerInternal(PhysicsOwnerType::Component, this);

		return collider;
	}

	bool CMeshCollider::IsValidParent(const HRigidbody& parent) const
	{
		// Triangle mesh colliders cannot be used for non-kinematic rigidbodies
		return !mMesh.isLoaded() || mMesh->GetType() == PhysicsMeshType::Convex || parent->getIsKinematic();
	}

	RTTITypeBase* CMeshCollider::GetRttiStatic()
	{
		return CMeshColliderRTTI::Instance();
	}

	RTTITypeBase* CMeshCollider::GetRtti() const
	{
		return CMeshCollider::GetRttiStatic();
	}
}
