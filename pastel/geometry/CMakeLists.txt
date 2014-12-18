project (PastelGeometry)

file (GLOB_RECURSE SourceSet ${PastelSourceGlobSet})

foreach (FilePath ${SourceSet})
	# Get the path to the source file, relative to the current directory.
    file (RELATIVE_PATH FileRelativePath ${CMAKE_CURRENT_LIST_DIR} ${FilePath})

    # Append / to the beginning, so that the regex-replacement
    # works also in the current directory.
    set (FileRelativePath "/${FileRelativePath}")

    # Get the directory-part of the path.
    # I could not find a way for specifying a non-capturing group, 
    # so I opted to append the / to the beginning, and then do
    # the following.
    string (REGEX REPLACE "(.*/)[^/]*$" "\\1" DirectoryRelativePath ${FileRelativePath})

    # Replace / with \.
    string (REPLACE "/" "\\" SourceGroupName ${DirectoryRelativePath})

    #message (STATUS ${FileRelativePath})
    #message (STATUS ${DirectoryRelativePath})
    #message (STATUS ${SourceGroupName})

    # Create a source group.
    source_group(${SourceGroupName} FILES ${FilePath})
endforeach()

add_library (pastelgeometry ${PastelLibraryType} ${SourceSet})

target_link_libraries (
	pastelgeometry 
	pastelsys 
	pastelmath
)

