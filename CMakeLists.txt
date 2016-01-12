# Description: Main CMake build file for the Pastel library
# Documentation: building.txt

cmake_minimum_required (VERSION 2.8)

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

# Configure external libraries
# ----------------------------

# Manual override for external library paths.
include ("Paths")

find_package("Boost_")
include_directories (${BoostIncludeDirectory})

find_package("Armadillo_")
include_directories (${ArmadilloIncludeDirectory})

find_package("Tbb_")
include_directories (${TbbIncludeDirectory})

find_package("Blas_")
find_package("Lapack_")

if (BuildMatlab)
	find_package("Matlab_")
	include_directories (${MatlabIncludeDirectory})
endif()

# Copy files
# ----------

if (WIN32)
	# Copy dll-libraries aside executables for
	# Windows to find them, when running an
	# executable.

	set (DllPathSet 
		"${BlasDllPath}"
		"${LapackDllPath}")

	foreach (DllPath ${DllPathSet})
		EcCopyAsideExecutables("${DllPath}")
	endforeach()
endif()

# Copy some data files.
file (COPY "${ProjectDirectory}/matlab/+pastelgeometry/fish.txt" 
	DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/matlab/+pastelgeometry")

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

