project (PastelMath)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_library (pastelmath ${PastelLibraryType} ${SourceSet})
target_link_libraries (
	pastelmath 
	pastelsys
)

