project (PastelExample)

EcAddLibrary (executable pastelexample "${PastelSourceGlobSet}")

set_target_properties(
    pastelexample PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY "${ProjectExecutableDirectory}")

target_link_libraries(
    pastelexample
    pastel
)
