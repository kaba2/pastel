project (PastelGeometryTest)

EcAddLibrary (executable pastelgeometrytest ${PastelSourceGlobSet})

target_link_libraries (
	pastelgeometrytest 
	pastelsys 
	pastelmath
	pastelgeometry
	pastelgfx
)
