project (PastelMathTest)

EcAddLibrary (executable pastelmathtest ${PastelSourceGlobSet})

target_link_libraries (
	pastelmathtest 
	pastelsys 
	pastelmath
)
