//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsVulkanPrerequisites.h"
#include "RenderAPI/BsGpuPipelineParamInfo.h"
#include "Allocators/BsGroupAlloc.h"

namespace bs { namespace ct
{
	/** @addtogroup Vulkan
	 *  @{
	 */

	/** Holds meta-data about a set of GPU parameters used by a single pipeline state. */
	class VulkanGpuPipelineParamInfo : public GpuPipelineParamInfo
	{
	public:
		VulkanGpuPipelineParamInfo(const GPU_PIPELINE_PARAMS_DESC& desc, GpuDeviceFlags deviceMask);
		~VulkanGpuPipelineParamInfo() = default;

		/** Returns the number of bindings present at the layout at the specified index. */
		UINT32 GetNumBindings(UINT32 layoutIdx) const { return mLayoutInfos[layoutIdx].numBindings; }

		/** Returns a pointer to an array of bindings for the layout at the specified index. */
		VkDescriptorSetLayoutBinding* GetBindings(UINT32 layoutIdx) const { return mLayoutInfos[layoutIdx].bindings; }

		/** Returns a pointer to any array of types expected by layout bindings. */
		GpuParamObjectType* GetLayoutTypes(UINT32 layoutIdx) const { return mLayoutInfos[layoutIdx].types; }

		/** Returns a pointer to any array of underlying element types for textures/buffers. */
		GpuBufferFormat* GetLayoutElementTypes(UINT32 layoutIdx) const { return mLayoutInfos[layoutIdx].elementTypes; }

		/** Returns the sequential index of the binding at the specificn set/slot. Returns -1 if slot is not used. */
		UINT32 GetBindingIdx(UINT32 set, UINT32 slot) const { return mSetExtraInfos[set].slotIndices[slot]; }

		/**
		 * Returns a layout for the specified device, at the specified index. Returns null if no layout for the specified
		 * device index.
		 */
		VulkanDescriptorLayout* GetLayout(UINT32 deviceIdx, UINT32 layoutIdx) const;

	private:
		/**	@copydoc GpuPipelineParamInfo::initialize */
		void Initialize() ;

		/** Data related to a single descriptor set layout. */
		struct LayoutInfo
		{
			VkDescriptorSetLayoutBinding* bindings;
			GpuParamObjectType* types;
			GpuBufferFormat* elementTypes;
			UINT32 numBindings;
		};

		/** Information about a single set in the param info object. Complements SetInfo. */
		struct SetExtraInfo
		{
			UINT32* slotIndices;
		};

		GpuDeviceFlags mDeviceMask;

		SetExtraInfo* mSetExtraInfos = nullptr;
		VulkanDescriptorLayout** mLayouts[BS_MAX_DEVICES];
		LayoutInfo* mLayoutInfos;

		GroupAlloc mAlloc;
	};

	/** @} */
}}
