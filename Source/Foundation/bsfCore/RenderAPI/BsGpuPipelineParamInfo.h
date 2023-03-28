//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "CoreThread/BsCoreObject.h"
#include "Allocators/BsGroupAlloc.h"

namespace bs
{
	/** @addtogroup RenderAPI-Internal
	 *  @{
	 */

	/** Helper structure used for initializing GpuPipelineParamInfo. */
	struct GPU_PIPELINE_PARAMS_DESC
	{
		SPtr<GpuParameterDescription> FragmentParams;
		SPtr<GpuParameterDescription> VertexParams;
		SPtr<GpuParameterDescription> GeometryParams;
		SPtr<GpuParameterDescription> HullParams;
		SPtr<GpuParameterDescription> DomainParams;
		SPtr<GpuParameterDescription> ComputeParams;
	};

	/** Binding location for a single GPU program parameter. */
	struct GpuParamBinding
	{
		u32 Set = (u32)-1;
		u32 Slot = (u32)-1;
	};

	/** Contains code common to both sim and core thread implementations of GpuPipelineParamInfo. */
	class B3D_CORE_EXPORT GpuPipelineParamInfoBase
	{
	public:
		/** Types of GPU parameters. */
		enum class GpuParameterType
		{
			UniformBuffer,
			SampledTexture,
			StorageTexture,
			StorageBuffer,
			Sampler,
			Count
		};

		/** Constructs the object using the provided GPU parameter descriptors. */
		GpuPipelineParamInfoBase(const GPU_PIPELINE_PARAMS_DESC& desc);
		virtual ~GpuPipelineParamInfoBase() = default;

		/** Gets the total number of sets. */
		u32 GetSetCount() const { return mSetCount; }

		/** Returns the total number of elements across all sets. */
		u32 GetResourceCount() const { return mResourceCount; }

		/** Returns the number of elements in all sets for the specified parameter type. */
		u32 GetResourceCount(GpuParameterType type) const { return mResourceCountPerType[(u32)type]; }

		/** Returns the total number of slots across all sets. */
		u32 GetBindingSlotCount() const { return mBindingSlotCount; }

		/** Returns the number of slots in all sets for the specified parameter type. */
		u32 GetBindingSlotCount(GpuParameterType type) { return mBindingSlotCountPerType[(u32)type]; }

		/**
		 * Converts a set/slot/array index combination into a sequential index that maps to the parameter in that parameter type's array.
		 *
		 * If the set, slot or array index is out of valid range, the method logs an error and returns ~0u. Only performs range checking in debug mode.
		 */
		u32 GetSequentialResourceIndex(GpuParameterType type, u32 set, u32 slot, u32 arrayIndex) const;

		/**
		 * Converts a set and slot combination into a sequential index that maps to the parameter in that parameter type's array. This is similar to
		 * GetSequentialResourceIndex(), but does not account for array indices.
		 *
		 * If the set or slot is out of valid range, the method logs an error and returns ~0u. Only performs range checking in debug mode.
		 */
		u32 GetSequentialBindingIndex(GpuParameterType type, u32 set, u32 slot) const;

		/** Converts a sequential binding index index into a set/slot combination. */
		void GetBinding(GpuParameterType type, u32 sequentialBindingIndex, u32& set, u32& slot) const;

		/**
		 * Finds set/slot indices of a parameter with the specified name for the specified GPU program stage. Set/slot
		 * indices are set to ~0u if a stage doesn't have a block with the specified name.
		 */
		void GetBinding(GpuProgramType progType, GpuParameterType type, const String& name, GpuParamBinding& binding);

		/**
		 * Finds set/slot indices of a parameter with the specified name for every GPU program stage. Set/slot indices are
		 * set to -1 if a stage doesn't have a block with the specified name.
		 */
		void GetBindings(GpuParameterType type, const String& name, GpuParamBinding (&bindings)[GPT_COUNT]);

		/** Returns the number of entries in the array at the specified binding index. */
		u32 GetArraySize(GpuParameterType type, u32 sequentialBindingIndex);

		/** Returns descriptions of individual parameters for the specified GPU program type. */
		const SPtr<GpuParameterDescription>& GetParamDesc(GpuProgramType type) const { return mParamDescs[(int)type]; }

	protected:
		/** Information about a single set in the param info object. */
		struct SetInfo
		{
			u32* SlotToSequentialBindingIndex = nullptr;
			u32* SlotToSequentialResourceIndex = nullptr;
			u32* SlotToSequentialSamplerBindingIndex = nullptr;
			u32* SlotToSequentialSamplerResourceIndex = nullptr;
			u32* SlotArraySizes = nullptr;
			GpuParameterType* SlotTypes = nullptr;
			u32 SlotCount = 0;
		};

		/** Information how a resource maps to a certain set/slot. */
		struct ResourceInfo
		{
			u32 Set = 0;
			u32 Slot = 0;
			u32 ArraySize = 1;
		};

		std::array<SPtr<GpuParameterDescription>, 6> mParamDescs;

		u32 mSetCount = 0;
		SetInfo* mSetInfos = nullptr;

		u32 mBindingSlotCount = 0;
		u32 mBindingSlotCountPerType[(int)GpuParameterType::Count];
		u32 mResourceCount = 0;
		u32 mResourceCountPerType[(int)GpuParameterType::Count];
		ResourceInfo* mResourceInfos[(int)GpuParameterType::Count];

		GroupAlloc mAlloc;
	};

	/** Holds meta-data about a set of GPU parameters used by a single pipeline state. */
	class B3D_CORE_EXPORT GpuPipelineParamInfo : public CoreObject, public GpuPipelineParamInfoBase
	{
	public:
		virtual ~GpuPipelineParamInfo() = default;

		/**
		 * Retrieves a core implementation of this object usable only from the core thread.
		 *
		 * @note	Core thread only.
		 */
		SPtr<ct::GpuPipelineParamInfo> GetCore() const;

		/**
		 * Constructs the object using the provided GPU parameter descriptors.
		 *
		 * @param[in]	desc	Object containing parameter descriptions for individual GPU program stages.
		 */
		static SPtr<GpuPipelineParamInfo> Create(const GPU_PIPELINE_PARAMS_DESC& desc);

	private:
		GpuPipelineParamInfo(const GPU_PIPELINE_PARAMS_DESC& desc);

		SPtr<ct::CoreObject> CreateCore() const override;
	};

	namespace ct
	{
		/** Core thread version of a bs::GpuPipelineParamInfo. */
		class B3D_CORE_EXPORT GpuPipelineParamInfo : public CoreObject, public GpuPipelineParamInfoBase
		{
		public:
			virtual ~GpuPipelineParamInfo() = default;

			/**
			 * @copydoc bs::GpuPipelineParamInfo::Create
			 * @param[in]	deviceMask		Mask that determines on which GPU devices should the buffer be created on.
			 */
			static SPtr<GpuPipelineParamInfo> Create(const GPU_PIPELINE_PARAMS_DESC& desc, GpuDeviceFlags deviceMask = GDF_DEFAULT);

		protected:
			friend class RenderStateManager;

			GpuPipelineParamInfo(const GPU_PIPELINE_PARAMS_DESC& desc, GpuDeviceFlags deviceMask);
		};
	} // namespace ct

	/** @} */
} // namespace bs
