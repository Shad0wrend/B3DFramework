#include "B3DFrameGraphPassResources.h"
#include "B3DFrameGraph.h"
#include "B3DFrameGraphPass.h"
#include "B3DFrameGraphResource.h"
#include "Image/B3DTexture.h"
#include "RenderAPI/B3DGpuBuffer.h"
#include "Debug/B3DDebug.h"

using namespace b3d;
using namespace b3d::render;

FrameGraphPassResources::FrameGraphPassResources(
	FrameGraph& frameGraph,
	const FrameGraphPass* pass)
	: mFrameGraph(frameGraph)
	, mPass(pass)
{
	B3D_ENSURE(pass != nullptr);
}

SPtr<render::Texture> FrameGraphPassResources::GetTexture(FrameGraphResourceId id) const
{
	B3D_ENSURE(id.IsValid()); // Invalid resource ID

	FrameGraphResource* resource = mFrameGraph.GetResource(id);
	B3D_ENSURE(resource != nullptr); // Resource does not exist in frame graph

	B3D_ENSURE(resource->GetType() == FrameGraphResourceType::Texture); // Resource is not a texture (type mismatch)

	auto* textureResource = static_cast<FrameGraphTextureResource*>(resource);
	const SPtr<render::Texture>& texture = textureResource->GetTexture();

	B3D_ENSURE(texture != nullptr); // Texture resource  is not allocated (transient resources must be allocated before use)

	return texture;
}

SPtr<render::GpuBuffer> FrameGraphPassResources::GetBuffer(FrameGraphResourceId id) const
{
	B3D_ENSURE(id.IsValid()); // Invalid resource ID

	FrameGraphResource* resource = mFrameGraph.GetResource(id);
	B3D_ENSURE(resource != nullptr); // Resource does not exist in frame graph

	B3D_ENSURE(resource->GetType() == FrameGraphResourceType::Buffer); // Resource is not a buffer (type mismatch)

	auto* bufferResource = static_cast<FrameGraphBufferResource*>(resource);
	const SPtr<render::GpuBuffer>& buffer = bufferResource->GetBuffer();

	B3D_ENSURE(buffer != nullptr); //Buffer resource is not allocated (transient resources must be allocated before use)

	return buffer;
}

SPtr<render::Texture> FrameGraphPassResources::TryGetTexture(FrameGraphResourceId id) const
{
	if (!id.IsValid())
		return nullptr;

	FrameGraphResource* resource = mFrameGraph.GetResource(id);
	if (!resource)
		return nullptr;

	if (resource->GetType() != FrameGraphResourceType::Texture)
		return nullptr;

	auto* textureResource = static_cast<FrameGraphTextureResource*>(resource);
	return textureResource->GetTexture();
}

SPtr<render::GpuBuffer> FrameGraphPassResources::TryGetBuffer(FrameGraphResourceId id) const
{
	if (!id.IsValid())
		return nullptr;

	FrameGraphResource* resource = mFrameGraph.GetResource(id);
	if (!resource)
		return nullptr;

	if (resource->GetType() != FrameGraphResourceType::Buffer)
		return nullptr;

	auto* bufferResource = static_cast<FrameGraphBufferResource*>(resource);
	return bufferResource->GetBuffer();
}
