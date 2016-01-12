if (BuildLibraries)
	add_subdirectory ("sys")
	add_subdirectory ("math")
	add_subdirectory ("geometry")
	add_subdirectory ("gfx")
	add_subdirectory ("doc")

	# Create a solution folder for the Pastel libraries.
	set (LibrarySet 
		pastelsys 
		pastelmath 
		pastelgeometry 
		pastelgfx
		pasteldoc)
	set_property( TARGET ${LibrarySet} PROPERTY FOLDER "Libraries")
endif()

if (BuildMatlab)
	add_subdirectory ("matlab")
	add_subdirectory ("sysmatlab")
	add_subdirectory ("mathmatlab")
	add_subdirectory ("geometrymatlab")

	# Create a solution folder for the Matlab libraries.
	set (MatlabLibrarySet
		pastelmatlab
		pastelsysmatlab
		pastelmathmatlab
		pastelgeometrymatlab)
	set_property( TARGET ${MatlabLibrarySet} PROPERTY FOLDER "Matlab Libraries")
endif()

