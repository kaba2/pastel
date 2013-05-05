project (PastelGeometryTest)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_executable (pastelgeometrytest ${SourceSet})
target_link_libraries (
	pastelgeometrytest 
	pastelsys 
	pastelmath
	pastelgeometry
	pastelgfx
)
