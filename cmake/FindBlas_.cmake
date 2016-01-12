# Description: Blas configuration
# Documentation: building.txt

# Optional arguments
# ------------------
#
# BlasLibraryPath:
#    Path to the Blas library (directory + filename). This
#    is for manually defining the library path (e.g. Windows).
#
# returns
# -------
#
# BlasLibraryPath (string):
#    Path to the Blas library.
#
# BlasDllPath (string):
#    Path to the Blas dll (on Windows).
#
# BlasLibraryFilename (string):
#    The filename-part of ${BlasLibraryPath} (e.g. libblas.a)
#
# BlasLibraryDirectory (string):
#    The directory-part of ${BlasLibraryPath}.
#
# BlasLibraryName (string):
#    The name of the Blas library (e.g. blas).

if (("${BlasLibraryPath}" STREQUAL "") OR (NOT EXISTS "${BlasLibraryPath}"))
	find_package(BLAS)
	if (BLAS_FOUND)
		set (BlasLibraryPath "${BLAS_LIBRARIES}")
	endif()
endif()

get_filename_component (BlasLibraryFilename "${BlasLibraryPath}" NAME)
get_filename_component (BlasLibraryDirectory "${BlasLibraryPath}" DIRECTORY)

EcLibraryNameFromFileName(BlasLibraryName "${BlasLibraryFilename}")

if (WIN32)
	string (REGEX REPLACE "\\.lib" ".dll" BlasDllPath ${BlasLibraryPath})
endif()

EcCheckPathExists("Blas (library)" "${BlasLibraryPath}")
