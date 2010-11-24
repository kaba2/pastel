-- Change the following directories to reflect your your own
-- build environment.

-- The directory of the Boost library's source code.
-- The includes are of the form 'boost/static_assert.hpp'.
boostDirectory = "e:/ohjelmointi/external/boost_1_43_0"

-- The directory of the SDL library's header files.
-- The includes are of the form 'SDL.h'.
sdlDirectory = "e:/ohjelmointi/external/SDL-1.2.13/include"

-- The directory of the GLEW library's header files.
-- The includes are of the form 'glew.h'.
glewDirectory = "e:/ohjelmointi/external/glew/include"

solution "Pastel"

	includeDirectorySet = 
	{
		"../", 
		boostDirectory,
		sdlDirectory,
		glewDirectory
	}
	
	configurations 
	{
		"Debug", 
		"Develop",
		"Release"
	}
	
	flags
	{
		"NativeWChar", 
		"No64BitChecks", 
		"NoPCH"
	}
	
	fileSet = 
	{
		"*.cpp",
		"*.hpp",
		"*.h"
	}

	language "C++"
	location("premake/" .. _ACTION)
	targetdir("premake/" .. _ACTION .. "/lib")
	
	configuration "vs*"
		-- Disable warnings.
		buildoptions
		{
			-- "'identifier' : truncation from 'type1' to 'type2'"
			"/wd4305",
			-- "'argument' : conversion from 'type1' to 'type2', possible loss of data."
			"/wd4244",
			-- "'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'"
			"/wd4251", 
			-- "new behavior: elements of array 'array' will be default initialized"
			"/wd4351",
			-- "'function': was declared deprecated" (referring to STL functions)
			"/wd4996",
			-- "'var' : conversion from 'size_t' to 'type', possible loss of data"
			"/wd4267",
			-- "'expression' : signed/unsigned mismatch"
			"/wd4018", 
			-- "'operation' : conversion from 'type1' to 'type2' of greater size"
			"/wd4312",
			-- "nonstandard extension used : formal parameter 'identifier' was previously defined as a type"
			"/wd4224",
			-- "qualifier applied to function type has no meaning; ignored"
			"/wd4180",
			-- "'type' : forcing value to bool 'true' or 'false' (performance warning)"
			"/wd4800", 
			-- "'operation' : unsafe use of type 'bool' in operation"
			"/wd4804"
		}

		-- Disable Microsoft's Secure STL
		defines
		{
			"_SECURE_SCL=0",
			"_HAS_ITERATOR_DEBUGGING=0",
			"PASTEL_VISUAL_STUDIO",
		}
		
		-- Disable language extensions
		buildoptions
		{
			"/Za"			
		}
	
	configuration "Debug"
		targetsuffix "_d"
		defines 
		{
			"PASTEL_DEBUG_MODE", 
			"PASTEL_ENABLE_PENSURES"
		}
		flags {"Symbols"}
		
	configuration "Develop"
		targetsuffix "_v"
		flags {"Optimize"}
		defines
		{
			"PASTEL_DEBUG_MODE", 
			"PASTEL_ENABLE_PENSURES",
			"PASTEL_ENABLE_OMP"
		}
	
	configuration "Release"
		targetsuffix ""
		flags {"Optimize"}
		defines
		{
			"PASTEL_ENABLE_OMP"
		}

	librarySet =
	{
		PastelSys = "sys/",
		PastelDsp = "dsp/",
		PastelGeometry = "geometry/",
		PastelMath = "math/",
		PastelGfx = "gfx/",
		PastelRay = "ray/",
		PastelDevice = "device/",
		PastelGfxUi = "gfxui/",
		PastelGl = "gl/"
	}
	
	libKind = "StaticLib"
	
	function addPrefix(prefix, stringSet)
		resultSet = {}
		for i, name in pairs(stringSet)
		do
			resultSet[#resultSet + 1] = prefix .. name
		end
		return resultSet
	end
	
	for projectName, projectDirectory in pairs(librarySet)
	do
		project(projectName)
		kind(libKind)
		includedirs(includeDirectorySet)
		files(addPrefix(projectDirectory, fileSet))
	end
				
		