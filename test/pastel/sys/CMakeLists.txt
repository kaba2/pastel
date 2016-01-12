project (PastelSysTest)

EcAddLibrary (executable pastelsystest "${PastelSourceGlobSet}")

target_link_libraries (
	pastelsystest 
	pastelsys
)
