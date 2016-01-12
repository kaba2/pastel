# Description: Tbb configuration
# Documentation: building.txt

# TbbDirectory (string):
#    The directory to the Tbb library.
#
# returns
# -------
#
# TbbIncludeDirectory (string):
#    A directory to add to include directories, such that
#    #include <tbb/blocked_range.h>
#    becomes valid.
#
# TbbLibraryPath (string):
#    Path to the Tbb library.
#
# TbbLibraryFilename (string):
#    Filename-part of ${TbbLibraryPath} (e.g. libtbb.a).
#
# TbbLibraryDirectory (string):
#    Directory-part of ${TbbLibraryPath}.
#
# TbbLibraryName (string):
#    The name of the library (e.g. tbb).

set (TbbIncludeDirectory "${TbbDirectory}/include")
set (TbbLibraryName "tbb")

if (WIN32)
	if (${GENERATOR_BITS} EQUAL 32)
		set (TbbLibraryPath 
			"${TbbDirectory}/lib/ia32/vc12/${TbbLibraryName}.lib")
	else()
		set (TbbLibraryPath
			"${TbbDirectory}/lib/intel64/vc12/${TbbLibraryName}.lib")
	endif()
elseif (APPLE)
	set (TbbLibraryPath 
		"${TbbDirectory}/local/lib/lib${TbbLibraryName}.dylib")
else()
	set (TbbLibraryPath 
		"${TbbDirectory}/lib/lib${TbbLibraryName}.so")
endif()

get_filename_component (TbbLibraryFilename "${TbbLibraryPath}" NAME)
get_filename_component (TbbLibraryDirectory "${TbbLibraryPath}" DIRECTORY)

EcCheckPathExists("Tbb (include)" "${TbbIncludeDirectory}")
EcCheckPathExists("Tbb (library)" "${TbbLibraryPath}")


