//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsVulkanPrerequisites.h"
#include "BsVulkanResource.h"

namespace bs { namespace ct
{
	class VulkanRenderPass;

	/** @addtogroup Vulkan
	 *  @{
	 */

	/** Represents a single attachment in a Vulkan frame-buffer. */
	struct VULKAN_ATTACHMENT_DESC
	{
		/** Image to attach or null if none. */
		VulkanImage* Image = nullptr;

		/** Surface representing the sub-resource of the image to use as an attachment. */
		TextureSurface Surface;

		/** Initial layer of the surface as pointed to by the provided image view. */
		UINT32 BaseLayer = 0;
	};

	/** Contains parameters used for initializing VulkanFrameBuffer object. */
	struct VULKAN_FRAMEBUFFER_DESC
	{
		/** Images describing the color attachments. */
		VULKAN_ATTACHMENT_DESC Color[BS_MAX_MULTIPLE_RENDER_TARGETS];

		/** Image describing the depth attachment. */
		VULKAN_ATTACHMENT_DESC Depth;

		/** Width of the images, in pixels. All images must be the same size. */
		UINT32 Width = 0;

		/** Height of the images, in pixels. All images must be the same size. */
		UINT32 Height = 0;

		/** Number of image layers to render to. This value is used for all provided surfaces. */
		UINT32 Layers = 0;
	};

	/** Information about a single framebuffer attachment. */
	struct VulkanFramebufferAttachment
	{
		VulkanImage* Image = nullptr;
		TextureSurface Surface;
		UINT32 BaseLayer = 0;
		VkImageLayout FinalLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		UINT32 Index = 0;
	};

	/** Vulkan frame buffer containing one or multiple color surfaces, and an optional depth surface. */
	class VulkanFramebuffer : public VulkanResource
	{
	public:
		/** Creates a new frame buffer with the specified image views attached.
		 *
		 * @param[in]	owner		Resource manager that allocated this resource.
		 * @param[in]	renderPass	Render pass that will be used for rendering to the frame buffer. Note that the
		 *							framebuffer will be usable with this specific render pass, but also with any compatible
		 *							render pass. Render passes are compatible if they use the same attachments and their
		 *							formats and sample counts match.
		 * @param[in]	desc		Description of the frame buffer.
		 */
		VulkanFramebuffer(VulkanResourceManager* owner, VulkanRenderPass* renderPass, const VULKAN_FRAMEBUFFER_DESC& desc);
		~VulkanFramebuffer();

		/** Returns a unique ID of this framebuffer. */
		UINT32 GetId() const { return mId; }

		/** Returns the width of the framebuffer, in pixels. */
		UINT32 GetWidth() const { return mWidth; }

		/** Returns the height of the framebuffer, in pixels. */
		UINT32 GetHeight() const { return mHeight; }

		/** Gets the internal Vulkan framebuffer object. */
		VkFramebuffer GetVkFramebuffer() const { return mVkFramebuffer; }

		/** Returns the render pass that this framebuffer is tied to. */
		VulkanRenderPass* GetRenderPass() const { return mRenderPass; }

		/**
		 * Gets the number of layers in each framebuffer surface. A layer is an element in a texture array, or a depth
		 * slice in a 3D texture).
		 */
		UINT32 GetNumLayers() const { return mNumLayers; }

		/** Returns information about a color attachment at the specified index. */
		const VulkanFramebufferAttachment& GetColorAttachment(UINT32 colorIdx) const { return mColorAttachments[colorIdx]; }

		/** Returns information about a depth-stencil attachment. */
		const VulkanFramebufferAttachment& GetDepthStencilAttachment() const { return mDepthStencilAttachment; }
	private:
		UINT32 mId;
		VkFramebuffer mVkFramebuffer;
		VulkanRenderPass* mRenderPass;

		UINT32 mWidth;
		UINT32 mHeight;
		UINT32 mNumLayers;
		VulkanFramebufferAttachment mColorAttachments[BS_MAX_MULTIPLE_RENDER_TARGETS] { };
		VulkanFramebufferAttachment mDepthStencilAttachment;

		static UINT32 sNextValidId;
	};

	/** @} */
}}
