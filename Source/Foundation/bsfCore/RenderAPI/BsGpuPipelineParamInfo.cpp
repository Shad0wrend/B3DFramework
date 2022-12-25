//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "RenderAPI/BsGpuPipelineParamInfo.h"
#include "RenderAPI/BsGpuParamDesc.h"
#include "Managers/BsRenderStateManager.h"
#include "Math/BsMath.h"

using namespace bs;

GpuPipelineParamInfoBase::GpuPipelineParamInfoBase(const GPU_PIPELINE_PARAMS_DESC& desc)
	: mResourceInfos()
{
	B3DZeroOut(mBindingSlotCountPerType);
	B3DZeroOut(mResourceCountPerType);

	mParamDescs[GPT_FRAGMENT_PROGRAM] = desc.FragmentParams;
	mParamDescs[GPT_VERTEX_PROGRAM] = desc.VertexParams;
	mParamDescs[GPT_GEOMETRY_PROGRAM] = desc.GeometryParams;
	mParamDescs[GPT_HULL_PROGRAM] = desc.HullParams;
	mParamDescs[GPT_DOMAIN_PROGRAM] = desc.DomainParams;
	mParamDescs[GPT_COMPUTE_PROGRAM] = desc.ComputeParams;

	auto fnCountElementsForType = [this](auto& entry, ParamType type)
	{
		const u32 typeIndex = (u32)type;

		if((entry.Set + 1) > mSetCount)
			mSetCount = entry.Set + 1;

		mBindingSlotCountPerType[typeIndex]++;
		mBindingSlotCount++;

		mResourceCountPerType[typeIndex]++;
		mResourceCount++;
	};

	auto fnCountElementsForTypeWithArraySupport = [this](auto& entry, ParamType type)
	{
		const u32 typeIndex = (u32)type;
		const u32 arraySize = Math::Max(1u, entry.ArraySize);

		if((entry.Set + 1) > mSetCount)
			mSetCount = entry.Set + 1;

		mBindingSlotCountPerType[typeIndex]++;
		mBindingSlotCount++;

		mResourceCountPerType[typeIndex] += arraySize;
		mResourceCount += arraySize;
	};

	const u32 gpuProgramParameterSetsCount = sizeof(mParamDescs) / sizeof(mParamDescs[0]);
	for(u32 gpuProgramParameterSetIndex = 0; gpuProgramParameterSetIndex < gpuProgramParameterSetsCount; gpuProgramParameterSetIndex++)
	{
		const SPtr<GpuParamDesc>& paramDesc = mParamDescs[gpuProgramParameterSetIndex];
		if(paramDesc == nullptr)
			continue;

		for(auto& paramBlock : paramDesc->ParamBlocks)
			fnCountElementsForType(paramBlock.second, ParamType::ParamBlock);

		for(auto& texture : paramDesc->Textures)
			fnCountElementsForTypeWithArraySupport(texture.second, ParamType::Texture);

		for(auto& texture : paramDesc->LoadStoreTextures)
			fnCountElementsForTypeWithArraySupport(texture.second, ParamType::LoadStoreTexture);

		for(auto& buffer : paramDesc->Buffers)
			fnCountElementsForTypeWithArraySupport(buffer.second, ParamType::Buffer);

		for(auto& sampler : paramDesc->Samplers)
			fnCountElementsForType(sampler.second, ParamType::SamplerState);
	}

	u32* slotCountPerSet = (u32*)B3DStackAllocate(mSetCount * sizeof(u32));
	B3DZeroOut(slotCountPerSet, mSetCount);

	for(u32 gpuProgramParameterSetIndex = 0; gpuProgramParameterSetIndex < gpuProgramParameterSetsCount; gpuProgramParameterSetIndex++)
	{
		const SPtr<GpuParamDesc>& paramDesc = mParamDescs[gpuProgramParameterSetIndex];
		if(paramDesc == nullptr)
			continue;

		for(auto& paramBlock : paramDesc->ParamBlocks)
			slotCountPerSet[paramBlock.second.Set] =
				std::max(slotCountPerSet[paramBlock.second.Set], paramBlock.second.Slot + 1);

		for(auto& texture : paramDesc->Textures)
			slotCountPerSet[texture.second.Set] =
				std::max(slotCountPerSet[texture.second.Set], texture.second.Slot + 1);

		for(auto& texture : paramDesc->LoadStoreTextures)
			slotCountPerSet[texture.second.Set] =
				std::max(slotCountPerSet[texture.second.Set], texture.second.Slot + 1);

		for(auto& buffer : paramDesc->Buffers)
			slotCountPerSet[buffer.second.Set] =
				std::max(slotCountPerSet[buffer.second.Set], buffer.second.Slot + 1);

		for(auto& sampler : paramDesc->Samplers)
			slotCountPerSet[sampler.second.Set] =
				std::max(slotCountPerSet[sampler.second.Set], sampler.second.Slot + 1);
	}

	u32 totalSlotCount = 0;
	for(u32 setIndex = 0; setIndex < mSetCount; setIndex++)
		totalSlotCount += slotCountPerSet[setIndex];

	mAlloc.Reserve<SetInfo>(mSetCount)
		.Reserve<u32>(totalSlotCount)
		.Reserve<u32>(totalSlotCount)
		.Reserve<u32>(totalSlotCount)
		.Reserve<u32>(totalSlotCount)
		.Reserve<u32>(totalSlotCount)
		.Reserve<ParamType>(totalSlotCount);

	for(u32 parameterTypeIndex = 0; parameterTypeIndex < (u32)ParamType::Count; parameterTypeIndex++)
		mAlloc.Reserve<ResourceInfo>(mBindingSlotCountPerType[parameterTypeIndex]);

	mAlloc.Init();

	mSetInfos = mAlloc.Alloc<SetInfo>(mSetCount);

	if(mSetInfos != nullptr)
		B3DZeroOut(mSetInfos, mSetCount);

	for(u32 i = 0; i < mSetCount; i++)
		mSetInfos[i].SlotCount = slotCountPerSet[i];

	B3DStackFree(slotCountPerSet);

	for(u32 setIndex = 0; setIndex < mSetCount; setIndex++)
	{
		mSetInfos[setIndex].SlotToSequentialBindingIndex = mAlloc.Alloc<u32>(mSetInfos[setIndex].SlotCount);
		memset(mSetInfos[setIndex].SlotToSequentialBindingIndex, -1, sizeof(u32) * mSetInfos[setIndex].SlotCount);

		mSetInfos[setIndex].SlotToSequentialResourceIndex = mAlloc.Alloc<u32>(mSetInfos[setIndex].SlotCount);
		memset(mSetInfos[setIndex].SlotToSequentialResourceIndex, -1, sizeof(u32) * mSetInfos[setIndex].SlotCount);

		mSetInfos[setIndex].SlotToSequentialSamplerBindingIndex = mAlloc.Alloc<u32>(mSetInfos[setIndex].SlotCount);
		memset(mSetInfos[setIndex].SlotToSequentialSamplerBindingIndex, -1, sizeof(u32) * mSetInfos[setIndex].SlotCount);

		mSetInfos[setIndex].SlotToSequentialSamplerResourceIndex = mAlloc.Alloc<u32>(mSetInfos[setIndex].SlotCount);
		memset(mSetInfos[setIndex].SlotToSequentialSamplerResourceIndex, -1, sizeof(u32) * mSetInfos[setIndex].SlotCount);

		mSetInfos[setIndex].SlotArraySizes = mAlloc.Alloc<u32>(mSetInfos[setIndex].SlotCount);
		memset(mSetInfos[setIndex].SlotArraySizes, 0, sizeof(u32) * mSetInfos[setIndex].SlotCount);

		mSetInfos[setIndex].SlotTypes = mAlloc.Alloc<ParamType>(mSetInfos[setIndex].SlotCount);
	}

	for(u32 parameterTypeIndex = 0; parameterTypeIndex < (u32)ParamType::Count; parameterTypeIndex++)
	{
		mResourceInfos[parameterTypeIndex] = mAlloc.Alloc<ResourceInfo>(mBindingSlotCountPerType[parameterTypeIndex]);
		mBindingSlotCountPerType[parameterTypeIndex] = 0;
		mResourceCountPerType[parameterTypeIndex] = 0;
	}

	auto fnPopulateSetInfo = [this](auto& entry, ParamType type)
	{
		const u32 typeIndex = (u32)type;
		const u32 sequentialBindingIndex = mBindingSlotCountPerType[typeIndex];
		const u32 sequentialResourceIndex = mResourceCountPerType[typeIndex];

		SetInfo& setInfo = mSetInfos[entry.Set];
		setInfo.SlotToSequentialBindingIndex[entry.Slot] = sequentialBindingIndex;
		setInfo.SlotToSequentialResourceIndex[entry.Slot] = sequentialResourceIndex;
		setInfo.SlotArraySizes[entry.Slot] = 1;
		setInfo.SlotTypes[entry.Slot] = type;

		mResourceInfos[typeIndex][sequentialBindingIndex].Set = entry.Set;
		mResourceInfos[typeIndex][sequentialBindingIndex].Slot = entry.Slot;
		mResourceInfos[typeIndex][sequentialBindingIndex].ArraySize = 1;

		mBindingSlotCountPerType[typeIndex]++;
		mResourceCountPerType[typeIndex]++;
	};

	auto fnPopulateSetInfoWithArraySupport = [this](auto& entry, ParamType type)
	{
		const u32 typeIndex = (u32)type;
		const u32 sequentialBindingIndex = mBindingSlotCountPerType[typeIndex];
		const u32 sequentialResourceIndex = mResourceCountPerType[typeIndex];
		const u32 arraySize = Math::Max(1u, entry.ArraySize);

		SetInfo& setInfo = mSetInfos[entry.Set];
		setInfo.SlotToSequentialBindingIndex[entry.Slot] = sequentialBindingIndex;
		setInfo.SlotToSequentialResourceIndex[entry.Slot] = sequentialResourceIndex;
		setInfo.SlotArraySizes[entry.Slot] = arraySize;
		setInfo.SlotTypes[entry.Slot] = type;

		mResourceInfos[typeIndex][sequentialBindingIndex].Set = entry.Set;
		mResourceInfos[typeIndex][sequentialBindingIndex].Slot = entry.Slot;
		mResourceInfos[typeIndex][sequentialBindingIndex].ArraySize = arraySize;

		mBindingSlotCountPerType[typeIndex]++;
		mResourceCountPerType[typeIndex] += arraySize;
	};

	for(u32 gpuProgramParameterSetIndex = 0; gpuProgramParameterSetIndex < gpuProgramParameterSetsCount; gpuProgramParameterSetIndex++)
	{
		const SPtr<GpuParamDesc>& paramDesc = mParamDescs[gpuProgramParameterSetIndex];
		if(paramDesc == nullptr)
			continue;

		for(auto& paramBlock : paramDesc->ParamBlocks)
			fnPopulateSetInfo(paramBlock.second, ParamType::ParamBlock);

		for(auto& texture : paramDesc->Textures)
			fnPopulateSetInfoWithArraySupport(texture.second, ParamType::Texture);

		for(auto& texture : paramDesc->LoadStoreTextures)
			fnPopulateSetInfoWithArraySupport(texture.second, ParamType::LoadStoreTexture);

		for(auto& buffer : paramDesc->Buffers)
			fnPopulateSetInfoWithArraySupport(buffer.second, ParamType::Buffer);

		// Samplers need to be handled specially because certain slots could be texture/buffer + sampler combinations
		{
			const u32 typeIndex = (u32)ParamType::SamplerState;
			for(auto& entry : paramDesc->Samplers)
			{
				const GpuObjectParameterInformation& samplerInformation = entry.second;
				const u32 sequentialBindingIndex = mBindingSlotCountPerType[typeIndex];
				const u32 sequentialResourceIndex = mResourceCountPerType[typeIndex];
				const u32 arraySize = Math::Max(1u, samplerInformation.ArraySize);

				SetInfo& setInfo = mSetInfos[samplerInformation.Set];
				if(setInfo.SlotToSequentialResourceIndex[samplerInformation.Slot] == ~0u) // Slot is sampler only
				{
					setInfo.SlotToSequentialBindingIndex[samplerInformation.Slot] = sequentialBindingIndex;
					setInfo.SlotToSequentialResourceIndex[samplerInformation.Slot] = sequentialResourceIndex;
					setInfo.SlotArraySizes[samplerInformation.Slot] = arraySize;
					setInfo.SlotTypes[samplerInformation.Slot] = ParamType::SamplerState;
				}
				else // Slot is a combination
				{
					setInfo.SlotToSequentialSamplerBindingIndex[samplerInformation.Slot] = sequentialBindingIndex;
					setInfo.SlotToSequentialSamplerResourceIndex[samplerInformation.Slot] = sequentialResourceIndex;
				}

				mResourceInfos[typeIndex][sequentialBindingIndex].Set = samplerInformation.Set;
				mResourceInfos[typeIndex][sequentialBindingIndex].Slot = samplerInformation.Slot;
				mResourceInfos[typeIndex][sequentialBindingIndex].ArraySize = arraySize;

				mBindingSlotCountPerType[typeIndex]++;
				mResourceCountPerType[typeIndex] += arraySize;
			}
		}
	}
}

u32 GpuPipelineParamInfoBase::GetSequentialResourceIndex(ParamType type, u32 set, u32 slot, u32 arrayIndex) const
{
#if B3D_DEBUG
	if(set >= mSetCount)
	{
		B3D_LOG(Error, RenderBackend, "Set index out of range: Valid range: [0, {0}). Requested: {1}.", mSetCount, set);
		return ~0u;
	}

	if(slot >= mSetInfos[set].SlotCount)
	{
		B3D_LOG(Error, RenderBackend, "Slot index out of range: Valid range: [0, {0}). Requested: {1}.", mSetInfos[set].SlotCount, slot);
		return ~0u;
	}

	if(arrayIndex >= mSetInfos[set].SlotArraySizes[slot])
	{
		B3D_LOG(Error, RenderBackend, "Cannot retrieve sequential resource index. Array index out of range: Valid range: [0, {0}). Requested: {1}.", mSetInfos[set].SlotArraySizes[slot], arrayIndex);
		return -1;
	}

	const ParamType slotType = mSetInfos[set].SlotTypes[slot];
	if(slotType != type)
	{
		// Allow sampler states & textures/buffers to share the same slot, as some APIs combine them
		if(type == ParamType::SamplerState)
		{
			if(mSetInfos[set].SlotToSequentialSamplerResourceIndex[slot] != ~0u)
				return mSetInfos[set].SlotToSequentialSamplerResourceIndex[slot] + arrayIndex;

		}

		B3D_LOG(Error, RenderBackend, "Requested parameter is not of the valid type. Requested: {0}. Actual: {1}.", (u32)type, (u32)mSetInfos[set].SlotTypes[slot]);
		return ~0u;
	}

#endif

	return mSetInfos[set].SlotToSequentialResourceIndex[slot] != ~0u ? mSetInfos[set].SlotToSequentialResourceIndex[slot] + arrayIndex : ~0u;
}


u32 GpuPipelineParamInfoBase::GetSequentialBindingIndex(ParamType type, u32 set, u32 slot) const
{
#if B3D_DEBUG
	if(set >= mSetCount)
	{
		B3D_LOG(Error, RenderBackend, "Set index out of range: Valid range: [0, {0}). Requested: {1}.", mSetCount, set);
		return ~0u;
	}

	if(slot >= mSetInfos[set].SlotCount)
	{
		B3D_LOG(Error, RenderBackend, "Slot index out of range: Valid range: [0, {0}). Requested: {1}.", mSetInfos[set].SlotCount, slot);
		return ~0u;
	}

	const ParamType slotType = mSetInfos[set].SlotTypes[slot];
	if(slotType != type)
	{
		// Allow sampler states & textures/buffers to share the same slot, as some APIs combine them
		if(type == ParamType::SamplerState)
		{
			if(mSetInfos[set].SlotToSequentialSamplerBindingIndex[slot] != ~0u)
				return mSetInfos[set].SlotToSequentialSamplerBindingIndex[slot];
		}

		B3D_LOG(Error, RenderBackend, "Requested parameter is not of the valid type. Requested: {0}. Actual: {1}.", (u32)type, (u32)mSetInfos[set].SlotTypes[slot]);
		return ~0u;
	}
#endif

	return mSetInfos[set].SlotToSequentialBindingIndex[slot];
}

void GpuPipelineParamInfoBase::GetBinding(ParamType type, u32 sequentialBindingIndex, u32& set, u32& slot) const
{
#if B3D_DEBUG
	if(sequentialBindingIndex >= mBindingSlotCountPerType[(int)type])
	{
		B3D_LOG(Error, RenderBackend, "Sequential slot index out of range: Valid range: [0, {0}). Requested: {1}.", mBindingSlotCountPerType[(int)type], sequentialBindingIndex);

		set = 0;
		slot = 0;
		return;
	}
#endif

	set = mResourceInfos[(u32)type][sequentialBindingIndex].Set;
	slot = mResourceInfos[(u32)type][sequentialBindingIndex].Slot;
}

void GpuPipelineParamInfoBase::GetBindings(ParamType type, const String& name, GpuParamBinding (&bindings)[GPT_COUNT])
{
	constexpr u32 numParamDescs = sizeof(mParamDescs) / sizeof(mParamDescs[0]);
	static_assert(
		numParamDescs == GPT_COUNT,
		"Number of param descriptor structures must match the number of GPU program stages.");

	for(u32 i = 0; i < numParamDescs; i++)
		GetBinding((GpuProgramType)i, type, name, bindings[i]);
}

void GpuPipelineParamInfoBase::GetBinding(GpuProgramType progType, ParamType type, const String& name, GpuParamBinding& binding)
{
	auto findBinding = [](auto& paramMap, const String& name, GpuParamBinding& binding)
	{
		auto iterFind = paramMap.find(name);
		if(iterFind != paramMap.end())
		{
			binding.Set = iterFind->second.Set;
			binding.Slot = iterFind->second.Slot;
		}
		else
			binding.Set = binding.Slot = (u32)-1;
	};

	const SPtr<GpuParamDesc>& paramDesc = mParamDescs[(u32)progType];
	if(paramDesc == nullptr)
	{
		binding.Set = binding.Slot = (u32)-1;
		return;
	}

	switch(type)
	{
	case ParamType::ParamBlock:
		findBinding(paramDesc->ParamBlocks, name, binding);
		break;
	case ParamType::Texture:
		findBinding(paramDesc->Textures, name, binding);
		break;
	case ParamType::LoadStoreTexture:
		findBinding(paramDesc->LoadStoreTextures, name, binding);
		break;
	case ParamType::Buffer:
		findBinding(paramDesc->Buffers, name, binding);
		break;
	case ParamType::SamplerState:
		findBinding(paramDesc->Samplers, name, binding);
		break;
	default:
		break;
	}
}

u32 GpuPipelineParamInfoBase::GetArraySize(ParamType type, u32 sequentialBindingIndex)
{
#if B3D_DEBUG
	if(sequentialBindingIndex >= mBindingSlotCountPerType[(int)type])
	{
		B3D_LOG(Error, RenderBackend, "Cannot retrieve array size. Sequential binding index out of range: Valid range: [0, {0}). Requested: {1}.", mBindingSlotCountPerType[(int)type], sequentialBindingIndex);

		return 0;
	}
#endif

	return mResourceInfos[(u32)type][sequentialBindingIndex].ArraySize;
}

GpuPipelineParamInfo::GpuPipelineParamInfo(const GPU_PIPELINE_PARAMS_DESC& desc)
	: GpuPipelineParamInfoBase(desc)
{}

SPtr<GpuPipelineParamInfo> GpuPipelineParamInfo::Create(const GPU_PIPELINE_PARAMS_DESC& desc)
{
	SPtr<GpuPipelineParamInfo> paramInfo =
		B3DMakeCoreFromExisting<GpuPipelineParamInfo>(new(B3DAllocate<GpuPipelineParamInfo>()) GpuPipelineParamInfo(desc));
	paramInfo->SetThisPtrInternal(paramInfo);
	paramInfo->Initialize();

	return paramInfo;
}

SPtr<ct::GpuPipelineParamInfo> GpuPipelineParamInfo::GetCore() const
{
	return std::static_pointer_cast<ct::GpuPipelineParamInfo>(mCoreSpecific);
}

SPtr<ct::CoreObject> GpuPipelineParamInfo::CreateCore() const
{
	GPU_PIPELINE_PARAMS_DESC desc;
	desc.FragmentParams = mParamDescs[GPT_FRAGMENT_PROGRAM];
	desc.VertexParams = mParamDescs[GPT_VERTEX_PROGRAM];
	desc.GeometryParams = mParamDescs[GPT_GEOMETRY_PROGRAM];
	desc.HullParams = mParamDescs[GPT_HULL_PROGRAM];
	desc.DomainParams = mParamDescs[GPT_DOMAIN_PROGRAM];
	desc.ComputeParams = mParamDescs[GPT_COMPUTE_PROGRAM];

	return ct::RenderStateManager::Instance().CreatePipelineParamInfoInternal(desc);
}

namespace bs { namespace ct
{
GpuPipelineParamInfo::GpuPipelineParamInfo(const GPU_PIPELINE_PARAMS_DESC& desc, GpuDeviceFlags deviceMask)
	: GpuPipelineParamInfoBase(desc)
{}

SPtr<GpuPipelineParamInfo> GpuPipelineParamInfo::Create(const GPU_PIPELINE_PARAMS_DESC& desc, GpuDeviceFlags deviceMask)
{
	return RenderStateManager::Instance().CreatePipelineParamInfo(desc, deviceMask);
}
}}
