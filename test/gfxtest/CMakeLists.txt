project (PastelGfxTest)

file (GLOB SourceSet ${PastelSourceGlobSet})
source_group (Code FILES ${SourceSet})

file (GLOB DocumentationSet ${PastelDocumentationGlobSet})
source_group (Documentation FILES ${DocumentationSet})

add_executable (pastelgfxtest ${SourceSet})
target_link_libraries (
	pastelgfxtest 
	pastelsys 
	pastelmath
	pastelgeometry
	pastelgfx
)
