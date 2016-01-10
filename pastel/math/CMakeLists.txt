project (PastelMath)

EcAddLibrary (library pastelmath)

target_link_libraries (
	pastelmath 
	pastelsys
	${ArmadilloLibraryPath}
	${BlasLibraryPath}
	${LapackLibraryPath}
)

