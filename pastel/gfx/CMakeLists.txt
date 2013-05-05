project (PastelGfx)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_library (pastelgfx ${SourceSet})
add_dependencies(pastelgfx pastelsys pastelmath pastelgeometry)

