project (PastelMatlab)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_library (pastelmatlab ${SourceSet})
target_link_libraries (
	pastelmatlab 
	pastelsys
)
