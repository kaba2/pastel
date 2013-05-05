project (PastelMathTest)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_executable (pastelmathtest ${SourceSet})
target_link_libraries (
	pastelmathtest 
	pastelsys 
	pastelmath
)
