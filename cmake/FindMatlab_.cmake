# Description: Matlab configuration
# Documentation: building.txt

# returns
# -------
#
# MatlabIncludeDirectory:
#    A directory to add to include directories, such that
#    #include <mex.h>
#    becomes valid.
#
# MatlabDirectory:
#    Matlab's installation directory.

if (WIN32)
	if (${GENERATOR_BITS} EQUAL 32)
		set (MatlabDirectory "C:/Program Files (x86)/MATLAB/R2015a")
	else()
		set (MatlabDirectory "C:/Program Files/MATLAB/R2015a")
	endif()

	set (MatlabIncludeDirectory "${MatlabDirectory}/extern/include")
endif()

if (UNIX)
	if (APPLE)
		set (MatlabDirectory "/Applications/MATLAB_R2015a.app")
	else()
		set (MatlabDirectory "/usr/local/matlab/r2015a")
	endif()

	set (MatlabIncludeDirectory "${MatlabDirectory}/extern/include")
endif()

CheckPathExists("Matlab (include)" "${MatlabIncludeDirectory}")

include_directories (${MatlabIncludeDirectory})

