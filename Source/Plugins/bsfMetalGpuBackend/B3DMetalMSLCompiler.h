//************************************* B3D Framework - Copyright 2026 Marko Pintera *************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DMetalPrerequisites.h"
#include "Material/B3DShaderCompiler.h"

namespace b3d
{
	namespace render
	{
		class GLSLToSPIRV;

		/** @addtogroup MetalGpuBackend
		 *  @{
		 */

		/**
		 * Bytecode compiler for the Metal backend's @c mvksl language. Cross-compiles engine VKSL / MVKSL source to
		 * argument-buffer Metal Shading Language in two steps: GLSL -> SPIR-V (via the owned GLSLToSPIRV) followed by
		 * SPIR-V -> MSL (via SPIRV-Cross), packing the result into the WriteMetalBytecode blob layout that
		 * MetalGpuProgram consumes. Device-independent, so it also runs in the headless shader-cook tool.
		 */
		class MetalMSLCompiler final : public IGpuBytecodeCompiler
		{
		public:
			MetalMSLCompiler(const char* compilerId, u32 compilerVersion);
			~MetalMSLCompiler();

			/** Performs the VKSL/MVKSL -> SPIR-V -> argument-buffer MSL conversion and packs the result. */
			TShared<GpuProgramBytecode> CompileBytecode(const GpuProgramCreateInformation& createInformation) override;

		private:
			TUnique<GLSLToSPIRV> mConverter;
		};

		/** @} */
	} // namespace render
} // namespace b3d
