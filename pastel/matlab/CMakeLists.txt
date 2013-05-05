project (PastelMatlab)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_library (pastelmatlab ${SourceSet})
add_dependencies(pastelmatlab pastelsys)

