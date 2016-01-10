# Description: Main CMake build file for the Pastel library
# Documentation: building.txt

cmake_minimum_required (VERSION 2.8)

project (Pastel)

# Directory for CMake to search for CMake-included() files.
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Initialize ECMake
include ("ecmake")

# Global variables for the build-script
# -------------------------------------

# The source files to include in a C++ build.
set (PastelSourceGlobSet *.cpp *.h *.hpp *.txt)

# The source files to include in a Matlab build.
set (PastelMatlabSourceGlobSet *.m *.m.cmake)

# The documentation files of the project.
set (PastelDocumentationGlobSet *.txt)

# The Threading Building Blocks dynamic library is 
# linked to every executable.
set (ExecutableLibrarySet tbb)

# CMake build options
# -------------------

option (BuildLibraries "Build Pastel's main libraries." ON)
option (BuildMatlab "Build Pastel's Matlab-libraries." ON)
option (BuildTests "Build Pastel's tests." ON)
option (BuildExamples "Build Pastel's examples." ON)

# Configure external libraries
# ----------------------------

# Manual override for paths.
include ("Paths")

find_package("Boost_")
find_package("Armadillo_")
find_package("Tbb_")
find_package("Blas_")
find_package("Lapack_")

if (BuildMatlab)
	find_package("Matlab_")
endif()

# Copy dll-libraries
# ------------------

if (WIN32 AND ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
	# On Windows under MinGW, copy the dll-libraries to 
	# where the executables are.
	CopyDllsTo("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif()

# This is for the multi-configuration build-scripts
# (such as Visual Studio and XCode).
foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string (TOUPPER ${OUTPUTCONFIG} UPPER_OUTPUTCONFIG)
    string (TOLOWER ${OUTPUTCONFIG} LOWER_OUTPUTCONFIG)

    if (WIN32)
    	# On Windows under MSVC, copy the dll-libraries to 
    	# where the executables are.
    	CopyDllsTo("${CMAKE_RUNTIME_OUTPUT_DIRECTORY_${UPPER_OUTPUTCONFIG}}")
	endif()
endforeach()

if (WIN32)
	# On Windows, copy the dll-libraries to where
	# the Matlab interface is.
	CopyDllsTo("${ProjectMatlabDirectory}")
endif()

# Copy some data files.
file (COPY "${ProjectDirectory}/matlab/+pastelgeometry/fish.txt" 
	DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/matlab/+pastelgeometry")

# Recurse to sub-projects
# -----------------------

add_subdirectory (pastel)

if (BuildMatlab)
	add_subdirectory (matlab)
endif()

if (BuildTests)
	add_subdirectory (test)
endif()

if (BuildExamples)
	add_subdirectory (example)
endif()

# Solution folders
# ----------------

set_property( GLOBAL PROPERTY USE_FOLDERS ON)

if (BuildTests)
	set (TestSet 
		pastelsystest 
		pastelmathtest 
		pastelgeometrytest 
		pastelgfxtest)
	set_property( TARGET ${TestSet} PROPERTY FOLDER "Tests")
endif()

if (BuildLibraries)
	set (LibrarySet 
		pastelsys 
		pastelmath 
		pastelgeometry 
		pastelgfx
		pasteldoc)
	set_property( TARGET ${LibrarySet} PROPERTY FOLDER "Libraries")
endif()

if (BuildMatlab)
	set (MatlabLibrarySet
		pastelmatlab
		pastelsysmatlab
		pastelmathmatlab
		pastelgeometrymatlab)
	set_property( TARGET ${MatlabLibrarySet} PROPERTY FOLDER "Matlab Libraries")
endif()

if (BuildExamples)
	set (ExampleSet)
	set_property( TARGET ${ExampleSet} PROPERTY FOLDER "Examples")
endif()

