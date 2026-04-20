//************************************* B3D Framework - Copyright 2026 Marko Pintera *************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DPrerequisites.h"

/** @addtogroup Plugins
 *  @{
 */

/** @defgroup MetalGpuBackend MetalGpuBackend
 *	Metal render API implementation (macOS / iOS).
 */

/** @} */

// Metal framework headers are only available to Objective-C++ translation units. Include them here
// guarded so that plain C++ .cpp consumers of the Metal backend headers still compile while .mm
// sources get full access to the Metal types through our backend headers.
#ifdef __OBJC__
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>
#endif

namespace b3d
{
	namespace render
	{
		/**
		 * Canonical per-type ordering used when assigning argument-buffer indices inside a Metal
		 * argument buffer. Two sites must agree on this order byte-for-byte:
		 *
		 *   1. @c MetalGpuPipelineParameterSetLayout's ctor, which walks the base-class per-type
		 *      uniform lists and appends bindings in this exact sequence before assigning
		 *      @c ArgIndex values.
		 *   2. @c MetalGpuDevice's SPIRV-Cross hook, which folds the same five lists into a single
		 *      packed u64 sort key (Set:24 | TypeOrder:8 | Slot:32) so the emitted MSL uses
		 *      identical @c msl_buffer / @c msl_texture / @c msl_sampler values as the layout's
		 *      MTLArgumentEncoder.
		 *
		 * Keeping both sites referring to these constants prevents drift — an earlier bug (phase-2
		 * review S2) hard-coded the values independently in each site and any edit to one would
		 * silently corrupt shader bindings until the other was updated in lockstep.
		 */
		static constexpr u64 kTypeOrderUniformBuffer  = 0;
		static constexpr u64 kTypeOrderSampledTexture = 1;
		static constexpr u64 kTypeOrderStorageTexture = 2;
		static constexpr u64 kTypeOrderStorageBuffer  = 3;
		static constexpr u64 kTypeOrderSampler        = 4;
	} // namespace render
} // namespace b3d
