//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Physics/BsPhysicsMesh.h"
#include "Private/RTTI/BsPhysicsMeshRTTI.h"
#include "Resources/BsResources.h"
#include "Physics/BsPhysics.h"

namespace bs
{
	PhysicsMesh::PhysicsMesh(const SPtr<MeshData>& meshData, PhysicsMeshType type)
		:mInitMeshData(meshData), mType(type)
	{
		// Derived class is responsible for initializing mInternal
	}

	PhysicsMeshType PhysicsMesh::GetType() const
	{
		return mInternal->mType;
	}

	SPtr<MeshData> PhysicsMesh::GetMeshData() const
	{
		return mInternal->getMeshData();
	}

	HPhysicsMesh PhysicsMesh::Create(const SPtr<MeshData>& meshData, PhysicsMeshType type)
	{
		SPtr<PhysicsMesh> newMesh = CreatePtrInternal(meshData, type);

		return static_resource_cast<PhysicsMesh>(gResources().CreateResourceHandleInternal(newMesh));
	}

	SPtr<PhysicsMesh> PhysicsMesh::CreatePtrInternal(const SPtr<MeshData>& meshData, PhysicsMeshType type)
	{
		SPtr<PhysicsMesh> newMesh = gPhysics().createMesh(meshData, type);
		newMesh->SetThisPtrInternal(newMesh);
		newMesh->initialize();

		return newMesh;
	}

	void PhysicsMesh::Initialize()
	{
		mInitMeshData = nullptr;

		Resource::initialize();
	}

	RTTITypeBase* PhysicsMesh::GetRttiStatic()
	{
		return PhysicsMeshRTTI::Instance();
	}

	RTTITypeBase* PhysicsMesh::GetRtti() const
	{
		return GetRttiStatic();
	}

	FPhysicsMesh::FPhysicsMesh(const SPtr<MeshData>& meshData, PhysicsMeshType type)
		:mType(type)
	{
		
	}

	FPhysicsMesh::~FPhysicsMesh()
	{
		
	}

	RTTITypeBase* FPhysicsMesh::GetRttiStatic()
	{
		return FPhysicsMeshRTTI::Instance();
	}

	RTTITypeBase* FPhysicsMesh::GetRtti() const
	{
		return GetRttiStatic();
	}
}
