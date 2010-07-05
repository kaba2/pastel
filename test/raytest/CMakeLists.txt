# This is a CMake build file for PastelRayTest.

cmake_minimum_required(VERSION 2.8)

set(PastelRayTest_Files
pastelraytest.cpp
pastelraytest.h
pastelraytest.txt
test_shape.cpp
test_thing.cpp
)

include_directories(${Pastel_SOURCE_DIR})
link_directories(${Pastel_Binary_Dir}/lib)
add_executable(PastelRayTest ${PastelRayTest_Files})
target_link_libraries(PastelRayTest 
PastelSys PastelMath PastelGeometry PastelDsp 
PastelGfx PastelRay)
