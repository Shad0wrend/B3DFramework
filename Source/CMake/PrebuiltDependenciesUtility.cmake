#######################################################################################
######################## Pre-built dependency download ################################
#######################################################################################

set(B3D_PREBUILT_DEPENDENCIES_URL "https://aigaion.feralhosting.com/bearishsun/banshee" CACHE STRING "The location that binary dependencies will be pulled from.")
mark_as_advanced(B3D_PREBUILT_DEPENDENCIES_URL)

# Downloads prebuilt binary dependencies and copies them into the dependencies folder.
#
# @param	dependencyPrefix 	Prefix identifying the dependency pack we're downloading (e.g. 'Framework', 'Editor', etc.)
# @param	dependencyFolder	Folder in which to unpack the dependencies, e.g. '${PROJECT_SOURCE_DIR}/Dependencies'.
# @param	folderName			Name of the folder in the downloaded package in which data is stored. Usually the same
#								as the last folder in @p dependencyFolder (e.g. 'Dependencies').
# @param	dependencyVersion	Version of the dependencies to download.
function(B3DUpdatePrebuiltDependencies dependencyPrefix dependencyFolder folderName dependencyVersion)
	# Clean and create a temporary folder
	execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_SOURCE_DIR}/Temp)
	execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/Temp)

	if(WIN32)
		set(dependencyType VS2015)
	elseif(LINUX)
		set(dependencyType Linux)
	elseif(APPLE)
		set(dependencyType macOS)
	endif()

	set(binaryDependenciesURL ${B3D_PREBUILT_DEPENDENCIES_URL}/${dependencyPrefix}_${dependencyType}_Master_${dependencyVersion}.zip)
	file(DOWNLOAD ${binaryDependenciesURL} ${PROJECT_SOURCE_DIR}/Temp/Dependencies.zip
			SHOW_PROGRESS
			STATUS DOWNLOAD_STATUS)

	list(GET DOWNLOAD_STATUS 0 statusCode)
	if(NOT statusCode EQUAL 0)
		message(FATAL_ERROR "Binary dependencies failed to download from URL: ${binaryDependenciesURL}")
	endif()

	message(STATUS "Extracting files. Please wait...")
	execute_process(
			COMMAND ${CMAKE_COMMAND} -E tar xzf ${PROJECT_SOURCE_DIR}/Temp/Dependencies.zip
			WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Temp
	)

	# Copy executables and dynamic libraries
	if(EXISTS ${PROJECT_SOURCE_DIR}/Temp/bin)
		execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Temp/bin ${dependencyFolder}/../bin)
	endif()

	# Copy static libraries, headers and tools
	execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${dependencyFolder})
	execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Temp/${folderName} ${dependencyFolder})

	# Clean up
	execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_SOURCE_DIR}/Temp)
endfunction()

# Checks if dependencies are out of date and if so, downloads prebuilt binary dependencies and copies them into the
# dependencies folder.
#
# @param	dependencyPrefix 	Prefix identifying the dependency pack we're downloading (e.g. 'Framework', 'Editor', etc.)
# @param	dependencyFolder	Folder in which to unpack the dependencies, e.g. '${PROJECT_SOURCE_DIR}/Dependencies'.
# @param	folderName			Name of the folder in the downloaded package in which data is stored. Usually the same
#								as the last folder in @p dependencyFolder (e.g. 'Dependencies').
# @param	dependencyVersion	Version of the dependencies to download.
function(B3DCheckAndUpdatePrebuiltDependencies dependencyPrefix dependencyFolder folderName dependencyVersion)
	set(builtinDependencyVersionFile ${dependencyFolder}/.version)
	if(NOT EXISTS ${builtinDependencyVersionFile})
		message(STATUS "Binary dependencies for '${dependencyPrefix}' are missing. Downloading package...")
		B3DUpdatePrebuiltDependencies(${dependencyPrefix} ${dependencyFolder} ${folderName} ${dependencyVersion})
	else()
		file (STRINGS ${builtinDependencyVersionFile} currentDependencyVersion)
		if(${dependencyVersion} GREATER ${currentDependencyVersion})
			message(STATUS "Your precomiled dependencies package for '${dependencyPrefix}' is out of date. Downloading latest package...")
			B3DUpdatePrebuiltDependencies(${dependencyPrefix} ${dependencyFolder} ${folderName} ${dependencyVersion})
		endif()
	endif()
endfunction()