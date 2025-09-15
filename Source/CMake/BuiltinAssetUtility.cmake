#######################################################################################
##################### Built-in asset import and versioning ############################
#######################################################################################

# Downloads assets with the provided version and unpacks them to the assets folder, overwriting existing assets
#
# @param 	assetPrefix		Prefix identifying the asset pack we're downloading (e.g. 'Framework', 'Editor', etc.)
# @param 	assetFolder		Folder in which to unpack the assets, e.g. '${PROJECT_SOURCE_DIR}/Data'
# @param 	folderName 		Name of the folder in the downloaded package in which assets are stored. Usually the same
#							as the last folder in @p assetFolder (e.g. 'Data')
# @param	assetVersion	Version of the assets to download.
function(B3DUpdateBuiltinAssets assetPrefix assetFolder folderName assetVersion)
	# Clean and create a temporary folder
	execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_SOURCE_DIR}/Temp)	
	execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/Temp)	
	
	set(assetDependenciesURL ${B3D_PREBUILT_DEPENDENCIES_URL}/${assetPrefix}Data_Master_${assetVersion}.zip)
	file(DOWNLOAD ${assetDependenciesURL} ${PROJECT_SOURCE_DIR}/Temp/Dependencies.zip
		SHOW_PROGRESS
		STATUS DOWNLOAD_STATUS)
		
	list(GET DOWNLOAD_STATUS 0 STATUS_CODE)
	if(NOT STATUS_CODE EQUAL 0)
		message(FATAL_ERROR "Builtin assets failed to download from URL: ${assetDependenciesURL}")
	endif()
	
	message(STATUS "Extracting files. Please wait...")
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E tar xzf ${PROJECT_SOURCE_DIR}/Temp/Dependencies.zip
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Temp
	)
	
	# Copy files
	execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Temp/${folderName} ${assetFolder})
	
	# Make sure timestamp modify date/times are newer (avoids triggering reimport)
	execute_process(COMMAND ${CMAKE_COMMAND} -E touch ${assetFolder}/Timestamp.asset )

	# Clean up
	execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_SOURCE_DIR}/Temp)	
endfunction()

# Checks the current version of the builtin assets, and performs update of the assets if out of date.
#
# @param 	assetPrefix		Prefix identifying the asset pack we're downloading (e.g. 'Framework', 'Editor', etc.)
# @param 	assetFolder		Folder in which to unpack the assets, e.g. '${PROJECT_SOURCE_DIR}/Data'
# @param 	folderName 		Name of the folder in the downloaded package in which assets are stored. Usually the same
#							as the last folder in @p assetFolder (e.g. 'Data')
# @param	assetVersion	Version of the assets to download.
function(B3DUpdateBuiltinAssetsIfNeeded assetPrefix assetFolder folderName assetVersion)
	set(builtinAssetVersionFile ${assetFolder}/.version)
	if(NOT EXISTS ${builtinAssetVersionFile})
		message(STATUS "Builtin assets for '${assetPrefix}' are missing. Downloading package...")
		B3DUpdateBuiltinAssets(${assetPrefix} ${assetFolder} ${folderName} ${assetVersion})
	else()
		file (STRINGS ${builtinAssetVersionFile} currentBuiltinAssetVersion)
		if(${assetVersion} GREATER ${currentBuiltinAssetVersion})
			message(STATUS "Your builtin asset package for '${assetPrefix}' is out of date. Downloading latest package...")
			B3DUpdateBuiltinAssets(${assetPrefix} ${assetFolder} ${folderName} ${assetVersion})
		endif()
	endif()
endfunction()

# Registers a target that performs asset import for builtin assets.
#
# @param	assetPrefix			Prefix identifying the asset pack to build (e.g. 'Framework', 'Editor', etc.)
# @param	inputFolder			Absolute folder in which to find raw assets to import.
# @param	outputFolder		Absolute folder in which to place imported assets.
# @param	additionalArguments	Optional additional arguments to pass to the import tool.
function(B3DAddRunAssetImportTarget assetPrefix inputFolder outputFolder additionalArguments)
	find_package(bsfImportTool)

	if(NOT bsfImportTool_FOUND)
		message("Cannot add asset import target because bsfImportTool binaries cannot be found. Build the bsfImportTool target and install it to Dependencies/tools/bsfImportTool.")
	else()
		set(RunAssetImport_EXECUTABLE ${bsfImportTool_EXECUTABLE})
		set(RunAssetImport_INPUT_FOLDER ${inputFolder})
		set(RunAssetImport_OUTPUT_FOLDER ${outputFolder})
		set(RunAssetImport_CMD_ARGS ${additionalArguments})
		set(RunAssetImport_PREFIX ${assetPrefix})

		configure_file(
			${B3D_FRAMEWORK_SOURCE_FOLDER}/CMake/Scripts/RunAssetImport.cmake.in
			${CMAKE_CURRENT_BINARY_DIR}/RunAssetImport_${assetPrefix}.cmake
			@ONLY)
		
		add_custom_target(RunAssetImport_${assetPrefix} COMMAND ${CMAKE_COMMAND} -P
			${CMAKE_CURRENT_BINARY_DIR}/RunAssetImport_${assetPrefix}.cmake)
			
		set_property(TARGET RunAssetImport_${assetPrefix} PROPERTY FOLDER Scripts)
	endif()
endfunction()

# Registers a target that performs archive and upload of current set of builtin assets. Updates the .reqversion
# file to the next available version.
#
# @param	assetPrefix		Prefix identifying the asset packet to upload (e.g. 'Framework', 'Editor', etc.)
# @param	inputFolder		Folder that contains the data list file, and the .version and .reqversion files.
# @param	dataListFile	Textual file that contains a newline separated list of assets to package.
function(B3DAddUploadAssetsTarget assetPrefix inputFolder dataListFile)
	set(UploadAssets_FTP_CREDENTIALS_FILE ${B3D_FTP_CREDENTIALS_FILE})
	set(UploadAssets_INPUT_FOLDER ${inputFolder})
	set(UploadAssets_INPUT_FILES ${dataListFile})
	set(UploadAssets_ARCHIVE_NAME ${assetPrefix}Data_Master)
	set(UploadAssets_TEMP_FOLDER ${PROJECT_SOURCE_DIR}/Temp)

	configure_file(
		${B3D_FRAMEWORK_SOURCE_FOLDER}/CMake/Scripts/UploadAssets.cmake.in
		${CMAKE_CURRENT_BINARY_DIR}/UploadAssets_${assetPrefix}.cmake
		@ONLY)
	
	add_custom_target(UploadAssets_${assetPrefix} COMMAND ${CMAKE_COMMAND} -P
		${CMAKE_CURRENT_BINARY_DIR}/UploadAssets_${assetPrefix}.cmake)
		
	set_property(TARGET UploadAssets_${assetPrefix} PROPERTY FOLDER Scripts)
endfunction()