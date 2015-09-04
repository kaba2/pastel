# Tbb configuration
#
# returns
# -------
#
# TbbIncludeDirectory:
#    A directory to add to include directories, such that
#    #include <tbb/blocked_range.h>
#    becomes valid.
#
# TbbLibraryPath:
#    Path to the Tbb library.
#
# TbbLibraryDirectory:
#    The directory-part of ${ArmadilloLibraryPath}.

if (WIN32)
	set (TbbDirectory "C:/code/tbb-4.4")
	set (TbbIncludeDirectory "${TbbDirectory}/include")
	set (TbbLibraryName "tbb")

	if (${GENERATOR_BITS} EQUAL 32)
		set (TbbLibraryDirectory "${TbbDirectory}/lib/ia32/vc12")
	else()
		set (TbbLibraryDirectory "${TbbDirectory}/lib/intel64/vc12")
	endif()

	set (TbbLibraryPath "${TbbLibraryDirectory}/${TbbLibraryName}.lib")
elseif (UNIX)
	set (TbbDirectory "/usr")
	set (TbbIncludeDirectory "${TbbDirectory}/include")
	set (TbbLibraryName "tbb")

	if (APPLE)
		set (TbbLibraryDirectory "${TbbDirectory}/local/lib")
		set (TbbLibraryPath "${TbbLibraryDirectory}/lib${TbbLibraryName}.dylib")
	else()
		set (TbbLibraryDirectory "${TbbDirectory}/lib")
		set (TbbLibraryPath "${TbbLibraryDirectory}/lib${TbbLibraryName}.so")
	endif()
endif()

CheckPathExists("Tbb (include)" "${TbbIncludeDirectory}")
CheckPathExists("Tbb (library)" "${TbbLibraryPath}")

include_directories (${TbbIncludeDirectory})

