# Source files and their filters
B3DGlobSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} "" "" BS_MONO_SRC)

# Ensure pre-built binary dependencies are up to date
if(B3D_USE_BUNDLED_LIBRARIES)
	if(B3D_USE_DOTNETCORE)
		B3DCheckAndUpdatePrebuiltDependency(DotNETCoreMono)
	else()
		B3DCheckAndUpdatePrebuiltDependency(Mono)
	endif()
endif()

# Find packages
if(B3D_USE_DOTNETCORE)
	find_package(DotNETCoreMono)
else()
	find_package(mono)
endif()

# Target
add_library(bsfMono STATIC ${BS_MONO_SRC})

# Common flags
B3DSetDefaultLinkAndCompileFlags(bsfMono)

# Includes
target_include_directories(bsfMono PUBLIC "./")

# Defines
target_compile_definitions(bsfMono PUBLIC -DBS_MONO_STATIC_LIB)

# Libraries
## External lib: Mono
if(B3D_USE_DOTNETCORE)
	target_link_libraries(bsfMono PRIVATE ${DotNETCoreMono_LIBRARIES})
else()
	target_link_libraries(bsfMono PRIVATE ${mono_LIBRARIES})
endif()

## Local libs
target_link_libraries(bsfMono PUBLIC bsf)
B3DSetUpPostBuildAndInstallSteps(bsfMono)

# Runtime executable
if(B3D_IS_ENGINE)
	# Target for running generic Mono code
	add_executable(MonoExec B3DMonoExec.cpp)
	target_link_libraries(MonoExec PRIVATE bsfMono)

	if(B3D_USE_DOTNETCORE)
		target_link_libraries(MonoExec PRIVATE ${DotNETCoreMono_LIBRARIES})
		target_compile_definitions(MonoExec PRIVATE -DB3D_USE_DOTNETCORE=1)
	else()
		target_link_libraries(MonoExec PRIVATE ${mono_LIBRARIES})
	endif()

	B3DSetUpPostBuildAndInstallSteps(MonoExec)
endif()

# IDE specific
set_property(TARGET bsfMono PROPERTY FOLDER Script)
