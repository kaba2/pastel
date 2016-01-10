# Description: Tbb configuration
# Documentation: building.txt

# returns
# -------
#
# TbbIncludeDirectory:
#    A directory to add to include directories, such that
#    #include <tbb/blocked_range.h>
#    becomes valid.
#
# TbbLibraryPath:
#    Path to the Tbb library.
#
# TbbLibraryDirectory:
#    The directory-part of ${ArmadilloLibraryPath}.

CheckPathExists("Tbb (include)" "${TbbIncludeDirectory}")
CheckPathExists("Tbb (library)" "${TbbLibraryPath}")

include_directories (${TbbIncludeDirectory})

