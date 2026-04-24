# Find ogg dependency
#
# This module defines
#  ogg_INCLUDE_DIRS
#  ogg_LIBRARIES
#  ogg_FOUND

B3DStartFindPackage(ogg)

if(B3D_USE_BUNDLED_LIBRARIES)
	set(ogg_INSTALL_DIR ${B3D_FRAMEWORK_SOURCE_FOLDER}/../Dependencies/libogg CACHE PATH "")
endif()
B3DPopulateDefaultPackageSearchPaths(ogg)

B3DFindImportedIncludes(ogg ogg/ogg.h)

if(WIN32)
	B3DFindImportedLibraryWithConfigurationNames(ogg ogg STATIC ogg oggd)
else()
	B3DFindImportedLibrary(ogg ogg STATIC)
endif()

B3DEndFindPackage(ogg ogg)
