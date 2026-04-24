# Find FLAC dependency
#
# This module defines
#  FLAC_INCLUDE_DIRS
#  FLAC_LIBRARIES
#  FLAC_FOUND

B3DStartFindPackage(FLAC)

if(B3D_USE_BUNDLED_LIBRARIES)
	set(FLAC_INSTALL_DIR ${B3D_FRAMEWORK_SOURCE_FOLDER}/../Dependencies/libFLAC CACHE PATH "")
endif()
B3DPopulateDefaultPackageSearchPaths(FLAC)

B3DFindImportedIncludes(FLAC FLAC/all.h)

if(WIN32)
	B3DFindImportedLibraryWithConfigurationNames(FLAC FLAC STATIC FLAC FLACd)
else()
	B3DFindImportedLibrary(FLAC FLAC STATIC)
endif()

B3DEndFindPackage(FLAC FLAC)
