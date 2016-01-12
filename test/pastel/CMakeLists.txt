add_subdirectory("geometry")
add_subdirectory("gfx")
add_subdirectory("math")
add_subdirectory("sys")

# Create a solution folder for test projects.
set (TestSet 
	pastelsystest 
	pastelmathtest 
	pastelgeometrytest 
	pastelgfxtest)
set_property(TARGET ${TestSet} PROPERTY FOLDER "Tests")
