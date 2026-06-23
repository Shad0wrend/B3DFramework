//************************************* B3D Framework - Copyright 2026 Marko Pintera *************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DVulkanPrerequisites.h"

#if B3D_PLATFORM_MACOS

#include "Material/B3DShaderCompiler.h"

namespace b3d
{
	namespace render
	{
		/** @addtogroup Vulkan
		 *  @{
		 */

		class GLSLToSPIRV;

		/**
		 * Compiles engine MVKSL source into MoltenVK MSL bytecode: GLSL / VKSL -> SPIR-V via GLSLToSPIRV, then
		 * SPIR-V -> MSL via SPIRV-Cross.
		 */
		class VulkanMSLCompiler final : public IGpuBytecodeCompiler
		{
		public:
			VulkanMSLCompiler(const char* compilerId, u32 compilerVersion);
			~VulkanMSLCompiler();

			TShared<GpuProgramBytecode> CompileBytecode(const GpuProgramCreateInformation& createInformation) override;

		private:
			TUnique<GLSLToSPIRV> mConverter;
		};

		/** @} */
	} // namespace render
} // namespace b3d

#endif // B3D_PLATFORM_MACOS
