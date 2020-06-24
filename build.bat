@echo off

if not exist build (
    mkdir build
)

if exist "build\CMakeCache.txt" (
    del "build\CMakeCache.txt"
)
cmake -S . -G"Visual Studio 16 2019" -T "ClangCL" -DCMAKE_BUILD_TYPE=Release -B build "-DCMAKE_TOOLCHAIN_FILE=C:\code\vcpkg\scripts\buildsystems\vcpkg.cmake"
REM start build\Pastel.sln
