project (PastelGfxTest)

EcAddLibrary (executable pastelgfxtest ${PastelSourceGlobSet})

target_link_libraries (
	pastelgfxtest 
	pastelsys 
	pastelmath
	pastelgeometry
	pastelgfx
)
