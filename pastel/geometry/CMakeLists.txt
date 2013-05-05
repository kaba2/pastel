project (PastelGeometry)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_library (pastelgeometry ${PastelLibraryType} ${SourceSet})
add_dependencies(pastelgeometry pastelsys pastelmath)

