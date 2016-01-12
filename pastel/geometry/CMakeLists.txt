project (PastelGeometry)

EcAddLibrary (library pastelgeometry "${PastelSourceGlobSet}")

target_link_libraries (
	pastelgeometry 
	pastelsys 
	pastelmath
)

