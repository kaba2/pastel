-- This is a Premake script for producing
-- build files for the Pastel library.

-- Change the following directories to reflect your own
-- build environment.

-- The directory of the Boost library's source code.
-- The includes are of the form 'boost/static_assert.hpp'.
boostIncludeDir = "../../external/boost_1_43_0"

-- The directory of the SDL library's header files.
-- The includes are of the form 'SDL.h'.
sdlIncludeDir = "../../external/SDL-1.2.14/include"
sdlLibraryDir = "../../external/SDL-1.2.14/lib"

-- The directory of the GLEW library's header files.
-- The includes are of the form 'glew.h'.
glewIncludeDir = "../../external/glew-1.5.7/include"
glewLibraryDir = "../../external/glew-1.5.7/lib"

outputDirectory = "build/" .. _ACTION

solution "Pastel"

	language "C++"

	location(outputDirectory)
	targetdir(outputDirectory .. "/lib")

	configurations 
	{
		-- Debug information, with ASSERTs and PENSURES, without OpenMP.
		"debug",
		-- With ASSERTs and PENSUREs, without OpenMP.
		"develop",
		-- Without ASSERTs and PENSUREs, without OpenMP.
		"release-without-openmp",
		-- Without ASSERTs and PENSUREs, with OpenMP.
		"release"
	}

	flags
	{
		-- Treat wchar_t as a native type.
		"NativeWChar", 
		-- Do not give warnings from 64-bit checks.
		"No64BitChecks", 
		-- Do not use precompiled headers.
		"NoPCH"
	}

	configuration "debug"
		-- Debug libraries are suffixed with 'd'.
		targetsuffix "_d"
		-- Enable debug information.
		flags {"Symbols"}
		-- Enable ASSERTs and PENSUREs.
		defines 
		{
			"PASTEL_DEBUG_MODE", 
			"PASTEL_ENABLE_PENSURES"
		}
		
	configuration "develop"
		-- Developer libraries are suffixed with 'v'.
		targetsuffix "_v"
		-- Enable optimizations.
		flags {"Optimize"}
		-- Enable ASSERTs and PENSUREs.
		defines
		{
			"PASTEL_DEBUG_MODE", 
			"PASTEL_ENABLE_PENSURES",
		}

	configuration "release*"
		-- Release libraries do not have a suffix.
		targetsuffix ""
		-- Enable optimizations.
		flags {"Optimize"}

	-- Determine the SDL library name.	
	sdlLibrary = "SDL"
	
	-- Determine the OpenGL library name.
	openGlLibrary = "GL"
	if os.get() == "windows" then
		-- Note the same file works for both
		-- 32bit and 64bit development. The
		-- naming is historical.
		openGlLibrary = "opengl32"
	end
	
	-- Determine the Glew library name.
	-- Note the same file works for both
	-- 32bit and 64bit development. The
	-- naming is historical.
	glewLibrary = "glew32"
	
	includeDirectorySet = 
	{
		"./",
		boostIncludeDir,
		sdlIncludeDir,
		glewIncludeDir
	}
	
	libraryDirectorySet =
	{
		sdlLibraryDir,
		glewLibraryDir
	}
	
	fileSet = 
	{
		"*.cpp",
		"*.hpp",
		"*.h"
	}

	-- Visual Studio specific build options
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
		
	-- GCC specific build options.
	configuration "gmake"
		-- Enables some additional warnings.
		buildoptions { "-Wall" }
		-- Disable some warnings.
		buildoptions 
		{ 
			-- Pragma warnings caused by OpenMP support not being enabled.
			"-Wno-unknown-pragmas", 
			-- Comparison between an unsigned and a signed integer.
			"-Wno-sign-compare", 
			-- Conversion between an unsigned and a signed integer.
			"-Wno-sign-conversion",
			-- Breaking strict aliasing rules.
			"-Wno-strict-aliasing",
			-- Compiler warns that it optimizes code based on the 
			-- assumption that signed integer overflows do not occur.
			"-Wno-strict-overflow"
		}
		
	-- Enable OpenMP if requested

	configuration "release"
		defines { "PASTEL_ENABLE_OMP" }

	configuration { "vs*",  "release" }
		buildoptions { "/openmp" }		

	configuration { "gmake",  "release" }
		buildoptions { "-fopenmp" }
		links { "gomp" }

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
			sdlLibrary,
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
			sdlLibrary,
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
			sdlLibrary,
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
			sdlLibrary,
			"PastelSys"
		}

	project "ConvexHull"
		kind("ConsoleApp")
		includedirs(includeDirectorySet)
		libdirs(libraryDirectorySet)
		files(addPrefix("example/ConvexHull/", fileSet))
		links 
		{
			"PastelDevice",
			sdlLibrary,
			openGlLibrary,
			glewLibrary,
			"PastelGl",
			"PastelGfxUi",
			"PastelGfx",
			"PastelDsp",
			"PastelGeometry",
			"PastelMath",
			"PastelSys"
		}

	project "LeastSquares"
		kind("ConsoleApp")
		includedirs(includeDirectorySet)
		libdirs(libraryDirectorySet)
		files(addPrefix("example/LeastSquares/", fileSet))
		links 
		{
			"PastelDevice",
			sdlLibrary,
			openGlLibrary,
			glewLibrary,
			"PastelGl",
			"PastelGfxUi",
			"PastelGfx",
			"PastelDsp",
			"PastelGeometry",
			"PastelMath",
			"PastelSys"
		}

	project "NearestNeighbour"
		kind("ConsoleApp")
		includedirs(includeDirectorySet)
		libdirs(libraryDirectorySet)
		files(addPrefix("example/NearestNeighbour/", fileSet))
		links 
		{
			"PastelDevice",
			sdlLibrary,
			openGlLibrary,
			glewLibrary,
			"PastelGl",
			"PastelGfxUi",
			"PastelGfx",
			"PastelDsp",
			"PastelGeometry",
			"PastelMath",
			"PastelSys"
		}
