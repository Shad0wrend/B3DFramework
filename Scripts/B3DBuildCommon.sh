#!/bin/sh

CurrentDirectory="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
Platform="${1:-$OSTYPE}"
PlatformDependencyFolder="$CurrentDirectory/../Dependencies"

if [[ $Platform == "win32" || $Platform == "msys" ]]; then
	SharedLibraryExtension=".dll"
	StaticLibraryExtension=".lib"
	ImportLibraryExtension=".lib"
	StaticLibraryPrefix=""
elif [[ $Platform == "darwin"* ]]; then
	CMakeGenerator="-G Xcode"
	SharedLibraryExtension=".dylib"
	StaticLibraryExtension=".a"
	ImportLibraryExtension=""
	StaticLibraryPrefix="lib"
elif [[ $Platform == "linux-gnu"* ]]; then
	CMakeGenerator="-G Ninja Multi-Config"
	SharedLibraryExtension=".so"
	StaticLibraryExtension=".a"
	ImportLibraryExtension=""
	StaticLibraryPrefix="lib"
else
	echo "[Error] Unknown platform. Supported values are: win32, darwin, linux-gnu; got $Platform."
	exit 1
fi