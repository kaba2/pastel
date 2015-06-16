# Description: Main CMake build file for the Pastel library
# Documentation: building.txt

cmake_minimum_required (VERSION 2.8)

# Interpret relative paths w.r.t. the source directory in
# in link_directories().
cmake_policy(SET CMP0015 NEW)

project (Pastel)

# Find out whether the generator is 32-bit or 64-bit.
math(EXPR GENERATOR_BITS "8*${CMAKE_SIZEOF_VOID_P}")

# We use a tool-set id to separate the outputs of 
# different compilers to different directories.
# The tool-set id consists of a compiler-id and
# the bitness of the generator.
string (TOLOWER ${CMAKE_CXX_COMPILER_ID} CompilerId)
set (ToolSet ${CompilerId}${GENERATOR_BITS})

# CMake build options
# -------------------

option (BuildLibraries "Build Pastel's main libraries." ON)
option (BuildMatlab "Build Pastel's Matlab-libraries." ON)
option (BuildTests "Build Pastel's tests." ON)
option (BuildExamples "Build Pastel's examples." ON)
option (BuildMatlabMex
	"Make libraries usable for Matlab mex (force release-mode C and C++ standard libraries)." 
	OFF)

# Force to use an out-of-source build
# -----------------------------------

if ("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
	message (SEND_ERROR 
   		"Pastel does not allow in-source builds (e.g. 'cmake .'); you should do an "
   		"out-of-source build instead (e.g. 'cmake ..' in 'build_${ToolSet}/' directory). "
   		"This call produced the file 'CMakeCache.txt' and the 'CMakeFiles' directory "
   		"in the Pastel's source directory. You must remove them for the out-of-source "
   		" build to work; otherwise CMake attempts an in-source build again."
	)

   return()
endif()

# Emit a warning about forcing release-mode libraries
# ---------------------------------------------------

if (BuildMatlabMex)
	message (WARNING
		"BuildMatlabMex: Forcing release-mode C and C++ "
		"standard libraries. Use this only when the intent is "
		"to make Pastel libraries usable for Matlab mex.")
endif()

# Set a default build type if none was specified
# ----------------------------------------------

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

# Paths
# -----

set (PastelDirectory "${CMAKE_SOURCE_DIR}")
set (PastelIncludeDirectory "${PastelDirectory}")
set (PastelLibraryDirectory "${PastelDirectory}/lib/${ToolSet}")
set (PastelExecutableDirectory "${PastelDirectory}/bin/${ToolSet}")
set (PastelMatlabDirectory "${PastelExecutableDirectory}/matlab")

include_directories (${PastelIncludeDirectory})

# The set of dll-libraries to copy into the 
# executable directory (Windows only).
set (DllSet "")

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include ("Utilities")

find_package("Boost_")
find_package("Armadillo_")
find_package("Tbb_")
find_package("Blas_")
find_package("Lapack_")

if (BuildMatlab)
	find_package("Matlab_")
endif()

# Global variables for the build-script
# -------------------------------------

# By default, the libraries are built static.
set (PastelLibraryType STATIC)

# The source files to include in a C++ build.
set (PastelSourceGlobSet *.cpp *.h *.hpp *.txt)

# The source files to include in a Matlab build.
set (PastelMatlabSourceGlobSet *.m *.m.cmake)

# The documentation files of the project.
set (PastelDocumentationGlobSet *.txt)

# The Threading Building Blocks dynamic library is 
# linked to every executable.
set (ExecutableLibrarySet tbb)

string (TOLOWER "${CMAKE_BUILD_TYPE}" LOWER_CMAKE_BUILD_TYPE)

# The directory to place the static libraries (e.g. lib/msvc64/release).
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PastelLibraryDirectory}/${LOWER_CMAKE_BUILD_TYPE}")

# The directory to place the shared libraries (e.g. lib/msvc64/release).
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PastelLibraryDirectory}/${LOWER_CMAKE_BUILD_TYPE}")

# The directory to place the built executables (e.g. bin/msvc64/release).
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PastelExecutableDirectory}/${LOWER_CMAKE_BUILD_TYPE}")

if (WIN32 AND NOT MSVC)
	# On Windows under MinGW, copy the dll-libraries to 
	# where the executables are.
	CopyDllsTo("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif()

# This is for the multi-configuration build-scripts
# (such as Visual Studio and XCode).
foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string (TOUPPER ${OUTPUTCONFIG} UPPER_OUTPUTCONFIG)
    string (TOLOWER ${OUTPUTCONFIG} LOWER_OUTPUTCONFIG)

	# The library output directory is of the form "lib/msvc64/release".
    set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${UPPER_OUTPUTCONFIG} 
    	"${PastelLibraryDirectory}/${LOWER_OUTPUTCONFIG}")
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_${UPPER_OUTPUTCONFIG} 
    	"${PastelLibraryDirectory}/${LOWER_OUTPUTCONFIG}")

	# The executable output directory is of the form "bin/msvc64/release".
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_${UPPER_OUTPUTCONFIG} 
    	"${PastelExecutableDirectory}/${LOWER_OUTPUTCONFIG}")

    if (WIN32)
    	# On Windows under MSVC, copy the dll-libraries to 
    	# where the executables are.
    	CopyDllsTo("${CMAKE_RUNTIME_OUTPUT_DIRECTORY_${UPPER_OUTPUTCONFIG}}")
	endif()
endforeach()

if (WIN32)
	# On Windows, copy the dll-libraries to where
	# the Matlab interface is.
	CopyDllsTo("${PastelMatlabDirectory}")
endif()

# Set compiler options.
include ("SetupCompilers")

# Adds a library, or an executable, and creates source-groups based on 
# the physical directory tree.

macro (PastelAdd Type LibraryName)
	file (GLOB_RECURSE SourceSet ${PastelSourceGlobSet})

	foreach (FilePath ${SourceSet})
		# Get the path to the source file, relative to the current directory.
	    file (RELATIVE_PATH FileRelativePath ${CMAKE_CURRENT_LIST_DIR} ${FilePath})

	    # Append / to the beginning, so that the regex-replacement
	    # works also in the current directory.
	    set (FileRelativePath "/${FileRelativePath}")

	    # Get the directory-part of the path.
	    # I could not find a way for specifying a non-capturing group, 
	    # so I opted to append the / to the beginning, and then do
	    # the following.
	    string (REGEX REPLACE "(.*/)[^/]*$" "\\1" DirectoryRelativePath ${FileRelativePath})

	    # Replace / with \.
	    string (REPLACE "/" "\\" SourceGroupName ${DirectoryRelativePath})

	    #message (STATUS ${FileRelativePath})
	    #message (STATUS ${DirectoryRelativePath})
	    #message (STATUS ${SourceGroupName})

	    # Create a source group.
	    source_group(${SourceGroupName} FILES ${FilePath})
	endforeach()

	#message (STATUS "${LibraryName} is ${Type}" )

	if ("${Type}" STREQUAL "library")
		add_library (${LibraryName} ${PastelLibraryType} ${SourceSet})
	else ("${Type}" STREQUAL "executable")
		add_executable (${LibraryName} ${SourceSet})
	else ()
		message (FATAL_ERROR "Unknown library type ${Type}.")
	endif()
endmacro()

# Configures a Pastel Matlab library.
macro (PastelAddMatlab)
	file (GLOB_RECURSE SourceSet RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${PastelMatlabSourceGlobSet})
	foreach(FilePath ${SourceSet})
		set (OutputFilePath ${PastelMatlabDirectory}/${FilePath})
		set (Options "")

		get_filename_component(FileExtension ${FilePath} EXT)

		if (${FileExtension} STREQUAL ".m.cmake")
			string (REGEX REPLACE "(.*).cmake$" "\\1" OutputFilePath ${OutputFilePath})
		else()
			set (Options COPYONLY)
		endif()
		
		configure_file(${FilePath} ${OutputFilePath} ${Options})
		#message (STATUS "Configured ${FilePath} to ${OutputFilePath}.")
		endforeach()
endmacro()

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

