# This is a CMake build file for PastelDevice.

cmake_minimum_required(VERSION 2.8)

source_group(
"Fx"
FILES
fxdevice.cpp
fxdevice.h
sound.txt
)

source_group(
"Gfx"
FILES
gfxdevice.cpp
gfxdevice.h
graphics.txt
)

source_group(
"Multithreading"
FILES
multithreading.txt
mutex.cpp
mutex.h
mutex.hpp
thread.cpp
thread.h
)

source_group(
"Timing"
FILES
timedevice.cpp
timedevice.h
timer.cpp
timer.h
timing.txt
)

source_group(
"Common"
FILES
devicelibrary.cpp
devicelibrary.h
devices.txt
devicesystem.cpp
devicesystem.h
pasteldevice.txt
)

source_group(
"Ui"
FILES
ui.cpp
ui.h
)

set(PastelDevice_Files
devicelibrary.cpp
devicelibrary.h
devices.txt
devicesystem.cpp
devicesystem.h
fxdevice.cpp
fxdevice.h
gfxdevice.cpp
gfxdevice.h
graphics.txt
multithreading.txt
mutex.cpp
mutex.h
mutex.hpp
pasteldevice.txt
sound.txt
thread.cpp
thread.h
timedevice.cpp
timedevice.h
timer.cpp
timer.h
timing.txt
ui.cpp
ui.h
)

include_directories(${Pastel_SOURCE_DIR})
link_directories(${Pastel_Binary_Dir}/lib)
add_library(PastelDevice ${PastelDevice_Files})
