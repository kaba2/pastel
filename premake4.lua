-- Description: Premake build files for the Pastel library
-- Documentation: building.txt

-- Available external libraries
-- ============================

-- Whether you have Matlab include files 
-- (say, for 2008a or newer). Note: Binaries are not
-- needed since the linking is done from Matlab.
gotMatlab = true

-- Whether you have Glew 1.7.0 include files and binaries.
gotGlew = true

-- Whether you have SDL 1.2.15 include files and binaries.
gotSdl = true

-- Whether you have Boost 1.49 include files.
gotBoost = true

-- Note: To succesfully _compile_ the libraries, 
-- you only need the header files for the external
-- libraries. This allows you to try the compilation
-- even if you do not have the external library
-- binaries. It is only in the _linking_ phase of 
-- executables and shared libraries (i.e. tests and 
-- examples) where the binaries are needed .

-- Main build switches
-- ===================

-- Whether to build the libraries.
buildLibraries = true

-- Whether to build the test projects.
buildTests = true

-- Whether to build the examples.
buildExamples = true

-- Whether to build dynamic libraries (vs static libraries)
wantDynamicLibraries = false

-- Whether the 'integer'-type should be as large as a pointer (vs 32-bit).
wantLargeIntegers = false

-- Detailed build switches
-- =======================

-- Requirements
basicRequirements = gotBoost
libraryRequirements = basicRequirements and buildLibraries
testRequirements = basicRequirements and buildTests and gotSdl
exampleRequirements = basicRequirements and buildExamples and gotGlew and gotSdl

-- Sub-libraries
buildPastelDsp = true and libraryRequirements
buildPastelMath = true and libraryRequirements
buildPastelGeometry = true and libraryRequirements
buildPastelGfx = true and libraryRequirements
buildPastelRay = true and libraryRequirements
buildPastelSys = true and libraryRequirements

buildPastelGl = true and libraryRequirements and gotGlew

buildPastelDevice = true and libraryRequirements and gotSdl
buildPastelGfxUi = true and libraryRequirements and gotSdl

buildPastelGeometryMatlab = true and libraryRequirements and gotMatlab
buildPastelMatlab = true and libraryRequirements and gotMatlab

-- Tests
buildPastelSysTest = true and testRequirements
buildPastelRayTest = true and testRequirements
buildPastelMathTest = true and testRequirements
buildPastelGfxTest = true and testRequirements
buildPastelGeometryTest = true and testRequirements
buildPastelDspTest = true and testRequirements
buildPastelDeviceTest = true and testRequirements

-- Examples
buildLeastSquares = true and exampleRequirements
buildNearestNeighbour = true and exampleRequirements
buildConvexHull = true and exampleRequirements

-- Paths
-- =====

-- Change the following directories to reflect your own
-- build environment. If a library is not used, the path
-- is ignored.

-- The directory of the Boost library's source code.
-- The includes are of the form 'boost/static_assert.hpp'.
boostIncludeDir = "../boost_1_49_0"

-- The directory of the SDL library's header files.
-- The includes are of the form 'SDL.h'.
sdlIncludeDir = "../SDL-1.2.15/include"
sdlLibraryDir = "../SDL-1.2.15/lib"

-- The directory of the GLEW library's header files.
-- The includes are of the form 'glew.h'.
glewIncludeDir = "../glew-1.7.0-64bit/include"
glewLibraryDir = "../glew-1.7.0-64bit/lib"

-- The directory of the Matlab header files.
-- The includes are of the form 'mex.h'.
matlabIncludeDir = "C:/Program Files/MATLAB/R2011b/extern/include"

-- No need to give a library path for Matlab:
-- Mex files are built from within Matlab.

if (_ACTION == nil) then
   return
end

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
		targetdir(outputDirectory .. "/lib/debug")
		-- Enable debug information.
		flags {"Symbols"}
		-- Enable ASSERTs and PENSUREs.
		defines 
		{
			"PASTEL_DEBUG_MODE", 
			"PASTEL_ENABLE_PENSURES"
		}
		
	configuration "develop"
		targetdir(outputDirectory .. "/lib/develop")
		-- Enable optimizations.
		flags {"Optimize"}
		-- Enable ASSERTs and PENSUREs.
		defines
		{
			"PASTEL_DEBUG_MODE", 
			"PASTEL_ENABLE_PENSURES",
		}

	configuration "release"
		targetdir(outputDirectory .. "/lib/release")
		-- Enable optimizations.
		flags {"Optimize"}

	configuration "release-without-openmp"
		targetdir(outputDirectory .. "/lib/release-without-openmp")
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
			"_HAS_ITERATOR_DEBUGGING=0"
		}
		
	-- GCC specific build options.
	configuration "gmake"
		-- Needed on 64-bit platforms to be able
		-- to link static libraries to shared libraries.
		buildoptions { "-fPIC" }
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

	-- Enable dynamic libraries if requested

	if wantDynamicLibraries
	then
		configuration "*"
			defines { "PASTEL_DYNAMIC_LIBRARIES" }
	end
		
	-- Enable large integers if requested

	if wantLargeIntegers
	then
		configuration "*"
			defines { "PASTEL_LARGE_INTEGERS" }
	end

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
	
	if buildPastelMatlab
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
