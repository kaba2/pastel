# This is a CMake build file for PastelGeometryTest.

cmake_minimum_required(VERSION 2.8)

set(PastelGeometryTest_Files
instantiate_bounding.cpp
instantiate_common.cpp
instantiate_distance.cpp
instantiate_intersect.cpp
instantiate_overlap.cpp
pastelgeometrytest.cpp
pastelgeometrytest.h
pastelgeometrytest.txt
test_area.cpp
test_halfmesh.cpp
test_halfmesh2.cpp
test_intersectlineaabb.cpp
test_intersectlinetriangle.cpp
test_kdtree.cpp
test_leastsquares.cpp
test_overlapaabbaabb.cpp
test_patternmatch.cpp
test_pointkdtree.cpp
test_pointpattern.cpp
test_simplehalfmesh.cpp
test_transformation.cpp
)

include_directories(${Pastel_Source_Dir})
link_directories(${Pastel_Binary_Dir}/lib)
add_executable(PastelGeometryTest ${PastelGeometryTest_Files})
target_link_libraries(PastelGeometryTest 
PastelSys PastelMath PastelGeometry PastelDsp PastelGfx
PastelDevice SDL)
