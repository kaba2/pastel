project (PastelMath)

PastelAdd (library pastelmath)

target_link_libraries (
	pastelmath 
	pastelsys
	${BlasLibraryPath}
	${LapackLibraryPath}
)

