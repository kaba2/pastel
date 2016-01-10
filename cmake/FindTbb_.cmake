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

EcCheckPathExists("Tbb (include)" "${TbbIncludeDirectory}")
EcCheckPathExists("Tbb (library)" "${TbbLibraryPath}")

include_directories (${TbbIncludeDirectory})

