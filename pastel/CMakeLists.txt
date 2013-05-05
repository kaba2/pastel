project (Pastel)

if (BuildLibraries)
	add_subdirectory ("sys")
	add_subdirectory ("math")
	add_subdirectory ("geometry")
	add_subdirectory ("gfx")
endif()

message (STATUS ${BuildMatlab})

if (BuildMatlab)
	add_subdirectory ("matlab")
	add_subdirectory ("sysmatlab")
	add_subdirectory ("mathmatlab")
	add_subdirectory ("geometrymatlab")
endif()