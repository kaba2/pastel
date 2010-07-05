# This is a CMake build file for PastelMathTest.

cmake_minimum_required(VERSION 2.8)

set(PastelMathTest_Files
mathinstantiate.cpp
pastelmathtest.cpp
pastelmathtest.h
pastelmathtest.txt
test_biginteger.cpp
test_cholesky_decomposition.cpp
test_common.cpp
test_complex.cpp
test_gcd.cpp
test_lowdiscrepancy.cpp
test_ludecomposition.cpp
test_matrix.cpp
test_minmax.cpp
test_piecewiselinear.cpp
test_polynomial.cpp
test_qr_decomposition.cpp
test_rational.cpp
test_smoothstep.cpp
test_uniformsampling.cpp
test_vector.cpp
)

include_directories(${Pastel_SOURCE_DIR})
link_directories(${Pastel_Binary_Dir}/lib)
add_executable(PastelMathTest ${PastelMathTest_Files})
target_link_libraries(PastelMathTest 
PastelSys PastelMath PastelGeometry PastelDsp PastelGfx
PastelDevice SDL)
