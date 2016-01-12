# Description: Armadillo configuration
# Documentation: building.txt

# Arguments
# ---------
#
# ArmadilloDirectory (string):
#    The directory to the Armadillo library.
#
# returns
# -------
#
# ArmadilloIncludeDirectory (string):
#    A directory to add to include directories, such that
#    #include <armadillo>
#    becomes valid.
#
# ArmadilloLibraryPath (string):
#    Path to the Armadillo library.
#
# ArmadilloLibraryFilename (string):
#    Filename-part of ${ArmadilloLibraryPath} (e.g. libarmadillo.a).
#
# ArmadilloLibraryDirectory (string):
#    The directory-part of ${ArmadilloLibraryPath}.
#
# ArmadilloLibraryName (string):
#    Name of the Armadillo library (e.g. armadillo).

if (("${ArmadilloDirectory}" STREQUAL "") OR (NOT EXISTS "${ArmadilloDirectory}"))
	find_package(Armadillo)
	if (ARMADILLO_FOUND)
		# The found path allows includes of the form:
		# #include <armadillo>
		set (ArmadilloDirectory "${ARMADILLO_INCLUDE_DIRS}")
		string (REGEX REPLACE "(.*)/include$" "\\1" ArmadilloDirectory "${ArmadilloDirectory}")
		set (ArmadilloLibraryPath "${ARMADILLO_LIBRARIES}")
	endif()
else()
	set (ArmadilloLibraryPath "")
endif()

set (ArmadilloIncludeDirectory "${ArmadilloDirectory}/include")

get_filename_component (ArmadilloLibraryFilename "${ArmadilloLibraryPath}" NAME)
get_filename_component (ArmadilloLibraryDirectory "${ArmadilloLibraryPath}" DIRECTORY)

EcLibraryNameFromFileName(ArmadilloLibraryName "${ArmadilloLibraryFilename}")

EcCheckPathExists("Armadillo (include)" "${ArmadilloIncludeDirectory}")

if (APPLE)
	EcCheckPathExists("Armadillo (library)" "${ArmadilloLibraryPath}")
endif()

include_directories (${ArmadilloIncludeDirectory})

# Make Armadillo use C++11 (e.g. move constructors).
add_definitions(-DARMA_USE_CXX11)
