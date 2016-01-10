# Description: Main CMake build file for the Pastel library
# Documentation: building.txt

cmake_minimum_required (VERSION 2.8)

project (Pastel)

# Directory for CMake to search for CMake-included() files.
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Initialize the generic build system.
include ("Initialize")

# CMake build options
# -------------------

option (BuildLibraries "Build Pastel's main libraries." ON)
option (BuildMatlab "Build Pastel's Matlab-libraries." ON)
option (BuildTests "Build Pastel's tests." ON)
option (BuildExamples "Build Pastel's examples." ON)

# Configure external libraries
# ----------------------------

include_directories (${ProjectIncludeDirectory})

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
		set (OutputFilePath ${ProjectMatlabDirectory}/${FilePath})
		set (Options "")

		get_filename_component(FileExtension ${FilePath} EXT)

		if (${FileExtension} MATCHES "template.(.+)$")
			string (REGEX REPLACE "(.+).template.(.+)$" "\\1.\\2" OutputFilePath ${OutputFilePath})
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

