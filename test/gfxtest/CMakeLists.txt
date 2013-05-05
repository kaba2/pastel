project (PastelGfxTest)

file (GLOB SourceSet ${PastelSourceGlobSet})
add_executable (pastelgfxtest ${SourceSet})
target_link_libraries (
	pastelgfxtest 
	pastelsys 
	pastelmath
	pastelgeometry
	pastelgfx
)
