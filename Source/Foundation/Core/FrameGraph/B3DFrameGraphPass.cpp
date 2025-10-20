#include "B3DFrameGraphPass.h"
#include "B3DFrameGraph.h"
#include "B3DFrameGraphResource.h"
#include "RenderAPI/B3DRenderTexture.h"
#include "Debug/B3DDebug.h"

using namespace b3d;
using namespace b3d::render;

FrameGraphPass::FrameGraphPass(
	u32 index,
	const StringView& name,
	GpuQueueUsage queue,
	FrameGraph* frameGraph,
	FrameGraphPassType type)
	: mIndex(index)
	, mName(name)
	, mQueue(queue)
	, mFrameGraph(frameGraph)
	, mType(type)
	, mDepthAttachment(kInvalidFrameGraphResourceId)
{
	B3D_ENSURE(frameGraph != nullptr);
}

void FrameGraphPass::Read(FrameGraphResourceId resource, GpuResourceUseFlags usage)
{
	B3D_ENSURE(resource.IsValid());
	mResourceAccesses.emplace_back(resource, usage, GpuAccessFlag::Read);
}

void FrameGraphPass::Write(FrameGraphResourceId resource, GpuResourceUseFlags usage)
{
	B3D_ENSURE(resource.IsValid());
	mResourceAccesses.emplace_back(resource, usage, GpuAccessFlag::Write);
}

void FrameGraphPass::ReadWrite(FrameGraphResourceId resource, GpuResourceUseFlags usage)
{
	B3D_ENSURE(resource.IsValid());
	mResourceAccesses.emplace_back(resource, usage, GpuAccessFlag::Read | GpuAccessFlag::Write);
}

void FrameGraphPass::WriteColor(FrameGraphResourceId resource, u32 index)
{
	B3D_ENSURE(mType == FrameGraphPassType::Render);
	B3D_ENSURE(resource.IsValid());

	mColorAttachments[index] = resource;

	// Automatically add as a write with RenderTarget usage
	Write(resource, GpuResourceUseFlag::ColorAttachment);
}

void FrameGraphPass::WriteDepth(FrameGraphResourceId resource)
{
	B3D_ENSURE(mType == FrameGraphPassType::Render);
	B3D_ENSURE(resource.IsValid());

	mDepthAttachment = resource;
	mDepthReadOnly = false;

	// Automatically add as a write with DepthStencil usage
	Write(resource, GpuResourceUseFlag::DepthStencilAttachment);
}

void FrameGraphPass::ReadDepth(FrameGraphResourceId resource)
{
	B3D_ENSURE(mType == FrameGraphPassType::Render);
	B3D_ENSURE(resource.IsValid());

	mDepthAttachment = resource;
	mDepthReadOnly = true;

	// Automatically add as a read with DepthStencil usage
	Read(resource, GpuResourceUseFlag::DepthStencilAttachment);
}

SPtr<render::RenderTarget> FrameGraphPass::CreateRenderTarget(
	FrameGraphResourceId* colorAttachments,
	u32 colorAttachmentCount,
	FrameGraphResourceId depthAttachment)
{
	B3D_ENSURE(colorAttachmentCount > 0 && colorAttachmentCount <= B3D_MAXIMUM_RENDER_TARGET_COUNT);

	// Declare writes to all attachments
	for (u32 i = 0; i < colorAttachmentCount; ++i)
	{
		B3D_ENSURE(colorAttachments[i].IsValid());
		Write(colorAttachments[i], GpuResourceUseFlag::ColorAttachment);
	}

	if (depthAttachment.IsValid())
		Write(depthAttachment, GpuResourceUseFlag::DepthStencilAttachment);

	// Create render texture from the resources
	RenderTextureCreateInformation rtCreateInfo;

	for (u32 i = 0; i < colorAttachmentCount; ++i)
	{
		FrameGraphResource* resource = mFrameGraph->GetResource(colorAttachments[i]);
		if (!B3D_ENSURE(resource != nullptr))
		{
			B3D_LOG(Error, RenderBackend, "Pass '{0}': Invalid color attachment resource ID {1}",
				mName, colorAttachments[i].Index);
			return nullptr;
		}

		if (!B3D_ENSURE(resource->GetType() == FrameGraphResourceType::Texture))
		{
			B3D_LOG(Error, RenderBackend, "Pass '{0}': Color attachment {1} is not a texture",
				mName, i);
			return nullptr;
		}

		auto* textureResource = static_cast<FrameGraphTextureResource*>(resource);
		rtCreateInfo.ColorSurfaces[i].Texture = textureResource->GetTexture();
	}

	if (depthAttachment.IsValid())
	{
		FrameGraphResource* resource = mFrameGraph->GetResource(depthAttachment);
		if (!B3D_ENSURE(resource != nullptr))
		{
			B3D_LOG(Error, RenderBackend, "Pass '{0}': Invalid depth attachment resource ID {1}",
				mName, depthAttachment.Index);
			return nullptr;
		}

		if (!B3D_ENSURE(resource->GetType() == FrameGraphResourceType::Texture))
		{
			B3D_LOG(Error, RenderBackend, "Pass '{0}': Depth attachment is not a texture", mName);
			return nullptr;
		}

		auto* textureResource = static_cast<FrameGraphTextureResource*>(resource);
		rtCreateInfo.DepthStencilSurface.Texture = textureResource->GetTexture();
	}

	// Cache the render target for this pass
	mRenderTarget = RenderTexture::Create(rtCreateInfo);
	return mRenderTarget;
}

void FrameGraphPass::ExecuteSetup()
{
	if (mSetupFunc)
		mSetupFunc(*this);
}

void FrameGraphPass::ExecuteCommands(GpuCommandBuffer& commandBuffer)
{
	if (mExecuteFunc)
		mExecuteFunc(commandBuffer);
}

void FrameGraphPass::Reset()
{
	// Clear resource accesses (will be repopulated during next setup)
	mResourceAccesses.clear();

	// Clear attachment data
	mColorAttachments.clear();
	mDepthAttachment = kInvalidFrameGraphResourceId;
	mDepthReadOnly = false;

	// Release cached render target
	mRenderTarget.reset();

	// Reset dependency metadata
	mIncomingDependencies.clear();
	mOutgoingDependencies.clear();
	mReferenceCount = 0;
	mCulled = false;
}

void FrameGraphPass::AddIncomingDependency(const FrameGraphPassDependency& dependency)
{
	mIncomingDependencies.push_back(dependency);
}

void FrameGraphPass::AddOutgoingDependency(const FrameGraphPassDependency& dependency)
{
	mOutgoingDependencies.push_back(dependency);
}
