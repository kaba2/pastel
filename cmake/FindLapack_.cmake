# Description: Lapack configuration
# Documentation: building.txt

# returns
# -------
#
# LapackLibraryPath:
#    Path to the Lapack library.
#
# LapackLibraryPath:
#    Path to the Lapack dll (on Windows).
#
# LapackLibraryName:
#    The filename-part of ${LapackLibraryPath}.
#
# LapackLibraryDirectory:
#    The directory-part of ${LapackLibraryPath}.

if (("${LapackLibraryDirectory}" STREQUAL "") OR (NOT EXISTS "${LapackLibraryDirectory}"))
	find_package(LAPACK)
	if (LAPACK_FOUND)
		set (LapackLibraryDirectory "${LAPACK_LIBRARIES}")
		set (LapackLibraryPath "${LapackLibraryDirectory}")
		get_filename_component (LapackLibraryName "${LapackLibraryPath}" NAME)
	endif()
endif()

EcCheckPathExists("Lapack (library)" "${LapackLibraryDirectory}")
