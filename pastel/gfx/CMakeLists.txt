project (PastelGfx)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_library (pastelgfx ${PastelLibraryType} ${SourceSet})
add_dependencies(pastelgfx pastelsys pastelmath pastelgeometry)

