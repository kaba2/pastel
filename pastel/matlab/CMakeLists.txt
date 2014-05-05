project (PastelMatlab)

file (GLOB SourceSet ${PastelSourceGlobSet})
source_group (Code FILES ${SourceSet})

file (GLOB DocumentationSet ${PastelDocumentationGlobSet})
source_group (Documentation FILES ${DocumentationSet})

add_library (pastelmatlab ${SourceSet})
target_link_libraries (
	pastelmatlab 
	pastelsys
)
