project (PastelGfx)

EcAddLibrary (library pastelgfx "${PastelSourceGlobSet}")

target_link_libraries (
	pastelgfx 
	pastelsys 
	pastelmath 
	pastelgeometry
)

