set (MatlabDirectoryWindows "C:/Program Files/Polyspace/R2020a")
set (MatlabDirectoryMac "/Applications/MATLAB_R2020a.app")
set (MatlabDirectoryLinux "/mnt/c/Program Files/Polyspace/R2020a")

if (WIN32)
    set (MatlabDirectory ${MatlabDirectoryWindows})
elseif(UNIX)
    if (APPLE)
        set (MatlabDirectory ${MatlabDirectoryMac})
    else()
        set (MatlabDirectory ${MatlabDirectoryLinux})
    endif()
endif()

find_package("Matlab_")
include_directories (${MatlabIncludeDirectory})

EcAddLibrary (library pastelmatlab "${PastelSourceGlobSet}")

add_subdirectory (matlab)
