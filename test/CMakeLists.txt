# This is a CMake build file for Pastel tests.

cmake_minimum_required(VERSION 2.8)

project(PastelTest)
add_subdirectory(devicetest)
add_subdirectory(geometrytest)
add_subdirectory(gfxtest)
add_subdirectory(mathtest)
add_subdirectory(raytest)
add_subdirectory(systest)
