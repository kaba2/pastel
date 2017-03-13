# Description: Ranges-v3 configuration
# Documentation: building.txt

# Optional arguments
# ------------------
#
# RangesDirectory (string):
#    The directory where Ranges resides. This provides
#    a way to specify the directory manually (e.g. Windows).
#
# returns
# -------
#
# RangesDirectory (string):
#    The directory where Ranges resides. Will only
#    be modified if non-empty or non-existing.
#
# RangesIncludeDirectory (string):
#    A directory to add to include directories, such that
#    #include <boost/type_traits/remove_ref.h>
#    becomes valid.

set (RangesIncludeDirectory "${RangesDirectory}/include")

EcCheckPathExists("Ranges (include)" "${RangesIncludeDirectory}")
