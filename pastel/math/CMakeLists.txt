project (PastelMath)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_library (pastelmath ${PastelLibraryType} ${SourceSet})
add_dependencies(pastelmath pastelsys)

