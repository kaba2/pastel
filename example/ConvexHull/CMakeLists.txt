# This is a CMake build file for ConvexHull.

cmake_minimum_required(VERSION 2.8)

set(ConvexHull_Files
convexhull.cpp
convexhullmain.cpp
)

include_directories(${Pastel_Source_Dir})
link_directories(${Pastel_Binary_Dir}/lib)
add_executable(ConvexHull ${ConvexHull_Files})
target_link_libraries(ConvexHull 
PastelSys PastelMath PastelGeometry PastelDevice 
PastelDsp PastelGfx PastelGfxUi PastelGl 
SDL glew32 opengl32)
