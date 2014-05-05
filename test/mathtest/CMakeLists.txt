project (PastelMathTest)

file (GLOB SourceSet ${PastelSourceGlobSet})
source_group (Code FILES ${SourceSet})

file (GLOB DocumentationSet ${PastelDocumentationGlobSet})
source_group (Documentation FILES ${DocumentationSet})

add_executable (pastelmathtest ${SourceSet})
target_link_libraries (
	pastelmathtest 
	pastelsys 
	pastelmath
)
