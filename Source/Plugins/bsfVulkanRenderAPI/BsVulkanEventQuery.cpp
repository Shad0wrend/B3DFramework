//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsVulkanEventQuery.h"
#include "BsVulkanDevice.h"
#include "BsVulkanCommandBuffer.h"
#include "Profiling/BsRenderStats.h"

namespace bs { namespace ct
{
	VulkanEvent::VulkanEvent(VulkanResourceManager* owner)
		:VulkanResource(owner, false)
	{
		VkDevice vkDevice = owner->GetDevice().GetLogical();

		VkEventCreateInfo eventCI;
		eventCI.sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO;
		eventCI.pNext = nullptr;
		eventCI.flags = 0;

		VkResult result = vkCreateEvent(vkDevice, &eventCI, gVulkanAllocator, &mEvent);
		assert(result == VK_SUCCESS);
	}

	VulkanEvent::~VulkanEvent()
	{
		VkDevice vkDevice = mOwner->GetDevice().GetLogical();
		vkDestroyEvent(vkDevice, mEvent, gVulkanAllocator);
	}

	bool VulkanEvent::IsSignaled() const
	{
		VkDevice vkDevice = mOwner->GetDevice().GetLogical();
		return vkGetEventStatus(vkDevice, mEvent) == VK_EVENT_SET;
	}

	void VulkanEvent::reset()
	{
		VkDevice vkDevice = mOwner->GetDevice().getLogical();

		VkResult result = vkResetEvent(vkDevice, mEvent);
		assert(result == VK_SUCCESS);
	}

	VulkanEventQuery::VulkanEventQuery(VulkanDevice& device)
		:mDevice(device), mEvent(nullptr)
	{
		BS_INC_RENDER_STAT_CAT(ResCreated, RenderStatObject_Query);
	}

	VulkanEventQuery::~VulkanEventQuery()
	{
		if (mEvent != nullptr)
			mEvent->Destroy();

		BS_INC_RENDER_STAT_CAT(ResDestroyed, RenderStatObject_Query);
	}

	void VulkanEventQuery::Begin(const SPtr<CommandBuffer>& cb)
	{
		if (mEvent != nullptr)
		{
			if (mEvent->isBound())
			{
				// Clear current event and create a new one
				mEvent->Destroy();
				mEvent = mDevice.getResourceManager().create<VulkanEvent>();
			}
			else
			{
				// Re-use existing event
				mEvent->reset();
			}
		}
		else // Create new event
			mEvent = mDevice.getResourceManager().create<VulkanEvent>();

		VulkanCommandBuffer* vulkanCB;
		if (cb != nullptr)
			vulkanCB = static_cast<VulkanCommandBuffer*>(cb.get());
		else
			vulkanCB = static_cast<VulkanCommandBuffer*>(gVulkanRenderAPI().GetMainCommandBufferInternal());

		VulkanCmdBuffer* internalCB = vulkanCB->GetInternal();
		internalCB->registerResource(mEvent, VulkanAccessFlag::Read);

		internalCB->SetEvent(mEvent);

		setActive(true);
	}

	bool VulkanEventQuery::IsReady() const
	{
		if (mEvent == nullptr)
			return false;

		return mEvent->isSignaled();
	}
}}
