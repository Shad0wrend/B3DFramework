//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsVulkanGpuParams.h"
#include "BsVulkanUtility.h"
#include "BsVulkanRenderAPI.h"
#include "BsVulkanDevice.h"
#include "BsVulkanGpuParamBlockBuffer.h"
#include "BsVulkanGpuBuffer.h"
#include "BsVulkanTexture.h"
#include "BsVulkanHardwareBuffer.h"
#include "BsVulkanDescriptorSet.h"
#include "BsVulkanDescriptorLayout.h"
#include "BsVulkanSamplerState.h"
#include "BsVulkanGpuPipelineParamInfo.h"
#include "BsVulkanCommandBuffer.h"
#include "Managers/BsVulkanTextureManager.h"
#include "Managers/BsVulkanHardwareBufferManager.h"
#include "RenderAPI/BsGpuParamDesc.h"

using namespace bs;
using namespace bs::ct;

VulkanGpuParams::VulkanGpuParams(const SPtr<GpuPipelineParamInfo>& paramInfo, GpuDeviceFlags deviceMask)
	: GpuParams(paramInfo, deviceMask), mPerDeviceData(), mDeviceMask(deviceMask)
{
}

VulkanGpuParams::~VulkanGpuParams()
{
	Lock lock(mMutex);

	u32 numSets = mParamInfo->GetSetCount();
	for(u32 i = 0; i < B3D_MAX_DEVICES; i++)
	{
		if(mPerDeviceData[i].PerSetData == nullptr)
			continue;

		for(u32 j = 0; j < numSets; j++)
		{
			for(auto& entry : mPerDeviceData[i].PerSetData[j].Sets)
				entry->Destroy();

			mPerDeviceData[i].PerSetData[j].Sets.~Vector<VulkanDescriptorSet*>();
		}
	}
}

void VulkanGpuParams::Initialize()
{
	VulkanGpuPipelineParamInfo& vkParamInfo = static_cast<VulkanGpuPipelineParamInfo&>(*mParamInfo);

	VulkanRenderAPI& rapi = static_cast<VulkanRenderAPI&>(RenderAPI::Instance());
	VulkanDevice* devices[B3D_MAX_DEVICES];

	VulkanUtility::GetDevices(rapi, mDeviceMask, devices);

	u32 deviceCount = 0;
	for(u32 deviceIndex = 0; deviceIndex < B3D_MAX_DEVICES; deviceIndex++)
	{
		if(devices[deviceIndex] != nullptr)
			deviceCount++;
	}

	const u32 parameterBlockCount = vkParamInfo.GetElementCount(GpuPipelineParamInfo::ParamType::ParamBlock);
	const u32 sampledTextureCount = vkParamInfo.GetElementCount(GpuPipelineParamInfo::ParamType::Texture);
	const u32 storageTextureCount = vkParamInfo.GetElementCount(GpuPipelineParamInfo::ParamType::LoadStoreTexture);
	const u32 bufferCount = vkParamInfo.GetElementCount(GpuPipelineParamInfo::ParamType::Buffer);
	const u32 samplerCount = vkParamInfo.GetElementCount(GpuPipelineParamInfo::ParamType::SamplerState);
	const u32 setCount = vkParamInfo.GetSetCount();
	const u32 bindingCount = vkParamInfo.GetElementCount();

	if(setCount == 0)
		return;

	// Note: I'm assuming a single WriteInfo per binding, but if arrays sizes larger than 1 are eventually supported
	// I'll need to adjust the code.
	mAlloc.Reserve<bool>(setCount)
		.Reserve<PerSetData>(setCount * deviceCount)
		.Reserve<VkWriteDescriptorSet>(bindingCount * deviceCount)
		.Reserve<WriteInfo>(bindingCount * deviceCount)
		.Reserve<VkImage>(sampledTextureCount * deviceCount)
		.Reserve<VkImage>(storageTextureCount * deviceCount)
		.Reserve<VkBuffer>(parameterBlockCount * deviceCount)
		.Reserve<VkBuffer>(bufferCount * deviceCount)
		.Reserve<VkSampler>(samplerCount * deviceCount)
		.Init();

	Lock lock(mMutex); // Set write operations need to be thread safe

	mSetsDirty = mAlloc.Alloc<bool>(setCount);
	B3DZeroOut(mSetsDirty, setCount);

	VulkanSamplerState* defaultSampler = static_cast<VulkanSamplerState*>(SamplerState::GetDefault().get());
	VulkanTextureManager& vkTexManager = static_cast<VulkanTextureManager&>(TextureManager::Instance());
	VulkanHardwareBufferManager& vkBufManager = static_cast<VulkanHardwareBufferManager&>(
		HardwareBufferManager::Instance());

	for(u32 deviceIndex = 0; deviceIndex < B3D_MAX_DEVICES; deviceIndex++)
	{
		if(devices[deviceIndex] == nullptr)
		{
			mPerDeviceData[deviceIndex].PerSetData = nullptr;

			continue;
		}

		mPerDeviceData[deviceIndex].PerSetData = mAlloc.Alloc<PerSetData>(setCount);
		mPerDeviceData[deviceIndex].SampledImages = mAlloc.Alloc<VkImage>(sampledTextureCount);
		mPerDeviceData[deviceIndex].StorageImages = mAlloc.Alloc<VkImage>(storageTextureCount);
		mPerDeviceData[deviceIndex].UniformBuffers = mAlloc.Alloc<VkBuffer>(parameterBlockCount);
		mPerDeviceData[deviceIndex].Buffers = mAlloc.Alloc<VkBuffer>(bufferCount);
		mPerDeviceData[deviceIndex].Samplers = mAlloc.Alloc<VkSampler>(samplerCount);

		B3DZeroOut(mPerDeviceData[deviceIndex].SampledImages, sampledTextureCount);
		B3DZeroOut(mPerDeviceData[deviceIndex].StorageImages, storageTextureCount);
		B3DZeroOut(mPerDeviceData[deviceIndex].UniformBuffers, parameterBlockCount);
		B3DZeroOut(mPerDeviceData[deviceIndex].Buffers, bufferCount);
		B3DZeroOut(mPerDeviceData[deviceIndex].Samplers, samplerCount);

		VulkanDescriptorManager& descManager = devices[deviceIndex]->GetDescriptorManager();
		VulkanSampler* vkDefaultSampler = defaultSampler->GetResource(deviceIndex);

		for(u32 setIndex = 0; setIndex < setCount; setIndex++)
		{
			const u32 bindingCountPerSet = vkParamInfo.GetBindingCount(setIndex);

			PerSetData& perSetData = mPerDeviceData[deviceIndex].PerSetData[setIndex];
			new(&perSetData.Sets) Vector<VulkanDescriptorSet*>();

			perSetData.WriteSetInfos = mAlloc.Alloc<VkWriteDescriptorSet>(bindingCountPerSet);
			perSetData.WriteInfos = mAlloc.Alloc<WriteInfo>(bindingCountPerSet);

			VulkanDescriptorLayout* layout = vkParamInfo.GetLayout(deviceIndex, setIndex);
			perSetData.ElementCount = bindingCountPerSet;
			perSetData.LastFreeSetIndex = 0;
			perSetData.LastUsedSet = descManager.CreateSet(layout);
			perSetData.Sets.push_back(perSetData.LastUsedSet);

			VkDescriptorSetLayoutBinding* perSetBindings = vkParamInfo.GetBindings(setIndex);
			GpuParamObjectType* types = vkParamInfo.GetLayoutTypes(setIndex);
			GpuBufferFormat* elementTypes = vkParamInfo.GetLayoutElementTypes(setIndex);
			for(u32 k = 0; k < bindingCountPerSet; k++)
			{
				// Note: Instead of using one structure per binding, it's possible to update multiple at once
				// by specifying larger descriptorCount, if they all share type and shader stages.
				VkWriteDescriptorSet& writeSetInfo = perSetData.WriteSetInfos[k];
				writeSetInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				writeSetInfo.pNext = nullptr;
				writeSetInfo.dstSet = VK_NULL_HANDLE;
				writeSetInfo.dstBinding = perSetBindings[k].binding;
				writeSetInfo.dstArrayElement = 0;
				writeSetInfo.descriptorCount = perSetBindings[k].descriptorCount;
				writeSetInfo.descriptorType = perSetBindings[k].descriptorType;

				u32 slot = perSetBindings[k].binding;

				bool isSampler = writeSetInfo.descriptorType == VK_DESCRIPTOR_TYPE_SAMPLER;
				if(isSampler)
				{
					VkDescriptorImageInfo& imageInfo = perSetData.WriteInfos[k].Image;
					imageInfo.sampler = vkDefaultSampler->GetHandle();
					imageInfo.imageView = VK_NULL_HANDLE;
					imageInfo.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;

					writeSetInfo.pImageInfo = &imageInfo;
					writeSetInfo.pBufferInfo = nullptr;
					writeSetInfo.pTexelBufferView = nullptr;
				}
				else
				{
					bool isImage = writeSetInfo.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ||
						writeSetInfo.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE ||
						writeSetInfo.descriptorType == VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;

					if(isImage)
					{
						VulkanImage* res = vkTexManager.GetDummyTexture(types[k])->GetResource(deviceIndex);
						VkFormat format = VulkanTextureManager::GetDummyViewFormat(elementTypes[k]);

						VkDescriptorImageInfo& imageInfo = perSetData.WriteInfos[k].Image;

						bool isLoadStore = writeSetInfo.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
						if(isLoadStore)
						{
							imageInfo.sampler = VK_NULL_HANDLE;
							imageInfo.imageView = res->GetView(format, false);
							imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;

							u32 sequentialIdx = vkParamInfo.GetSequentialSlot(GpuPipelineParamInfo::ParamType::LoadStoreTexture, setIndex, slot);
							mPerDeviceData[deviceIndex].StorageImages[sequentialIdx] = res->GetHandle();
						}
						else
						{
							bool isCombinedImageSampler = writeSetInfo.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

							if(isCombinedImageSampler)
								imageInfo.sampler = vkDefaultSampler->GetHandle();
							else
								imageInfo.sampler = VK_NULL_HANDLE;

							imageInfo.imageView = res->GetView(format, false);
							imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

							u32 sequentialIdx = vkParamInfo.GetSequentialSlot(GpuPipelineParamInfo::ParamType::Texture, setIndex, slot);
							mPerDeviceData[deviceIndex].SampledImages[sequentialIdx] = res->GetHandle();
						}

						writeSetInfo.pImageInfo = &imageInfo;
						writeSetInfo.pBufferInfo = nullptr;
						writeSetInfo.pTexelBufferView = nullptr;
					}
					else
					{
						bool useView = writeSetInfo.descriptorType != VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER &&
							writeSetInfo.descriptorType != VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC &&
							writeSetInfo.descriptorType != VK_DESCRIPTOR_TYPE_STORAGE_BUFFER &&
							writeSetInfo.descriptorType != VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;

						if(!useView)
						{
							VkDescriptorBufferInfo& bufferInfo = perSetData.WriteInfos[k].Buffer;
							bufferInfo.offset = 0;
							bufferInfo.range = VK_WHOLE_SIZE;

							if(writeSetInfo.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER || writeSetInfo.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC)
							{
								VulkanHardwareBuffer* buffer = vkBufManager.GetDummyUniformBuffer();
								bufferInfo.buffer = buffer->GetResource(deviceIndex)->GetHandle();

								u32 sequentialIdx = vkParamInfo.GetSequentialSlot(GpuPipelineParamInfo::ParamType::ParamBlock, setIndex, slot);
								mPerDeviceData[deviceIndex].UniformBuffers[sequentialIdx] = bufferInfo.buffer;
							}
							else
							{
								VulkanHardwareBuffer* buffer = vkBufManager.GetDummyUniformBuffer();
								bufferInfo.buffer = buffer->GetResource(deviceIndex)->GetHandle();

								u32 sequentialIdx = vkParamInfo.GetSequentialSlot(GpuPipelineParamInfo::ParamType::Buffer, setIndex, slot);
								mPerDeviceData[deviceIndex].Buffers[sequentialIdx] = bufferInfo.buffer;
							}

							writeSetInfo.pBufferInfo = &bufferInfo;
							writeSetInfo.pTexelBufferView = nullptr;
						}
						else
						{
							writeSetInfo.pBufferInfo = nullptr;

							bool isLoadStore = writeSetInfo.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;

							VulkanBuffer* buffer;
							if(isLoadStore)
								buffer = vkBufManager.GetDummyStorageBuffer()->GetResource(deviceIndex);
							else
								buffer = vkBufManager.GetDummyReadBuffer()->GetResource(deviceIndex);

							VkFormat format = VulkanUtility::GetBufferFormat(elementTypes[k]);
							VkBufferView bufferView = buffer->GetView(format);

							perSetData.WriteInfos[k].BufferView = bufferView;
							writeSetInfo.pBufferInfo = nullptr;
							writeSetInfo.pTexelBufferView = &perSetData.WriteInfos[k].BufferView;

							u32 sequentialIdx = vkParamInfo.GetSequentialSlot(GpuPipelineParamInfo::ParamType::Buffer, setIndex, slot);
							mPerDeviceData[deviceIndex].Buffers[sequentialIdx] = buffer->GetHandle();
						}

						writeSetInfo.pImageInfo = nullptr;
					}
				}
			}
		}
	}

	GpuParams::Initialize();
}

void VulkanGpuParams::SetParamBlockBuffer(u32 set, u32 slot, const SPtr<GpuParamBlockBuffer>& paramBlockBuffer)
{
	GpuParams::SetParamBlockBuffer(set, slot, paramBlockBuffer);

	VulkanGpuPipelineParamInfo& pipelineParameterInformation = static_cast<VulkanGpuPipelineParamInfo&>(*mParamInfo);
	const u32 bindingIndex = pipelineParameterInformation.GetBindingIdx(set, slot);
	if(bindingIndex == ~0u)
	{
		B3D_LOG(Error, RenderBackend, "Provided set/slot combination is not used by the GPU program: {0},{1}.", set, slot);
		return;
	}

	const u32 sequentialIndex = pipelineParameterInformation.GetSequentialSlot(GpuPipelineParamInfo::ParamType::ParamBlock, set, slot);

	Lock lock(mMutex);

	auto* vulkanParamBlockBuffer = static_cast<VulkanGpuParamBlockBuffer*>(paramBlockBuffer.get());
	for(u32 deviceIndex = 0; deviceIndex < B3D_MAX_DEVICES; deviceIndex++)
	{
		if(mPerDeviceData[deviceIndex].PerSetData == nullptr)
			continue;

		VulkanBuffer* vulkanBuffer;
		VkDeviceSize bufferSize;
		if(vulkanParamBlockBuffer != nullptr)
		{
			vulkanBuffer = vulkanParamBlockBuffer->GetResource(deviceIndex);
			bufferSize = vulkanParamBlockBuffer->GetSize();
		}
		else
		{
			vulkanBuffer = nullptr;
			bufferSize = VK_WHOLE_SIZE;
		}

		PerSetData& perSetData = mPerDeviceData[deviceIndex].PerSetData[set];

		VkBuffer vkBuffer = VK_NULL_HANDLE;
		if(vulkanBuffer != nullptr)
		{
			vkBuffer = vulkanBuffer->GetHandle();
		}
		else
		{
			auto& vulkanBufferManager = static_cast<VulkanHardwareBufferManager&>(HardwareBufferManager::Instance());
			vkBuffer = vulkanBufferManager.GetDummyUniformBuffer()->GetResource(deviceIndex)->GetHandle();
		}

		if(vkBuffer != mPerDeviceData[deviceIndex].UniformBuffers[sequentialIndex])
		{
			perSetData.WriteInfos[bindingIndex].Buffer.buffer = vkBuffer;
			perSetData.WriteInfos[bindingIndex].Buffer.range = bufferSize;
			mPerDeviceData[deviceIndex].UniformBuffers[sequentialIndex] = vkBuffer;

			mSetsDirty[set] = true;
		}
	}
}

void VulkanGpuParams::SetTexture(u32 set, u32 slot, const SPtr<Texture>& texture, const TextureSurface& surface)
{
	GpuParams::SetTexture(set, slot, texture, surface);

	VulkanGpuPipelineParamInfo& pipelineParameterInformation = static_cast<VulkanGpuPipelineParamInfo&>(*mParamInfo);
	const u32 bindingIndex = pipelineParameterInformation.GetBindingIdx(set, slot);
	if(bindingIndex == ~0u)
	{
		B3D_LOG(Error, RenderBackend, "Provided set/slot combination is not used by the GPU program: {0},{1}.", set, slot);
		return;
	}

	const u32 sequentialIndex = pipelineParameterInformation.GetSequentialSlot(GpuPipelineParamInfo::ParamType::Texture, set, slot);

	Lock lock(mMutex);

	VulkanTexture* vulkanTexture = static_cast<VulkanTexture*>(texture.get());
	for(u32 deviceIndex = 0; deviceIndex < B3D_MAX_DEVICES; deviceIndex++)
	{
		if(mPerDeviceData[deviceIndex].PerSetData == nullptr)
			continue;

		VulkanImage* vulkanImage;
		if(vulkanTexture != nullptr)
			vulkanImage = vulkanTexture->GetResource(deviceIndex);
		else
			vulkanImage = nullptr;

		PerSetData& perSetData = mPerDeviceData[deviceIndex].PerSetData[set];

		VkImageView vkImageView = VK_NULL_HANDLE;
		VkImage vkImage = VK_NULL_HANDLE;
		if(vulkanImage != nullptr)
		{
			vkImageView = vulkanImage->GetView(surface, false);
			vkImage = vulkanImage->GetHandle();
		}
		else
		{
			auto& vkTexManager = static_cast<VulkanTextureManager&>(TextureManager::Instance());

			GpuParamObjectType* types = pipelineParameterInformation.GetLayoutTypes(set);
			GpuBufferFormat* elementTypes = pipelineParameterInformation.GetLayoutElementTypes(set);

			vulkanImage = vkTexManager.GetDummyTexture(types[bindingIndex])->GetResource(deviceIndex);
			VkFormat format = VulkanTextureManager::GetDummyViewFormat(elementTypes[bindingIndex]);

			vkImageView = vulkanImage->GetView(format, false);
			vkImage = vulkanImage->GetHandle();
		}

		if(vkImage != mPerDeviceData[deviceIndex].SampledImages[sequentialIndex] || vkImageView != perSetData.WriteInfos[bindingIndex].Image.imageView)
		{
			perSetData.WriteInfos[bindingIndex].Image.imageView = vkImageView;
			mPerDeviceData[deviceIndex].SampledImages[sequentialIndex] = vkImage;

			mSetsDirty[set] = true;
		}
	}
}

void VulkanGpuParams::SetLoadStoreTexture(u32 set, u32 slot, const SPtr<Texture>& texture, const TextureSurface& surface)
{
	GpuParams::SetLoadStoreTexture(set, slot, texture, surface);

	VulkanGpuPipelineParamInfo& pipelineParameterInformation = static_cast<VulkanGpuPipelineParamInfo&>(*mParamInfo);
	const u32 bindingIndex = pipelineParameterInformation.GetBindingIdx(set, slot);
	if(bindingIndex == ~0u)
	{
		B3D_LOG(Error, RenderBackend, "Provided set/slot combination is not used by the GPU program: {0},{1}.", set, slot);
		return;
	}

	const u32 sequentialIndex = pipelineParameterInformation.GetSequentialSlot(GpuPipelineParamInfo::ParamType::LoadStoreTexture, set, slot);

	Lock lock(mMutex);

	VulkanTexture* vulkanTexture = static_cast<VulkanTexture*>(texture.get());
	for(u32 deviceIndex = 0; deviceIndex < B3D_MAX_DEVICES; deviceIndex++)
	{
		if(mPerDeviceData[deviceIndex].PerSetData == nullptr)
			continue;

		VulkanImage* vulkanImage;
		if(vulkanTexture != nullptr)
			vulkanImage = vulkanTexture->GetResource(deviceIndex);
		else
			vulkanImage = nullptr;

		PerSetData& perSetData = mPerDeviceData[deviceIndex].PerSetData[set];
		VkImageView vkImageView = VK_NULL_HANDLE;
		VkImage vkImage = VK_NULL_HANDLE;
		if(vulkanImage != nullptr)
		{
			vkImageView = vulkanImage->GetView(surface, false);
			vkImage = vulkanImage->GetHandle();
		}
		else
		{
			auto& vkTexManager = static_cast<VulkanTextureManager&>(TextureManager::Instance());

			GpuParamObjectType* types = pipelineParameterInformation.GetLayoutTypes(set);
			GpuBufferFormat* elementTypes = pipelineParameterInformation.GetLayoutElementTypes(set);

			vulkanImage = vkTexManager.GetDummyTexture(types[bindingIndex])->GetResource(deviceIndex);
			VkFormat format = VulkanTextureManager::GetDummyViewFormat(elementTypes[bindingIndex]);

			vkImageView = vulkanImage->GetView(format, false);
			vkImage = vulkanImage->GetHandle();
		}

		if(vkImage != mPerDeviceData[deviceIndex].StorageImages[sequentialIndex] || vkImageView != perSetData.WriteInfos[bindingIndex].Image.imageView)
		{
			perSetData.WriteInfos[bindingIndex].Image.imageView = vkImageView;
			mPerDeviceData[deviceIndex].StorageImages[sequentialIndex] = vkImage;

			mSetsDirty[set] = true;
		}
	}
}

void VulkanGpuParams::SetBuffer(u32 set, u32 slot, const SPtr<GpuBuffer>& buffer)
{
	GpuParams::SetBuffer(set, slot, buffer);

	VulkanGpuPipelineParamInfo& vkParamInfo = static_cast<VulkanGpuPipelineParamInfo&>(*mParamInfo);
	const u32 bindingIndex = vkParamInfo.GetBindingIdx(set, slot);
	if(bindingIndex == ~0u)
	{
		B3D_LOG(Error, RenderBackend, "Provided set/slot combination is not used by the GPU program: {0},{1}.", set, slot);
		return;
	}

	const u32 sequentialIndex = vkParamInfo.GetSequentialSlot(GpuPipelineParamInfo::ParamType::Buffer, set, slot);

	Lock lock(mMutex);

	VulkanGpuBuffer* vulkanBuffer = static_cast<VulkanGpuBuffer*>(buffer.get());
	for(u32 deviceIndex = 0; deviceIndex < B3D_MAX_DEVICES; deviceIndex++)
	{
		if(mPerDeviceData[deviceIndex].PerSetData == nullptr)
			continue;

		VulkanBuffer* bufferRes;

		if(vulkanBuffer != nullptr)
			bufferRes = vulkanBuffer->GetResource(deviceIndex);
		else
			bufferRes = nullptr;

		PerSetData& perSetData = mPerDeviceData[deviceIndex].PerSetData[set];
		VkWriteDescriptorSet& writeSetInfo = perSetData.WriteSetInfos[bindingIndex];

		const bool useView = writeSetInfo.descriptorType != VK_DESCRIPTOR_TYPE_STORAGE_BUFFER && writeSetInfo.descriptorType != VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
		VkBufferView vkBufferView = VK_NULL_HANDLE;
		if(bufferRes == nullptr)
		{
			auto& vulkanBufferManager = static_cast<VulkanHardwareBufferManager&>(HardwareBufferManager::Instance());
			if(useView)
			{
				const bool isLoadStore = writeSetInfo.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
				if(isLoadStore)
					bufferRes = vulkanBufferManager.GetDummyStorageBuffer()->GetResource(deviceIndex);
				else
					bufferRes = vulkanBufferManager.GetDummyReadBuffer()->GetResource(deviceIndex);

				const GpuBufferFormat* const elementTypes = vkParamInfo.GetLayoutElementTypes(set);
				const VkFormat format = VulkanUtility::GetBufferFormat(elementTypes[bindingIndex]);
				vkBufferView = bufferRes->GetView(format);
			}
			else
			{
				bufferRes = vulkanBufferManager.GetDummyStructuredBuffer()->GetResource(deviceIndex);
			}
		}
		else
		{
			if(useView)
			{
				vkBufferView = vulkanBuffer->GetView(deviceIndex);
			}
		}

		const VkBuffer vkBuffer = bufferRes->GetHandle();

		const bool isBufferViewChanged = useView && perSetData.WriteInfos[bindingIndex].BufferView != vkBufferView;
		if(mPerDeviceData[deviceIndex].Buffers[sequentialIndex] != vkBuffer || isBufferViewChanged)
		{
			if(useView)
			{
				perSetData.WriteInfos[bindingIndex].BufferView = vkBufferView;
				writeSetInfo.pTexelBufferView = &perSetData.WriteInfos[bindingIndex].BufferView;
			}
			else // Structured storage buffer
			{
				perSetData.WriteInfos[bindingIndex].Buffer.buffer = vkBuffer;
				mPerDeviceData[deviceIndex].Buffers[sequentialIndex] = vkBuffer;

				writeSetInfo.pTexelBufferView = nullptr;
			}

			mPerDeviceData[deviceIndex].Buffers[sequentialIndex] = vkBuffer;
			mSetsDirty[set] = true;
		}
	}
}

void VulkanGpuParams::SetSamplerState(u32 set, u32 slot, const SPtr<SamplerState>& sampler)
{
	GpuParams::SetSamplerState(set, slot, sampler);

	VulkanGpuPipelineParamInfo& vkParamInfo = static_cast<VulkanGpuPipelineParamInfo&>(*mParamInfo);
	const u32 bindingIndex = vkParamInfo.GetBindingIdx(set, slot);
	if(bindingIndex == (u32)-1)
	{
		B3D_LOG(Error, RenderBackend, "Provided set/slot combination is not used by the GPU program: {0},{1}.", set, slot);
		return;
	}

	const u32 sequentialIndex = vkParamInfo.GetSequentialSlot(GpuPipelineParamInfo::ParamType::SamplerState, set, slot);

	Lock lock(mMutex);

	VulkanSamplerState* vulkanSampler = static_cast<VulkanSamplerState*>(sampler.get());
	for(u32 deviceIndex = 0; deviceIndex < B3D_MAX_DEVICES; deviceIndex++)
	{
		if(mPerDeviceData[deviceIndex].PerSetData == nullptr)
			continue;

		PerSetData& perSetData = mPerDeviceData[deviceIndex].PerSetData[set];

		VulkanSampler* samplerRes;
		if(vulkanSampler != nullptr)
			samplerRes = vulkanSampler->GetResource(deviceIndex);
		else
		{
			const VulkanSamplerState* const defaultSampler =
				static_cast<const VulkanSamplerState* const>(SamplerState::GetDefault().get());

			samplerRes = defaultSampler->GetResource(deviceIndex);
		}

		VkSampler vkSampler = samplerRes->GetHandle();
		if(mPerDeviceData[deviceIndex].Samplers[sequentialIndex] != vkSampler)
		{
			perSetData.WriteInfos[bindingIndex].Image.sampler = vkSampler;
			mPerDeviceData[deviceIndex].Samplers[sequentialIndex] = vkSampler;

			mSetsDirty[set] = true;
		}
	}

	mSetsDirty[set] = true;
}

u32 VulkanGpuParams::GetSetCount() const
{
	return mParamInfo->GetSetCount();
}

void VulkanGpuParams::PrepareForBind(VulkanInternalCommandBuffer& buffer, VkDescriptorSet* outSets, Vector<u32>& outDynamicOffsets)
{
	u32 deviceIdx = buffer.GetDeviceIdx();

	PerDeviceData& perDeviceData = mPerDeviceData[deviceIdx];
	if(perDeviceData.PerSetData == nullptr)
		return;

	VulkanGpuPipelineParamInfo& vkParamInfo = static_cast<VulkanGpuPipelineParamInfo&>(*mParamInfo);

	u32 parameterBlockCount = vkParamInfo.GetElementCount(GpuPipelineParamInfo::ParamType::ParamBlock);
	u32 sampledTextureCount = vkParamInfo.GetElementCount(GpuPipelineParamInfo::ParamType::Texture);
	u32 storageTextureCount = vkParamInfo.GetElementCount(GpuPipelineParamInfo::ParamType::LoadStoreTexture);
	u32 bufferCount = vkParamInfo.GetElementCount(GpuPipelineParamInfo::ParamType::Buffer);
	u32 samplerCount = vkParamInfo.GetElementCount(GpuPipelineParamInfo::ParamType::SamplerState);
	u32 setCount = vkParamInfo.GetSetCount();

	FrameScope frameScope;
	FrameVector<u32> dynamicOffsetMapping(perDeviceData.PerSetData->ElementCount, ~0u);

	Lock lock(mMutex);

	// Registers resources with the command buffer, and check if internal resource handled changed (in which case set
	// needs updating - this can happen due to resource writes, as internally system might find it more performant
	// to discard used resources and create new ones).
	// Note: Makes the assumption that this object (and all of the resources it holds) are externally locked, and will
	// not be modified on another thread while being bound.
	for(u32 parameterBlockIndex = 0; parameterBlockIndex < parameterBlockCount; parameterBlockIndex++)
	{
		VulkanBuffer* resource = nullptr;
		VkDeviceSize bufferSize = VK_WHOLE_SIZE;
		u32 dynamicOffset = 0;

		if(mParamBlockBuffers[parameterBlockIndex] != nullptr)
		{
			VulkanGpuParamBlockBuffer* element = static_cast<VulkanGpuParamBlockBuffer*>(mParamBlockBuffers[parameterBlockIndex].get());
			resource = element->GetResource(deviceIdx);
			bufferSize = element->GetSize();
			dynamicOffset = element->GetOffset();
		}

		if(resource == nullptr)
		{
			auto& vkBufManager = static_cast<VulkanHardwareBufferManager&>(HardwareBufferManager::Instance());
			resource = vkBufManager.GetDummyUniformBuffer()->GetResource(deviceIdx);

			if(resource == nullptr)
				continue;
		}

		u32 set, slot;
		mParamInfo->GetBinding(GpuPipelineParamInfo::ParamType::ParamBlock, parameterBlockIndex, set, slot);

		u32 bindingIdx = vkParamInfo.GetBindingIdx(set, slot);
		VkDescriptorSetLayoutBinding* perSetBindings = vkParamInfo.GetBindings(set);
		VkPipelineStageFlags stages = VulkanUtility::ShaderToPipelineStage(perSetBindings[bindingIdx].stageFlags);

		// Register with command buffer
		buffer.RegisterBuffer(resource, BufferUseFlagBits::Parameter, VulkanAccessFlag::Read, stages);

		// Check if internal resource changed from what was previously bound in the descriptor set
		B3D_ASSERT(perDeviceData.UniformBuffers[parameterBlockIndex] != VK_NULL_HANDLE);

		VkBuffer vkBuffer = resource->GetHandle();
		if(perDeviceData.UniformBuffers[parameterBlockIndex] != vkBuffer)
		{
			perDeviceData.UniformBuffers[parameterBlockIndex] = vkBuffer;
			perDeviceData.PerSetData[set].WriteInfos[bindingIdx].Buffer.buffer = vkBuffer;
			perDeviceData.PerSetData[set].WriteInfos[bindingIdx].Buffer.range = bufferSize;

			mSetsDirty[set] = true;
		}

		dynamicOffsetMapping[bindingIdx] = dynamicOffset;
	}

	for(u32 i = 0; i < bufferCount; i++)
	{
		u32 set, slot;
		mParamInfo->GetBinding(GpuPipelineParamInfo::ParamType::Buffer, i, set, slot);
		u32 bindingIdx = vkParamInfo.GetBindingIdx(set, slot);

		GpuParamObjectType* types = vkParamInfo.GetLayoutTypes(set);
		GpuParamObjectType type = types[bindingIdx];

		VulkanAccessFlags useFlags = VulkanAccessFlag::Read;
		VulkanBuffer* resource = nullptr;
		if(mBuffers[i] != nullptr)
		{
			auto* element = static_cast<VulkanGpuBuffer*>(mBuffers[i].get());
			resource = element->GetResource(deviceIdx);

			if((element->GetProperties().GetUsage() & GBU_LOADSTORE) == GBU_LOADSTORE)
				useFlags |= VulkanAccessFlag::Write;
		}

		if(resource == nullptr)
		{
			auto& vkBufManager = static_cast<VulkanHardwareBufferManager&>(HardwareBufferManager::Instance());

			switch(type)
			{
			case GPOT_BYTE_BUFFER:
				resource = vkBufManager.GetDummyReadBuffer()->GetResource(deviceIdx);
				break;
			case GPOT_RWBYTE_BUFFER:
				resource = vkBufManager.GetDummyStorageBuffer()->GetResource(deviceIdx);
				useFlags |= VulkanAccessFlag::Write;
				break;
			case GPOT_STRUCTURED_BUFFER:
			case GPOT_RWSTRUCTURED_BUFFER:
				resource = vkBufManager.GetDummyStructuredBuffer()->GetResource(deviceIdx);
				useFlags |= VulkanAccessFlag::Write;
				break;
			default:
				break;
			}

			if(resource == nullptr)
				continue;
		}

		// Register with command buffer
		VkDescriptorSetLayoutBinding* perSetBindings = vkParamInfo.GetBindings(set);
		VkPipelineStageFlags stages = VulkanUtility::ShaderToPipelineStage(perSetBindings[bindingIdx].stageFlags);
		buffer.RegisterBuffer(resource, BufferUseFlagBits::Generic, useFlags, stages);

		// Check if internal resource changed from what was previously bound in the descriptor set
		B3D_ASSERT(perDeviceData.Buffers[i] != VK_NULL_HANDLE);

		VkBuffer vkBuffer = resource->GetHandle();
		if(perDeviceData.Buffers[i] != vkBuffer)
		{
			perDeviceData.Buffers[i] = vkBuffer;

			VkBufferView view = VK_NULL_HANDLE;
			if(type != GPOT_STRUCTURED_BUFFER && type != GPOT_RWSTRUCTURED_BUFFER)
			{
				if(mBuffers[i] != nullptr)
				{
					auto* element = static_cast<VulkanGpuBuffer*>(mBuffers[i].get());
					view = element->GetView(deviceIdx);
				}
				else
				{
					GpuBufferFormat* elementTypes = vkParamInfo.GetLayoutElementTypes(set);
					view = resource->GetView(VulkanUtility::GetBufferFormat(elementTypes[bindingIdx]));
				}
			}

			PerSetData& perSetData = perDeviceData.PerSetData[set];
			if(view)
			{
				perSetData.WriteInfos[bindingIdx].BufferView = view;
				perSetData.WriteSetInfos[bindingIdx].pTexelBufferView = &perSetData.WriteInfos[bindingIdx].BufferView;
			}
			else // Structured storage buffer
			{
				perSetData.WriteInfos[bindingIdx].Buffer.buffer = vkBuffer;
				perSetData.WriteSetInfos[bindingIdx].pTexelBufferView = nullptr;
			}

			mSetsDirty[set] = true;
		}
	}

	for(u32 i = 0; i < samplerCount; i++)
	{
		if(mSamplerStates[i] == nullptr)
			continue;

		VulkanSamplerState* element = static_cast<VulkanSamplerState*>(mSamplerStates[i].get());
		VulkanSampler* resource = element->GetResource(deviceIdx);
		if(resource == nullptr)
			continue;

		// Register with command buffer
		buffer.RegisterResource(resource, VulkanAccessFlag::Read);

		// Check if internal resource changed from what was previously bound in the descriptor set
		B3D_ASSERT(perDeviceData.Samplers[i] != VK_NULL_HANDLE);

		VkSampler vkSampler = resource->GetHandle();
		if(perDeviceData.Samplers[i] != vkSampler)
		{
			perDeviceData.Samplers[i] = vkSampler;

			u32 set, slot;
			mParamInfo->GetBinding(GpuPipelineParamInfo::ParamType::SamplerState, i, set, slot);

			u32 bindingIdx = vkParamInfo.GetBindingIdx(set, slot);
			perDeviceData.PerSetData[set].WriteInfos[bindingIdx].Image.sampler = vkSampler;

			mSetsDirty[set] = true;
		}
	}

	for(u32 i = 0; i < storageTextureCount; i++)
	{
		u32 set, slot;
		mParamInfo->GetBinding(GpuPipelineParamInfo::ParamType::LoadStoreTexture, i, set, slot);
		u32 bindingIdx = vkParamInfo.GetBindingIdx(set, slot);

		VulkanImage* resource = nullptr;
		if(mLoadStoreTextureData[i].Texture != nullptr)
		{
			auto* element = static_cast<VulkanTexture*>(mLoadStoreTextureData[i].Texture.get());
			resource = element->GetResource(deviceIdx);
		}

		if(resource == nullptr)
		{
			auto& vkTexManager = static_cast<VulkanTextureManager&>(TextureManager::Instance());

			GpuParamObjectType* types = vkParamInfo.GetLayoutTypes(set);
			resource = vkTexManager.GetDummyTexture(types[bindingIdx])->GetResource(deviceIdx);

			if(resource == nullptr)
				continue;
		}

		const TextureSurface& surface = mLoadStoreTextureData[i].Surface;
		VkImageSubresourceRange range = resource->GetRange(surface);

		VkDescriptorSetLayoutBinding* perSetBindings = vkParamInfo.GetBindings(set);
		VkPipelineStageFlags stages = VulkanUtility::ShaderToPipelineStage(perSetBindings[bindingIdx].stageFlags);

		// Register with command buffer
		VulkanAccessFlags useFlags = VulkanAccessFlag::Read | VulkanAccessFlag::Write;
		buffer.RegisterImageShader(resource, range, VK_IMAGE_LAYOUT_GENERAL, useFlags, stages);

		// Check if internal resource changed from what was previously bound in the descriptor set
		B3D_ASSERT(perDeviceData.StorageImages[i] != VK_NULL_HANDLE);

		VkImage vkImage = resource->GetHandle();
		if(perDeviceData.StorageImages[i] != vkImage)
		{
			perDeviceData.StorageImages[i] = vkImage;

			VkImageView view;
			if(mLoadStoreTextureData[i].Texture)
				view = resource->GetView(surface, false);
			else
			{
				GpuBufferFormat* elementTypes = vkParamInfo.GetLayoutElementTypes(set);
				view = resource->GetView(VulkanTextureManager::GetDummyViewFormat(elementTypes[bindingIdx]));
			}

			perDeviceData.PerSetData[set].WriteInfos[bindingIdx].Image.imageView = view;
			mSetsDirty[set] = true;
		}
	}

	for(u32 i = 0; i < sampledTextureCount; i++)
	{
		u32 set, slot;
		mParamInfo->GetBinding(GpuPipelineParamInfo::ParamType::Texture, i, set, slot);
		u32 bindingIdx = vkParamInfo.GetBindingIdx(set, slot);

		VulkanImage* resource = nullptr;
		VkImageLayout layout;
		if(mSampledTextureData[i].Texture != nullptr)
		{
			VulkanTexture* element = static_cast<VulkanTexture*>(mSampledTextureData[i].Texture.get());
			resource = element->GetResource(deviceIdx);

			const TextureProperties& props = element->GetProperties();
			// Keep dynamic textures in general layout, so they can be easily mapped by CPU
			if(props.GetUsage() & TU_DYNAMIC)
				layout = VK_IMAGE_LAYOUT_GENERAL;
			else
				layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		}

		if(resource == nullptr)
		{
			auto& vkTexManager = static_cast<VulkanTextureManager&>(TextureManager::Instance());

			GpuParamObjectType* types = vkParamInfo.GetLayoutTypes(set);
			resource = vkTexManager.GetDummyTexture(types[bindingIdx])->GetResource(deviceIdx);
			layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			if(resource == nullptr)
				continue;
		}

		// Register with command buffer
		const TextureSurface& surface = mSampledTextureData[i].Surface;
		VkImageSubresourceRange range = resource->GetRange(surface);

		VkDescriptorSetLayoutBinding* perSetBindings = vkParamInfo.GetBindings(set);
		VkPipelineStageFlags stages = VulkanUtility::ShaderToPipelineStage(perSetBindings[bindingIdx].stageFlags);

		buffer.RegisterImageShader(resource, range, layout, VulkanAccessFlag::Read, stages);

		// Actual layout might be different than requested if the image is also used as a FB attachment
		layout = buffer.GetCurrentLayout(resource, range, true);

		// Check if internal resource changed from what was previously bound in the descriptor set
		B3D_ASSERT(perDeviceData.SampledImages[i] != VK_NULL_HANDLE);

		VkDescriptorImageInfo& imgInfo = perDeviceData.PerSetData[set].WriteInfos[bindingIdx].Image;

		VkImage vkImage = resource->GetHandle();
		if(perDeviceData.SampledImages[i] != vkImage)
		{
			perDeviceData.SampledImages[i] = vkImage;

			VkImageView view;
			if(mSampledTextureData[i].Texture)
				view = resource->GetView(surface, false);
			else
			{
				GpuBufferFormat* elementTypes = vkParamInfo.GetLayoutElementTypes(set);
				view = resource->GetView(VulkanTextureManager::GetDummyViewFormat(elementTypes[bindingIdx]));
			}

			imgInfo.imageView = view;
			mSetsDirty[set] = true;
		}

		if(imgInfo.imageLayout != layout)
		{
			imgInfo.imageLayout = layout;
			mSetsDirty[set] = true;
		}
	}

	// Output dynamic offsets
	for(u32 dynamicOffset : dynamicOffsetMapping)
	{
		if(dynamicOffset != ~0u)
			outDynamicOffsets.push_back(dynamicOffset);
	}

	// Acquire sets as needed, and updated their contents if dirty
	VulkanRenderAPI& rapi = static_cast<VulkanRenderAPI&>(RenderAPI::Instance());
	VulkanDevice& device = *rapi.GetDevice(deviceIdx);
	VulkanDescriptorManager& descManager = device.GetDescriptorManager();

	for(u32 setIndex = 0; setIndex < setCount; setIndex++)
	{
		PerSetData& perSetData = perDeviceData.PerSetData[setIndex];

		if(!mSetsDirty[setIndex]) // Set not dirty, just use the last one we wrote (this is fine even across multiple command buffers)
			continue;

		// Set is dirty, we need to update
		//// Use latest unless already used, otherwise try to find an unused one
		if(perSetData.LastUsedSet->IsBound()) // Checking this is okay, because it's only modified below when we call registerResource, which is under the same lock as this
		{
			perSetData.LastUsedSet = nullptr;

			for(size_t setCacheIndex = 0; setCacheIndex < perSetData.Sets.size(); setCacheIndex++)
			{
				const u32 setIndex = (perSetData.LastFreeSetIndex + setCacheIndex) % (u32)perSetData.Sets.size();
				if(!perSetData.Sets[setIndex]->IsBound())
				{
					perSetData.LastUsedSet = perSetData.Sets[setIndex];
					perSetData.LastFreeSetIndex = setIndex;

					break;
				}
			}

			// Cannot find an empty set, allocate a new one
			if(perSetData.LastUsedSet == nullptr)
			{
				VulkanDescriptorLayout* layout = vkParamInfo.GetLayout(deviceIdx, setIndex);
				perSetData.LastUsedSet = descManager.CreateSet(layout);
				perSetData.Sets.push_back(perSetData.LastUsedSet);
			}
		}

		// Note: Currently I write to the entire set at once, but it might be beneficial to remember only the exact
		// entries that were updated, and only write to them individually.
		perSetData.LastUsedSet->Write(perSetData.WriteSetInfos, perSetData.ElementCount);

		mSetsDirty[setIndex] = false;
	}

	for(u32 i = 0; i < setCount; i++)
	{
		VulkanDescriptorSet* set = perDeviceData.PerSetData[i].LastUsedSet;

		buffer.RegisterResource(set, VulkanAccessFlag::Read);
		outSets[i] = set->GetHandle();
	}
}
