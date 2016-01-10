project (PastelSysMatlab)

EcAddLibrary (library pastelsysmatlab ${PastelSourceGlobSet})

target_link_libraries (
	pastelsysmatlab 
	pastelsys 
	pastelmath 
	pastelgeometry 
	pastelmatlab
)

