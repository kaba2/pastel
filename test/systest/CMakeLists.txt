project (PastelSysTest)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_executable (pastelsystest ${SourceSet})
target_link_libraries (
	pastelsystest 
	pastelsys
)
