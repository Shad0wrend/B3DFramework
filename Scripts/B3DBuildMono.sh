#!/bin/sh

. ./B3DBuildCommon.sh

# Determine platform
if [[ "$Platform" == "win32" || "$Platform" == "msys" ]]; then
	echo "Building for Windows."
    echo ""
    echo "IMPORTANT: "
    echo " - Make sure to install all prerequisites as specified here: https://github.com/dotnet/runtime/blob/main/docs/workflow/requirements/windows-requirements.md"
    echo " - If you receive an error that .NET runtime is in use, shut down all programs that may use it (such as Visual Studio)"
    echo " - If you receive an error that files cannot be created or opened, try running the script at disk root, as long paths can be a problem"
    echo " - If you receive a ILLinker error during compilation, try running the script again until it succeeds"
    echo ""
    sleep 2
else
	echo "[Error] This build script is not currently supported on the current platform: $Platform."
	exit 1
fi

# Create intermediate folders
cd ..

mkdir -p Intermediate
cd Intermediate

mkdir -p DependencySources
cd DependencySources

# Clone
if [ -d "DotNetRuntime" ]; then
    cd DotNetRuntime
    git stash
    git pull origin release/9.0
    git stash pop
else
    git clone https://github.com/dotnet/runtime.git DotNetRuntime
    cd DotNetRuntime
    git checkout release/9.0
	git apply "$CurrentDirectory/Patches/Mono.patch" || exit 1
fi

# Setup Mono output folders
MonoOutputFolder="$PlatformDependencyFolder/DotNETCoreMono"

rm -rf $MonoOutputFolder
mkdir -p "$MonoOutputFolder/include/"
mkdir -p "$MonoOutputFolder/lib/"
mkdir -p "$MonoOutputFolder/bin/"
mkdir -p "$MonoOutputFolder/bin/Assemblies/"

if [[ "$Platform" == "win32" || "$Platform" == "msys" ]]; then
    mkdir -p "$MonoOutputFolder/lib/Release/"
    mkdir -p "$MonoOutputFolder/lib/Debug/"

    mkdir -p "$MonoOutputFolder/bin/Release/"
    mkdir -p "$MonoOutputFolder/bin/Debug/"
fi

# Helper to copy libraries for different configurations/platforms/architectures
copy_libraries()
{
    cp -p -a -- "artifacts/bin/mono/$1.$2.$3/IL/." "$MonoOutputFolder/bin/Assemblies/"
    cp -p -a -- "artifacts/bin/runtime/net9.0-$1-$3-$2/." "$MonoOutputFolder/bin/Assemblies/"
    cp -p -- "artifacts/bin/mono/$1.$2.$3/coreclr$SharedLibraryExtension" "$MonoOutputFolder/bin/$4"

    if [[ "$Platform" == "win32" || "$Platform" == "msys" ]]; then
        cp -p -- "artifacts/obj/mono/$1.$2.$3/out/lib/coreclr.import.lib" "$MonoOutputFolder/lib/$4"

        if [[ "$3" == "Debug" ]]; then
            cp -p -- "artifacts/bin/mono/$1.$2.$3/coreclr.pdb" "$MonoOutputFolder/bin/$4"
        fi
    fi
}


if [[ "$Platform" == "win32" || "$Platform" == "msys" ]]; then
    # Build & copy debug
    ./build.cmd mono+libs -configuration Debug
    copy_libraries windows x64 Debug Debug/ 

    # Build & copy release
    ./build.cmd mono+libs -configuration Release
    copy_libraries windows x64 Release Release/ 

    # Copy includes
    cp -a -r -- "artifacts/bin/mono/windows.x64.Debug/include/mono-2.0/." "$MonoOutputFolder/include/"
elif [[ "$Platform" == "darwin"* ]]; then
    # Build & copy release
    ./build.sh mono+libs -configuration Release
    copy_libraries osx arm64 Release ""

    # Copy includes
    cp -a -r -- "artifacts/bin/mono/osx.arm64.Release/include/mono-2.0/." "$MonoOutputFolder/include/"
else
    # Build & copy release
    ./build.sh mono+libs -configuration Release
    copy_libraries linux x64 Release ""

    # Copy includes
    cp -a -r -- "artifacts/bin/mono/linux.x64.Release/include/mono-2.0/." "$MonoOutputFolder/include/"
fi

echo ""
echo "======================================================================"
echo "Build complete!"
echo "======================================================================"
echo ""
echo "Mono has been built and installed to:"
echo "  $MonoOutputFolder"
echo ""
