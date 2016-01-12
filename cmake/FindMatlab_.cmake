# Description: Matlab configuration
# Documentation: building.txt

# Arguments
# ---------
#
# MatlabDirectory (string):
#    Matlab's installation directory.
# 
# returns
# -------
#
# MatlabIncludeDirectory (string):
#    A directory to add to include directories, such that
#    #include <mex.h>
#    becomes valid.

set (MatlabIncludeDirectory "${MatlabDirectory}/extern/include")

EcCheckPathExists("Matlab (include)" "${MatlabIncludeDirectory}")


