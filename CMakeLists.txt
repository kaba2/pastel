# This is a CMake build file for all of Pastel, 
# including the library, tests, and examples.

cmake_minimum_required(VERSION 2.8)

project(PastelAll)

# These directories you must set in any case,
# since there are no Find modules for them:

set(EXTERNAL_DIR "E:/Ohjelmointi/external")

set(MANUAL_GLEW_INCLUDE_DIR ${EXTERNAL_DIR}/glew/include)

# These directories need to be set only in case
# automatic deduction fails.

set(MANUAL_SDL_INCLUDE_DIR ${EXTERNAL_DIR}/SDL-1.2.13/include)
set(MANUAL_BOOST_INCLUDE_DIR ${EXTERNAL_DIR}/boost_1_43_0)

set(BOOST_INCLUDEDIR ${MANUAL_BOOST_INCLUDE_DIR})

find_package(SDL 1.2 REQUIRED)
if(SDL_FOUND)
	message("SDL found.")
else()
	message("SDL not found.")
endif()

find_package(Boost 1.43 REQUIRED)
if(Boost_FOUND)
	message("Boost found.")
else()
	message("Boost not found.")
	set(Boost_INCLUDE_DIRS ${MANUAL_BOOST_INCLUDE_DIR})
endif()

if(SDL_INCLUDE_DIR MATCHES SDL_INCLUDE_DIR-NOTFOUND)
	# The FindSDL module may say it found SDL,
	# but still not fill the SDL_INCLUDE_DIR.
	set(SDL_INCLUDE_DIR ${MANUAL_SDL_INCLUDE_DIR})
endif()

set(GLEW_INCLUDE_DIR ${MANUAL_GLEW_INCLUDE_DIR})

include_directories(${SDL_INCLUDE_DIR}) 
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIR})

link_directories(${EXTERNAL_DIR}/lib)

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
