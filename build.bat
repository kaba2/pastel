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

if exist "%buildDir%\CMakeCache.txt" (
    echo Removing existing CMakeCache.txt...
    del "%buildDir%\CMakeCache.txt"
)

echo Type: %buildType%
echo Directory: %buildDir%

cmake -S . -G"Visual Studio 16 2019" -DCMAKE_BUILD_TYPE=%buildType% -B %buildDir%
