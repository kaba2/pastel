# This is a CMake build file for PastelGfxTest.

cmake_minimum_required(VERSION 2.8)

set(PastelGfxTest_Files
log.txt
pastelgfxtest.cpp
pastelgfxtest.h
pastelgfxtest.txt
test_adaptedarray.cpp
test_array.cpp
test_color.cpp
test_convolute.cpp
test_drawing.cpp
test_filter.cpp
test_fourier.cpp
test_gray.cpp
test_hsv.cpp
test_imagepdf.cpp
test_imageprocessing.cpp
test_mipmap.cpp
test_noise.cpp
test_palette.cpp
test_pcx.cpp
test_quantize.cpp
test_reconstructimage.cpp
test_ripmap.cpp
test_stretchcontrast.cpp
test_summedareatable.cpp
test_texture.cpp
test_texture_examples.cpp
)

include_directories(${Pastel_Source_Dir})
link_directories(${Pastel_Binary_Dir}/lib)
add_executable(PastelGfxTest ${PastelGfxTest_Files})
target_link_libraries(PastelGfxTest 
PastelSys PastelMath PastelGeometry PastelDsp PastelGfx
PastelDevice SDL)
