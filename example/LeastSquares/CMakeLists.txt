# This is a CMake build file for LeastSquares.

cmake_minimum_required(VERSION 2.8)

set(LeastSquares_Files
leastsquares.cpp
leastsquaresmain.cpp
)

include_directories(${Pastel_Source_Dir})
link_directories(${Pastel_Binary_Dir}/lib)
add_executable(LeastSquares ${LeastSquares_Files})
target_link_libraries(LeastSquares 
PastelSys PastelMath PastelGeometry PastelDevice 
PastelDsp PastelGfx PastelGfxUi PastelGl 
SDL glew32 opengl32)

