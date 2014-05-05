project (PastelSysMatlab)

file (GLOB SourceSet ${PastelSourceGlobSet})
source_group (Code FILES ${SourceSet})

file (GLOB DocumentationSet ${PastelDocumentationGlobSet})
source_group (Documentation FILES ${DocumentationSet})

add_library (pastelsysmatlab ${SourceSet})
target_link_libraries (
	pastelsysmatlab 
	pastelsys 
	pastelmath 
	pastelgeometry 
	pastelmatlab
)

