@echo off
setlocal enabledelayedexpansion 

set buildDir=build
if not exist %buildDir% (
    echo Creating directory %buildDir%/...
    mkdir %buildDir%
)

set buildType=RelWithDebInfo
if NOT [%1]==[] (
    if not "%1"=="Debug" if not "%1"=="Release" if not "%1"=="RelWithDebInfo" (
        echo Unknown build type %1.
        echo Build type must be one of Debug, Release, or RelWithDebInfo.
        goto NARP
    )
    set buildType=%1
)

echo Type: !buildType!
echo Directory: !buildDir!

if exist "!buildDir!\CMakeCache.txt" (
    echo Removing existing CMakeCache.txt...
    del "!buildDir!\CMakeCache.txt"
)

cmake -S . -G"Visual Studio 16 2019" -DCMAKE_BUILD_TYPE=!buildType! -B !buildDir!

:NARP
