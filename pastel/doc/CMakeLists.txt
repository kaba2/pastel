project (pasteldoc)

# Gather the documentation at the root directory.
file (GLOB RootDocumentationSet ../../*.txt)

set (SourceSet pasteldoc.cpp ${RootDocumentationSet})
source_group (Code FILES ${SourceSet})

source_group (Documentation FILES ${RootDocumentationSet})

add_library (pasteldoc ${PastelLibraryType} ${SourceSet})
