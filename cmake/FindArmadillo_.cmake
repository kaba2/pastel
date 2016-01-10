# Description: Armadillo configuration
# Documentation: building.txt

# returns
# -------
#
# ArmadilloIncludeDirectory:
#    A directory to add to include directories, such that
#    #include <armadillo>
#    becomes valid.
#
# ArmadilloLibraryPath:
#    Path to the Armadillo library.
#
# ArmadilloLibraryDirectory:
#    The directory-part of ${ArmadilloLibraryPath}.

if (("${ArmadilloIncludeDirectory}" STREQUAL "") OR (NOT EXISTS "${ArmadilloIncludeDirectory}"))
	find_package(Armadillo)
	if (ARMADILLO_FOUND)
		# The found path allows includes of the form:
		# #include <armadillo>
		set (ArmadilloIncludeDirectory "${ARMADILLO_INCLUDE_DIRS}")
		set (ArmadilloLibraryPath "${ARMADILLO_LIBRARIES}")
		get_filename_component (ArmadilloLibraryName "${ArmadilloLibraryPath}" NAME)
		string (REGEX REPLACE "lib(.*).dylib$" "\\1" ArmadilloLibraryName "${ArmadilloLibraryName}")
		get_filename_component (ArmadilloLibraryDirectory "${ArmadilloLibraryPath}" DIRECTORY)
	endif()
endif()

EcCheckPathExists("Armadillo (include)" "${ArmadilloIncludeDirectory}")

if (APPLE)
	EcCheckPathExists("Armadillo (library)" "${ArmadilloLibraryPath}")
endif()

include_directories (${ArmadilloIncludeDirectory})

# Make Armadillo use C++11 (e.g. move constructors).
add_definitions(-DARMA_USE_CXX11)
