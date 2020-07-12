project (PastelTest)

EcAddLibrary (executable pasteltest "${PastelSourceGlobSet}")

set_target_properties(
    pasteltest PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY "${ProjectExecutableDirectory}/${LOWER_CMAKE_BUILD_TYPE}")

target_link_libraries(
    pasteltest
    pastel
)
