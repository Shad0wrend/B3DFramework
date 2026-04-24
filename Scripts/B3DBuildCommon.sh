#!/bin/sh

CurrentDirectory="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
Platform="${1:-$OSTYPE}"
PlatformDependencyFolder="$CurrentDirectory/../Dependencies"

if [[ $Platform == "win32" || $Platform == "msys" ]]; then
	DefaultCMakeGenerator="Visual Studio 17 2022"
	SharedLibraryExtension=".dll"
	StaticLibraryExtension=".lib"
	ImportLibraryExtension=".lib"
	StaticLibraryPrefix=""
elif [[ $Platform == "darwin"* ]]; then
	DefaultCMakeGenerator="Ninja Multi-Config"
	SharedLibraryExtension=".dylib"
	StaticLibraryExtension=".a"
	ImportLibraryExtension=""
	StaticLibraryPrefix="lib"
elif [[ $Platform == "linux-gnu"* ]]; then
	DefaultCMakeGenerator="Ninja Multi-Config"
	SharedLibraryExtension=".so"
	StaticLibraryExtension=".a"
	ImportLibraryExtension=""
	StaticLibraryPrefix="lib"
else
	echo "[Error] Unknown platform. Supported values are: win32, darwin, linux-gnu; got $Platform."
	exit 1
fi

# CMake generator used by B3DBuild*.sh scripts. Override by exporting B3D_CMAKE_GENERATOR
# before running a script, e.g. `B3D_CMAKE_GENERATOR="Ninja" ./B3DBuildSnappy.sh`.
CMakeGenerator="${B3D_CMAKE_GENERATOR:-$DefaultCMakeGenerator}"

# Wipes a dependency output folder while preserving .reqversion
B3DCleanDependencyFolder() {
	FolderToClean="$1"
	if [ -z "$FolderToClean" ]; then
		echo "[Error] B3DCleanDependencyFolder called without a folder argument."
		return 1
	fi

	if [ ! -d "$FolderToClean" ]; then
		mkdir -p "$FolderToClean"
		return 0
	fi

	ReqVersionFile="$FolderToClean/.reqversion"
	if [ -f "$ReqVersionFile" ]; then
		SavedReqVersion=$(cat "$ReqVersionFile")
		rm -rf "$FolderToClean"
		mkdir -p "$FolderToClean"
		printf '%s' "$SavedReqVersion" > "$ReqVersionFile"
	else
		rm -rf "$FolderToClean"
		mkdir -p "$FolderToClean"
	fi
}