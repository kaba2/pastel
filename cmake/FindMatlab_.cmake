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

EcCheckPathExists("Matlab (include)" "${MatlabIncludeDirectory}")

include_directories (${MatlabIncludeDirectory})

