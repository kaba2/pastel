project (PastelGeometryMatlab)

file (GLOB SourceSet ${PastelSourceGlobSet})
source_group (Code FILES ${SourceSet})

file (GLOB DocumentationSet ${PastelDocumentationGlobSet})
source_group (Documentation FILES ${DocumentationSet})

add_library (pastelgeometrymatlab ${SourceSet})
target_link_libraries (
	pastelgeometrymatlab 
	pastelsys 
	pastelmath 
	pastelgeometry 
	pastelmatlab
)

