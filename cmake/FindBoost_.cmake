# Description: Boost configuration
# Documentation: building.txt

# Optional arguments
# ------------------
#
# BoostDirectory (string):
#    The directory where Boost resides. This provides
#    a way to specify the directory manually (e.g. Windows).
#
# returns
# -------
#
# BoostDirectory (string):
#    The directory where Boost resides. Will only
#    be modified if non-empty or non-existing.
#
# BoostIncludeDirectory (string):
#    A directory to add to include directories, such that
#    #include <boost/type_traits/remove_ref.h>
#    becomes valid.

# Uncomment this to debug CMake's Boost search.
#set (Boost_DEBUG 1)

set (BoostMinimumVersion 1.55.0)
#string (REGEX REPLACE \\. _ BoostVersion_ ${BoostVersion})

if (("${BoostDirectory}" STREQUAL "") OR (NOT EXISTS "${BoostDirectory}"))
	# Try to find Boost automatically.
	find_package(Boost ${BoostMinimumVersion})
	if (Boost_FOUND)
		# The found path allows includes of the form:
		# boost/type_traits/remove_ref.h
		set (BoostDirectory "${Boost_INCLUDE_DIRS}")
	endif()
endif()

set (BoostIncludeDirectory "${BoostDirectory}")

EcCheckPathExists("Boost (include)" "${BoostIncludeDirectory}")

include_directories (${BoostIncludeDirectory})

# Make the special functions in Boost return infinity on overflow
# (rather than throw an exception).
add_definitions(-DBOOST_MATH_OVERFLOW_ERROR_POLICY=ignore_error)
