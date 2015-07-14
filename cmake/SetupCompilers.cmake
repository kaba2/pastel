# Debug-mode definitions.
set (CMAKE_CXX_FLAGS_DEBUG 
	"${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG"
)

# Clang and g++
# -------------

if ((${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU") OR 
	(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")) 
	add_definitions (
		# Enables C++14 compiler support.
		-std=c++1y 
		# Enables some additional warnings.
		-Wall 
		# Enables position-independent code.
		# This is needed to build the Matlab 
		# libraries.
		#-fPIC
		# Stop build after one error.
 		-Wfatal-errors
	)

	# Disable some warnings.
	add_definitions (
		-Wno-parentheses
		# Pragma warnings caused by OpenMP support not being enabled.
		-Wno-unknown-pragmas
		# Comparison between an unsigned and a signed integer.
		-Wno-sign-compare
		# Conversion between an unsigned and a signed integer.
		-Wno-sign-conversion
		# Unused variables.
		#-Wno-unused-variable
		# Unused values.
		-Wno-unused-value
		# Unused functions.
		-Wno-unused-function
		# Breaking strict aliasing rules.
		-Wno-strict-aliasing
		# Compiler warns that it optimizes code based on the 
		# assumption that signed integer overflows do not occur.
		-Wno-strict-overflow
	)
endif()

# g++
# ---

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU") 
	# Disable some warnings.
	add_definitions (
		# Unused but set variable.
		-Wno-unused-but-set-variable
		# Unused local typedef
		-Wno-unused-local-typedefs
		-Wno-deprecated-declarations
	)
endif()

# Clang
# -----

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang") 
	add_definitions (
		# Enables C++14 library support.
		-stdlib=libc++
		# Not sure why.
		-arch x86_64
	)

	# Enables C++11 linker support.
	set (CMAKE_EXE_LINKER_FLAGS 
		"${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
	set (CMAKE_SHARED_LINKER_FLAGS 
		"${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")

	# Disable some warnings.
	add_definitions (
		# Compiler warns 'that >= 0' is always true for an 
		# unsigned integer.
		-Wno-tautological-compare
		# Absolute value function on unsigned types.
		-Wno-absolute-value
		-Wno-unsupported-friend
	)
endif()

# Visual Studio
# -------------

if (MSVC)
	# Do not add ZERO_CHECK project into the Visual Studio solution.
	# From VS2008 to VS2013, the ZERO_CHECK project is always out
	# of date, which causes the Visual Studio to ask at every build
	# whether ZERO_CHECK should be built, although nothing was changed.
	# The purpose of the ZERO_CHECK project is to check whether there
	# are changes to the CMake files themselves, and to regenerate the
	# project files if so. But even then the projects are regenerated
	# during the build, and they need to be reloaded, and that is not 
	# very smooth. It is better to suppress this feature and to 
	# regenerate the project files manually whenever the CMake files
	# are changed.
	set(CMAKE_SUPPRESS_REGENERATION TRUE)

	if (BuildMatlabMex)
		# Force Visual Studio to use release-mode C and C++ standard libraries.
		# This is needed for Matlab, because otherwise there will be LNK4098
		# about conflicting versions of the standard library.
		string(REPLACE "/MDd" "/MD" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
		string(REPLACE "/D_DEBUG" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
	endif()

	add_definitions (
		# Disable Microsoft's Secure STL.
		/D_ITERATOR_DEBUG_LEVEL=0
		# Use multiple processes for compiling.
		/MP
	)

	# Disable some warnings.
	add_definitions (
		# "'expression' : signed/unsigned mismatch"
		/wd4018 
   		# unary minus operator applied to unsigned type, result still unsigned
   		/wd4146
		# "qualifier applied to function type has no meaning; ignored"
		/wd4180
		# "nonstandard extension used : formal parameter 'identifier' was previously defined as a type"
		/wd4224
		# "'argument' : conversion from 'type1' to 'type2', possible loss of data."
		/wd4244
		# "'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'"
		/wd4251 
		# "'var' : conversion from 'size_t' to 'type', possible loss of data"
		/wd4267
		# "'identifier' : truncation from 'type1' to 'type2'"
		/wd4305
		#  integral constant overflow
		/wd4307
		# "'operation' : conversion from 'type1' to 'type2' of greater size"
		/wd4312
		# "new behavior: elements of array 'array' will be default initialized"
		/wd4351
		# declaration of 'variable' hides previous local declaration
		/wd4456
		# declaration of 'variable' hides function parameter
		/wd4457
		# declaration of 'type' hides class member
		/wd4458
		# declaration of 'type' hides global declaration
		/wd4459
		# 'type' : base-class 'type2' is already a base-class of 'type3'
		/wd4584
		# "'type' : forcing value to bool 'true' or 'false' (performance warning)"
		/wd4800 
		# "'operation' : unsafe use of type 'bool' in operation"
   		/wd4804
		# "'function': was declared deprecated" (referring to STL functions)
		/wd4996
	)

endif()

if (${CMAKE_GENERATOR} STREQUAL "NMake Makefiles" OR
	${CMAKE_GENERATOR} STREQUAL "NMake Makefiles JOM")
	add_definitions (
		# Enable exceptions (for some reason they are not 
		# enabled for NMake Makefiles).
		/EHsc
	)
endif()
