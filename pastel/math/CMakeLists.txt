project (PastelMath)

EcAddLibrary (library pastelmath ${PastelSourceGlobSet})

target_link_libraries (
	pastelmath 
	pastelsys
	${ArmadilloLibraryPath}
	${BlasLibraryPath}
	${LapackLibraryPath}
)

