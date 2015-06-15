# Lapack configuration
#
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

if (WIN32)
	set (LapackLibraryName "lapack_win${GENERATOR_BITS}_MT")
	set (LapackLibraryDirectory "${PastelDirectory}/external/${GENERATOR_BITS}")
	set (LapackLibraryPath "${LapackLibraryDirectory}/${LapackLibraryName}.lib")
	set (DllSet 
		${DllSet}
		"${LapackLibraryDirectory}/${LapackLibraryName}.dll")

elseif (UNIX)
	set (LapackLibraryName "")
	set (LapackLibraryPath "")
endif()

if (("${LapackLibraryDirectory}" STREQUAL "") OR (NOT EXISTS "${LapackLibraryDirectory}"))
	find_package(LAPACK)
	if (LAPACK_FOUND)
		set (LapackLibraryDirectory "${LAPACK_LIBRARIES}")
		set (LapackLibraryPath "${LapackLibraryDirectory}")
		get_filename_component (LapackLibraryName "${LapackLibraryPath}" NAME)
	endif()
endif()

CheckPathExists("Lapack (library)" "${LapackLibraryDirectory}")
