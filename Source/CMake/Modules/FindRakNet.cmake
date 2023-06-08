# Find RakNet dependency
#
# This module defines
#  RakNet_INCLUDE_DIRS
#  RakNet_LIBRARIES
#  RakNet_FOUND

B3DStartFindPackage(RakNet)

if(B3D_USE_BUNDLED_LIBRARIES)
	set(RakNet_INSTALL_DIR ${BSF_SOURCE_DIR}/../Dependencies/RakNet CACHE PATH "")
endif()

B3DPopulateDefaultPackageSearchPaths(RakNet)
B3DFindImportedIncludes(RakNet RakNet/RakPeer.h)
B3DFindImportedLibrary(RakNet RakNetLibStatic SHARED)

B3DEndFindPackage(RakNet RakNetLibStatic)
