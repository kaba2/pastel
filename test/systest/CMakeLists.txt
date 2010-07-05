# This is a CMake build file for PastelSysTest.

cmake_minimum_required(VERSION 2.8)

set(PastelSysTest_Files
CMakeLists.txt
pastelsystest.cpp
pastelsystest.h
pastelsystest.txt
poolspeed.txt
test_arenaallocator.cpp
test_array2.cpp
test_config.cpp
test_countedptr.cpp
test_disjointgroups.cpp
test_endian.cpp
test_eps.cpp
test_fastlist.cpp
test_indexallocator.cpp
test_insertionsort.cpp
test_iteratorrange.cpp
test_iterators.cpp
test_lineararray.cpp
test_math_functions.cpp
test_parallelarray.cpp
test_poolallocator.cpp
test_random.cpp
test_smallmap.cpp
test_smallset.cpp
test_statreal.cpp
test_string_tools.cpp
test_unordered_set.cpp
test_view.cpp
)

include_directories(${Pastel_SOURCE_DIR})
link_directories(${Pastel_Binary_Dir}/lib)

add_executable(PastelSysTest ${PastelSysTest_Files})
target_link_libraries(PastelSysTest 
PastelSys PastelDsp PastelGfx)
