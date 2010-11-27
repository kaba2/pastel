-- Change the following directories to reflect your own
-- build environment.

-- The directory of the Boost library's source code.
-- The includes are of the form 'boost/static_assert.hpp'.
boostInclude = "e:/ohjelmointi/external/boost_1_43_0"

-- The directory of the SDL library's header files.
-- The includes are of the form 'SDL.h'.
sdlInclude = "e:/ohjelmointi/external/SDL-1.2.13/include"
sdlLib = "e:/ohjelmointi/external/SDL-1.2.13/lib"

-- The directory of the GLEW library's header files.
-- The includes are of the form 'glew.h'.
glewInclude = "e:/ohjelmointi/external/glew/include"
glewLib = "e:/ohjelmointi/external/glew/lib"

solution "Pastel"

	outputDirectory = "build/" .. _ACTION

	includeDirectorySet = 
	{
		"./",
		boostInclude,
		sdlInclude,
		glewInclude
	}
	
	libraryDirectorySet =
	{
		sdlLib,
		glewLib
	}
	
	configurations 
	{
		"debug", 
		"develop",
		"release"
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
	location(outputDirectory)
	targetdir(outputDirectory .. "/lib")
	
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
	
	configuration "debug"
		targetsuffix "_d"
		defines 
		{
			"PASTEL_DEBUG_MODE", 
			"PASTEL_ENABLE_PENSURES"
		}
		flags {"Symbols"}
		
	configuration "develop"
		targetsuffix "_v"
		flags {"Optimize"}
		defines
		{
			"PASTEL_DEBUG_MODE", 
			"PASTEL_ENABLE_PENSURES",
			"PASTEL_ENABLE_OMP"
		}
	
	configuration "release"
		targetsuffix ""
		flags {"Optimize"}
		defines
		{
			"PASTEL_ENABLE_OMP"
		}

	function addPrefix(prefix, stringSet)
		resultSet = {}
		for i, name in pairs(stringSet)
		do
			resultSet[#resultSet + 1] = prefix .. name
		end
		return resultSet
	end

	libKind = "StaticLib"

	project "PastelSys"
		kind(libKind)
		includedirs(includeDirectorySet)
		libdirs(libraryDirectorySet)
		files(addPrefix("pastel/sys/", fileSet))
	
	project "PastelMath"
		kind(libKind)
		includedirs(includeDirectorySet)
		libdirs(libraryDirectorySet)
		files(addPrefix("pastel/math/", fileSet))
		links
		{
			"PastelSys"
		}
		
	project "PastelDevice"
		kind(libKind)
		includedirs(includeDirectorySet)
		libdirs(libraryDirectorySet)
		files(addPrefix("pastel/device/", fileSet))
		links
		{
			"PastelSys"
		}
		
	project "PastelGeometry"
		kind(libKind)
		includedirs(includeDirectorySet)
		libdirs(libraryDirectorySet)
		files(addPrefix("pastel/geometry/", fileSet))
		links
		{
			"PastelSys",
			"PastelMath"
		}

	project "PastelDsp"
		kind(libKind)
		includedirs(includeDirectorySet)
		libdirs(libraryDirectorySet)
		files(addPrefix("pastel/dsp/", fileSet))
		links
		{
			"PastelSys",
			"PastelMath"
		}

	project "PastelGfx"
		kind(libKind)
		includedirs(includeDirectorySet)
		libdirs(libraryDirectorySet)
		files(addPrefix("pastel/gfx/", fileSet))
		links
		{
			"PastelSys",
			"PastelMath",
			"PastelGeometry",
			"PastelDsp"
		}

	project "PastelRay"
		kind(libKind)
		includedirs(includeDirectorySet)
		libdirs(libraryDirectorySet)
		files(addPrefix("pastel/ray/", fileSet))
		links
		{
			"PastelSys",
			"PastelMath",
			"PastelGeometry",
			"PastelDsp",
			"PastelGfx"
		}

	project "PastelGl"
		kind(libKind)
		includedirs(includeDirectorySet)
		libdirs(libraryDirectorySet)
		files(addPrefix("pastel/gl/", fileSet))
		links
		{
			"PastelSys",
			"PastelMath",
			"PastelGeometry",
			"PastelDsp",
			"PastelGfx",
			"PastelDevice"
		}

	project "PastelGfxUi"
		kind(libKind)
		includedirs(includeDirectorySet)
		libdirs(libraryDirectorySet)
		files(addPrefix("pastel/gfxui/", fileSet))
		links
		{
			"PastelSys",
			"PastelMath",
			"PastelGeometry",
			"PastelDsp",
			"PastelGfx",
			"PastelDevice"
		}

	project "PastelSysTest"
		kind("ConsoleApp")
		includedirs(includeDirectorySet)
		libdirs(libraryDirectorySet)
		files(addPrefix("test/systest/", fileSet))
		links 
		{
			"PastelDsp",
			"PastelGfx", 
			"PastelSys"  
		}
	
	project "PastelRayTest"
		kind("ConsoleApp")
		includedirs(includeDirectorySet)
		libdirs(libraryDirectorySet)
		files(addPrefix("test/raytest/", fileSet))
		links 
		{
			"PastelRay",
			"PastelGfx", 
			"PastelGeometry",
			"PastelDsp",
			"PastelMath",
			"PastelSys" 
		}

	project "PastelMathTest"
		kind("ConsoleApp")
		includedirs(includeDirectorySet)
		libdirs(libraryDirectorySet)
		files(addPrefix("test/mathtest/", fileSet))
		links 
		{
			"PastelGfx",
			"PastelMath",
			"PastelSys" 
		}

	project "PastelGfxTest"
		kind("ConsoleApp")
		includedirs(includeDirectorySet)
		libdirs(libraryDirectorySet)
		files(addPrefix("test/gfxtest/", fileSet))
		links 
		{
			"PastelDevice",
			"SDL",
			"PastelGfx", 
			"PastelDsp",
			"PastelGeometry",
			"PastelMath",
			"PastelSys"
		}

	project "PastelGeometryTest"
		kind("ConsoleApp")
		includedirs(includeDirectorySet)
		libdirs(libraryDirectorySet)
		files(addPrefix("test/geometrytest/", fileSet))
		links 
		{
			"PastelDevice",
			"SDL",
			"PastelGfx", 
			"PastelDsp",
			"PastelGeometry",
			"PastelMath",
			"PastelSys"
		}

	project "PastelDspTest"
		kind("ConsoleApp")
		includedirs(includeDirectorySet)
		libdirs(libraryDirectorySet)
		files(addPrefix("test/dsptest/", fileSet))
		links 
		{
			"PastelDevice",
			"SDL",
			"PastelRay",
			"PastelGfx", 
			"PastelDsp",
			"PastelGeometry",
			"PastelMath",
			"PastelSys"
		}

	project "PastelDeviceTest"
		kind("ConsoleApp")
		includedirs(includeDirectorySet)
		libdirs(libraryDirectorySet)
		files(addPrefix("test/devicetest/", fileSet))
		links 
		{
			"PastelDevice",
			"SDL",
			"PastelSys"
		}
