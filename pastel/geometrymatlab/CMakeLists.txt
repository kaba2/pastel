project (PastelGeometryMatlab)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_library (pastelgeometrymatlab ${SourceSet})
add_dependencies(pastelgeometrymatlab 
	pastelsys 
	pastelmath 
	pastelgeometry 
	pastelmatlab)

