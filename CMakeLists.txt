# Description: Main CMake build file for the Pastel library
# Documentation: building.txt

cmake_minimum_required (VERSION 2.8)

# Interpret relative paths w.r.t. the source directory in
# in link_directories().
cmake_policy(SET CMP0015 NEW)

project (Pastel)

# Paths
# -----

# Change the following directories to reflect your own
# build environment. If a library is not used, the path
# is ignored.

# The directory of the Boost library's source code.
# The includes are of the form 'boost/static_assert.hpp'.
set (BoostIncludeDirectory ../boost_1_56_0)

# The directory of the Threading Building Blocks library's 
# source code. The includes are of the form 'tbb/parallel_for.h'.
set (TbbIncludeDirectory ../tbb42/include)

# The directory of the Threading Building Blocks library's 
# dynamic libraries.
#set (TbbLibraryDirectory ../tbb42/lib/libc++)
set (TbbLibraryDirectory ../tbb42/lib)

# The directory of the Matlab header files.
# The includes are of the form 'mex.h'.
set (MatlabIncludeDirectory "C:/Program Files/MATLAB/R2013a/extern/include")
#set (MatlabIncludeDirectory /Applications/MATLAB_R2013a.app/extern/include)

# Note: To successfully _compile_ Pastel's Matlab-libraries, 
# you only need the header files for Matlab. 
# This allows you to try the compilation
# even if you do not have the external library
# binaries. It is only in the _linking_ phase of 
# executables and shared libraries (i.e. tests and 
# examples) where the binaries are needed .

# No need to give a library path for Matlab:
# Mex files are built from within Matlab.

# For Pastel, the includes are of the form 'pastel/sys/mytypes.h'.
include_directories (.)
include_directories (${BoostIncludeDirectory})
include_directories (${TbbIncludeDirectory})
include_directories (${MatlabIncludeDirectory})

link_directories (${TbbLibraryDirectory})

# CMake build options
# -------------------

option (BuildLibraries "Build Pastel's main libraries." ON)
option (BuildMatlab "Build Pastel's Matlab-libraries." ON)
option (BuildTests "Build Pastel's tests." ON)
option (BuildExamples "Build Pastel's examples." ON)

# Force to use an out-of-source build
# -----------------------------------

if ("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
	message (SEND_ERROR 
   		"Pastel does not allow in-source builds (e.g. 'cmake .'); you should do an "
   		"out-of-source build instead (e.g. 'cmake ..' in 'release/' directory). "
   		"This call produced the file 'CMakeCache.txt' and the 'CMakeFiles' directory "
   		"in the Pastel's source directory. You must remove them for the out-of-source "
   		" build to work; otherwise CMake attempts an in-source build again."
	)

   return()
endif()

# Global variables for the build-script
# -------------------------------------

# By default, the libraries are built static.
set (PastelLibraryType STATIC)

# The source files to include in the build.
set (PastelSourceGlobSet *.cpp *.h *.hpp *.txt)

# The documentation files of the project.
set (PastelDocumentationGlobSet *.txt)

# The Threading Building Blocks dynamic library is 
# linked to every executable.
set (ExecutableLibrarySet tbb)

# Set a default build type if none was specified.
# This only applies to single-configuration tool-sets,
# such as Unix Makefiles.
if (NOT CMAKE_BUILD_TYPE AND 
	NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS 
		"Setting build type to 'Release' as none was specified.")
	set(CMAKE_BUILD_TYPE Release CACHE STRING 
		"Choose the type of build." FORCE)
	# Set the possible values of build types for cmake-gui.
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS 
 		"Debug" 
  		"Release")
endif()

string (TOLOWER "${CMAKE_BUILD_TYPE}" LOWER_CMAKE_BUILD_TYPE)

# Find out whether the generator is 32-bit or 64-bit.
math(EXPR GENERATOR_BITS "8*${CMAKE_SIZEOF_VOID_P}")

# The directory to place the static libraries.
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY 
	${CMAKE_SOURCE_DIR}/lib/${GENERATOR_BITS}/${LOWER_CMAKE_BUILD_TYPE})

# The directory to place the shared libraries.
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY 
	${CMAKE_SOURCE_DIR}/lib/${GENERATOR_BITS}/${LOWER_CMAKE_BUILD_TYPE})

# The directory to place the built executables.
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY 
	${CMAKE_SOURCE_DIR}/bin/${GENERATOR_BITS}/${LOWER_CMAKE_BUILD_TYPE})

# This is for the multi-configuration build-scripts
# (such as Visual Studio or XCode).
foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string (TOUPPER ${OUTPUTCONFIG} UPPER_OUTPUTCONFIG)
    string (TOLOWER ${OUTPUTCONFIG} LOWER_OUTPUTCONFIG)
    set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${UPPER_OUTPUTCONFIG} 
    	${CMAKE_SOURCE_DIR}/lib/${GENERATOR_BITS}/${LOWER_OUTPUTCONFIG})
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_${UPPER_OUTPUTCONFIG} 
    	${CMAKE_SOURCE_DIR}/lib/${GENERATOR_BITS}/${LOWER_OUTPUTCONFIG})
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_${UPPER_OUTPUTCONFIG} 
    	${CMAKE_SOURCE_DIR}/bin/${GENERATOR_BITS}/${LOWER_OUTPUTCONFIG})
endforeach()

# Pastel's global definitions
# ---------------------------

# Debug-mode definitions.
set (CMAKE_CXX_FLAGS_DEBUG 
	"${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -D_DEBUG"
)

# Clang and g++
# -------------

if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") OR 
	("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")) 
	add_definitions (
		# Enables C++14 compiler support.
		-std=c++1y 
		# Enables some additional warnings.
		-Wall 
		# Enables position-independent code.
		# This is needed to build the Matlab 
		# libraries.
		-fPIC
	)

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
		# Unused functions.
		-Wno-unused-function
		# Breaking strict aliasing rules.
		-Wno-strict-aliasing
		# Compiler warns that it optimizes code based on the 
		# assumption that signed integer overflows do not occur.
		-Wno-strict-overflow
	)
endif()

# g++
# ---

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") 
	# Disable some warnings.
	add_definitions (
		# Unused but set variable.
		-Wno-unused-but-set-variable
		# Unused local typedef
		-Wno-unused-local-typedefs
	)
endif()

# Clang
# -----

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") 
	add_definitions (
		# Enables C++14 library support.
		-stdlib=libc++
		# Not sure why.
		-arch x86_64
	)

	# Enables C++11 linker support.
	set (CMAKE_EXE_LINKER_FLAGS 
		"${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
	set (CMAKE_SHARED_LINKER_FLAGS 
		"${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")

	# Disable some warnings.
	add_definitions (
		# Compiler warns 'that >= 0' is always true for an 
		# unsigned integer.
		-Wno-tautological-compare
	)
endif()

# Visual Studio
# -------------

if (MSVC)
	# Do not add ZERO_CHECK project into the Visual Studio solution.
	# From VS2008 to VS2013, the ZERO_CHECK project is always out
	# of date, which causes the Visual Studio to ask at every build
	# whether ZERO_CHECK should be built, although nothing was changed.
	# The purpose of the ZERO_CHECK project is to check whether there
	# are changes to the CMake files themselves, and to regenerate the
	# project files if so. But even then the projects are regenerated
	# during the build, and they need to be reloaded, and that is not 
	# very smooth. It is better to suppress this feature and to 
	# regenerate the project files manually whenever the CMake files
	# are changed.
	set(CMAKE_SUPPRESS_REGENERATION TRUE)

	add_definitions (
		# Disable Microsoft's Secure STL.
		/D_ITERATOR_DEBUG_LEVEL=0
		# Use multiple processes for compiling.
		/MP
	)

	# Disable some warnings.
	add_definitions (
		# "'expression' : signed/unsigned mismatch"
		/wd4018 
   		# unary minus operator applied to unsigned type, result still unsigned
   		/wd4146
		# "qualifier applied to function type has no meaning; ignored"
		/wd4180
		# "nonstandard extension used : formal parameter 'identifier' was previously defined as a type"
		/wd4224
		# "'argument' : conversion from 'type1' to 'type2', possible loss of data."
		/wd4244
		# "'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'"
		/wd4251 
		# "'var' : conversion from 'size_t' to 'type', possible loss of data"
		/wd4267
		# "'identifier' : truncation from 'type1' to 'type2'"
		/wd4305
		# "'operation' : conversion from 'type1' to 'type2' of greater size"
		/wd4312
		# "new behavior: elements of array 'array' will be default initialized"
		/wd4351
		# declaration of 'variable' hides previous local declaration
		/wd4456
		# declaration of 'variable' hides function parameter
		/wd4457
		# declaration of 'type' hides class member
		/wd4458
		# declaration of 'type' hides global declaration
		/wd4459
		# 'type' : base-class 'type2' is already a base-class of 'type3'
		/wd4584
		# "'type' : forcing value to bool 'true' or 'false' (performance warning)"
		/wd4800 
		# "'operation' : unsafe use of type 'bool' in operation"
   		/wd4804
		# "'function': was declared deprecated" (referring to STL functions)
		/wd4996
	)

endif()

# Recurse to sub-projects
# -----------------------

add_subdirectory (pastel)

if (BuildTests)
	add_subdirectory (test)
endif()

if (BuildExamples)
	add_subdirectory (example)
endif()

# Solution folders
# ----------------

set (TestSet 
	pastelsystest 
	pastelmathtest 
	pastelgeometrytest 
	pastelgfxtest)

set (LibrarySet 
	pastelsys 
	pastelmath 
	pastelgeometry 
	pastelgfx
	pasteldoc)

set (MatlabLibrarySet
	pastelmatlab
	pastelsysmatlab
	pastelmathmatlab
	pastelgeometrymatlab)

set (ExampleSet)

set_property( GLOBAL PROPERTY USE_FOLDERS ON)

if (BuildTests)
	set_property( TARGET ${TestSet} PROPERTY FOLDER "Tests")
endif()

if (BuildLibraries)
	set_property( TARGET ${LibrarySet} PROPERTY FOLDER "Libraries")
endif()

if (BuildMatlab)
	set_property( TARGET ${MatlabLibrarySet} PROPERTY FOLDER "Matlab Libraries")
endif()

if (BuildExamples)
	set_property( TARGET ${ExampleSet} PROPERTY FOLDER "Examples")
endif()

