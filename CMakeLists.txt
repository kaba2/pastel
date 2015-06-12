# Description: Main CMake build file for the Pastel library
# Documentation: building.txt

cmake_minimum_required (VERSION 2.8)

# Interpret relative paths w.r.t. the source directory in
# in link_directories().
cmake_policy(SET CMP0015 NEW)

project (Pastel)

# Find out whether the generator is 32-bit or 64-bit.
math(EXPR GENERATOR_BITS "8*${CMAKE_SIZEOF_VOID_P}")

# Paths
# -----

# An explicitly-specified non-empty library path
# will be used as it is --- provided it exists. 
# Otherwise the path is attempted to be detected 
# automatically. The latter is possible only on
# Linux and Mac OS X.

# There is no need to give a library path for Matlab;
# mex files are built from within Matlab.

set (MatlabVersion 2013b)

set (PastelDirectory ${CMAKE_SOURCE_DIR})
set (PastelIncludeDirectory ${PastelDirectory})
set (PastelLibraryDirectory ${PastelDirectory}/lib)

if (WIN32)
	# Under Windows, there are no standard
	# directories to store libraries, so
	# we must specify the directories
	# manually.

	set (BoostDirectory "C:/code/boost_1_58_0")

	set (ArmadilloDirectory "C:/code/armadillo-5.200.1")

	set (TbbDirectory "C:/code/tbb-4.3")
	set (TbbLibraryName "tbb")
	set (TbbLibraryNameDebug "tbb_debug")
	if (${GENERATOR_BITS} EQUAL 32)
		set (TbbLibraryDirectory ${TbbDirectory}/lib/ia32/vc12)
	else()
		set (TbbLibraryDirectory ${TbbDirectory}/lib/intel64/vc12)
	endif()


	set (BlasLibraryDirectory "${PastelDirectory}/pastel")
	set (BlasLibraryName "blas_win64_MT")

	set (LapackLibraryDirectory "${PastelDirectory}/pastel")
	set (LapackLibraryName "lapack_win64_MT")

	if (${GENERATOR_BITS} EQUAL 32)
		set (MatlabDirectory "C:/Program Files (x86)/MATLAB/R${MatlabVersion}")
	else()
		set (MatlabDirectory "C:/Program Files/MATLAB/R${MatlabVersion}")
	endif()
else(UNIX)
	# On Linux things are stored in standard places, so it 
	# is often possible to find libraries automatically; 
	# empty strings denote that.

	set (BoostDirectory "")

	set (ArmadilloDirectory "")

	set (TbbDirectory "~/code/tbb-4.3")
	set (TbbLibraryName "tbb")
	set (TbbLibraryNameDebug "tbb_debug")
	set (TbbLibraryDirectory ${TbbDirectory}/lib)

	set (BlasLibraryDirectory "")
	set (BlasLibraryName "")
	
	set (LapackLibraryDirectory "")
	set (LapackLibraryName "")

	set (MatlabDirectory "/usr/local/matlab/r${MatlabVersion}")
else(APPLE)
	# On Mac OS X things are stored in standard places, so it 
	# is often possible to find libraries automatically; 
	# empty strings denote that.

	set (BoostDirectory "")

	set (ArmadilloDirectory "")

	set (TbbDirectory "~/code/tbb-4.3")
	set (TbbLibraryName "tbb")
	set (TbbLibraryNameDebug "tbb_debug")
	set (TbbLibraryDirectory ${TbbDirectory}/lib/libc++)

	set (BlasLibraryDirectory "")
	set (BlasLibraryName "")

	set (LapackLibraryDirectory "")
	set (LapackLibraryName "")

	set (MatlabDirectory "/Applications/MATLAB_R${MatlabVersion}.app")
endif()

# Attempt to find missing paths automatically
# -------------------------------------------

# This often only works on Linux and Mac OS X.

if (${BoostDirectory} STREQUAL "" OR NOT EXISTS ${BoostDirectory})
	set (BoostVersion 1.58.0)
	find_package(Boost ${BoostVersion})
	if (BOOST_FOUND)
		set (BoostDirectory ${BOOST_INCLUDE_DIRS}/..)
	endif()
endif()

if (${ArmadilloDirectory} STREQUAL "" OR NOT EXISTS ${ArmadilloDirectory})
	find_package(Armadillo)
	if (ARMADILLO_FOUND)
		set (ArmadilloDirectory ${ARMADILLO_INCLUDE_DIRS}/..)
	endif()
endif()

if (${LapackLibraryDirectory} STREQUAL "" OR NOT EXISTS ${LapackLibraryDirectory})
	find_package(LAPACK)
	if (LAPACK_FOUND)
		set (LapackLibraryName LAPACK_LIBRARIES)
		get_filename_component(LapackLibraryDirectory ${LapackLibraryName} DIRECTORY)
	endif()
endif()

if (${BlasLibraryDirectory} STREQUAL "" OR NOT EXISTS ${BlasLibraryDirectory})
	find_package(BLAS)
	if (BLAS_FOUND)
		set (BlasLibraryName BLAS_LIBRARIES)
		get_filename_component(BlasLibraryDirectory ${BlasLibraryName} DIRECTORY)
	endif()
endif()

macro (CheckPathExists Name Directory)
	if (EXISTS ${Directory})
		message (STATUS "Using ${Name} from ${Directory}.")
	else()
		message (SEND_ERROR "Cannot find ${Name} (tried ${Directory}). Specify the path in Pastel's root CMakeLists.txt.")
		return()
	endif()
endmacro()

# Verify that the library paths exist.
# ------------------------------------

CheckPathExists(Boost ${BoostDirectory})
CheckPathExists(Armadillo ${ArmadilloDirectory})
CheckPathExists(Tbb ${TbbDirectory})
CheckPathExists("Tbb library" ${TbbLibraryDirectory})
CheckPathExists("Lapack library" ${LapackLibraryDirectory})
CheckPathExists("Blas library" ${BlasLibraryDirectory})

if (BuildMatlab)
	CheckPathExists(Matlab ${MatlabDirectory})
endif()

# Compute some additional directories.
# ------------------------------------

set (BoostIncludeDirectory ${BoostDirectory})
set (TbbIncludeDirectory ${TbbDirectory}/include)
set (ArmadilloIncludeDirectory ${ArmadilloDirectory}/include)
set (MatlabIncludeDirectory ${MatlabDirectory}/extern/include)

# Add the include directories.
# ----------------------------

include_directories (${PastelIncludeDirectory})
include_directories (${BoostIncludeDirectory})
include_directories (${TbbIncludeDirectory})
include_directories (${MatlabIncludeDirectory})
include_directories (${ArmadilloIncludeDirectory})

# Add the library directories.
# ----------------------------

link_directories (${TbbLibraryDirectory})
link_directories (${LapackLibraryDirectory})
link_directories (${BlasLibraryDirectory})

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
   		"out-of-source build instead (e.g. 'cmake ..' in 'release/' directory). "
   		"This call produced the file 'CMakeCache.txt' and the 'CMakeFiles' directory "
   		"in the Pastel's source directory. You must remove them for the out-of-source "
   		" build to work; otherwise CMake attempts an in-source build again."
	)

   return()
endif()

# Emit a warning about forcing release-mode libraries.
# ----------------------------------------------------

if (BuildMatlabMex)
	message (WARNING
		"BuildMatlabMex: Forcing release-mode C and C++ "
		"standard libraries. Use this only when the intent is "
		"to make Pastel libraries usable for Matlab mex.")
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

# The set of dll-libraries to copy into the 
# executable directory (Windows only).
set (DllSet "")

if (WIN32)
	set (DllSet
		${LapackLibraryName}.dll
		${BlasLibraryName}.dll)
endif()

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

    foreach (dllFile ${DllSet})
		file (COPY pastel/${dllFile} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_${UPPER_OUTPUTCONFIG}})
    endforeach()
endforeach()


# Pastel's global definitions
# ---------------------------

# Debug-mode definitions.
set (CMAKE_CXX_FLAGS_DEBUG 
	"${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG"
)

# Make Armadillo use C++11 (e.g. move constructors).
add_definitions(-DARMA_USE_CXX11)

# Make the special functions in Boost return infinity on overflow
# (rather than throw an exception).
add_definitions(-DBOOST_MATH_OVERFLOW_ERROR_POLICY=ignore_error)

# Clang and g++
# -------------

if ((${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU") OR 
	(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")) 
	add_definitions (
		# Enables C++14 compiler support.
		-std=c++1y 
		# Enables some additional warnings.
		-Wall 
		# Enables position-independent code.
		# This is needed to build the Matlab 
		# libraries.
		#-fPIC
		# Stop build after one error.
 		-Wfatal-errors
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
		#-Wno-unused-variable
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

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU") 
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

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang") 
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
		# Absolute value function on unsigned types.
		-Wno-absolute-value
		-Wno-unsupported-friend
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

	if (BuildMatlabMex)
		# Force Visual Studio to use release-mode C and C++ standard libraries.
		# This is needed for Matlab, because otherwise there will be LNK4098
		# about conflicting versions of the standard library.
		string(REPLACE "/MDd" "/MD" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
		string(REPLACE "/D_DEBUG" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
	endif()

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
		#  integral constant overflow
		/wd4307
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

	if (${Type} STREQUAL "library")
		add_library (${LibraryName} ${PastelLibraryType} ${SourceSet})
	else (${Type} STREQUAL "executable")
		add_executable (${LibraryName} ${SourceSet})
	else ()
		message (FATAL_ERROR "Unknown library type ${Type}.")
	endif()
endmacro()

# Configures a Pastel Matlab library.
macro (PastelAddMatlab)
	file (GLOB_RECURSE SourceSet RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${PastelMatlabSourceGlobSet})
	foreach(FilePath ${SourceSet})
		set (OutputFilePath ${CMAKE_SOURCE_DIR}/bin/matlab/${FilePath})
		set (Options "")

		get_filename_component(FileExtension ${FilePath} EXT)

		if (${FileExtension} STREQUAL ".m.cmake")
			string (REGEX REPLACE "(.*).cmake$" "\\1" OutputFilePath ${OutputFilePath})
		else()
			set (Options COPY_ONLY)
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

