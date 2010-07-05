# This is a CMake build file for PastelGfx.

cmake_minimum_required(VERSION 2.8)

source_group(
"Color"
FILES
color.h
color.txt
color_tools.cpp
color_tools.h
color_tools.hpp
)

source_group(
"Color\\Color spaces"
FILES
color_hsv.cpp
color_hsv.h
color_lab.cpp
color_lab.h
color_lch.cpp
color_lch.h
color_lms.cpp
color_lms.h
color_space.h
color_space.txt
color_srgb.cpp
color_srgb.h
color_xyy.cpp
color_xyy.h
color_yuv.cpp
color_yuv.h
)

source_group(
"Color\\Color adapters"
FILES
coloradapter.cpp
coloradapter.h
coloradapter.txt
)

source_group(
"Color\\Color palettes"
FILES
color_palette.cpp
color_palette.h
color_palette.txt
)

source_group(
"Color\\Standard illuminants"
FILES
color_illuminant.cpp
color_illuminant.h
color_illuminant.hpp
color_illuminant.txt
)

source_group(
"Common"
FILES
common.txt
gfxlibrary.cpp
gfxlibrary.h
)

source_group(
"Common\\Image"
FILES
image_tools.cpp
image_tools.h
image_tools.hpp
)

source_group(
"2d drawing"
FILES
draw.h
draw.txt
draw_box.h
draw_box.hpp
draw_floodfill.h
draw_floodfill.hpp
draw_horizontal_line.h
draw_horizontal_line.hpp
draw_perspective_triangle.h
draw_perspective_triangle.hpp
draw_pixel.h
draw_pixel.hpp
draw_projective_quad.h
draw_projective_quad.hpp
draw_segment.h
draw_segment.hpp
draw_segment_notes.txt
draw_textured_box.h
draw_textured_box.hpp
draw_textured_triangle.h
draw_textured_triangle.hpp
draw_triangle.h
draw_triangle.hpp
draw_vertical_line.h
draw_vertical_line.hpp
draw_view.h
draw_view.hpp
marchingtriangles.h
)

source_group(
"Noise"
FILES
noise.h
noise.txt
)

source_group(
"Noise\\Gradient field"
FILES
gradientfield.h
gradientfield.hpp
gradientfield.txt
)

source_group(
"Noise\\Perlin noise"
FILES
perlin_noise.h
perlin_noise.hpp
perlin_noise.txt
)

source_group(
"Noise\\Simplex noise"
FILES
simplex_noise.h
simplex_noise.hpp
simplex_noise.txt
simplex_noise_notes.txt
)

source_group(
"Image files"
FILES
)

source_group(
"Image files\\Pcx"
FILES
loadpcx.cpp
loadpcx.h
loadpcx.hpp
pcx.h
Pcx.txt
pcxheader.cpp
pcxheader.h
savepcx.cpp
savepcx.h
savepcx.hpp
)

source_group(
"Textures"
FILES
texture.h
texture.txt
textures.h
)

source_group(
"Textures\\Modifier textures"
FILES
combine_texture.h
mix_texture.h
modifier_textures.txt
)

source_group(
"Textures\\Image textures"
FILES
image_textures.txt
)

source_group(
"Textures\\Image textures\\Rip-mapping"
FILES
ripimage_texture.h
ripimage_texture.hpp
ripimage_texture.txt
)

source_group(
"Textures\\Image textures\\Mip-mapping"
FILES
mipimage_texture.h
mipimage_texture.hpp
mipimage_texture.txt
)

source_group(
"Textures\\Image textures\\Linear"
FILES
linearimage_texture.h
linearimage_texture.hpp
linearimage_texture.txt
)

source_group(
"Textures\\Image textures\\Ewa"
FILES
ewaimage_texture.h
ewaimage_texture.hpp
ewaimage_texture.txt
)

source_group(
"Textures\\Image textures\\Nearest"
FILES
nearestimage_texture.h
nearestimage_texture.hpp
nearestimage_texture.txt
)

source_group(
"Textures\\Synthetic textures"
FILES
color_texture.h
linearcolor_texture.h
linearsimplex_texture.h
noise_texture.h
synthetic_textures.txt
turbulence_texture.h
)

source_group(
"Textures\\Distortion textures"
FILES
distortion_textures.txt
radial_texture.h
radial_texture.txt
sub_texture.h
transform_texture.h
transform_texture.txt
)

source_group(
"Reconstruction"
FILES
reconstruct_all.h
reconstruct_filter.h
reconstruct_filter.hpp
reconstruct_nearest.h
reconstruct_nearest.hpp
reconstruct_rbf.h
)

source_group(
"Discrete poisson disk pattern"
FILES
discretepoissondiskpattern.h
)

source_group(
"Probability distribution from image"
FILES
imagepdf.h
imagepdf.hpp
)

source_group(
"Rendering"
FILES
gfxrenderer.cpp
gfxrenderer.h
gfxrenderer.hpp
gfxrenderer_tools.cpp
gfxrenderer_tools.h
gfxrenderer_tools.hpp
renderer.txt
)

source_group(
"Rendering\\Image_GfxRenderer"
FILES
image_gfxrenderer.cpp
image_gfxrenderer.h
image_gfxrenderer.hpp
)

source_group(
"Not sure"
FILES
)

source_group(
"Not sure\\Color mixers"
FILES
additivecolormixer.h
assigncolormixer.h
colormixer.h
colormixers.h
functorcolormixer.h
transparentcolormixer.h
)

source_group(
"Not sure\\Image processing"
FILES
)

source_group(
"Not sure\\Image processing\\Labeling"
FILES
boxlabel.cpp
boxlabel.h
connectedcomponents.h
connectedcomponents.hpp
)

source_group(
"Not sure\\Image processing\\Morphology"
FILES
dilate.h
dilate.hpp
erode.h
erode.hpp
)

source_group(
"Not sure\\Image processing\\Convolution"
FILES
convolute.h
convolute.hpp
)

source_group(
"Not sure\\Image processing\\Miscellaneous"
FILES
branchingnumber.cpp
branchingnumber.h
gfxresource.h
labeltoimage.cpp
labeltoimage.h
packrange.h
pixelneighbours.cpp
pixelneighbours.h
quantize.h
quantize.hpp
)

source_group(
"Not sure\\Image processing\\Structuring elements"
FILES
structuringelement.h
structuringelement.hpp
)

source_group(
"Summed-area tables"
FILES
summedareatable.h
summedareatable.hpp
)

set(PastelGfx_Files
Pcx.txt
additivecolormixer.h
assigncolormixer.h
boxlabel.cpp
boxlabel.h
branchingnumber.cpp
branchingnumber.h
color.h
color.txt
color_hsv.cpp
color_hsv.h
color_illuminant.cpp
color_illuminant.h
color_illuminant.hpp
color_illuminant.txt
color_lab.cpp
color_lab.h
color_lch.cpp
color_lch.h
color_lms.cpp
color_lms.h
color_palette.cpp
color_palette.h
color_palette.txt
color_space.h
color_space.txt
color_srgb.cpp
color_srgb.h
color_texture.h
color_tools.cpp
color_tools.h
color_tools.hpp
color_xyy.cpp
color_xyy.h
color_yuv.cpp
color_yuv.h
coloradapter.cpp
coloradapter.h
coloradapter.txt
colormixer.h
colormixers.h
combine_texture.h
common.txt
connectedcomponents.h
connectedcomponents.hpp
convolute.h
convolute.hpp
dilate.h
dilate.hpp
discretepoissondiskpattern.h
distortion_textures.txt
draw.h
draw.txt
draw_box.h
draw_box.hpp
draw_floodfill.h
draw_floodfill.hpp
draw_horizontal_line.h
draw_horizontal_line.hpp
draw_perspective_triangle.h
draw_perspective_triangle.hpp
draw_pixel.h
draw_pixel.hpp
draw_projective_quad.h
draw_projective_quad.hpp
draw_segment.h
draw_segment.hpp
draw_segment_notes.txt
draw_textured_box.h
draw_textured_box.hpp
draw_textured_triangle.h
draw_textured_triangle.hpp
draw_triangle.h
draw_triangle.hpp
draw_vertical_line.h
draw_vertical_line.hpp
draw_view.h
draw_view.hpp
erode.h
erode.hpp
ewaimage_texture.h
ewaimage_texture.hpp
ewaimage_texture.txt
functorcolormixer.h
gfxlibrary.cpp
gfxlibrary.h
gfxrenderer.cpp
gfxrenderer.h
gfxrenderer.hpp
gfxrenderer_tools.cpp
gfxrenderer_tools.h
gfxrenderer_tools.hpp
gfxresource.h
gradientfield.h
gradientfield.hpp
gradientfield.txt
image_gfxrenderer.cpp
image_gfxrenderer.h
image_gfxrenderer.hpp
image_textures.txt
image_tools.cpp
image_tools.h
image_tools.hpp
imagepdf.h
imagepdf.hpp
labeltoimage.cpp
labeltoimage.h
linearcolor_texture.h
linearimage_texture.h
linearimage_texture.hpp
linearimage_texture.txt
linearsimplex_texture.h
loadpcx.cpp
loadpcx.h
loadpcx.hpp
marchingtriangles.h
mipimage_texture.h
mipimage_texture.hpp
mipimage_texture.txt
mix_texture.h
modifier_textures.txt
nearestimage_texture.h
nearestimage_texture.hpp
nearestimage_texture.txt
noise.h
noise.txt
noise_texture.h
packrange.h
pastelgfx.txt
pcx.h
pcxheader.cpp
pcxheader.h
perlin_noise.h
perlin_noise.hpp
perlin_noise.txt
pixelneighbours.cpp
pixelneighbours.h
quantize.h
quantize.hpp
radial_texture.h
radial_texture.txt
reconstruct_all.h
reconstruct_filter.h
reconstruct_filter.hpp
reconstruct_nearest.h
reconstruct_nearest.hpp
reconstruct_rbf.h
renderer.txt
ripimage_texture.h
ripimage_texture.hpp
ripimage_texture.txt
savepcx.cpp
savepcx.h
savepcx.hpp
simplex_noise.h
simplex_noise.hpp
simplex_noise.txt
simplex_noise_notes.txt
structuringelement.h
structuringelement.hpp
sub_texture.h
summedareatable.h
summedareatable.hpp
synthetic_textures.txt
texture.h
texture.txt
textures.h
transform_texture.h
transform_texture.txt
transparentcolormixer.h
turbulence_texture.h
)

include_directories(${Pastel_SOURCE_DIR})
link_directories(${Pastel_Binary_Dir}/lib)
add_library(PastelGfx ${PastelGfx_Files})
