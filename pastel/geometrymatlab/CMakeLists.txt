project (PastelGeometryMatlab)

EcAddLibrary (library pastelgeometrymatlab "${PastelSourceGlobSet}")

target_link_libraries (
	pastelgeometrymatlab 
	pastelsys 
	pastelmath 
	pastelgeometry 
	pastelmatlab
)

