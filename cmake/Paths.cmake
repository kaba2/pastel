# Description: Manual paths for external libraries
# Documentation: building.txt

# Manual overrides for paths under Windows
# ----------------------------------------

# Under Windows, the paths to external libraries need to 
# be specified manually. 

if (WIN32)
	# Boost paths
	set (BoostIncludeDirectory "C:/code/boost_1_59_0")

	# Armadillo paths
	set (ArmadilloIncludeDirectory "C:/code/armadillo-5.200.1/include")
	set (ArmadilloLibraryDirectory "")
	set (ArmadilloLibraryPath "")

	# Blas paths
	set (BlasLibraryName "blas_win${GENERATOR_BITS}_MT")
	set (BlasLibraryDirectory "${ProjectDirectory}/external/${GENERATOR_BITS}")
	set (BlasLibraryPath "${BlasLibraryDirectory}/${BlasLibraryName}.lib")
	set (DllSet 
		${DllSet}
		"${BlasLibraryDirectory}/${BlasLibraryName}.dll")

	# Lapack paths
	set (LapackLibraryName "lapack_win${GENERATOR_BITS}_MT")
	set (LapackLibraryDirectory "${ProjectDirectory}/external/${GENERATOR_BITS}")
	set (LapackLibraryPath "${LapackLibraryDirectory}/${LapackLibraryName}.lib")
	set (DllSet 
		${DllSet}
		"${LapackLibraryDirectory}/${LapackLibraryName}.dll")

	# Matlab paths
	if (${GENERATOR_BITS} EQUAL 32)
		set (MatlabDirectory "C:/Program Files (x86)/MATLAB/R2015a")
	else()
		set (MatlabDirectory "C:/Program Files/MATLAB/R2015a")
	endif()

	set (MatlabIncludeDirectory "${MatlabDirectory}/extern/include")

	# Threading Building Blocks paths
	set (TbbDirectory "C:/code/tbb-4.4")
	set (TbbIncludeDirectory "${TbbDirectory}/include")
	set (TbbLibraryName "tbb")

	if (${GENERATOR_BITS} EQUAL 32)
		set (TbbLibraryDirectory "${TbbDirectory}/lib/ia32/vc12")
	else()
		set (TbbLibraryDirectory "${TbbDirectory}/lib/intel64/vc12")
	endif()

	set (TbbLibraryPath "${TbbLibraryDirectory}/${TbbLibraryName}.lib")
endif()

# Manual overrides for paths under Linux and Mac OS X
# ---------------------------------------------------

# Under Linux and Mac OS X, most of the libraries can be found
# automatically. However, some of the paths have to be specified
# manually, such as Matlab.

if (UNIX)
	# Boost paths
	set (BoostIncludeDirectory "")

	# Armadillo paths
	set (ArmadilloIncludeDirectory "")
	set (ArmadilloLibraryDirectory "")
	set (ArmadilloLibraryPath "")

	# Blas paths
	set (BlasLibraryName "")
	set (BlasLibraryPath "")

	# Lapack paths
	set (LapackLibraryName "")
	set (LapackLibraryPath "")

	# Matlab paths
	if (APPLE)
		set (MatlabDirectory "/Applications/MATLAB_R2015a.app")
	else()
		set (MatlabDirectory "/usr/local/matlab/r2015a")
	endif()

	set (MatlabIncludeDirectory "${MatlabDirectory}/extern/include")

	# Threading Building Blocks paths
	set (TbbDirectory "/usr")
	set (TbbIncludeDirectory "${TbbDirectory}/include")
	set (TbbLibraryName "tbb")

	if (APPLE)
		set (TbbLibraryDirectory "${TbbDirectory}/local/lib")
		set (TbbLibraryPath "${TbbLibraryDirectory}/lib${TbbLibraryName}.dylib")
	else()
		set (TbbLibraryDirectory "${TbbDirectory}/lib")
		set (TbbLibraryPath "${TbbLibraryDirectory}/lib${TbbLibraryName}.so")
	endif()
endif()

