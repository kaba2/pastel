# This is a CMake build file for Pastel library.

cmake_minimum_required(VERSION 2.8)

project(PastelCore)
add_subdirectory(sys)
add_subdirectory(math)
add_subdirectory(geometry)
add_subdirectory(dsp)
add_subdirectory(gfx)
add_subdirectory(gfxui)
add_subdirectory(gl)
add_subdirectory(geometry_matlab)
add_subdirectory(ray)
add_subdirectory(device)

