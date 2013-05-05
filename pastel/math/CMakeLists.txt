project (PastelMath)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_library (pastelmath ${SourceSet})
add_dependencies(pastelmath pastelsys)

