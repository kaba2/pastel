# Interpret relative paths w.r.t. the source directory in
# in link_directories().
cmake_policy(SET CMP0015 NEW)

# Find out whether the generator is 32-bit or 64-bit.
math(EXPR GENERATOR_BITS "8*${CMAKE_SIZEOF_VOID_P}")

# We use a tool-set id to separate the outputs of 
# different compilers to different directories.
# The tool-set id consists of a compiler-id and
# the bitness of the generator.
string (TOLOWER ${CMAKE_CXX_COMPILER_ID} CompilerId)
set (ToolSet ${CompilerId}${GENERATOR_BITS})

# Force to use an out-of-source build
# -----------------------------------

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
	# This is an in-source build. Report an error.
	message (SEND_ERROR 
   		"${CMAKE_PROJECT_NAME} does not allow in-source builds (e.g. 'cmake .'); you should do an "
   		"out-of-source build instead (e.g. 'cmake ..' in 'build_${ToolSet}/' directory). "
   		"This call produced the file 'CMakeCache.txt' and the 'CMakeFiles' directory "
   		"in the ${CMAKE_PROJECT_NAME}'s source directory. You must remove them for the out-of-source "
   		" build to work; otherwise CMake attempts an in-source build again."
	)

   return()
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

option (BuildMatlabMex
	"Make libraries usable for Matlab mex (force release-mode C and C++ standard libraries)." 
	ON)

# The set of dll-libraries to copy into the 
# executable directory (Windows only).
set (DllSet "")

# Define output directories
# -------------------------

set (ProjectDirectory "${CMAKE_SOURCE_DIR}")
set (ProjectIncludeDirectory "${ProjectDirectory}")
set (ProjectLibraryDirectory "${ProjectDirectory}/lib/${ToolSet}")
set (ProjectExecutableDirectory "${ProjectDirectory}/bin/${ToolSet}")
set (ProjectMatlabDirectory "${ProjectExecutableDirectory}/matlab")

include_directories (${ProjectIncludeDirectory})

# Set output directories
# ----------------------

string (TOLOWER "${CMAKE_BUILD_TYPE}" LOWER_CMAKE_BUILD_TYPE)

# The directory to place the static libraries (e.g. lib/msvc64/release).
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${ProjectLibraryDirectory}/${LOWER_CMAKE_BUILD_TYPE}")

# The directory to place the shared libraries (e.g. lib/msvc64/release).
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${ProjectLibraryDirectory}/${LOWER_CMAKE_BUILD_TYPE}")

# The directory to place the built executables (e.g. bin/msvc64/release).
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${ProjectExecutableDirectory}/${LOWER_CMAKE_BUILD_TYPE}")

# This is for the multi-configuration build-scripts
# (such as Visual Studio and XCode).
foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string (TOUPPER ${OUTPUTCONFIG} UPPER_OUTPUTCONFIG)
    string (TOLOWER ${OUTPUTCONFIG} LOWER_OUTPUTCONFIG)

	# The library output directory is of the form "lib/msvc64/release".
    set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${UPPER_OUTPUTCONFIG} 
    	"${ProjectLibraryDirectory}/${LOWER_OUTPUTCONFIG}")
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_${UPPER_OUTPUTCONFIG} 
    	"${ProjectLibraryDirectory}/${LOWER_OUTPUTCONFIG}")

	# The executable output directory is of the form "bin/msvc64/release".
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_${UPPER_OUTPUTCONFIG} 
    	"${ProjectExecutableDirectory}/${LOWER_OUTPUTCONFIG}")
endforeach()

# Set compiler options
# --------------------

include ("SetupCompilers")

# Helper macros
# -------------

macro(CheckPathExists Name PathSet)
	foreach(Path ${PathSet})
		if(EXISTS ${Path})
			message (STATUS "${Name}: ${Path}")
		else()
			set (Tried "")
			if (NOT ("${Path}" STREQUAL ""))
				set (Tried " (tried ${Path})")
			endif()
			message (SEND_ERROR "Cannot find ${Name}${Tried}. Either install ${Name}, or correct the path in Pastel's root CMakeLists.txt.")
			return()
		endif()
	endforeach()
endmacro()

macro (CopyDllsTo Directory)
    foreach (dllPath ${DllSet})
		file (COPY "${dllPath}" DESTINATION ${Directory})
    endforeach()
endmacro()
