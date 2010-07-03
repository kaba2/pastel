# This is a CMake build file for all of Pastel, 
# including the library, tests, and examples.

cmake_minimum_required(VERSION 2.8)

project(PastelAll)

#set(CMAKE_BUILD_TYPE Debug)
#set(EXECUTABLE_OUTPUT_PATH
#${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
#set(LIBRARY_OUTPUT_PATH
#${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})

include_directories("E:/Ohjelmointi/external/boost_1_41_0")
include_directories("E:/Ohjelmointi/external/rply-1.01")
include_directories("E:/Ohjelmointi/external/SDL-1.2.13/include")
include_directories("E:/Ohjelmointi/external/glew/include")

link_directories("E:/Ohjelmointi/externallib")

# Visual Studio specific stuff
# ----------------------------

if(${CMAKE_GENERATOR} MATCHES "Visual Studio.*")

# Disable some warnings.
add_definitions("/wd4305 /wd4244 /wd4251 /wd4351")
add_definitions("/wd4996 /wd4267 /wd4018 /wd4312")
add_definitions("/wd4224 /wd4180 /wd4800 /wd4804")

# Disable language extensions.
add_definitions("/Za")

# Disable Microsoft's Secure STL stuff.
add_definitions("/D_SECURE_SCL=0")
add_definitions("/D_HAS_ITERATOR_DEBUGGING=0")

endif()

add_subdirectory(pastel)
add_subdirectory(test)
add_subdirectory(example)
