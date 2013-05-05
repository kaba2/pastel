project (PastelMathMatlab)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_library (pastelmathmatlab ${SourceSet})
target_link_libraries (
	pastelmathmatlab 
	pastelsys 
	pastelmath 
	pastelgeometry 
	pastelmatlab
)

