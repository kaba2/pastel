project (PastelMatlab)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_library (pastelmatlab ${PastelLibraryType} ${SourceSet})
add_dependencies(pastelmatlab pastelsys)

