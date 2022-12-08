//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsVulkanSwapChain.h"
#include "BsVulkanTexture.h"
#include "BsVulkanRenderAPI.h"
#include "BsVulkanDevice.h"
#include "Managers/BsVulkanCommandBufferManager.h"
#include "BsVulkanRenderPass.h"

using namespace bs;
using namespace bs::ct;

VulkanSwapChain::VulkanSwapChain(VulkanResourceManager* owner, VkSurfaceKHR surface, u32 width, u32 height, bool vsync, VkFormat colorFormat, VkColorSpaceKHR colorSpace, bool createDepth, VkFormat depthFormat, VulkanSwapChain* oldSwapChain)
	: VulkanResource(owner, false)
{
	VulkanDevice& device = owner->GetDevice();
	mDevice = device.GetLogical();

	VkResult result;
	VkPhysicalDevice physicalDevice = device.GetPhysical();

	// Determine swap chain dimensions
	VkSurfaceCapabilitiesKHR surfaceCaps;
	result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCaps);
	B3D_ASSERT(result == VK_SUCCESS);

	VkExtent2D swapchainExtent;
	// If width/height is 0xFFFFFFFF, we can manually specify width, height
	if(surfaceCaps.currentExtent.width == (uint32_t)-1 || surfaceCaps.currentExtent.height == (uint32_t)-1)
	{
		swapchainExtent.width = Math::Clamp(width, surfaceCaps.minImageExtent.width, surfaceCaps.maxImageExtent.width);
		swapchainExtent.height = Math::Clamp(height, surfaceCaps.minImageExtent.height, surfaceCaps.maxImageExtent.height);
	}
	else // Otherwise we must use the size we're given
		swapchainExtent = surfaceCaps.currentExtent;

	mWidth = swapchainExtent.width;
	mHeight = swapchainExtent.height;

	// Find present mode
	uint32_t numPresentModes;
	result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &numPresentModes, nullptr);
	B3D_ASSERT(result == VK_SUCCESS);
	B3D_ASSERT(numPresentModes > 0);

	VkPresentModeKHR* presentModes = B3DStackAllocate<VkPresentModeKHR>(numPresentModes);
	result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &numPresentModes, presentModes);
	B3D_ASSERT(result == VK_SUCCESS);

	VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
	if(!vsync)
	{
		for(u32 i = 0; i < numPresentModes; i++)
		{
			if(presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)
			{
				presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
				break;
			}

			if(presentModes[i] == VK_PRESENT_MODE_FIFO_RELAXED_KHR)
				presentMode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
		}
	}
	else
	{
		// Mailbox comes with lower input latency than FIFO, but can waste GPU power by rendering frames that are never
		// displayed, especially if the app runs much faster than the refresh rate. This is a concern for mobiles.
#if B3D_PLATFORM != B3D_PLATFORM_ID_ANDROID && B3D_PLATFORM != B3D_PLATFORM_ID_IOS
		for(u32 i = 0; i < numPresentModes; i++)
		{

			if(presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
				break;
			}
		}
#endif
	}

	B3DStackFree(presentModes);

	// Note: Ideally we refactor the submit thread so it can work properly with two images
	constexpr u32 kPreferredImageCount = 3; // One extra required due to the separate submit thread.
	u32 imageCount = Math::Clamp(kPreferredImageCount, surfaceCaps.minImageCount, surfaceCaps.maxImageCount);

	VkSurfaceTransformFlagsKHR transform;
	if(surfaceCaps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
		transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	else
		transform = surfaceCaps.currentTransform;

	VkSwapchainCreateInfoKHR swapChainCI;
	swapChainCI.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapChainCI.pNext = nullptr;
	swapChainCI.flags = 0;
	swapChainCI.surface = surface;
	swapChainCI.minImageCount = imageCount;
	swapChainCI.imageFormat = colorFormat;
	swapChainCI.imageColorSpace = colorSpace;
	swapChainCI.imageExtent = { swapchainExtent.width, swapchainExtent.height };
	swapChainCI.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapChainCI.preTransform = (VkSurfaceTransformFlagBitsKHR)transform;
	swapChainCI.imageArrayLayers = 1;
	swapChainCI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapChainCI.queueFamilyIndexCount = 0;
	swapChainCI.pQueueFamilyIndices = nullptr;
	swapChainCI.presentMode = presentMode;
	swapChainCI.oldSwapchain = oldSwapChain ? oldSwapChain->mSwapChain : VK_NULL_HANDLE;
	swapChainCI.clipped = VK_TRUE;
	swapChainCI.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	result = vkCreateSwapchainKHR(mDevice, &swapChainCI, gVulkanAllocator, &mSwapChain);
	B3D_ASSERT(result == VK_SUCCESS);

	result = vkGetSwapchainImagesKHR(mDevice, mSwapChain, &imageCount, nullptr);
	B3D_ASSERT(result == VK_SUCCESS);

	// Get the swap chain images
	VkImage* images = B3DStackAllocate<VkImage>(imageCount);
	result = vkGetSwapchainImagesKHR(mDevice, mSwapChain, &imageCount, images);
	B3D_ASSERT(result == VK_SUCCESS);

	VulkanImageCreateInformation imageDesc;
	imageDesc.Format = colorFormat;
	imageDesc.Type = TEX_TYPE_2D;
	imageDesc.Usage = TU_RENDERTARGET;
	imageDesc.Layout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageDesc.FaceCount = 1;
	imageDesc.MipLevelCount = 1;
	imageDesc.DepthSliceCount = 1;
	imageDesc.Allocation = VK_NULL_HANDLE;

	mSurfaces.resize(imageCount);
	for(u32 imageIndex = 0; imageIndex < imageCount; imageIndex++)
	{
		imageDesc.Image = images[imageIndex];

		mSurfaces[imageIndex].Acquired = false;
		mSurfaces[imageIndex].NeedsWait = false;
		mSurfaces[imageIndex].Image = owner->Create<VulkanImage>(imageDesc, false, false);
		mSurfaces[imageIndex].Semaphore = owner->Create<VulkanSemaphore>();

		if(mSurfaces[imageIndex].Image != nullptr)
		{
			mSurfaces[imageIndex].Image->SetName(StringUtil::Format("Color attachment #{0}", imageIndex));
		}
	}

	B3DStackFree(images);

	// Create depth stencil image
	if(createDepth)
	{
		VkImageCreateInfo depthStencilImageCI;
		depthStencilImageCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		depthStencilImageCI.pNext = nullptr;
		depthStencilImageCI.flags = 0;
		depthStencilImageCI.imageType = VK_IMAGE_TYPE_2D;
		depthStencilImageCI.format = depthFormat;
		depthStencilImageCI.extent = { mWidth, mHeight, 1 };
		depthStencilImageCI.mipLevels = 1;
		depthStencilImageCI.arrayLayers = 1;
		depthStencilImageCI.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthStencilImageCI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		depthStencilImageCI.samples = VK_SAMPLE_COUNT_1_BIT;
		depthStencilImageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
		depthStencilImageCI.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		depthStencilImageCI.pQueueFamilyIndices = nullptr;
		depthStencilImageCI.queueFamilyIndexCount = 0;

		VkImage depthStencilImage;
		result = vkCreateImage(mDevice, &depthStencilImageCI, gVulkanAllocator, &depthStencilImage);
		B3D_ASSERT(result == VK_SUCCESS);

		imageDesc.Image = depthStencilImage;
		imageDesc.Usage = TU_DEPTHSTENCIL;
		imageDesc.Format = depthFormat;
		imageDesc.Allocation = device.AllocateMemory(depthStencilImage, VMA_MEMORY_USAGE_GPU_ONLY);

		mDepthStencilImage = owner->Create<VulkanImage>(imageDesc, true, false);

		if(mDepthStencilImage != nullptr)
		{
			mDepthStencilImage->SetName("Depth-stencil attachment");
		}
	}
	else
		mDepthStencilImage = nullptr;

	// Create a render pass
	VulkanRenderPassCreateInformation rpDesc;
	rpDesc.SampleCount = 1;
	rpDesc.IsOffscreenSurface = false;
	rpDesc.ColorAttachments[0].Format = colorFormat;
	rpDesc.ColorAttachments[0].IsShaderReadAllowed = false;
	rpDesc.ColorAttachments[0].IsEnabled = true;

	if(mDepthStencilImage)
	{
		rpDesc.DepthAttachment.Format = depthFormat;
		rpDesc.DepthAttachment.IsShaderReadAllowed = false;
		rpDesc.DepthAttachment.IsEnabled = true;
	}

	VulkanRenderPass* renderPass = VulkanRenderPassCache::Instance().FindOrCreateRenderPass(mDevice, rpDesc);

	// Create a framebuffer for each swap chain buffer
	u32 numFramebuffers = (u32)mSurfaces.size();
	for(u32 i = 0; i < numFramebuffers; i++)
	{
		VulkanFramebufferInformation& desc = mSurfaces[i].FramebufferInformation;
		desc.Width = mWidth;
		desc.Height = mHeight;
		desc.Layers = 1;
		desc.Color[0].Image = mSurfaces[i].Image;
		desc.Color[0].Surface = TextureSurface::kComplete;
		desc.Color[0].BaseLayer = 0;
		desc.Depth.Image = mDepthStencilImage;
		desc.Depth.Surface = TextureSurface::kComplete;
		desc.Depth.BaseLayer = 0;

		mSurfaces[i].Framebuffer = owner->Create<VulkanFramebuffer>(renderPass, desc);
	}
}

VulkanSwapChain::~VulkanSwapChain()
{
	if(mSwapChain != VK_NULL_HANDLE)
	{
		for(auto& surface : mSurfaces)
		{
			// Swap chain images only live as long as the swap chain, so its invalid if they are being used somewhere,
			// and same goes for the framebuffer since it depends on those images.
			B3D_ASSERT(!surface.Image->IsBound());
			B3D_ASSERT(!surface.Framebuffer->IsBound());

			surface.Framebuffer->Destroy();
			surface.Framebuffer = nullptr;

			surface.Image->Destroy();
			surface.Image = nullptr;

			surface.Semaphore->Destroy();
			surface.Semaphore = nullptr;
		}

		vkDestroySwapchainKHR(mDevice, mSwapChain, gVulkanAllocator);
	}

	if(mDepthStencilImage != nullptr)
	{
		mDepthStencilImage->Destroy();
		mDepthStencilImage = nullptr;
	}
}

VkResult VulkanSwapChain::AcquireImage(u32& outAcquiredImageIndex)
{
	const u32 maximumImageCount = (u32)(mSurfaces.size() - 1);
	if(mAcquiredImageCount >= maximumImageCount)
	{
		B3D_LOG(Error, RenderBackend, "Unable to acquire a swap chain image. Maximum number of images has already been acquired.");
		return VK_NOT_READY;
	}

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(mDevice, mSwapChain, UINT64_MAX, mSurfaces[mLastAcquiredSemaphoreIndex].Semaphore->GetHandle(), VK_NULL_HANDLE, &imageIndex);

	if(result != VK_SUCCESS)
		return result;

	// In case surfaces aren't being distributed in round-robin fashion the image and semaphore indices might not match,
	// in which case just move the semaphores
	if(imageIndex != mLastAcquiredSemaphoreIndex)
		std::swap(mSurfaces[mLastAcquiredSemaphoreIndex].Semaphore, mSurfaces[imageIndex].Semaphore);

	mLastAcquiredSemaphoreIndex = (mLastAcquiredSemaphoreIndex + 1) % mSurfaces.size();

	B3D_ASSERT(!mSurfaces[imageIndex].Acquired && "Same swap chain surface being acquired twice in a row without present().");
	mSurfaces[imageIndex].Acquired = true;
	mSurfaces[imageIndex].NeedsWait = true;

	mLastAcquiredImageIndex = imageIndex;
	mAcquiredImageCount++;

	outAcquiredImageIndex = imageIndex;
	return VK_SUCCESS;
}

bool VulkanSwapChain::PrepareForPresent(u32& outImageIndex)
{
	if(mAcquiredImageCount == 0)
		return false;

	const u32 imageCount = (UINT32)mSurfaces.size();
	const u32 offset = (imageCount - mAcquiredImageCount) + 1;
	const u32 imageToPresent = (mLastAcquiredImageIndex + offset) % imageCount;

	if(!mSurfaces[imageToPresent].Acquired)
		return false;

	mSurfaces[imageToPresent].Acquired = false;
	mAcquiredImageCount--;

	outImageIndex = imageToPresent;
	return true;
}

void VulkanSwapChain::NotifyBackBufferWaitIssued(u32 imageIndex)
{
	mSurfaces[imageIndex].NeedsWait = false;
}
