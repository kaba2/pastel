project (PastelMathMatlab)

file (GLOB SourceSet ${PastelSourceGlobSet})
source_group (Code FILES ${SourceSet})

file (GLOB DocumentationSet ${PastelDocumentationGlobSet})
source_group (Documentation FILES ${DocumentationSet})

add_library (pastelmathmatlab ${SourceSet})
target_link_libraries (
	pastelmathmatlab 
	pastelsys 
	pastelmath 
	pastelgeometry 
	pastelmatlab
)

