//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"

namespace bs
{
	/** @addtogroup RenderAPI
	 *  @{
	 */

	/** Describes a single GPU program data (for example int, float, Vector2) parameter. */
	struct GpuParamDataDesc
	{
		String Name;
		u32 ElementSize; /**< In multiples of 4 bytes. */
		u32 ArraySize;
		u32 ArrayElementStride; /**< In multiples of 4 bytes. */
		GpuParamDataType Type;

		u32 ParamBlockSlot;
		u32 ParamBlockSet;
		u32 GpuMemOffset; /**< In multiples of 4 bytes, or index for parameters not in a buffer. */
		u32 CpuMemOffset; /**< In multiples of 4 bytes. */
	};

	/**	Describes a single GPU program object (for example texture, sampler state) parameter. */
	struct GpuParamObjectDesc
	{
		String Name;
		GpuParamObjectType Type;

		/** Slot within a set. Uniquely identifies bind location in the GPU pipeline, together with the set. */
		u32 Slot;

		/** Uniquely identifies the bind location in the GPU pipeline, together with the slot. */
		u32 Set;

		/** Underlying type of individual elements in the buffer or texture. */
		GpuBufferFormat ElementType = BF_UNKNOWN;
	};

	/**	Describes a GPU program parameter block (collection of GPU program data parameters). */
	struct GpuParamBlockDesc
	{
		String Name;
		u32 Slot; /** Slot within a set. Uniquely identifies bind location in the GPU pipeline, together with the set. */
		u32 Set; /** Uniquely identifies the bind location in the GPU pipeline, together with the slot. */
		u32 BlockSize; /**< In multiples of 4 bytes. */
		bool IsShareable; /** True for blocks that can be shared between different GPU pipeline stages. */
	};

	/** Contains all parameter information for a GPU program, including data and object parameters, plus parameter blocks. */
	struct BS_CORE_EXPORT GpuParamDesc : IReflectable
	{
		Map<String, GpuParamBlockDesc> ParamBlocks;
		Map<String, GpuParamDataDesc> Params;

		Map<String, GpuParamObjectDesc> Samplers;
		Map<String, GpuParamObjectDesc> Textures;
		Map<String, GpuParamObjectDesc> LoadStoreTextures;
		Map<String, GpuParamObjectDesc> Buffers;

		/************************************************************************/
		/* 								SERIALIZATION                      		*/
		/************************************************************************/
	public:
		friend class GpuParamDescRTTI;
		static RTTITypeBase* GetRttiStatic();
		RTTITypeBase* GetRtti() const ;
	};

	/** @} */
}
