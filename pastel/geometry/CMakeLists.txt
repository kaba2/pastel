project (PastelGeometry)

file (GLOB_RECURSE SourceSet ${PastelSourceGlobSet})

file (GLOB_RECURSE DocumentationSet ${PastelDocumentationGlobSet})
source_group (Documentation FILES ${DocumentationSet})

foreach (FilePath ${SourceSet})
	# Get the path to the source file, relative to the current directory.
    file (RELATIVE_PATH FileRelativePath ${CMAKE_CURRENT_LIST_DIR} ${FilePath})
    set (FileRelativePath "code/${FileRelativePath}")

    #message (STATUS ${FileRelativePath})

    # Extract the folder, ie remove the filename part
    string (REGEX REPLACE "(.*)(/[^/]*)$" "\\1" DirectoryRelativePath ${FileRelativePath})

    #message (STATUS ${DirectoryRelativePath})

    # Replace / with \.
    string (REPLACE "/" "\\" SourceGroupName ${DirectoryRelativePath})

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

