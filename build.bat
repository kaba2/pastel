@echo off

set buildDir=build
if not exist %buildDir% (
    echo Creating directory %buildDir%/...
    mkdir %buildDir%
)

set buildType=RelWithDebInfo
if NOT [%1]==[] (
    set buildType=%1
)

set toolchainFile="C:\code\vcpkg\scripts\buildsystems\vcpkg.cmake"

if exist "%buildDir%\CMakeCache.txt" (
    echo Removing existing CMakeCache.txt...
    del "%buildDir%\CMakeCache.txt"
)

echo Type: %buildType%
echo Directory: %buildDir%
echo Toolchain: %toolchainFile%

REM goto NARP

REM cmake -S . -G"Visual Studio 16 2019" -T ClangCL -DCMAKE_BUILD_TYPE=%buildType% -B %buildDir% -DCMAKE_TOOLCHAIN_FILE=%toolchainFile%
cmake -S . -G"Visual Studio 16 2019" -DCMAKE_BUILD_TYPE=%buildType% -B %buildDir% -DCMAKE_TOOLCHAIN_FILE=%toolchainFile%

REM start %buildDir%\Pastel.sln

:NARP