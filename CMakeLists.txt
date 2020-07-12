# Description: Main CMake build file for the Pastel library
# Documentation: building.txt

cmake_minimum_required (VERSION 3.18)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif()

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
list (INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

# Initialize ECMake.
include ("ecmake")

# Setup compilers.
include ("SetupCompilers")

message(STATUS 
	"Current compiler is ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

add_definitions(
	-DPASTEL_ENABLE_ASSERTS
)

# Dependencies
# ------------

# Range v3
find_package(range-v3 CONFIG REQUIRED)
link_libraries(range-v3::range-v3)
add_definitions(-DRANGES_NO_STD_FORWARD_DECLARATIONS)

get_target_property(RangesIncludeDirectory range-v3 INTERFACE_INCLUDE_DIRECTORIES)

# Boost
find_package(Boost REQUIRED)
link_libraries(Boost::boost)
add_definitions(
	-DBOOST_BIND_GLOBAL_PLACEHOLDERS
)

# Eigen3
find_package(Eigen3 3.3 CONFIG REQUIRED NO_MODULE)
link_libraries(Eigen3::Eigen)
add_definitions (
	# std::result_of was removed in C++20.
	-DEIGEN_HAS_STD_RESULT_OF=0
)

# Threading Building Blocks
find_package(TBB CONFIG REQUIRED)
link_libraries(TBB::tbb)

get_target_property(TbbIncludeDirectory TBB::tbb INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(TbbLibraryDirectory TBB::tbb IMPORTED_LOCATION_RELEASE)
get_filename_component(TbbLibraryDirectory ${TbbLibraryDirectory} DIRECTORY)

if (BuildMatlab)
	set (MatlabDirectoryWindows "C:/Program Files/Polyspace/R2020a")
	set (MatlabDirectoryMac "/Applications/MATLAB_R2020a.app")
	set (MatlabDirectoryLinux "/mnt/c/Program Files/Polyspace/R2020a")

	if (WIN32)
		set (MatlabDirectory ${MatlabDirectoryWindows})
	elseif(UNIX)
		if (APPLE)
			set (MatlabDirectory ${MatlabDirectoryMac})
		else()
			set (MatlabDirectory ${MatlabDirectoryLinux})
		endif()
	endif()

	find_package("Matlab_")
	include_directories (${MatlabIncludeDirectory})
endif()

message(STATUS "Range-v3: ${RangesIncludeDirectory}")
message(STATUS "Boost: ${Boost_INCLUDE_DIRS} ${Boost_LIBRARIES}")
message(STATUS "Eigen: ${EIGEN3_INCLUDE_DIRS}")
message(STATUS "TBB: ${TbbIncludeDirectory} ${TbbLibraryDirectory}")

# Copy files
# ----------

# if (WIN32)
# 	# Copy dll-libraries aside executables for
# 	# Windows to find them, when running an
# 	# executable.

# 	set (DllPathSet 
# 		"${BlasDllPath}"
# 		"${LapackDllPath}")

# 	foreach (DllPath ${DllPathSet})
# 		EcCopyAsideExecutables("${DllPath}")
# 	endforeach()
# endif()

# # Copy some data files.
# file (COPY "${ProjectDirectory}/matlab/+pastelgeometry/fish.txt" 
# 	DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/matlab/+pastelgeometry")

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

