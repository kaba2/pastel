project (PastelSysMatlab)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_library (pastelsysmatlab ${SourceSet})
target_link_libraries (
	pastelsysmatlab 
	pastelsys 
	pastelmath 
	pastelgeometry 
	pastelmatlab
)

