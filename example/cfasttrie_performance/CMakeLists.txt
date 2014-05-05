project (cfasttrie_performance)

file (GLOB SourceSet ${PastelSourceGlobSet})
source_group (Code FILES ${SourceSet})

file (GLOB DocumentationSet ${PastelDocumentationGlobSet})
source_group (Documentation FILES ${DocumentationSet})

add_executable (cfasttrie_performance ${SourceSet})
target_link_libraries (
	cfasttrie_performance
	pastelsys 
)
