@echo off
setlocal enabledelayedexpansion 

echo Installing dependencies...

set "vcpkgDir=vcpkg"
if NOT [%1]==[] (
    set "vcpkgDir=%1"
)

if not exist !vcpkgDir! (
    echo Cloning vcpkg to !vcpkgDir!/...
    git clone https://github.com/microsoft/vcpkg !vcpkgDir!
    goto NARP
)

"!vcpkgDir!/vcpkg" install tbb eigen3 range-v3 boost

:NARP
