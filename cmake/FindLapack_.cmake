# Description: Lapack configuration
# Documentation: building.txt

# Optional arguments
# ------------------
#
# LapackLibraryPath:
#    Path to the Lapack library (directory + filename). This
#    is for manually defining the library path (e.g. Windows).
#
# returns
# -------
#
# LapackLibraryPath (string):
#    Path to the Lapack library.
#
# LapackDllPath (string):
#    Path to the Lapack dll (on Windows).
#
# LapackLibraryFilename (string):
#    The filename-part of ${LapackLibraryPath} (e.g. liblapack.a).
#
# LapackLibraryDirectory (string):
#    The directory-part of ${LapackLibraryPath}.
#
# LapackLibraryName (string):
#    The name of the Lapack library (e.g. lapack).

if (("${LapackLibraryPath}" STREQUAL "") OR (NOT EXISTS "${LapackLibraryPath}"))
	find_package(LAPACK)
	if (LAPACK_FOUND)
		set (LapackLibraryPath "${LAPACK_LIBRARIES}")
	endif()
endif()

get_filename_component (LapackLibraryFilename "${LapackLibraryPath}" NAME)
get_filename_component (LapackLibraryDirectory "${LapackLibraryPath}" DIRECTORY)

EcLibraryNameFromFileName(LapackLibraryName "${LapackLibraryFilename}")

if (WIN32)
	string (REGEX REPLACE "\\.lib" ".dll" LapackDllPath ${LapackLibraryPath})
endif()

EcCheckPathExists("Lapack (library)" "${LapackLibraryPath}")
