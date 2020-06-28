# Description: Main CMake build file for the Pastel library
# Documentation: building.txt

cmake_minimum_required (VERSION 3.18)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif()

project (Pastel)

# CMake build options
# -------------------

option (BuildLibraries "Build Pastel's main libraries." ON)
option (BuildMatlab "Build Pastel's Matlab-libraries." ON)
option (BuildTests "Build Pastel's tests." ON)
option (BuildExamples "Build Pastel's examples." ON)

# ECMake
# ------

# Directory for CMake to search for CMake-included() files.
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Initialize ECMake.
include ("ecmake")

# Setup compilers.
include ("SetupCompilers")

message(STATUS 
	"Current compiler is ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

add_definitions(
	-DPASTEL_ENABLE_ASSERTS
)

# Configure external libraries
# ----------------------------

# Manual override for external library paths.
include ("Paths")

# find_package("Ranges")
# include_directories (${RangesIncludeDirectory})
add_definitions(-DRANGES_NO_STD_FORWARD_DECLARATIONS)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_libraries(${Boost_LIBRARIES})

add_definitions(
	-DBOOST_BIND_GLOBAL_PLACEHOLDERS
)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
include_directories(${EIGEN3_INCLUDE_DIRS})
link_libraries(Eigen3::Eigen)

add_definitions (
	# std::result_of was removed in C++20.
	-DEIGEN_HAS_STD_RESULT_OF=0
)

find_package(TBB REQUIRED)
# include_directories (${TbbIncludeDirectory})

if (BuildMatlab)
	find_package("Matlab_")
	include_directories (${MatlabIncludeDirectory})
endif()

message(STATUS "Boost: ${Boost_INCLUDE_DIRS} ${Boost_LIBRARIES}")
message(STATUS "Eigen: ${EIGEN3_INCLUDE_DIRS}")

# Copy files
# ----------

# if (WIN32)
# 	# Copy dll-libraries aside executables for
# 	# Windows to find them, when running an
# 	# executable.

# 	set (DllPathSet 
# 		"${BlasDllPath}"
# 		"${LapackDllPath}")

# 	foreach (DllPath ${DllPathSet})
# 		EcCopyAsideExecutables("${DllPath}")
# 	endforeach()
# endif()

# # Copy some data files.
# file (COPY "${ProjectDirectory}/matlab/+pastelgeometry/fish.txt" 
# 	DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/matlab/+pastelgeometry")

# Source file globs
# -----------------

# The source files to include in a C++ project.
set (PastelSourceGlobSet *.cpp *.h *.hpp *.txt)

# The source files to include in a Matlab project.
set (PastelMatlabSourceGlobSet *.m *.m.cmake)

# The documentation files of the project.
set (PastelDocumentationGlobSet *.txt)

# Recurse to sub-projects
# -----------------------

add_subdirectory (pastel)

if (BuildTests)
	add_subdirectory (test)
endif()

if (BuildMatlab)
	add_subdirectory (matlab)
endif()

if (BuildExamples)
	add_subdirectory (example)
endif()

