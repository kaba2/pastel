project (PastelGfx)

file (GLOB SourceSet ${PastelSourceGlobSet})
source_group (Code FILES ${SourceSet})

file (GLOB DocumentationSet ${PastelDocumentationGlobSet})
source_group (Documentation FILES ${DocumentationSet})

add_library (pastelgfx ${PastelLibraryType} ${SourceSet})
target_link_libraries (
	pastelgfx 
	pastelsys 
	pastelmath 
	pastelgeometry
)

