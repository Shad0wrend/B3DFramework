# Utility Test (kept as explicit file list per user request)
add_executable(UtilityTest
	B3DUtilityTest.cpp
	B3DUtilityTestSuite.cpp
	B3DFileSystemTestSuite.cpp
	B3DECSTestSuite.cpp)

# Common flags
B3DSetDefaultLinkAndCompileFlags(UtilityTest)

# Working directory
set_target_properties(UtilityTest PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)")

# Includes
target_include_directories(UtilityTest PRIVATE
	"${B3D_FRAMEWORK_SOURCE_FOLDER}/Engine/Utility"
	"${B3D_FRAMEWORK_SOURCE_FOLDER}/Engine/Utility/ThirdParty"
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/RTTI")

# Libraries
target_link_libraries(UtilityTest bsf)

# IDE specific
set_property(TARGET UtilityTest PROPERTY FOLDER Tests)

# Register test
add_test(NAME UtilityTests COMMAND $<TARGET_FILE:UtilityTest>)

# Core Test - using glob pattern for source files
set(CORE_TEST_FOLDERS_TO_IGNORE "")

# Files that belong to other test executables
set(CORE_TEST_FILES_TO_EXCLUDE
	"B3DUtilityTest.cpp"
	"B3DUtilityTestSuite.cpp"
	"B3DUtilityTestSuite.h"
	"B3DFileSystemTestSuite.cpp"
	"B3DFileSystemTestSuite.h"
	"B3DECSTestSuite.cpp"
	"B3DECSTestSuite.h"
	"B3DNetworkTest.cpp"
)

B3DGlobSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} "" "${CORE_TEST_FOLDERS_TO_IGNORE}" CORE_TEST_SRC)

# Filter out files that belong to other test targets
foreach(excludeFile ${CORE_TEST_FILES_TO_EXCLUDE})
	list(FILTER CORE_TEST_SRC EXCLUDE REGEX "${excludeFile}$")
endforeach()

# Target
add_executable(CoreTest ${CORE_TEST_SRC})

# Common flags
B3DSetDefaultLinkAndCompileFlags(CoreTest)

# Working directory
set_target_properties(CoreTest PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)")

# Includes
target_include_directories(CoreTest PRIVATE
	"${B3D_FRAMEWORK_SOURCE_FOLDER}/Engine/Utility"
	"${B3D_FRAMEWORK_SOURCE_FOLDER}/Engine/Utility/ThirdParty"
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/RTTI")

# Libraries
target_link_libraries(CoreTest bsf)

# Plugin dependencies
B3DAddRuntimeDependencies(CoreTest)
add_dependencies(CoreTest bsfFBXImporter bsfFontImporter bsfFreeImgImporter bsfSL)

# IDE specific
set_property(TARGET CoreTest PROPERTY FOLDER Tests)

# Register test
add_test(NAME CoreTests COMMAND $<TARGET_FILE:CoreTest>)
