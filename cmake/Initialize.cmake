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

# Set compiler options.
include ("SetupCompilers")
