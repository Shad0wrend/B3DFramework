# Find GameNetworkingSockets
#
# This module defines
#  GameNetworkingSockets_INCLUDE_DIRS
#  GameNetworkingSockets_LIBRARIES
#  GameNetworkingSockets_FOUND

B3DStartFindPackage(GameNetworkingSockets)

# Always use bundled library
set(GameNetworkingSockets_INSTALL_DIR ${B3D_FRAMEWORK_SOURCE_FOLDER}/../Dependencies/GameNetworkingSockets CACHE PATH "")
B3DPopulateDefaultPackageSearchPaths(GameNetworkingSockets)

B3DFindImportedIncludes(GameNetworkingSockets steam/steamnetworkingsockets.h)
B3DFindImportedLibrary(GameNetworkingSockets GameNetworkingSockets SHARED)

B3DEndFindPackage(GameNetworkingSockets GameNetworkingSockets)
