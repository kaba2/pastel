project (PastelSysMatlab)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_library (pastelsysmatlab ${SourceSet})
add_dependencies(pastelsysmatlab 
	pastelsys 
	pastelmath 
	pastelgeometry 
	pastelmatlab)

