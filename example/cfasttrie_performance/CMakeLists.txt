project (cfasttrie_performance)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_executable (cfasttrie_performance ${SourceSet})
target_link_libraries (
	cfasttrie_performance
	pastelsys 
)
