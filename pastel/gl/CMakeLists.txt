# This is a CMake build file for PastelGl.

cmake_minimum_required(VERSION 2.8)

source_group(
"Common"
FILES
common.txt
gllibrary.cpp
gllibrary.h
)

source_group(
"GlGfxRenderer"
FILES
glgfxrenderer.cpp
glgfxrenderer.h
rendering.txt
)

set(PastelGl_Files
common.txt
glgfxrenderer.cpp
glgfxrenderer.h
gllibrary.cpp
gllibrary.h
pastelgl.txt
rendering.txt
)

include_directories(${Pastel_SOURCE_DIR})
link_directories(${Pastel_Binary_Dir}/lib)
add_library(PastelGl ${PastelGl_Files})
