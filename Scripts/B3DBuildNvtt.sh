#!/bin/sh
# Copyright 2025-2026 Marko Pintera. All rights reserved.

. ./B3DBuildCommon.sh

echo "Builds NVIDIA Texture Tools (nvtt) from source"
echo ""

cd ..

mkdir -p Intermediate
cd Intermediate

mkdir -p DependencySources
cd DependencySources

NVTT_REPO="https://github.com/castano/nvidia-texture-tools.git"
NVTT_VERSION="2.1.2"

if [ -d "nvtt" ]; then
	cd nvtt
	git remote set-url origin "$NVTT_REPO"
	git fetch --tags origin
	git reset --hard $NVTT_VERSION
else
	git clone "$NVTT_REPO" nvtt
	cd nvtt
	git checkout $NVTT_VERSION
fi

echo "Applying Nvtt patch..."
git apply --check "$CurrentDirectory/Patches/Nvtt.patch" 2>/dev/null && \
	git apply "$CurrentDirectory/Patches/Nvtt.patch"

OutputFolder="$PlatformDependencyFolder/nvtt"
B3DCleanDependencyFolder "$OutputFolder"

mkdir -p "$OutputFolder/include"
mkdir -p "$OutputFolder/lib/Release"
mkdir -p "$OutputFolder/lib/Debug"

cmake -S . -B build -G "$CMakeGenerator" \
	-DCMAKE_POSITION_INDEPENDENT_CODE=ON \
	-DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
	-DCMAKE_POLICY_DEFAULT_CMP0091=NEW \
	-DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreaded\$<\$<CONFIG:Debug>:Debug>DLL" || exit 1

cmake --build build --config Release || exit 1
cmake --build build --config Debug || exit 1

BuildFolder="build"

CopyLib() {
	LibName="$1"
	SrcSubdir="$2"
	SrcReleaseName="${3:-$LibName}"
	SrcDebugName="${4:-$LibName}"

	cp -p "$BuildFolder/$SrcSubdir/Release/${SrcReleaseName}${StaticLibraryExtension}" \
		"$OutputFolder/lib/Release/${LibName}${StaticLibraryExtension}" || return 1
	cp -p "$BuildFolder/$SrcSubdir/Debug/${SrcDebugName}${StaticLibraryExtension}" \
		"$OutputFolder/lib/Debug/${LibName}${StaticLibraryExtension}" || return 1
}

CopyLib nvtt      src/nvtt || exit 1
CopyLib nvimage   src/nvimage || exit 1
CopyLib nvcore    src/nvcore || exit 1
CopyLib nvmath    src/nvmath || exit 1
CopyLib nvthread  src/nvthread || exit 1
CopyLib nvsquish  src/nvtt/squish || exit 1
CopyLib bc6h      src/bc6h || exit 1
CopyLib bc7       src/bc7 || exit 1
CopyLib squish    extern/libsquish-1.15 squish squishd || exit 1

cp -p src/nvtt/nvtt.h         "$OutputFolder/include/" || exit 1
cp -p src/nvtt/nvtt_wrapper.h "$OutputFolder/include/" || exit 1

echo ""
echo "======================================================================"
echo "Build complete!"
echo "======================================================================"
echo ""
echo "nvtt has been built and installed to:"
echo "  $OutputFolder"
echo ""
