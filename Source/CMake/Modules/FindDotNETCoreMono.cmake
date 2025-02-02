# Find mono dependency
#
# This module defines
#  DotNETCoreMono_INCLUDE_DIRS
#  DotNETCoreMono_LIBRARIES
#  DotNETCoreMono_FOUND

B3DStartFindPackage(DotNETCoreMono)

set(DotNETCoreMono_INSTALL_DIR ${BSF_SOURCE_DIR}/../Dependencies/DotNETCoreMono CACHE PATH "")

B3DPopulateDefaultPackageSearchPaths(DotNETCoreMono)
list(APPEND DotNETCoreMono_INCLUDE_SEARCH_DIRS ${DotNETCoreMono_INSTALL_DIR}/include)

B3DFindImportedIncludes(DotNETCoreMono mono/jit/jit.h)
B3DFindImportedLibraryWithAlternateBinaryName(DotNETCoreMono coreclr.import SHARED coreclr)

B3DEndFindPackage(DotNETCoreMono coreclr.import)

# Install the managed assemblies
install(
	DIRECTORY ${DotNETCoreMono_INSTALL_DIR}/bin/Assemblies
	DESTINATION bin/
)
