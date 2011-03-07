-- This is a Premake script for producing
-- build files for the Pastel library.

-- Main build switches
-- ===================

-- Whether to build the actual libraries.
buildBasic = true

-- Whether to build the test projects.
buildTests = true

-- Whether to build the examples.
buildExamples = true

-- Whether you have Matlab (say, 2008a or never).
gotMatlab = true

-- Whether you have Glew 1.5.7.
gotGlew = true

-- Whether you have SDL 1.2.
gotSdl = true

-- Detailed build switches
-- =======================

-- Basic set.
-- Require Boost.
buildPastelDsp = true and buildBasic
buildPastelMath = true and buildBasic
buildPastelGeometry = true and buildBasic
buildPastelGfx = true and buildBasic
buildPastelRay = true and buildBasic
buildPastelSys = true and buildBasic

-- Requires Glew and Boost.
buildPastelGl = true and buildBasic and gotGlew

-- Requires SDL and Boost.
buildPastelDevice = true and buildBasic and gotSdl
buildPastelGfxUi = true and buildBasic and gotSdl

-- Require Matlab.
buildPastelGeometryMatlab = true and buildBasic and gotMatlab
buildPastelMatlab = true and buildBasic and gotMatlab

-- Require SDL and Boost.
buildPastelSysTest = true and buildTests and gotSdl
buildPastelRayTest = true and buildTests and gotSdl
buildPastelMathTest = true and buildTests and gotSdl
buildPastelGfxTest = true and buildTests and gotSdl
buildPastelGeometryTest = true and buildTests and gotSdl
buildPastelDspTest = true and buildTests and gotSdl
buildPastelDeviceTest = true and buildTests and gotSdl

-- Require SDL and Boost and Glew.
buildLeastSquares = true and buildExamples and gotGlew and gotSdl
buildNearestNeighbour = true and buildExamples and gotGlew and gotSdl
buildConvexHull = true and buildExamples and gotGlew and gotSdl

-- Paths
-- =====

-- Change the following directories to reflect your own
-- build environment. If a library is not used, the path
-- is ignored.

-- The directory of the Boost library's source code.
-- The includes are of the form 'boost/static_assert.hpp'.
boostIncludeDir = "../external/boost_1_45_0"

-- The directory of the SDL library's header files.
-- The includes are of the form 'SDL.h'.
sdlIncludeDir = "../external/SDL-1.2.14/include"
sdlLibraryDir = "../external/SDL-1.2.14/lib"

-- The directory of the GLEW library's header files.
-- The includes are of the form 'glew.h'.
glewIncludeDir = "../external/glew-1.5.7/include"
glewLibraryDir = "../external/glew-1.5.7/lib"

-- The directory of the Matlab header files.
-- The includes are of the form 'mex.h'.
matlabIncludeDir = "C:/Program Files/MATLAB/R2008a/extern/include"

-- No need to give a library path for Matlab:
-- Mex files are built from within Matlab.

outputDirectory = "build/" .. _ACTION

solution "Pastel"

	language "C++"
	platforms
	{
		"native",
		"x32",
		"x64"
	}

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
	glewLibrary = "GLEW"
	if os.get() == "windows" then
		-- Note the same file works for both
		-- 32bit and 64bit development. The
		-- naming is historical.
		glewLibrary = "glew32"
	end
	
	includeDirectorySet = 
	{
		"./",
		boostIncludeDir,
		sdlIncludeDir,
		glewIncludeDir,
		matlabIncludeDir
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

	if buildPastelSys
	then
		project "PastelSys"
			kind(libKind)
			includedirs(includeDirectorySet)
			libdirs(libraryDirectorySet)
			files(addPrefix("pastel/sys/", fileSet))
	end
	
	if buildMatlabStuff 
	then
		project "PastelMatlab"
			kind(libKind)
			includedirs(includeDirectorySet)
			libdirs(libraryDirectorySet)
			files(addPrefix("pastel/matlab/", fileSet))
	end

	if buildPastelMath
	then
		project "PastelMath"
			kind(libKind)
			includedirs(includeDirectorySet)
			libdirs(libraryDirectorySet)
			files(addPrefix("pastel/math/", fileSet))
	end
		
	if buildPastelDevice
	then
	project "PastelDevice"
		kind(libKind)
		includedirs(includeDirectorySet)
		libdirs(libraryDirectorySet)
		files(addPrefix("pastel/device/", fileSet))
	end
		
	if buildPastelGeometry
	then
		project "PastelGeometry"
			kind(libKind)
			includedirs(includeDirectorySet)
			libdirs(libraryDirectorySet)
			files(addPrefix("pastel/geometry/", fileSet))
	end

	if buildPastelGeometryMatlab
	then
		project "PastelGeometryMatlab"
			kind(libKind)
			includedirs(includeDirectorySet)
			libdirs(libraryDirectorySet)
			files(addPrefix("pastel/geometrymatlab/", fileSet))
	end

	if buildPastelDsp
	then
		project "PastelDsp"
			kind(libKind)
			includedirs(includeDirectorySet)
			libdirs(libraryDirectorySet)
			files(addPrefix("pastel/dsp/", fileSet))
	end

	if buildPastelGfx
	then
		project "PastelGfx"
			kind(libKind)
			includedirs(includeDirectorySet)
			libdirs(libraryDirectorySet)
			files(addPrefix("pastel/gfx/", fileSet))
	end

	if buildPastelRay
	then
		project "PastelRay"
			kind(libKind)
			includedirs(includeDirectorySet)
			libdirs(libraryDirectorySet)
			files(addPrefix("pastel/ray/", fileSet))
	end

	if buildPastelGl
	then
		project "PastelGl"
			kind(libKind)
			includedirs(includeDirectorySet)
			libdirs(libraryDirectorySet)
			files(addPrefix("pastel/gl/", fileSet))
	end

	if buildPastelGfxUi
	then
		project "PastelGfxUi"
			kind(libKind)
			includedirs(includeDirectorySet)
			libdirs(libraryDirectorySet)
			files(addPrefix("pastel/gfxui/", fileSet))
	end

	if buildPastelSysTest
	then
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
	end
	
	if buildPastelRayTest
	then
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
	end

	if buildPastelMathTest
	then
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
	end

	if buildPastelGfxTest
	then
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
	end

	if buildPastelGeometryTest
	then
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
	end

	if buildPastelDspTest
	then
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
	end

	if buildPastelDeviceTest
	then
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
	end

	if buildConvexHull
	then
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
	end

	if buildLeastSquares
	then
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
	end

	if buildNearestNeighbour
	then
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
	end
