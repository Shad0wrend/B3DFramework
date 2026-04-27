# Find nvtt dependency
#
# This module defines
#  nvtt_INCLUDE_DIRS
#  nvtt_LIBRARIES
#  nvtt_FOUND

B3DStartFindPackage(nvtt)

set(nvtt_BUNDLED_INSTALL_DIR ${B3D_FRAMEWORK_SOURCE_FOLDER}/../Dependencies/nvtt)
if(B3D_USE_BUNDLED_LIBRARIES OR NOT nvtt_INSTALL_DIR)
	set(nvtt_INSTALL_DIR ${nvtt_BUNDLED_INSTALL_DIR} CACHE PATH "Path to nvtt dependency" FORCE)
endif()
B3DPopulateDefaultPackageSearchPaths(nvtt)

B3DFindImportedIncludes(nvtt nvtt.h)
B3DFindImportedLibrary(nvtt nvtt STATIC)
B3DFindImportedLibrary(nvtt nvimage STATIC)
B3DFindImportedLibrary(nvtt bc6h STATIC)
B3DFindImportedLibrary(nvtt bc7 STATIC)
B3DFindImportedLibrary(nvtt nvcore STATIC)
B3DFindImportedLibrary(nvtt nvmath STATIC)
B3DFindImportedLibrary(nvtt nvthread STATIC)
B3DFindImportedLibrary(nvtt squish STATIC)
B3DFindImportedLibrary(nvtt nvsquish STATIC)

B3DEndFindPackage(nvtt nvtt)
