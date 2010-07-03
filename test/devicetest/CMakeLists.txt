# This is a CMake build file for PastelDeviceTest.

cmake_minimum_required(VERSION 2.8)

set(PastelDeviceTest_Files
pasteldevicetest.cpp
pasteldevicetest.h
pasteldevicetest.txt
test_thread.cpp
)

include_directories(${Pastel_Source_Dir})
link_directories(${Pastel_Binary_Dir}/lib)
add_executable(PastelDeviceTest ${PastelDeviceTest_Files})
target_link_libraries(PastelDeviceTest 
PastelSys PastelDevice SDL)
