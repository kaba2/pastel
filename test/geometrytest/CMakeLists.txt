project (PastelGeometryTest)

file (GLOB SourceSet ${PastelSourceGlobSet})
source_group (Code FILES ${SourceSet})

file (GLOB DocumentationSet ${PastelDocumentationGlobSet})
source_group (Documentation FILES ${DocumentationSet})

add_executable (pastelgeometrytest ${SourceSet})
target_link_libraries (
	pastelgeometrytest 
	pastelsys 
	pastelmath
	pastelgeometry
	pastelgfx
)
