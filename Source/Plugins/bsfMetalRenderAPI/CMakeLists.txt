# Metal backend is only available on Apple platforms.
if(NOT APPLE)
	return()
endif()

# Source files and their filters
B3DGlobSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} "" "" BS_METALRENDERAPI_SRC)
B3DGlobSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} "../Shared/" "" BS_METALRENDERAPI_SRC)

# Packages: shader cross-compilation depends on glslang + SPIRV-Cross (same bundled versions used by
# the Vulkan backend). The Metal plugin accepts engine shaders in VKSL form, converts to SPIR-V via
# glslang, then cross-compiles to MSL via SPIRV-Cross with argument-buffer emission enabled.
if(B3D_RENDER_BACKEND MATCHES "Metal" OR B3D_BUILD_ALL_PLUGINS)
	if(B3D_USE_BUNDLED_LIBRARIES)
		B3DCheckAndUpdatePrebuiltDependency(glslang)
		B3DCheckAndUpdatePrebuiltDependency(SPIRVCross)
	endif()

	find_package(glslang)
	find_package(SPIRVCross)
endif()

# Target
B3DAddPlugin(bsfMetalRenderAPI ENGINE ${BS_METALRENDERAPI_SRC})

# Common flags
B3DSetDefaultLinkAndCompileFlags(bsfMetalRenderAPI)

# Includes
target_include_directories(bsfMetalRenderAPI PRIVATE
	"./"
	"../Shared/"
)

# Libraries
# Note: glslang must be linked before any system Vulkan to ensure our bundled version wins.
target_link_libraries(bsfMetalRenderAPI PRIVATE ${glslang_LIBRARIES})
target_link_libraries(bsfMetalRenderAPI PRIVATE ${SPIRVCross_LIBRARIES})

# Apple frameworks required by the Metal backend
B3DTargetLinkFramework(bsfMetalRenderAPI Foundation)
B3DTargetLinkFramework(bsfMetalRenderAPI Metal)
B3DTargetLinkFramework(bsfMetalRenderAPI QuartzCore)
B3DTargetLinkFramework(bsfMetalRenderAPI CoreGraphics)
B3DTargetLinkFramework(bsfMetalRenderAPI CoreVideo)
B3DTargetLinkFramework(bsfMetalRenderAPI IOKit)
B3DTargetLinkFramework(bsfMetalRenderAPI AppKit)
B3DTargetLinkFramework(bsfMetalRenderAPI IOSurface)

# IDE specific
set_property(TARGET bsfMetalRenderAPI PROPERTY FOLDER Plugins)

# Install
if(B3D_RENDER_BACKEND MATCHES "Metal" OR B3D_BUILD_ALL_PLUGINS)
	B3DSetUpPostBuildAndInstallSteps(bsfMetalRenderAPI)
endif()
