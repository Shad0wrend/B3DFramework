//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "B3DRendererRenderable.h"

#include "Renderer/B3DRendererUtility.h"
#include "Mesh/B3DMesh.h"
#include "Utility/B3DBitwise.h"

namespace b3d {
namespace render {

PerObjectUniformDefinition gPerObjectUniformDefinition;

void PerObjectBuffer::Update(SPtr<GpuBuffer>& buffer, const Matrix4& tfrm, const Matrix4& tfrmNoScale, const Matrix4& prevTfrm, u32 layer)
{
	gPerObjectUniformDefinition.gMatWorld.Set(buffer, tfrm);
	gPerObjectUniformDefinition.gMatInvWorld.Set(buffer, tfrm.InverseAffine());
	gPerObjectUniformDefinition.gMatWorldNoScale.Set(buffer, tfrmNoScale);
	gPerObjectUniformDefinition.gMatInvWorldNoScale.Set(buffer, tfrmNoScale.InverseAffine());
	gPerObjectUniformDefinition.gMatPrevWorld.Set(buffer, prevTfrm);
	gPerObjectUniformDefinition.gWorldDeterminantSign.Set(buffer, tfrm.Determinant3x3() >= 0.0f ? 1.0f : -1.0f);
	gPerObjectUniformDefinition.gLayer.Set(buffer, (i32)layer);
}

void RenderableElement::Draw(GpuCommandBuffer& commandBuffer) const
{
	if(MorphVertexDefinition == nullptr)
		GetRendererUtility().Draw(commandBuffer, Mesh, SubMesh);
	else
		GetRendererUtility().DrawMorph(commandBuffer, Mesh, SubMesh, MorphShapeBuffer, MorphVertexDefinition);
}

RendererRenderable::RendererRenderable()
{
	PerObjectParamBuffer = gPerObjectUniformDefinition.CreateBuffer();
}

void RendererRenderable::UpdatePerObjectBuffer()
{
	const Matrix4 worldNoScaleTransform = Renderable->GetWorldTransformMatrixWithoutScale();
	const u32 layer = Bitwise::MostSignificantBit(Renderable->GetLayer());

	PerObjectBuffer::Update(PerObjectParamBuffer, WorldTfrm, worldNoScaleTransform, PrevWorldTfrm, layer);
}
}} // namespace b3d::render
