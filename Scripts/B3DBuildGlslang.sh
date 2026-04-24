#!/bin/sh
# Copyright 2025-2026 Marko Pintera. All rights reserved.

. ./B3DBuildCommon.sh

echo "Builds glslang from source"
echo ""

# Create intermediate folders
cd ..

mkdir -p Intermediate
cd Intermediate

mkdir -p DependencySources
cd DependencySources

# Pinned glslang commit (matches dependencies.md)
GLSLANG_VERSION="377bccb143941ec4931e6aed9ac07752ccefb979"

# Clone from official KhronosGroup repo
if [ -d "glslang" ]; then
	cd glslang
	git fetch
	git checkout $GLSLANG_VERSION
else
	git clone https://github.com/KhronosGroup/glslang.git glslang
	cd glslang
	git checkout $GLSLANG_VERSION
fi

# Setup output folders
OutputFolder="$PlatformDependencyFolder/glslang"
B3DCleanDependencyFolder "$OutputFolder"

# Configure (identical options across all platforms)
cmake -S . -B build -G "$CMakeGenerator" \
	-DCMAKE_INSTALL_PREFIX="$OutputFolder" \
	-DCMAKE_POSITION_INDEPENDENT_CODE=ON \
	-DCMAKE_DEBUG_POSTFIX=d \
	-DBUILD_SHARED_LIBS=OFF \
	-DENABLE_GLSLANG_BINARIES=OFF \
	-DENABLE_HLSL=ON || exit 1

# Build and install. Headers land at include/glslang/... and include/SPIRV/... — the
# latter location is what GlslangToSpv.h's internal relative include
# (`../glslang/Include/...`) expects, so we keep it as-is. B3D sources include it as
# "SPIRV/GlslangToSpv.h" to match.
cmake --build build --config Release --target install || exit 1
cmake --build build --config Debug --target install || exit 1

echo ""
echo "======================================================================"
echo "Build complete!"
echo "======================================================================"
echo ""
echo "glslang has been built and installed to:"
echo "  $OutputFolder"
echo ""
