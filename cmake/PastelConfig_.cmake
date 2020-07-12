# Description: Pastel configuration

# PastelDirectory (string):
#    The directory to the Pastel library.
#
# returns
# -------
#
# PastelIncludeDirectory (string):
#    A directory to add to include directories, such that
#    #include <pastel/sys/mytypes.h>
#    becomes valid.
#
# PastelLibraryPath (string):
#    Path to the Pastel library.
#
# PastelLibraryFilename (string):
#    Filename-part of ${PastelLibraryPath} (e.g. libpastelsys.a).
#
# PastelLibraryDirectory (string):
#    Directory-part of ${PastelLibraryPath}.
#
# PastelLibraryName (string):
#    The name of the library (e.g. pastelsys).

string (TOLOWER "${CMAKE_BUILD_TYPE}" LOWER_CMAKE_BUILD_TYPE)

set (PastelDirectory "${CMAKE_CURRENT_LIST_DIR}/..")
set (PastelIncludeDirectory "${PastelDirectory}")
set (PastelLibraryDirectoryBase "${PastelDirectory}/lib/${ToolSet}")
set (PastelLibraryDirectory "${PastelLibraryDirectoryBase}/${LOWER_CMAKE_BUILD_TYPE}")

EcCheckPathExists("Pastel (include)" "${PastelIncludeDirectory}")
EcCheckPathExists("Pastel (library base)" "${PastelLibraryDirectoryBase}")
EcCheckPathExists("Pastel (library)" "${PastelLibraryDirectory}")

set (librarySet
	"PastelSys"
	"PastelMath"
	"PastelGfx"
	"PastelGeometry")

foreach (library ${librarySet})
	add_library("${library}" STATIC IMPORTED)
    string (TOUPPER ${library} upperLibrary)
    string (TOLOWER ${library} lowerLibrary)

	foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
	    string (TOUPPER ${OUTPUTCONFIG} UPPER_OUTPUTCONFIG)
	    string (TOLOWER ${OUTPUTCONFIG} LOWER_OUTPUTCONFIG)

	    set (hint "${PastelDirectory}/lib/${ToolSet}/${LOWER_OUTPUTCONFIG}")

	    unset(libraryPath CACHE)
	    find_library(libraryPath "${lowerLibrary}" HINTS ${hint})

	    #message (STATUS "${hint} ${libraryPath}")

	    set_target_properties ("${library}"
		    PROPERTIES IMPORTED_LOCATION_${UPPER_OUTPUTCONFIG}
		    "${libraryPath}")
	endforeach()
endforeach()

