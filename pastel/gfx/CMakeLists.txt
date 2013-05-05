project (PastelGfx)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_library (pastelgfx ${PastelLibraryType} ${SourceSet})
target_link_libraries (
	pastelgfx 
	pastelsys 
	pastelmath 
	pastelgeometry
)

