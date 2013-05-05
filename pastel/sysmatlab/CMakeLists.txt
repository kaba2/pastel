project (PastelSysMatlab)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_library (pastelsysmatlab ${PastelLibraryType} ${SourceSet})
add_dependencies(pastelsysmatlab 
	pastelsys 
	pastelmath 
	pastelgeometry 
	pastelmatlab)

