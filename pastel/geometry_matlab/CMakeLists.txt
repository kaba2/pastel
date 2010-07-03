# This is a CMake build file for PastelGeometryMatlab.

cmake_minimum_required(VERSION 2.8)

source_group(
"Matlab files"
FILES
check.m
pastel_point_pattern_match.m
)

source_group(
"Mex files"
FILES
matlab_point_pattern_match.cpp
pastelgeometry_matlab.cpp
pastelgeometry_matlab.h
pastelgeometry_mex.cpp
pastelgeometry_mex.h
pastelgeometry_mex.hpp
)

set(PastelGeometryMatlab_Files
check.m
matlab_point_pattern_match.cpp
pastel_point_pattern_match.m
pastelgeometry_matlab.cpp
pastelgeometry_matlab.h
pastelgeometry_mex.cpp
pastelgeometry_mex.h
pastelgeometry_mex.hpp
)

include_directories(${Pastel_Source_Dir})
link_directories(${Pastel_Binary_Dir}/lib)
add_library(PastelGeometryMatlab ${PastelGeometryMatlab_Files})
