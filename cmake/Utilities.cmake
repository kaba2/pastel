# Description: CMake utilities
# Documentation: building.txt

macro(CheckPathExists Name PathSet)
	foreach(Path ${PathSet})
		if(EXISTS ${Path})
			message (STATUS "${Name}: ${Path}")
		else()
			set (Tried "")
			if (NOT ("${Path}" STREQUAL ""))
				set (Tried " (tried ${Path})")
			endif()
			message (SEND_ERROR "Cannot find ${Name}${Tried}. Either install ${Name}, or correct the path in Pastel's root CMakeLists.txt.")
			return()
		endif()
	endforeach()
endmacro()

macro (CopyDllsTo Directory)
    foreach (dllPath ${DllSet})
		file (COPY "${dllPath}" DESTINATION ${Directory})
    endforeach()
endmacro()
