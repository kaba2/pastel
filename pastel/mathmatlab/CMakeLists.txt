project (PastelMathMatlab)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_library (pastelmathmatlab ${SourceSet})
add_dependencies(pastelmathmatlab 
	pastelsys 
	pastelmath 
	pastelgeometry 
	pastelmatlab)

