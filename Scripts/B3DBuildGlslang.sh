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

# Fetch external sources (SPIRV-Tools + SPIRV-Headers) into External/ so glslang's CMake
# enables ENABLE_OPT (the spirv-opt optimizer path)
#
# We clone the repos directly at glslang's known-good revisions rather than running glslang's
# own update_glslang_sources.py, which imports Python's distutils module (removed in Python
# 3.12+). The commits below are mirrored from this glslang checkout's known_good.json.
SPIRV_TOOLS_COMMIT="117a1fd11f11e9bef9faa563c3d5156cc6ab529c"
SPIRV_HEADERS_COMMIT="79b6681aadcb53c27d1052e5f8a0e82a981dbf2f"

# Clone (or update) a pinned git dependency into a target directory.
#   $1 = repo URL, $2 = target directory, $3 = commit to check out
B3DFetchPinnedRepo() {
	_repoUrl="$1"
	_targetDir="$2"
	_commit="$3"
	if [ -d "$_targetDir/.git" ]; then
		( cd "$_targetDir" && git fetch --quiet && git checkout --quiet "$_commit" ) || return 1
	else
		git clone "$_repoUrl" "$_targetDir" || return 1
		( cd "$_targetDir" && git checkout --quiet "$_commit" ) || return 1
	fi
}

B3DFetchPinnedRepo https://github.com/KhronosGroup/SPIRV-Tools.git \
	External/spirv-tools "$SPIRV_TOOLS_COMMIT" || exit 1
B3DFetchPinnedRepo https://github.com/KhronosGroup/SPIRV-Headers.git \
	External/spirv-tools/external/spirv-headers "$SPIRV_HEADERS_COMMIT" || exit 1

# Python 3.11 removed the 'U' (universal-newlines) file mode. This pinned SPIRV-Tools
# revision predates that and its build-version generator still opens with mode='rU', which
# now raises ValueError. Rewrite it to plain 'r' (text mode already does newline
# translation). Idempotent: a no-op once already patched.
sed -i "s/mode='rU'/mode='r'/g" External/spirv-tools/utils/update_build_version.py || exit 1

# Preflight: SPIRV-Tools' build-time code generators (invoked by its CMake custom commands)
# require a Python interpreter that can import 'distutils'. Python 3.12+ removed distutils from
# the standard library; it is provided by the 'setuptools' package (auto-loaded via its
# distutils-precedence .pth). Check up front and fail with actionable guidance rather than
# letting the build die later with a cryptic MSBuild "custom build ... exited with code 1".
PythonExe=""
if command -v python >/dev/null 2>&1; then
	PythonExe="python"
elif command -v python3 >/dev/null 2>&1; then
	PythonExe="python3"
fi

if [ -z "$PythonExe" ]; then
	echo "[Error] Python is required to build SPIRV-Tools (spirv-opt) but was not found on PATH." 1>&2
	exit 1
fi

if ! "$PythonExe" -c "import distutils" >/dev/null 2>&1; then
	echo "======================================================================" 1>&2
	echo "[Error] '$PythonExe' cannot import the 'distutils' module." 1>&2
	echo "        SPIRV-Tools' build-time generators require it, but Python 3.12+" 1>&2
	echo "        removed distutils from the standard library. Install setuptools" 1>&2
	echo "        (it ships a distutils compatibility shim) and re-run this script:" 1>&2
	echo "" 1>&2
	echo "            $PythonExe -m pip install setuptools" 1>&2
	echo "======================================================================" 1>&2
	exit 1
fi

# Setup output folders
OutputFolder="$PlatformDependencyFolder/glslang"
B3DCleanDependencyFolder "$OutputFolder"

# Configure (identical options across all platforms). ENABLE_OPT=ON pulls in the bundled
# SPIRV-Tools (fetched above) so the spirv-opt optimizer is built and installed alongside
# glslang. SPIRV-Tools tests/executables are skipped to keep the build lean.
cmake -S . -B build -G "$CMakeGenerator" \
	-DCMAKE_INSTALL_PREFIX="$OutputFolder" \
	-DCMAKE_POSITION_INDEPENDENT_CODE=ON \
	-DCMAKE_DEBUG_POSTFIX=d \
	-DBUILD_SHARED_LIBS=OFF \
	-DENABLE_GLSLANG_BINARIES=OFF \
	-DENABLE_OPT=ON \
	-DSPIRV_SKIP_TESTS=ON \
	-DSPIRV_SKIP_EXECUTABLES=ON \
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
