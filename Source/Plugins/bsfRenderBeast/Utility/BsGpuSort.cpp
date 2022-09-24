//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Utility/BsGpuSort.h"
#include "RenderAPI/BsGpuBuffer.h"
#include "Math/BsRandom.h"
#include "Renderer/BsRendererUtility.h"

namespace bs { namespace ct
{
	static constexpr UINT32 BIT_COUNT = 32;
	static constexpr UINT32 RADIX_NUM_BITS = 4;
	static constexpr UINT32 NUM_DIGITS = 1 << RADIX_NUM_BITS;
	static constexpr UINT32 KEY_MASK = (NUM_DIGITS - 1);
	static constexpr UINT32 NUM_PASSES = BIT_COUNT / RADIX_NUM_BITS;

	static constexpr UINT32 NUM_THREADS = 128;
	static constexpr UINT32 KEYS_PER_LOOP = 8;
	static constexpr UINT32 TILE_SIZE = NUM_THREADS * KEYS_PER_LOOP;
	static constexpr UINT32 MAX_NUM_GROUPS = 64;

	RadixSortParamsDef gRadixSortParamsDef;

	/** Contains various constants required during the GpuSort algorithm. */
	struct GpuSortProperties
	{
		GpuSortProperties(UINT32 count)
			: Count(count)
		{ }

		const UINT32 Count;
		const UINT32 NumTiles = Count / TILE_SIZE;
		const UINT32 NumGroups = Math::Clamp(NumTiles, 1U, MAX_NUM_GROUPS);

		const UINT32 TilesPerGroup = NumTiles / NumGroups;
		const UINT32 ExtraTiles = NumTiles % NumGroups;
		const UINT32 ExtraKeys = Count % TILE_SIZE;
	};

	/** Set up common defines required by all radix sort shaders. */
	void initCommonDefines(ShaderDefines& defines)
	{
		defines.Set("RADIX_NUM_BITS", RADIX_NUM_BITS);
		defines.Set("NUM_THREADS", NUM_THREADS);
		defines.Set("KEYS_PER_LOOP", KEYS_PER_LOOP);
		defines.Set("MAX_NUM_GROUPS", MAX_NUM_GROUPS);
	}

	void runSortTest();

	/**
	 * Creates a new GPU parameter block buffer according to gRadixSortParamDef definition and writes GpuSort properties
	 * into the buffer.
	 */
	SPtr<GpuParamBlockBuffer> createGpuSortParams(const GpuSortProperties& props)
	{
		SPtr<GpuParamBlockBuffer> buffer = gRadixSortParamsDef.CreateBuffer();

		gRadixSortParamsDef.gTilesPerGroup.Set(buffer, props.TilesPerGroup);
		gRadixSortParamsDef.gNumGroups.Set(buffer, props.NumGroups);
		gRadixSortParamsDef.gNumExtraTiles.Set(buffer, props.ExtraTiles);
		gRadixSortParamsDef.gNumExtraKeys.Set(buffer, props.ExtraKeys);
		gRadixSortParamsDef.gBitOffset.Set(buffer, 0);

		return buffer;
	}

	/**
	 * Checks can the provided buffer be used for GPU sort operation. Returns a pointer to the error message if check failed
	 * or nullptr if check passed.
	 */
	const char* checkSortBuffer(GpuBuffer& buffer)
	{
		static constexpr const char* INVALID_GPU_WRITE_MSG =
			"All buffers provided to GpuSort must be created with GBU_LOADSTORE flags enabled.";
		static constexpr const char* INVALID_TYPE_MSG =
			"All buffers provided to GpuSort must be of GBT_STANDARD type.";
		static constexpr const char* INVALID_FORMAT_MSG =
			"All buffers provided to GpuSort must use a 32-bit unsigned integer format.";

		const GpuBufferProperties& bufferProps = buffer.GetProperties();
		if ((bufferProps.GetUsage() & GBU_LOADSTORE) != GBU_LOADSTORE)
			return INVALID_GPU_WRITE_MSG;

		if(bufferProps.GetType() != GBT_STANDARD)
			return INVALID_TYPE_MSG;

		if(bufferProps.GetFormat() != BF_32X1U)
			return INVALID_FORMAT_MSG;

		return nullptr;
	}

	/** Creates a helper buffers used for storing intermediate information during GpuSort::sort. */
	SPtr<GpuBuffer> createHelperBuffer()
	{
		GPU_BUFFER_DESC desc;
		desc.ElementCount = MAX_NUM_GROUPS * NUM_DIGITS;
		desc.Format = BF_32X1U;
		desc.Usage = GBU_LOADSTORE;
		desc.Type = GBT_STANDARD;

		return GpuBuffer::Create(desc);
	}

	RadixSortClearMat::RadixSortClearMat()
	{
		mParams->GetBufferParam(GPT_COMPUTE_PROGRAM, "gOutput", MOutputParam);
	}

	void RadixSortClearMat::InitDefinesInternal(ShaderDefines& defines)
	{
		initCommonDefines(defines);
	}

	void RadixSortClearMat::Execute(const SPtr<GpuBuffer>& outputOffsets)
	{
		BS_RENMAT_PROFILE_BLOCK

		MOutputParam.Set(outputOffsets);

		Bind();
		RenderAPI::Instance().DispatchCompute(1);
	}

	RadixSortCountMat::RadixSortCountMat()
	{
		mParams->GetBufferParam(GPT_COMPUTE_PROGRAM, "gInputKeys", MInputKeysParam);
		mParams->GetBufferParam(GPT_COMPUTE_PROGRAM, "gOutputCounts", MOutputCountsParam);
	}

	void RadixSortCountMat::InitDefinesInternal(ShaderDefines& defines)
	{
		initCommonDefines(defines);
	}

	void RadixSortCountMat::Execute(UINT32 numGroups, const SPtr<GpuParamBlockBuffer>& params,
		const SPtr<GpuBuffer>& inputKeys, const SPtr<GpuBuffer>& outputOffsets)
	{
		BS_RENMAT_PROFILE_BLOCK

		MInputKeysParam.Set(inputKeys);
		MOutputCountsParam.Set(outputOffsets);

		mParams->SetParamBlockBuffer("Params", params);

		Bind();
		RenderAPI::Instance().DispatchCompute(numGroups);
	}

	RadixSortPrefixScanMat::RadixSortPrefixScanMat()
	{
		mParams->GetBufferParam(GPT_COMPUTE_PROGRAM, "gInputCounts", MInputCountsParam);
		mParams->GetBufferParam(GPT_COMPUTE_PROGRAM, "gOutputOffsets", MOutputOffsetsParam);
	}

	void RadixSortPrefixScanMat::InitDefinesInternal(ShaderDefines& defines)
	{
		initCommonDefines(defines);
	}

	void RadixSortPrefixScanMat::Execute(const SPtr<GpuParamBlockBuffer>& params, const SPtr<GpuBuffer>& inputCounts,
		const SPtr<GpuBuffer>& outputOffsets)
	{
		BS_RENMAT_PROFILE_BLOCK

		MInputCountsParam.Set(inputCounts);
		MOutputOffsetsParam.Set(outputOffsets);

		mParams->SetParamBlockBuffer("Params", params);

		Bind();
		RenderAPI::Instance().DispatchCompute(1);
	}

	RadixSortReorderMat::RadixSortReorderMat()
	{
		mParams->GetBufferParam(GPT_COMPUTE_PROGRAM, "gInputOffsets", MInputOffsetsBufferParam);
		mParams->GetBufferParam(GPT_COMPUTE_PROGRAM, "gInputKeys", MInputKeysBufferParam);
		mParams->GetBufferParam(GPT_COMPUTE_PROGRAM, "gInputValues", MInputValuesBufferParam);
		mParams->GetBufferParam(GPT_COMPUTE_PROGRAM, "gOutputKeys", MOutputKeysBufferParam);
		mParams->GetBufferParam(GPT_COMPUTE_PROGRAM, "gOutputValues", MOutputValuesBufferParam);
	}

	void RadixSortReorderMat::InitDefinesInternal(ShaderDefines& defines)
	{
		initCommonDefines(defines);
	}

	void RadixSortReorderMat::Execute(UINT32 numGroups, const SPtr<GpuParamBlockBuffer>& params,
		const SPtr<GpuBuffer>& inputPrefix, const GpuSortBuffers& buffers, UINT32 inputBufferIdx)
	{
		BS_RENMAT_PROFILE_BLOCK

		const UINT32 outputBufferIdx = (inputBufferIdx + 1) % 2;

		MInputOffsetsBufferParam.Set(inputPrefix);
		MInputKeysBufferParam.Set(buffers.Keys[inputBufferIdx]);
		MInputValuesBufferParam.Set(buffers.Values[inputBufferIdx]);
		MOutputKeysBufferParam.Set(buffers.Keys[outputBufferIdx]);
		MOutputValuesBufferParam.Set(buffers.Values[outputBufferIdx]);
		
		mParams->SetParamBlockBuffer("Params", params);

		Bind();
		RenderAPI::Instance().DispatchCompute(numGroups);
	}

	GpuSort::GpuSort()
	{
		mHelperBuffers[0] = createHelperBuffer();
		mHelperBuffers[1] = createHelperBuffer();
	}

	UINT32 GpuSort::Sort(const GpuSortBuffers& buffers, UINT32 numKeys, UINT32 keyMask)
	{
		// Nothing to do if no input or output key buffers
		if(buffers.Keys[0] == nullptr || buffers.Keys[1] == nullptr)
			return 0;

		// Check if all buffers have been created with required options
		const char* errorMsg = nullptr;
		for(UINT32 i = 0; i < 2; i++)
		{
			errorMsg = checkSortBuffer(*buffers.Keys[i]);
			if(errorMsg) break;

			if(buffers.Values[i])
			{
				errorMsg = checkSortBuffer(*buffers.Values[i]);
				if(errorMsg) break;
			}
		}

		if(errorMsg)
		{
			BS_LOG(Error, Renderer, "GpuSort failed: {0}", errorMsg);
			return 0;
		}

		// Check if all buffers have the same size
		bool validSize = buffers.Keys[0]->GetSize() == buffers.Keys[1]->GetSize();
		if(buffers.Values[0] && buffers.Values[1])
		{
			validSize = buffers.Keys[0]->GetSize() == buffers.Values[0]->GetSize() &&
				buffers.Keys[0]->GetSize() == buffers.Values[1]->GetSize();

		}

		if (!validSize)
		{
			BS_LOG(Error, Renderer, "GpuSort failed: All sort buffers must have the same size.");
			return 0;
		}

		const GpuSortProperties gpuSortProps(numKeys);
		SPtr<GpuParamBlockBuffer> params = createGpuSortParams(gpuSortProps);

		UINT32 bitOffset = 0;
		UINT32 inputBufferIdx = 0;
		for(UINT32 i = 0; i < NUM_PASSES; i++)
		{
			if(((KEY_MASK << bitOffset) & keyMask) != 0)
			{
				gRadixSortParamsDef.gBitOffset.Set(params, bitOffset);

				RadixSortClearMat::Get()->Execute(mHelperBuffers[0]);
				RadixSortCountMat::Get()->Execute(gpuSortProps.NumGroups, params, buffers.Keys[inputBufferIdx], mHelperBuffers[0]);
				RadixSortPrefixScanMat::Get()->Execute(params, mHelperBuffers[0], mHelperBuffers[1]);
				RadixSortReorderMat::Get()->Execute(gpuSortProps.NumGroups, params, mHelperBuffers[1], buffers, inputBufferIdx);

				inputBufferIdx = (inputBufferIdx + 1) % 2;
			}

			bitOffset += RADIX_NUM_BITS;
		}

		return inputBufferIdx;
	}

	GpuSortBuffers GpuSort::CreateSortBuffers(UINT32 numElements, bool values)
	{
		GpuSortBuffers output;

		GPU_BUFFER_DESC bufferDesc;
		bufferDesc.ElementCount = numElements;
		bufferDesc.Format = BF_32X1U;
		bufferDesc.Type = GBT_STANDARD;
		bufferDesc.Usage = GBU_LOADSTORE;

		output.Keys[0] = GpuBuffer::Create(bufferDesc);
		output.Keys[1] = GpuBuffer::Create(bufferDesc);

		if(values)
		{
			output.Values[0] = GpuBuffer::Create(bufferDesc);
			output.Values[1] = GpuBuffer::Create(bufferDesc);
		}

		return output;
	}

	// Note: This test isn't currently hooked up anywhere. It might be a good idea to set it up as a unit test, but it would
	// require exposing parts of GpuSort to the public, which I don't feel like it's worth doing just for a test. So instead
	// just make sure to run the test below if you modify any of the GpuSort code.
	void runSortTest()
	{
		// Generate test keys
		static constexpr UINT32 NUM_INPUT_KEYS = 10000;
		Vector<UINT32> inputKeys;
		inputKeys.reserve(NUM_INPUT_KEYS);

		Random random;
		for(UINT32 i = 0; i < NUM_INPUT_KEYS; i++)
			inputKeys.push_back(random.GetRange(0, 15) << 4 | std::min(NUM_DIGITS - 1, (i / (NUM_INPUT_KEYS / 16))));

		const auto count = (UINT32)inputKeys.size();
		UINT32 bitOffset = 4;
		UINT32 bitMask = (1 << RADIX_NUM_BITS) - 1;

		// Prepare buffers
		const GpuSortProperties gpuSortProps(count);
		SPtr<GpuParamBlockBuffer> params = createGpuSortParams(gpuSortProps);

		gRadixSortParamsDef.gBitOffset.Set(params, bitOffset);

		GpuSortBuffers sortBuffers = GpuSort::CreateSortBuffers(count);
		sortBuffers.Keys[0]->WriteData(0, sortBuffers.Keys[0]->GetSize(), inputKeys.data(), BWT_DISCARD);

		SPtr<GpuBuffer> helperBuffers[2];
		helperBuffers[0] = createHelperBuffer();
		helperBuffers[1] = createHelperBuffer();

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////// Count keys per group //////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// SERIAL:
		Vector<UINT32> counts(gpuSortProps.NumGroups * NUM_DIGITS);
		for(UINT32 groupIdx = 0; groupIdx < gpuSortProps.NumGroups; groupIdx++)
		{
			// Count keys per thread
			UINT32 localCounts[NUM_THREADS * NUM_DIGITS] = { 0 };

			UINT32 tileIdx;
			UINT32 numTiles;
			if(groupIdx < gpuSortProps.ExtraTiles)
			{
				numTiles = gpuSortProps.TilesPerGroup + 1;
				tileIdx = groupIdx * numTiles;
			}
			else
			{
				numTiles = gpuSortProps.TilesPerGroup;
				tileIdx = groupIdx * numTiles + gpuSortProps.ExtraTiles;
			}

			UINT32 keyBegin = tileIdx * TILE_SIZE;
			UINT32 keyEnd = keyBegin + numTiles * TILE_SIZE;	

			while(keyBegin < keyEnd)
			{
				for(UINT32 threadIdx = 0; threadIdx < NUM_THREADS; threadIdx++)
				{
					UINT32 key = inputKeys[keyBegin + threadIdx];
					UINT32 digit = (key >> bitOffset) & bitMask;

					localCounts[threadIdx * NUM_DIGITS + digit] += 1;
				}

				keyBegin += NUM_THREADS;
			}

			if(groupIdx == (gpuSortProps.NumGroups - 1))
			{
				keyBegin = keyEnd;
				keyEnd = keyBegin + gpuSortProps.ExtraKeys;

				while(keyBegin < keyEnd)
				{
					for (UINT32 threadIdx = 0; threadIdx < NUM_THREADS; threadIdx++)
					{
						if((keyBegin + threadIdx) < keyEnd)
						{
							UINT32 key = inputKeys[keyBegin + threadIdx];
							UINT32 digit = (key >> bitOffset) & bitMask;

							localCounts[threadIdx * NUM_DIGITS + digit] += 1;
						}
					}

					keyBegin += NUM_THREADS;
				}
			}

			// Sum up all key counts in a group
			static constexpr UINT32 NUM_REDUCE_THREADS = 64;
			static constexpr UINT32 NUM_REDUCE_THREADS_PER_DIGIT = NUM_REDUCE_THREADS / NUM_DIGITS;
			static constexpr UINT32 NUM_REDUCE_ELEMS_PER_THREAD_PER_DIGIT = NUM_THREADS / NUM_REDUCE_THREADS_PER_DIGIT;

			UINT32 reduceCounters[NUM_REDUCE_THREADS] = { 0 };
			UINT32 reduceTotals[NUM_REDUCE_THREADS] = { 0 };
			for (UINT32 threadId = 0; threadId < NUM_THREADS; threadId++)
			{
				if(threadId < NUM_REDUCE_THREADS)
				{
					UINT32 digitIdx = threadId / NUM_REDUCE_THREADS_PER_DIGIT;
					UINT32 setIdx = threadId & (NUM_REDUCE_THREADS_PER_DIGIT - 1);

					UINT32 total = 0;
					for(UINT32 i = 0; i < NUM_REDUCE_ELEMS_PER_THREAD_PER_DIGIT; i++)
					{
						UINT32 threadIdx = (setIdx * NUM_REDUCE_ELEMS_PER_THREAD_PER_DIGIT + i) * NUM_DIGITS;
						total += localCounts[threadIdx + digitIdx];
					}

					reduceCounters[digitIdx * NUM_REDUCE_THREADS_PER_DIGIT + setIdx] = total;
					reduceTotals[threadId] = total;

				}
			}

			// And do parallel reduction on the result of serial additions
			for (UINT32 i = 1; i < NUM_REDUCE_THREADS_PER_DIGIT; i <<= 1)
			{
				for (UINT32 threadId = 0; threadId < NUM_THREADS; threadId++)
				{
					if (threadId < NUM_REDUCE_THREADS)
					{
						UINT32 digitIdx = threadId / NUM_REDUCE_THREADS_PER_DIGIT;
						UINT32 setIdx = threadId & (NUM_REDUCE_THREADS_PER_DIGIT - 1);

						reduceTotals[threadId] += reduceCounters[digitIdx * NUM_REDUCE_THREADS_PER_DIGIT + setIdx + i];
					}
				}

				for (UINT32 threadId = 0; threadId < NUM_THREADS; threadId++)
				{
					if (threadId < NUM_REDUCE_THREADS)
					{
						UINT32 digitIdx = threadId / NUM_REDUCE_THREADS_PER_DIGIT;
						UINT32 setIdx = threadId & (NUM_REDUCE_THREADS_PER_DIGIT - 1);

						reduceCounters[digitIdx * NUM_REDUCE_THREADS_PER_DIGIT + setIdx] = reduceTotals[threadId];
					}
				}
			}

			for (UINT32 threadId = 0; threadId < NUM_THREADS; threadId++)
			{
				if(threadId < NUM_DIGITS)
					counts[groupIdx * NUM_DIGITS + threadId] = reduceCounters[threadId * NUM_REDUCE_THREADS_PER_DIGIT];
			}
		}

		// PARALLEL:
		RadixSortClearMat::Get()->Execute(helperBuffers[0]);
		RadixSortCountMat::Get()->Execute(gpuSortProps.NumGroups, params, sortBuffers.Keys[0], helperBuffers[0]);
		RenderAPI::Instance().SubmitCommandBuffer(nullptr);

		// Compare with GPU count
		const UINT32 helperBufferLength = helperBuffers[0]->GetProperties().GetElementCount();
		Vector<UINT32> bufferCounts(helperBufferLength);
		helperBuffers[0]->ReadData(0, helperBufferLength * sizeof(UINT32), bufferCounts.data());

		for(UINT32 i = 0; i < (UINT32)counts.size(); i++)
			assert(bufferCounts[i] == counts[i]);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////// Calculate offsets //////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// SERIAL:
		// Prefix sum over per-digit counts over all groups
		Vector<UINT32> perDigitPrefixSum(NUM_DIGITS * MAX_NUM_GROUPS);
		for(UINT32 groupIdx = 0; groupIdx < gpuSortProps.NumGroups; groupIdx++)
		{
			for (UINT32 j = 0; j < NUM_DIGITS; j++)
				perDigitPrefixSum[groupIdx * NUM_DIGITS + j] = counts[groupIdx * NUM_DIGITS + j];
		}

		// Prefix sum over per-digit counts over all groups
		//// Upsweep
		UINT32 offset = 1;
		for (UINT32 i = MAX_NUM_GROUPS >> 1; i > 0; i >>= 1)
		{
			for (UINT32 groupIdx = 0; groupIdx < MAX_NUM_GROUPS; groupIdx++)
			{
				if (groupIdx < i)
				{
					for (UINT32 j = 0; j < NUM_DIGITS; j++)
					{
						UINT32 idx0 = (offset * (2 * groupIdx + 1) - 1) * NUM_DIGITS + j;
						UINT32 idx1 = (offset * (2 * groupIdx + 2) - 1) * NUM_DIGITS + j;

						perDigitPrefixSum[idx1] += perDigitPrefixSum[idx0];
					}
				}
			}

			offset <<= 1;
		}

		//// Downsweep
		UINT32 totalsPrefixSum[NUM_DIGITS] = { 0 };
		for(UINT32 groupIdx = 0; groupIdx < NUM_DIGITS; groupIdx++)
		{
			if (groupIdx < NUM_DIGITS)
			{
				UINT32 idx = (MAX_NUM_GROUPS - 1) * NUM_DIGITS + groupIdx;
				totalsPrefixSum[groupIdx] = perDigitPrefixSum[idx];
				perDigitPrefixSum[idx] = 0;
			}
		}

		for (UINT32 i = 1; i < MAX_NUM_GROUPS; i <<= 1)
		{
			offset >>= 1;

			for (UINT32 groupIdx = 0; groupIdx < MAX_NUM_GROUPS; groupIdx++)
			{
				if (groupIdx < i)
				{
					for (UINT32 j = 0; j < NUM_DIGITS; j++)
					{
						UINT32 idx0 = (offset * (2 * groupIdx + 1) - 1) * NUM_DIGITS + j;
						UINT32 idx1 = (offset * (2 * groupIdx + 2) - 1) * NUM_DIGITS + j;

						UINT32 temp = perDigitPrefixSum[idx0];
						perDigitPrefixSum[idx0] = perDigitPrefixSum[idx1];
						perDigitPrefixSum[idx1] += temp;
					}
				}
			}
		}

		// Prefix sum over the total count
		for(UINT32 i = 1; i < NUM_DIGITS; i++)
			totalsPrefixSum[i] += totalsPrefixSum[i - 1];

		// Make it exclusive by shifting
		for(UINT32 i = NUM_DIGITS - 1; i > 0; i--)
			totalsPrefixSum[i] = totalsPrefixSum[i - 1];

		totalsPrefixSum[0] = 0;

		Vector<UINT32> offsets(gpuSortProps.NumGroups * NUM_DIGITS);
		for (UINT32 groupIdx = 0; groupIdx < gpuSortProps.NumGroups; groupIdx++)
		{
			for (UINT32 i = 0; i < NUM_DIGITS; i++)
				offsets[groupIdx * NUM_DIGITS + i] = totalsPrefixSum[i] + perDigitPrefixSum[groupIdx * NUM_DIGITS + i];
		}

		// PARALLEL:
		RadixSortPrefixScanMat::Get()->Execute(params, helperBuffers[0], helperBuffers[1]);
		RenderAPI::Instance().SubmitCommandBuffer(nullptr);

		// Compare with GPU offsets
		Vector<UINT32> bufferOffsets(helperBufferLength);
		helperBuffers[1]->ReadData(0, helperBufferLength * sizeof(UINT32), bufferOffsets.data());

		for(UINT32 i = 0; i < (UINT32)offsets.size(); i++)
			assert(bufferOffsets[i] == offsets[i]);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////// Reorder ////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// SERIAL:
		// Reorder within each tile
		Vector<UINT32> sortedKeys(inputKeys.size());
		UINT32 sGroupOffsets[NUM_DIGITS];
		UINT32 sLocalScratch[NUM_DIGITS * NUM_THREADS];
		UINT32 sTileTotals[NUM_DIGITS];
		UINT32 sCurrentTileTotal[NUM_DIGITS];

		for(UINT32 groupIdx = 0; groupIdx < gpuSortProps.NumGroups; groupIdx++)
		{
			for(UINT32 i = 0; i < NUM_DIGITS; i++)
			{
				// Load offsets for this group to local memory
				sGroupOffsets[i] = offsets[groupIdx * NUM_DIGITS + i];

				// Clear tile totals
				sTileTotals[i] = 0;
			}

			// Handle case when number of tiles isn't exactly divisible by number of groups, in
			// which case first N groups handle those extra tiles
			UINT32 tileIdx;
			UINT32 numTiles;
			if(groupIdx < gpuSortProps.ExtraTiles)
			{
				numTiles = gpuSortProps.TilesPerGroup + 1;
				tileIdx = groupIdx * numTiles;
			}
			else
			{
				numTiles = gpuSortProps.TilesPerGroup;
				tileIdx = groupIdx * numTiles + gpuSortProps.ExtraTiles;
			}

			// We need to generate per-thread offsets (prefix sum) of where to store the keys at
			// (This is equivalent to what was done in count & prefix sum shaders, except that was done per-group)

			//// First, count all digits
			UINT32 keyBegin[NUM_THREADS];
			for(UINT32 threadId = 0; threadId < NUM_THREADS; threadId++)
				keyBegin[threadId] = tileIdx * TILE_SIZE;

			auto prefixSum = [&sLocalScratch, &sCurrentTileTotal]()
			{
				// Upsweep to generate partial sums
				UINT32 offsets[NUM_THREADS];
				for (UINT32 threadId = 0; threadId < NUM_THREADS; threadId++)
					offsets[threadId] = 1;

				for (UINT32 i = NUM_THREADS >> 1; i > 0; i >>= 1)
				{
					for (UINT32 threadId = 0; threadId < NUM_THREADS; threadId++)
					{
						if (threadId < i)
						{
							// Note: If I run more than NUM_THREADS threads I wouldn't have to
							// iterate over all digits in a single thread
							// Note: Perhaps run part of this step serially for better performance
							for (UINT32 j = 0; j < NUM_DIGITS; j++)
							{
								UINT32 idx0 = (offsets[threadId] * (2 * threadId + 1) - 1) * NUM_DIGITS + j;
								UINT32 idx1 = (offsets[threadId] * (2 * threadId + 2) - 1) * NUM_DIGITS + j;

								// Note: Check and remove bank conflicts
								sLocalScratch[idx1] += sLocalScratch[idx0];
							}
						}

						offsets[threadId] <<= 1;
					}
				}

				// Set tree roots to zero (prepare for downsweep)
				for (UINT32 threadId = 0; threadId < NUM_THREADS; threadId++)
				{
					if (threadId < NUM_DIGITS)
					{
						UINT32 idx = (NUM_THREADS - 1) * NUM_DIGITS + threadId;
						sCurrentTileTotal[threadId] = sLocalScratch[idx];

						sLocalScratch[idx] = 0;
					}
				}

				// Downsweep to calculate the prefix sum from partial sums that were generated
				// during upsweep		
				for (UINT32 i = 1; i < NUM_THREADS; i <<= 1)
				{
					for (UINT32 threadId = 0; threadId < NUM_THREADS; threadId++)
					{
						offsets[threadId] >>= 1;

						if (threadId < i)
						{
							for (UINT32 j = 0; j < NUM_DIGITS; j++)
							{
								UINT32 idx0 = (offsets[threadId] * (2 * threadId + 1) - 1) * NUM_DIGITS + j;
								UINT32 idx1 = (offsets[threadId] * (2 * threadId + 2) - 1) * NUM_DIGITS + j;

								// Note: Check and resolve bank conflicts
								UINT32 temp = sLocalScratch[idx0];
								sLocalScratch[idx0] = sLocalScratch[idx1];
								sLocalScratch[idx1] += temp;
							}
						}
					}
				}
			};

			for(UINT32 tileIdx = 0; tileIdx < numTiles; tileIdx++)
			{
				// Zero out local counter
				for (UINT32 threadId = 0; threadId < NUM_THREADS; threadId++)
					for (UINT32 i = 0; i < NUM_DIGITS; i++)
						sLocalScratch[i * NUM_THREADS + threadId] = 0;

				for (UINT32 threadId = 0; threadId < NUM_THREADS; threadId++)
				{
					for (UINT32 i = 0; i < KEYS_PER_LOOP; i++)
					{
						UINT32 idx = keyBegin[threadId] + threadId * KEYS_PER_LOOP + i;
						UINT32 key = inputKeys[idx];
						UINT32 digit = (key >> bitOffset) & KEY_MASK;

						sLocalScratch[threadId * NUM_DIGITS + digit] += 1;
					}
				}

				prefixSum();

				// Actually re-order the keys
				for (UINT32 threadId = 0; threadId < NUM_THREADS; threadId++)
				{
					UINT32 localOffsets[NUM_DIGITS];
					for (UINT32 i = 0; i < NUM_DIGITS; i++)
						localOffsets[i] = 0;

					for (UINT32 i = 0; i < KEYS_PER_LOOP; i++)
					{
						UINT32 idx = keyBegin[threadId] + threadId * KEYS_PER_LOOP + i;
						UINT32 key = inputKeys[idx];
						UINT32 digit = (key >> bitOffset) & KEY_MASK;

						UINT32 offset = sGroupOffsets[digit] + sTileTotals[digit] + sLocalScratch[threadId * NUM_DIGITS + digit] + localOffsets[digit];
						localOffsets[digit]++;

						// Note: First write to local memory then attempt to coalesce when writing to global?
						sortedKeys[offset] = key;
					}
				}

				// Update tile totals
				for (UINT32 threadId = 0; threadId < NUM_THREADS; threadId++)
				{
					if (threadId < NUM_DIGITS)
						sTileTotals[threadId] += sCurrentTileTotal[threadId];
				}

				for (UINT32 threadId = 0; threadId < NUM_THREADS; threadId++)
					keyBegin[threadId] += TILE_SIZE;
			}

			if (groupIdx == (gpuSortProps.NumGroups - 1) && gpuSortProps.ExtraKeys > 0)
			{
				// Zero out local counter
				for (UINT32 threadId = 0; threadId < NUM_THREADS; threadId++)
					for (UINT32 i = 0; i < NUM_DIGITS; i++)
						sLocalScratch[i * NUM_THREADS + threadId] = 0;

				for (UINT32 threadId = 0; threadId < NUM_THREADS; threadId++)
				{
					for (UINT32 i = 0; i < KEYS_PER_LOOP; i++)
					{
						UINT32 localIdx = threadId * KEYS_PER_LOOP + i;

						if (localIdx >= gpuSortProps.ExtraKeys)
							continue;

						UINT32 idx = keyBegin[threadId] + localIdx;
						UINT32 key = inputKeys[idx];
						UINT32 digit = (key >> bitOffset) & KEY_MASK;

						sLocalScratch[threadId * NUM_DIGITS + digit] += 1;
					}
				}

				prefixSum();

				// Actually re-order the keys
				for (UINT32 threadId = 0; threadId < NUM_THREADS; threadId++)
				{
					UINT32 localOffsets[NUM_DIGITS];
					for (UINT32 i = 0; i < NUM_DIGITS; i++)
						localOffsets[i] = 0;

					for (UINT32 i = 0; i < KEYS_PER_LOOP; i++)
					{
						UINT32 localIdx = threadId * KEYS_PER_LOOP + i;

						if (localIdx >= gpuSortProps.ExtraKeys)
							continue;

						UINT32 idx = keyBegin[threadId] + localIdx;
						UINT32 key = inputKeys[idx];
						UINT32 digit = (key >> bitOffset) & KEY_MASK;

						UINT32 offset = sGroupOffsets[digit] + sTileTotals[digit] + sLocalScratch[threadId * NUM_DIGITS + digit] + localOffsets[digit];
						localOffsets[digit]++;

						// Note: First write to local memory then attempt to coalesce when writing to global?
						sortedKeys[offset] = key;
					}
				}
			}
		}

		// PARALLEL:
		RadixSortReorderMat::Get()->Execute(gpuSortProps.NumGroups, params, helperBuffers[1], sortBuffers, 0);
		RenderAPI::Instance().SubmitCommandBuffer(nullptr);

		// Compare with GPU keys
		Vector<UINT32> bufferSortedKeys(count);
		sortBuffers.Keys[1]->ReadData(0, count * sizeof(UINT32), bufferSortedKeys.data());

		for(UINT32 i = 0; i < count; i++)
			assert(bufferSortedKeys[i] == sortedKeys[i]);

		// Ensure everything is actually sorted
		assert(std::is_sorted(sortedKeys.begin(), sortedKeys.end()));
	}
}}
