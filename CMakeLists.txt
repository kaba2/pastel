# Description: Main CMake build file for the Pastel library
# Documentation: building.txt

cmake_minimum_required (VERSION 3.16)

if(NOT "${CMAKE_CXX_STANDARD}")
  set(CMAKE_CXX_STANDARD 20)
endif()

# Set up VCPKG
# ------------

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")

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

set (BoostIncludeDirectory ${Boost_INCLUDE_DIRS})

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
get_target_property(TbbLibraryPath TBB::tbb IMPORTED_LOCATION_RELEASE)
get_filename_component(TbbLibraryDirectory ${TbbLibraryPath} DIRECTORY)
get_filename_component(TbbLibraryName ${TbbLibraryPath} NAME)

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

	set (MatlabIncludeDirectory "${MatlabDirectory}/extern/include")

	EcCheckPathExists("Matlab (include)" "${MatlabIncludeDirectory}")

	include_directories (${MatlabIncludeDirectory})
endif()

message(STATUS "Range-v3: ${RangesIncludeDirectory}")
message(STATUS "Boost: ${Boost_INCLUDE_DIRS} ${Boost_LIBRARIES}")
message(STATUS "Eigen: ${EIGEN3_INCLUDE_DIRS}")
message(STATUS "TBB: ${TbbIncludeDirectory} ${TbbLibraryPath}")

# Copy files
# ----------

set (CopySet 
	${ProjectDirectory}/test/pastel/gfx/lena.pcx
	${ProjectDirectory}/test/pastel/gfx/resample_text.pcx
	${ProjectDirectory}/test/pastel/gfx/testpcx_1bit.pcx
	${ProjectDirectory}/test/pastel/gfx/testpcx_4bit.pcx
	${ProjectDirectory}/test/pastel/gfx/testpcx_8bit.pcx
	${ProjectDirectory}/test/pastel/gfx/testpcx_rgb.pcx
)

foreach (Filename ${CopySet})
	EcCopyAsideExecutables("${Filename}")
endforeach()

# Copy some data files.
file (COPY "${ProjectDirectory}/pastelmatlab/matlab/+pastelmatlab/fish.txt" 
	DESTINATION "${ProjectMatlabDirectory}/+pastelmatlab")

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
	add_subdirectory (pastelmatlab)
endif()

if (BuildExamples)
	add_subdirectory (example)
endif()

# message (STATUS "CMAKE_INSTALL_LIBDIR = ${CMAKE_INSTALL_LIBDIR}")
# message (STATUS "CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}")
# message (STATUS "CMAKE_INSTALL_DATADIR = ${CMAKE_INSTALL_DATADIR}")

# add_library(Pastel INTERFACE)
# add_library(Pastel::Pastel ALIAS Pastel)
# target_link_libraries(
# 	Pastel
# 	INTERFACE
# 	PastelSys::PastelSys
# 	PastelMath::PastelMath
# 	PastelGeometry:PastelGeometry
# 	PastelGfx::PastelGfx
# )

# install(
# 	TARGETS
#     Pastel
#     EXPORT PastelConfig
#     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )

# export(
# 	TARGETS
#     Pastel
#     NAMESPACE Pastel::
#     FILE "${CMAKE_CURRENT_BINARY_DIR}/PastelConfig.cmake"
# )

# install(
# 	EXPORT
#     PastelConfig
#     DESTINATION "${CMAKE_INSTALL_DATADIR}/Pastel/cmake"
#     NAMESPACE Pastel::
# )
