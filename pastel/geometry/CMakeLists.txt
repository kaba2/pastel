project (PastelGeometry)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_library (pastelgeometry ${SourceSet})
add_dependencies(pastelgeometry pastelsys pastelmath)

