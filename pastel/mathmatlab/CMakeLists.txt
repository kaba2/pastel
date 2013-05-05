project (PastelMathMatlab)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_library (pastelmathmatlab ${PastelLibraryType} ${SourceSet})
add_dependencies(pastelmathmatlab 
	pastelsys 
	pastelmath 
	pastelgeometry 
	pastelmatlab)

