# Description: Blas configuration
# Documentation: building.txt

# returns
# -------
#
# BlasLibraryPath:
#    Path to the Blas library.
#
# BlasLibraryPath:
#    Path to the Blas dll (on Windows).
#
# BlasLibraryName:
#    The filename-part of ${BlasLibraryPath}.
#
# BlasLibraryDirectory:
#    The directory-part of ${BlasLibraryPath}.

if (("${BlasLibraryDirectory}" STREQUAL "") OR (NOT EXISTS "${BlasLibraryDirectory}"))
	find_package(BLAS)
	if (BLAS_FOUND)
		set (BlasLibraryDirectory "${BLAS_LIBRARIES}")
		set (BlasLibraryPath "${BlasLibraryDirectory}")
		get_filename_component (BlasLibraryName "${BlasLibraryPath}" NAME)
	endif()
endif()

EcCheckPathExists("Blas (library)" "${BlasLibraryDirectory}")
