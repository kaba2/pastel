project (PastelGeometryMatlab)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_library (pastelgeometrymatlab ${PastelLibraryType} ${SourceSet})
add_dependencies(pastelgeometrymatlab 
	pastelsys 
	pastelmath 
	pastelgeometry 
	pastelmatlab)

