# Description: Boost configuration
# Documentation: building.txt

# Arguments
# ---------
#
# BoostIncludeDirectory (string):
#     Overrides Boost include directory manually,
#     when a non-empty string.
#
# returns
# -------
#
# BoostIncludeDirectory (string):
#    A directory to add to include directories, such that
#    #include <boost/type_traits/remove_ref.h>
#    becomes valid.

# Uncomment this to debug CMake's Boost search.
#set (Boost_DEBUG 1)

set (BoostVersion 1.55.0)
string (REGEX REPLACE \\. _ BoostVersion_ ${BoostVersion})

if (("${BoostIncludeDirectory}" STREQUAL "") OR (NOT EXISTS "${BoostIncludeDirectory}"))
	# Try to find Boost automatically.
	find_package(Boost ${BoostVersion})
	if (Boost_FOUND)
		# The found path allows includes of the form:
		# boost/type_traits/remove_ref.h
		set (BoostIncludeDirectory "${Boost_INCLUDE_DIRS}")
	endif()
endif()

CheckPathExists("Boost (include)" "${BoostIncludeDirectory}")

include_directories (${BoostIncludeDirectory})

# Make the special functions in Boost return infinity on overflow
# (rather than throw an exception).
add_definitions(-DBOOST_MATH_OVERFLOW_ERROR_POLICY=ignore_error)
