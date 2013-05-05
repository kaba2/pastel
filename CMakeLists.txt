# Description: Main CMake build file for the Pastel library
# Documentation: building.txt

# Pastel build script
# ===================

cmake_minimum_required (VERSION 2.8)

project (PastelSolution)

# Paths
# -----

# Change the following directories to reflect your own
# build environment. If a library is not used, the path
# is ignored.

# The directory of the Boost library's source code.
# The includes are of the form 'boost/static_assert.hpp'.
set (BoostIncludeDirectory ../boost_1_53_0)

# The directory of the Matlab header files.
# The includes are of the form 'mex.h'.
set (MatlabIncludeDirectory /Applications/MATLAB_R2013a.app/extern/include)

# Note: To succesfully _compile_ Pastel's Matlab-libraries, 
# you only need the header files for Matlab. 
# This allows you to try the compilation
# even if you do not have the external library
# binaries. It is only in the _linking_ phase of 
# executables and shared libraries (i.e. tests and 
# examples) where the binaries are needed .

# No need to give a library path for Matlab:
# Mex files are built from within Matlab.

# For Pastel, the include are of the form 'pastel/sys/mytypes.h'.
include_directories (.)
include_directories (${BoostIncludeDirectory})
include_directories (${MatlabIncludeDirectory})

# CMake build options
# -------------------

option (BuildLibraries "Build Pastel's main libraries." ON)
option (BuildMatlab "Build Pastel's Matlab-libraries." OFF)
option (BuildTests "Build Pastel's tests." OFF)
option (BuildExamples "Build Pastel's examples." OFF)
option (SharedLibraries "Produce shared libraries." OFF)
# Whether the 'integer'-type should be as large as a pointer (vs 32-bit).
option (LargeIntegers "Use pointer-sized integers." OFF)

# Global variables for the build-script
# -------------------------------------

# By default, the libraries are built static.
set (PastelLibraryType STATIC)

# The source files to include in the build.
set (PastelSourceGlobSet *.cpp *.h *.hpp)

# Pastel's global definitions
# ---------------------------

# Enable shared libraries if requested
if (SharedLibraries)
	add_definitions(
		-DPASTEL_DYNAMIC_LIBRARIES
	)	
	set (PastelLibraryType SHARED)
endif()
	
# Enable large integers if requested
if (LargeIntegers)
	add_definitions(
		-DPASTEL_LARGE_INTEGERS
	)
endif()

# Enable ASSERTs and PENSUREs in debug mode.
set_property (GLOBAL APPEND PROPERTY 
	COMPILE_DEFINITIONS_DEBUG
	PASTEL_DEBUG_MODE
	PASTEL_ENABLE_PENSURES)

# Visual Studio
# -------------

if ("${CMAKE_COMPILER_ID}" STREQUAL "MSVC")
	add_definitions (
		# Disable Microsoft's Secure STL
		_ITERATOR_DEBUG_LEVEL=0
	)
endif()

# clang and g++
# -------------

# Do-it-yourself clang detection
# Under Mac Os X the clang compiler is aliased to c++
# (or something like that; I don't know why cmake
# finds my compiler as /usr/bin/c++).
if (CMAKE_CXX_COMPILER MATCHES "(.*clang)|(.*c\\+\\+)")
	set (CMAKE_COMPILER_IS_CLANGXX 1)
	set (CMAKE_COMPILER_ID Clang)
endif ()

if (CMAKE_COMPILER_IS_GNUGXX OR CMAKE_COMPILER_IS_CLANGXX) 
	# Same flags apply to both g++ and clang.

	add_definitions (
		# Enables C++11 compiler support.
		-std=c++11 
		-stdlib=libc++
		# Enables some additional warnings.
		-Wall 
	)

	# Enables C++11 linker support.
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")

	# Disable some warnings.
	add_definitions (
		# Pragma warnings caused by OpenMP support not being enabled.
		-Wno-unknown-pragmas
		# Comparison between an unsigned and a signed integer.
		-Wno-sign-compare
		# Conversion between an unsigned and a signed integer.
		-Wno-sign-conversion
		# Unused variables.
		-Wno-unused-variable
		# Unused values.
		-Wno-unused-value
		# Unused but set variable.
		# Commented out; Clang does not recognize this.
		# "-Wno-unused-but-set-variable",
		# Unused functions.
		-Wno-unused-function
		# Breaking strict aliasing rules.
		-Wno-strict-aliasing
		# Compiler warns that it optimizes code based on the 
		# assumption that signed integer overflows do not occur.
		-Wno-strict-overflow
		# Compiler warns 'that >= 0' is always true for an 
		# unsigned integer.
		-Wno-tautological-compare
	)
endif()

# Recurse to sub-projects
# -----------------------

add_subdirectory (pastel)

if (BuildTests)
	add_subdirectory (test)
endif()

if (BuildExamples)
	#add_subdirectory (example)
endif()

