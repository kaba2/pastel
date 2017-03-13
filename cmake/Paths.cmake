# Description: Manual paths for external libraries
# Documentation: building.txt

# Manual overrides for paths under Windows
# ----------------------------------------

# Under Windows, the paths to external libraries need to 
# be specified manually. 

if (WIN32)
	# Ranges-v3 path
	set (RangesDirectory "${CMAKE_SOURCE_DIR}/../ranges-v3")

	# Boost path
	set (BoostDirectory "${CMAKE_SOURCE_DIR}/../boost_1_63_0")

	# Armadillo path
	set (ArmadilloDirectory "${CMAKE_SOURCE_DIR}/../armadillo-7.700.0")

	# Threading Building Blocks paths
	set (TbbDirectory "${CMAKE_SOURCE_DIR}/../tbb2017_20161128oss")

	# Blas library path
	set (BlasLibraryPath 
		"${ProjectDirectory}/external/${GENERATOR_BITS}/blas_win${GENERATOR_BITS}_MT.lib")

	# Lapack library path
	set (LapackLibraryPath 
		"${ProjectDirectory}/external/${GENERATOR_BITS}/lapack_win${GENERATOR_BITS}_MT.lib")

	# Matlab paths
	if (${GENERATOR_BITS} EQUAL 32)
		set (MatlabDirectory "C:/Program Files (x86)/MATLAB/R2016a")
	else()
		set (MatlabDirectory "C:/Program Files/MATLAB/R2016a")
	endif()
endif()

# Manual overrides for paths under Linux and Mac OS X
# ---------------------------------------------------

# Under Linux and Mac OS X, most of the libraries can be found
# automatically. However, some of the paths have to be specified
# manually, such as Matlab and Threading Building Blocks.

if (UNIX)
	# Ranges-v3 path
	set (RangesDirectory "${CMAKE_SOURCE_DIR}/../ranges-v3")

	# Boost path
	set (BoostDirectory "")

	# Armadillo path
	set (ArmadilloDirectory "")

	# Threading Building Blocks paths
	set (TbbDirectory "/usr")

	# Blas path
	set (BlasLibraryPath "")

	# Lapack path
	set (LapackLibraryPath "")

	# Matlab paths
	if (APPLE)
		set (MatlabDirectory "/Applications/MATLAB_R2015a.app")
	else()
		set (MatlabDirectory "/usr/local/matlab/r2015a")
	endif()
endif()

