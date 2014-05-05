project (PastelGeometry)

file (GLOB SourceSet ${PastelSourceGlobSet})
source_group (Code FILES ${SourceSet})

file (GLOB DocumentationSet ${PastelDocumentationGlobSet})
source_group (Documentation FILES ${DocumentationSet})

add_library (pastelgeometry ${PastelLibraryType} ${SourceSet})

target_link_libraries (
	pastelgeometry 
	pastelsys 
	pastelmath
)

