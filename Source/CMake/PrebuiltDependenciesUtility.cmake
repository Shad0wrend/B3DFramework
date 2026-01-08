#######################################################################################
######################## Pre-built dependency download ################################
#######################################################################################

set(B3D_PREBUILT_DEPENDENCIES_URL "http://bearishsun.brontes.feralhosting.com/Banshee/PrebuiltDependencies/" CACHE STRING "The location that binary dependencies will be pulled from.")
mark_as_advanced(B3D_PREBUILT_DEPENDENCIES_URL)

# Downloads a single prebuilt dependency and extracts it into the dependencies folder.
#
# @param	dependencyName		Name of the dependency (e.g. 'XShaderCompiler', 'Mono', etc.)
# @param	dependencyVersion	Version of the dependency to download.
function(B3DUpdatePrebuiltDependency dependencyName dependencyVersion)
	set(dependencyFolder ${B3D_FRAMEWORK_SOURCE_FOLDER}/../Dependencies/${dependencyName})
	set(tempFolder ${B3D_FRAMEWORK_SOURCE_FOLDER}/../Temp)

	# Clean and create a temporary folder
	execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${tempFolder})
	execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${tempFolder})

	if(WIN32)
		set(platformSuffix Win32)
	elseif(LINUX)
		set(platformSuffix Linux)
	elseif(APPLE)
		set(platformSuffix macOS)
	endif()

	set(archiveName ${dependencyName}_${platformSuffix}_${dependencyVersion}.tar.gz)
	set(binaryDependencyURL ${B3D_PREBUILT_DEPENDENCIES_URL}/${archiveName})

	message(STATUS "Downloading ${archiveName}...")
	file(DOWNLOAD ${binaryDependencyURL} ${tempFolder}/${archiveName}
			SHOW_PROGRESS
			STATUS DOWNLOAD_STATUS)

	list(GET DOWNLOAD_STATUS 0 statusCode)
	if(NOT statusCode EQUAL 0)
		message(FATAL_ERROR "Dependency '${dependencyName}' failed to download from URL: ${binaryDependencyURL}")
	endif()

	message(STATUS "Extracting ${archiveName}...")
	execute_process(
			COMMAND ${CMAKE_COMMAND} -E tar xzf ${tempFolder}/${archiveName}
			WORKING_DIRECTORY ${tempFolder}
	)

	# Remove old dependency contents (preserve .reqversion which is in Git)
	file(GLOB dependencyContents "${dependencyFolder}/*")
	foreach(item ${dependencyContents})
		get_filename_component(itemName ${item} NAME)
		if(NOT itemName STREQUAL ".reqversion")
			if(IS_DIRECTORY ${item})
				execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${item})
			else()
				execute_process(COMMAND ${CMAKE_COMMAND} -E remove ${item})
			endif()
		endif()
	endforeach()

	# Copy new dependency contents
	file(GLOB extractedContents "${tempFolder}/${dependencyName}/*")
	foreach(item ${extractedContents})
		get_filename_component(itemName ${item} NAME)
		if(IS_DIRECTORY ${item})
			execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${item} ${dependencyFolder}/${itemName})
		else()
			execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${item} ${dependencyFolder}/${itemName})
		endif()
	endforeach()

	# Clean up
	execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${tempFolder})
endfunction()

# Checks if a single dependency is out of date and if so, downloads it.
# Version is read from .reqversion file in the dependency folder.
#
# @param	dependencyName		Name of the dependency (e.g. 'XShaderCompiler', 'Mono', etc.)
function(B3DCheckAndUpdatePrebuiltDependency dependencyName)
	set(dependencyFolder ${B3D_FRAMEWORK_SOURCE_FOLDER}/../Dependencies/${dependencyName})
	set(versionFile ${dependencyFolder}/.version)
	set(reqVersionFile ${dependencyFolder}/.reqversion)

	# Check if .reqversion file exists
	if(NOT EXISTS ${reqVersionFile})
		message(WARNING "No .reqversion file found for dependency '${dependencyName}'. Skipping update check.")
		return()
	endif()

	# Read required version
	file(STRINGS ${reqVersionFile} requiredVersion)

	# Check if dependency needs to be downloaded
	if(NOT EXISTS ${versionFile})
		message(STATUS "Dependency '${dependencyName}' is missing. Downloading version ${requiredVersion}...")
		B3DUpdatePrebuiltDependency(${dependencyName} ${requiredVersion})
	else()
		file(STRINGS ${versionFile} currentVersion)
		if(${requiredVersion} GREATER ${currentVersion})
			message(STATUS "Dependency '${dependencyName}' is out of date (have v${currentVersion}, need v${requiredVersion}). Downloading...")
			B3DUpdatePrebuiltDependency(${dependencyName} ${requiredVersion})
		endif()
	endif()
endfunction()