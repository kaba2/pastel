# Interpret relative paths w.r.t. the source directory in
# in link_directories().
cmake_policy(SET CMP0015 NEW)

option (BuildMatlabMex
	"Make libraries usable for Matlab mex (force release-mode C and C++ standard libraries)." 
	ON)

# Form tool-set string to differentiate builds
# --------------------------------------------

# Find out whether the generator is 32-bit or 64-bit.
math(EXPR GENERATOR_BITS "8*${CMAKE_SIZEOF_VOID_P}")

# Find out the compiler-id in lower-case.
# For example: msvc, gnu, clang
string (TOLOWER ${CMAKE_CXX_COMPILER_ID} CompilerId)

# We use a tool-set id to separate the outputs of 
# different compilers to different directories.
# The tool-set id consists of a compiler-id and
# the bitness of the generator. For example:
# msvc64: Visual Studio, 64 bits
# gnu32: GCC, 32 bits,
# clang64: Clang, 64 bits
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

# The set of dll-libraries to copy into the 
# executable directory (Windows only).
set (DllSet "")

macro (CopyDllsTo Directory)
    foreach (dllPath ${DllSet})
		file (COPY "${dllPath}" DESTINATION ${Directory})
    endforeach()
endmacro()

# Adds a library, or an executable, and creates source-groups based on 
# the physical directory tree.
macro (EcAddLibrary Type LibraryName SourceGlobSet)
	file (GLOB_RECURSE SourceSet ${SourceGlobSet})

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
		add_library (${LibraryName} STATIC ${SourceSet})
	else ("${Type}" STREQUAL "executable")
		add_executable (${LibraryName} ${SourceSet})
	else ()
		message (FATAL_ERROR "Unknown library type ${Type}.")
	endif()
endmacro()

# Configures a Pastel Matlab library.
macro (EcAddMatlabLibrary SourceGlobSet)
	file (GLOB_RECURSE SourceSet RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SourceGlobSet})
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
