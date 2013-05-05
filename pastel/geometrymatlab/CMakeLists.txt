project (PastelGeometryMatlab)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_library (pastelgeometrymatlab ${SourceSet})
target_link_libraries (
	pastelgeometrymatlab 
	pastelsys 
	pastelmath 
	pastelgeometry 
	pastelmatlab
)

