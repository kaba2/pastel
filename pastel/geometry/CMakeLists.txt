project (PastelGeometry)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_library (pastelgeometry ${PastelLibraryType} ${SourceSet})
target_link_libraries (
	pastelgeometry 
	pastelsys 
	pastelmath
)

