# Blas configuration
#
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

if (WIN32)
	set (BlasLibraryName "blas_win${GENERATOR_BITS}_MT")
	set (BlasLibraryDirectory "${PastelDirectory}/external/${GENERATOR_BITS}")
	set (BlasLibraryPath "${BlasLibraryDirectory}/${BlasLibraryName}.lib")
	set (DllSet 
		${DllSet}
		"${BlasLibraryDirectory}/${BlasLibraryName}.dll")
elseif (UNIX)
	set (BlasLibraryName "")
	set (BlasLibraryPath "")
endif()

if (("${BlasLibraryDirectory}" STREQUAL "") OR (NOT EXISTS "${BlasLibraryDirectory}"))
	find_package(BLAS)
	if (BLAS_FOUND)
		set (BlasLibraryDirectory "${BLAS_LIBRARIES}")
		set (BlasLibraryPath "${BlasLibraryDirectory}")
		get_filename_component (BlasLibraryName "${BlasLibraryPath}" NAME)
	endif()
endif()

CheckPathExists("Blas (library)" "${BlasLibraryDirectory}")