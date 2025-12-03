//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "B3DRendererDecal.h"
#include "B3DRendererRenderable.h"
#include "Components/B3DDecal.h"
#include "Mesh/B3DMesh.h"
#include "Renderer/B3DRendererUtility.h"

namespace b3d {
namespace render {

DecalParamDef gDecalParamDef;

void DecalRenderElement::Draw(GpuCommandBuffer& commandBuffer) const
{
	GetRendererUtility().Draw(commandBuffer, Mesh, SubMesh);
}

RendererDecal::RendererDecal()
{
	DecalParamBuffer = gDecalParamDef.CreateBuffer();
}

void RendererDecal::UpdateDecalParamBuffer()
{
	const Transform& tfrm = Decal->GetWorldTransform();

	const Vector2 size = Decal->GetWorldSize();
	const Vector2 extent = size * 0.5f;
	const float maxDistance = Decal->GetWorldMaxDistance();

	const Matrix4 view = Matrix4::View(tfrm.GetPosition(), tfrm.GetRotation());
	const Matrix4 proj = Matrix4::ProjectionOrthographic(-extent.X, extent.X, -extent.Y, extent.Y, 0.0f, maxDistance);

	const Matrix4 worldToDecal = proj * view;
	const Vector3 decalNormal = -tfrm.GetRotation().ZAxis();
	const float normalTolerance = -0.05f;

	float flipDerivatives = 1.0f;

	const SPtr<GpuDevice>& gpuDevice = GetApplication().GetPrimaryGpuDevice();
	const GpuBackendConventions& gpuBackendConventions = gpuDevice->GetCapabilities().Conventions;
	if(gpuBackendConventions.UvYAxis == GpuBackendConventions::Axis::Up)
		flipDerivatives = -1.0f;

	gDecalParamDef.gWorldToDecal.Set(DecalParamBuffer, worldToDecal);
	gDecalParamDef.gDecalNormal.Set(DecalParamBuffer, decalNormal);
	gDecalParamDef.gNormalTolerance.Set(DecalParamBuffer, normalTolerance);
	gDecalParamDef.gFlipDerivatives.Set(DecalParamBuffer, flipDerivatives);
	gDecalParamDef.gLayerMask.Set(DecalParamBuffer, (i32)Decal->GetLayerMask());
}

void RendererDecal::UpdatePerObjectData()
{
	const Vector2 size = Decal->GetWorldSize();
	const Vector2 extent = size * 0.5f;
	const float maxDistance = Decal->GetWorldMaxDistance();

	const Vector3 scale(extent.X, extent.Y, maxDistance * 0.5f);
	const Vector3 offset(0.0f, 0.0f, -maxDistance * 0.5f);
	const Matrix4 scaleAndOffset = Matrix4::TRS(offset, Quaternion::kIdentity, scale);

	WorldTransform = Decal->GetWorldTransformMatrix() * scaleAndOffset;
	WorldNoScale = Decal->GetWorldTransformMatrixWithoutScale() * scaleAndOffset;
	PrevWorldTransform = WorldTransform; // Decals don't track previous frame
	Layer = 0;
}

}} // namespace b3d::render
