# Description: Premake build files for the Pastel library
# Documentation: building.txt

cmake_minimum_required (VERSION 2.8)

project (PastelSolution)

# Available external libraries
# ============================

# Whether you have Matlab include files 
# (say, for 2013a or newer). Note: Binaries are not
# needed since the linking is done from Matlab.
set (GotMatlab 1)

# Whether you have Boost 1.53 include files.
set (GotBoost 1)

# Note: To succesfully _compile_ the libraries, 
# you only need the header files for the external
# libraries. This allows you to try the compilation
# even if you do not have the external library
# binaries. It is only in the _linking_ phase of 
# executables and shared libraries (i.e. tests and 
# examples) where the binaries are needed .

# Main build switches
# ===================

# Whether to build the libraries.
set (BuildLibraries 1)

# Whether to build the test projects.
set (BuildTests 1)

# Whether to build the example projects.
set (BuildExamples 0)

# Whether to build dynamic libraries (vs static libraries)
set (WantDynamicLibraries 0)

# Whether the 'integer'-type should be as large as a pointer (vs 32-bit).
set (WantLargeIntegers 0)

# Paths
# =====

# Change the following directories to reflect your own
# build environment. If a library is not used, the path
# is ignored.

# The directory of the Boost library's source code.
# The includes are of the form 'boost/static_assert.hpp'.
set (BoostIncludeDirectory ../boost_1_53_0)

# The directory of the Matlab header files.
# The includes are of the form 'mex.h'.
set (MatlabIncludeDirectory /Applications/MATLAB_R2013a.app/extern/include)

# No need to give a library path for Matlab:
# Mex files are built from within Matlab.

# Build script
# ============

# The source files to include in the build.
set (PastelSourceGlobSet *.cpp *.h *.hpp)

include_directories (.)
include_directories (${BoostIncludeDirectory})
include_directories (${MatlabIncludeDirectory})

# Do-it-yourself clang-detection.
# Under Mac Os X the clang compiler is aliased to c++
# (or something like that; I don't know why cmake
# finds my compiler as /usr/bin/c++).
if (CMAKE_CXX_COMPILER MATCHES "(.*clang)|(.*c\\+\\+)")
	set (CMAKE_COMPILER_IS_CLANGXX 1)
	set (CMAKE_COMPILER_ID Clang)
endif ()

if ("${CMAKE_COMPILER_ID}" STREQUAL "MSVC")
	add_definitions (
		# Disable Microsoft's Secure STL
		_ITERATOR_DEBUG_LEVEL=0
	)
endif()

# Enable ASSERTs and PENSUREs in debug mode.
set_property (GLOBAL APPEND PROPERTY 
	COMPILE_DEFINITIONS_DEBUG
	PASTEL_DEBUG_MODE
	PASTEL_ENABLE_PENSURES)

if (CMAKE_COMPILER_IS_GNUGXX OR CMAKE_COMPILER_IS_CLANGXX) 
	# Same flags apply to both g++ and clang.

	add_definitions (
		# Enables C++11 compiler support.
		-std=c++11 
		-stdlib=libc++
		# Enables some additional warnings.
		-Wall 
	)

	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")

	set_property (GLOBAL APPEND PROPERTY 
		LINK_FLAGS
		# Enables C++11 linker support.
		-stdlib=libc++
	)

	#get_property (Jorma GLOBAL PROPERTY LINK_FLAGS)
	#message (STATUS ${Jorma})

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

if (BuildLibraries)
	add_subdirectory (pastel)
endif()

if (BuildTests)
	add_subdirectory (test)
endif()

if (BuildExamples)
	#add_subdirectory (example)
endif()

