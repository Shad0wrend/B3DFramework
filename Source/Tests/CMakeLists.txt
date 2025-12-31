# Unified Test Runner - combines all test layers
set(UNIT_TEST_RUNNER_FOLDERS_TO_IGNORE "")

# Files to exclude from the test runner
set(UNIT_TEST_RUNNER_FILES_TO_EXCLUDE
	"B3DNetworkTest.cpp"
)

B3DGlobSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} "" "${UNIT_TEST_RUNNER_FOLDERS_TO_IGNORE}" UNIT_TEST_RUNNER_SRC)

# Filter out excluded files
foreach(excludeFile ${UNIT_TEST_RUNNER_FILES_TO_EXCLUDE})
	list(FILTER UNIT_TEST_RUNNER_SRC EXCLUDE REGEX "${excludeFile}$")
endforeach()

# Target
add_executable(UnitTestRunner ${UNIT_TEST_RUNNER_SRC})

# Common flags
B3DSetDefaultLinkAndCompileFlags(UnitTestRunner)

# Working directory
set_target_properties(UnitTestRunner PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)")

# Includes
target_include_directories(UnitTestRunner PRIVATE
	"${B3D_FRAMEWORK_SOURCE_FOLDER}/Engine/Utility"
	"${B3D_FRAMEWORK_SOURCE_FOLDER}/Engine/Utility/ThirdParty"
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/TestSuites"
	"${CMAKE_CURRENT_SOURCE_DIR}/Factories"
	"${CMAKE_CURRENT_SOURCE_DIR}/Utility"
	"${CMAKE_CURRENT_SOURCE_DIR}/RTTI")

# Libraries
target_link_libraries(UnitTestRunner bsf)

# Plugin dependencies (needed for Core layer tests)
B3DAddRuntimeDependencies(UnitTestRunner)
add_dependencies(UnitTestRunner bsfFBXImporter bsfFontImporter bsfFreeImgImporter bsfSL)

# IDE specific
set_property(TARGET UnitTestRunner PROPERTY FOLDER Tests)

# Register test (runs all layers by default)
add_test(NAME UnitTests COMMAND $<TARGET_FILE:UnitTestRunner>)
