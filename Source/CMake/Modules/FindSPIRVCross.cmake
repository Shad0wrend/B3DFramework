# Find SPIRVCross
#
# This module defines
#  SPIRVCross_INCLUDE_DIRS
#  SPIRVCross_LIBRARIES
#  SPIRVCross_FOUND

B3DStartFindPackage(SPIRVCross)

if(B3D_USE_BUNDLED_LIBRARIES)
	set(SPIRVCross_INSTALL_DIR ${BSF_SOURCE_DIR}/../Dependencies/SPIRVCross CACHE PATH "")
endif()

B3DPopulateDefaultPackageSearchPaths(SPIRVCross)

B3DFindImportedIncludes(SPIRVCross spirv_cross/spirv_cross.hpp)
B3DFindImportedLibrary(SPIRVCross spirv-cross-msl STATIC)
B3DFindImportedLibrary(SPIRVCross spirv-cross-glsl STATIC)
B3DFindImportedLibrary(SPIRVCross spirv-cross-core STATIC)

B3DEndFindPackage(SPIRVCross spirv-cross-msl)
