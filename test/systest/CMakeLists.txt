project (PastelSysTest)

file (GLOB SourceSet ${PastelSourceGlobSet})
source_group (Code FILES ${SourceSet})

file (GLOB DocumentationSet ${PastelDocumentationGlobSet})
source_group (Documentation FILES ${DocumentationSet})

add_executable (pastelsystest ${SourceSet})
target_link_libraries (
	pastelsystest 
	pastelsys
)
