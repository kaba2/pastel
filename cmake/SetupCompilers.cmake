# Description: Compiler setup
# Documentation: building.txt

# Debug-mode definitions
# ----------------------

set (CMAKE_CXX_FLAGS_DEBUG 
	"${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG"
)

if (NOT BuildMatlabMex)
	# The flag _DEBUG enables additional debugging in the
	# C++ Standard Library under Visual Studio. Using it
	# requires the debug-mode C-runtime. However, BuildMatlabMex 
	# forces release-mode C-runtime.
	set (CMAKE_CXX_FLAGS_DEBUG 
		"${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG"
	)
endif()

# Clang and g++
# -------------

if ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU") OR 
	(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")) 

	if (NOT CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
		add_definitions (
			# Enables position-independent code.
			# This is needed to build the Matlab 
			# libraries.
			-fPIC
			# Enables some additional warnings.
			-Wall 
			# Stop build after one error.
			-Wfatal-errors
		)
	endif()

	# Disable some warnings.
	add_definitions (
		# Pragma warnings caused by OpenMP support not being enabled.
		-Wno-unknown-pragmas
		# Comparison between an unsigned and a signed integer.
		-Wno-sign-compare
		# Conversion between an unsigned and a signed integer.
		-Wno-sign-conversion
		# Unused variables.
		-Wno-unused-variable
		# Unused values.
		-Wno-unused-value
		# Unused functions.
		-Wno-unused-function
		# Breaking strict aliasing rules.
		-Wno-strict-aliasing
		# Compiler warns that it optimizes code based on the 
		# assumption that signed integer overflows do not occur.
		-Wno-strict-overflow
		# Compiler warns about code that is marked as deprecated.
		-Wno-deprecated-declarations
	)
endif()

# g++
# ---

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") 
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

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang") 

	if (NOT (CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC"))
		add_definitions (
			# Not sure why.
			-arch x86_64
			)
	endif()

	# Disable some warnings.
	add_definitions (
		# Eigen does bitwise operations between different enums
		-Wno-deprecated-anon-enum-enum-conversion
		# Eigen forward-declares inline functions with.
		-Wno-undefined-inline
		# volatile-qualified parameter type 'const volatile long long' is deprecated
		# These errors come from the TBB library.
		-Wno-deprecated-volatile
		-Wno-parentheses
		# Assigning objects to themselves is useful in testing.
		-Wno-self-assign-overloaded
		-Wno-self-move
		# Compiler warns 'that >= 0' is always true for an 
		# unsigned integer.
		-Wno-tautological-compare
		# Absolute value function on unsigned types.
		-Wno-absolute-value
		-Wno-unsupported-friend
		-Wno-unused-local-typedef
		-Wno-unused-variable
		# For conceptArg()
		-Wno-undefined-internal
	)
endif()

# Visual Studio
# -------------

if (MSVC)
	if (BuildMatlabMex)
		if (CMAKE_BUILD_TYPE MATCHES Debug) 
			# Force Visual Studio to use release-mode C and C++ standard libraries.
			# This is needed for Matlab, because its mex compilation flags only
			# include /MD; there is no debug version. Otherwise there will be LNK4098
			# about conflicting versions of the standard library.
			string(REPLACE "/MDd" "/MD" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
			string(REPLACE "/D_DEBUG" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})

			# Emit a warning about forcing release-mode libraries
			message (
				"BuildMatlabMex: Forcing release-mode C and C++ standard libraries "
				"in Visual Studio. Use this only when the intent is to make "
				"${CMAKE_PROJECT_NAME} usable for Matlab mex.")
		endif()
	endif()

	add_definitions (
		# Disable Microsoft's Secure STL.
		/D_ITERATOR_DEBUG_LEVEL=0
		# Embed debugging information into static libraries.
		/Z7
		# Use multiple processes for compiling.
		/MP
		# Enable exceptions.
		/EHsc
		# Increase number of sections in .obj file.
		/bigobj
		# Boost uses std::unary_function etc which are removed from C++17.
		# Bring them back.
		/D_HAS_AUTO_PTR_ETC=1
		# Disable language extensions
		/permissive-
	)

	# Disable some warnings.
	add_definitions (
		# '<<': result of 32-bit shift implicitly converted to 64 bits (was 64-bit shift intended?)
		/wd4334
		# 'expression' : signed/unsigned mismatch
		/wd4018
		# 'variable' : unreferenced formal parameter
		/wd4100
		# conditional expression is constant
		/wd4127
   		# unary minus operator applied to unsigned type, result still unsigned
   		/wd4146
		# "qualifier applied to function type has no meaning; ignored"
		/wd4180
		# "nonstandard extension used : formal parameter 'identifier' was previously defined as a type"
		/wd4224
		# "'argument' : conversion from 'type1' to 'type2', possible loss of data."
		/wd4244
		# "'return' : conversion from 'A' to 'B', signed/unsigned mismatch"
		/wd4245
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
		# decorated name length exceeded, name was truncated
		/wd4503		
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

if ((CMAKE_GENERATOR STREQUAL "NMake Makefiles") OR
	(CMAKE_GENERATOR STREQUAL "NMake Makefiles JOM"))
	add_definitions (
		# Enable exceptions (for some reason they are not 
		# enabled for NMake Makefiles).
		/EHsc
	)
endif()
