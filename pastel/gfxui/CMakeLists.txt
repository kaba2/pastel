# This is a CMake build file for PastelGfxUi.

cmake_minimum_required(VERSION 2.8)

set(PastelGfxUi_Files
common.txt
gfx_ui.cpp
gfx_ui.h
gfx_ui.txt
gfxuilibrary.h
pastelgfxui.txt
)

include_directories(${Pastel_SOURCE_DIR})
link_directories(${Pastel_Binary_Dir}/lib)
add_library(PastelGfxUi ${PastelGfxUi_Files})
