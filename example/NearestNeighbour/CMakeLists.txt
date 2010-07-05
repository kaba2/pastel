# This is a CMake build file for NearestNeighbour.

cmake_minimum_required(VERSION 2.8)

set(NearestNeighbour_Files
nearestneighbour.cpp
nearestneighbourmain.cpp
readme.txt
)

include_directories(${Pastel_SOURCE_DIR})
link_directories(${Pastel_Binary_Dir}/lib)
add_executable(NearestNeighbour ${NearestNeighbour_Files})
target_link_libraries(NearestNeighbour 
PastelSys PastelMath PastelGeometry PastelDevice 
PastelDsp PastelGfx PastelGfxUi PastelGl 
SDL glew32 opengl32)
