project (PastelMathMatlab)

EcAddLibrary (library pastelmathmatlab ${PastelSourceGlobSet})

target_link_libraries (
	pastelmathmatlab 
	pastelsys 
	pastelmath 
	pastelgeometry 
	pastelmatlab
)

