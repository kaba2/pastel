# This is a CMake build file for PastelDsp.

cmake_minimum_required(VERSION 2.8)

source_group(
"Filters"
FILES
box_filter.cpp
box_filter.h
cubic_filter.cpp
cubic_filter.h
filter.cpp
filter.h
filter.txt
filter_all.h
gaussian_filter.cpp
gaussian_filter.h
lanczos_filter.cpp
lanczos_filter.h
mitchell_filter.cpp
mitchell_filter.h
table_filter.cpp
table_filter.h
triangle_filter.cpp
triangle_filter.h
)

source_group(
"Precomputed filtering"
FILES
precomputed_filtering.txt
)

source_group(
"Precomputed filtering\\MipMap"
FILES
mipmap.h
mipmap.hpp
mipmap.txt
mipmap_tools.h
mipmap_tools.hpp
)

source_group(
"Precomputed filtering\\RipMap"
FILES
ripmap.h
ripmap.hpp
ripmap.txt
ripmap_tools.h
ripmap_tools.hpp
)

source_group(
"Resampling"
FILES
resample.h
resample.hpp
resample.txt
resample_optimal_order.txt
)

source_group(
"Miscellaneous"
FILES
discretefourier.h
discretefourier.hpp
dsplibrary.cpp
dsplibrary.h
miscellaneous.txt
reconstructimage.h
reconstructimage.hpp
)

source_group(
"Deprecated"
FILES
filterdesign.cpp
filterdesign.h
filterform.h
filterform.hpp
)

set(PastelDsp_Files
box_filter.cpp
box_filter.h
cubic_filter.cpp
cubic_filter.h
discretefourier.h
discretefourier.hpp
dsplibrary.cpp
dsplibrary.h
filter.cpp
filter.h
filter.txt
filter_all.h
filterdesign.cpp
filterdesign.h
filterform.h
filterform.hpp
gaussian_filter.cpp
gaussian_filter.h
lanczos_filter.cpp
lanczos_filter.h
mipmap.h
mipmap.hpp
mipmap.txt
mipmap_tools.h
mipmap_tools.hpp
miscellaneous.txt
mitchell_filter.cpp
mitchell_filter.h
pasteldsp.txt
precomputed_filtering.txt
reconstructimage.h
reconstructimage.hpp
resample.h
resample.hpp
resample.txt
resample_optimal_order.txt
ripmap.h
ripmap.hpp
ripmap.txt
ripmap_tools.h
ripmap_tools.hpp
table_filter.cpp
table_filter.h
triangle_filter.cpp
triangle_filter.h
)

include_directories(${Pastel_SOURCE_DIR})
link_directories(${Pastel_Binary_Dir}/lib)
add_library(PastelDsp ${PastelDsp_Files})
