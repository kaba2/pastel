# This is a CMake build file for PastelGeometry.

cmake_minimum_required(VERSION 2.8)

source_group(
"Data structures"
FILES
data_structures.txt
)

source_group(
"Data structures\\Meshes"
FILES
meshes.txt
)

source_group(
"Data structures\\Meshes\\HalfMesh"
FILES
halfmesh.h
halfmesh.hpp
halfmesh.txt
halfmesh_tools.h
propertyhalfmesh.cpp
propertyhalfmesh.h
propertyhalfmesh.hpp
simplehalfmesh.h
simplehalfmesh.hpp
)

source_group(
"Data structures\\BihTree"
FILES
bihtree.h
bihtree.hpp
bihtree.txt
bihtree_more.h
bihtree_tools.h
bihtree_tools.hpp
concept_shapequery.h
)

source_group(
"Data structures\\Bsp trees"
FILES
bsp_tree.txt
)

source_group(
"Data structures\\Bsp trees\\KdTree"
FILES
kdtree.h
kdtree.hpp
kdtree.txt
kdtree_raytraversal.h
kdtree_raytraversal.hpp
kdtree_refine.h
kdtree_refine.hpp
kdtree_tools.h
kdtree_tools.hpp
)

source_group(
"Data structures\\Bsp trees\\PointKdTree"
FILES
multilevel_kdtree.txt
pointkdtree.h
pointkdtree.hpp
pointkdtree.txt
pointkdtree_cursor.h
pointkdtree_design.txt
pointkdtree_node.h
pointkdtree_private.hpp
pointkdtree_splitpredicate.h
pointkdtree_tools.h
pointkdtree_tools.hpp
)

source_group(
"Data structures\\Bsp trees\\PointKdTree\\Split rules"
FILES
fair_splitrule_pointkdtree.h
fair_splitrule_pointkdtree.hpp
fair_splitrule_pointkdtree.txt
longestmedian_splitrule_pointkdtree.h
longestmedian_splitrule_pointkdtree.hpp
longestmedian_splitrule_pointkdtree.txt
minimumvolume_splitrule_pointkdtree.h
minimumvolume_splitrule_pointkdtree.hpp
minimumvolume_splitrule_pointkdtree.txt
slidingmidpoint2_splitrule_pointkdtree.h
slidingmidpoint2_splitrule_pointkdtree.hpp
slidingmidpoint2_splitrule_pointkdtree.txt
slidingmidpoint_splitrule_pointkdtree.h
slidingmidpoint_splitrule_pointkdtree.hpp
slidingmidpoint_splitrule_pointkdtree.txt
splitrule_pointkdtree.h
splitrule_pointkdtree.txt
splitrule_pointkdtree_concept.h
)

source_group(
"Common"
FILES
barycentric.h
barycentric.hpp
common.txt
geometrylibrary.cpp
geometrylibrary.h
nearest_main_axis.h
nearest_main_axis.hpp
pastelgeometry.txt
predicates.h
predicates.hpp
)

source_group(
"Shapes"
FILES
alignedplane.h
alignedplane.hpp
flat.h
flat.hpp
mathobjects.h
polygon.h
polygon.hpp
ray.h
ray.hpp
shapes.txt
simplex.h
triangle.h
)

source_group(
"Shapes\\Aligned box"
FILES
alignedbox.h
alignedbox.hpp
alignedbox.txt
alignedbox_tools.h
alignedbox_tools.hpp
alignedboxbase.h
alignedboxbase.hpp
)

source_group(
"Shapes\\Oriented box"
FILES
box.h
box.hpp
box.txt
boxtools.h
boxtools.hpp
)

source_group(
"Shapes\\Capsule"
FILES
capsule.h
capsule.hpp
capsule.txt
)

source_group(
"Shapes\\Line"
FILES
line.h
line.hpp
line.txt
)

source_group(
"Shapes\\Plane"
FILES
plane.h
plane.hpp
plane.txt
)

source_group(
"Shapes\\Segment"
FILES
segment.h
segment.hpp
segment.txt
)

source_group(
"Shapes\\Sphere"
FILES
sphere.h
sphere.hpp
sphere.txt
)

source_group(
"Shapes\\Ellipsoid"
FILES
ellipsoid.h
ellipsoid.hpp
ellipsoid.txt
)

source_group(
"Algorithms for shapes"
FILES
shape_algorithms.txt
)

source_group(
"Algorithms for shapes\\Distance"
FILES
distance.txt
distance_alignedbox_alignedbox.h
distance_alignedbox_alignedbox.hpp
distance_alignedbox_point.h
distance_alignedbox_point.hpp
distance_alignedbox_sphere.h
distance_alignedbox_sphere.hpp
distance_line_line.h
distance_line_line.hpp
distance_line_point.h
distance_line_point.hpp
distance_plane_point.h
distance_plane_point.hpp
distance_plane_sphere.h
distance_plane_sphere.hpp
distance_point_point.h
distance_point_point.hpp
distance_segment_point.h
distance_segment_point.hpp
distance_segment_segment.h
distance_segment_segment.hpp
distance_segment_sphere.h
distance_segment_sphere.hpp
)

source_group(
"Algorithms for shapes\\Closest points"
FILES
closest.txt
closest_alignedbox_point.h
closest_alignedbox_point.hpp
closest_line_line.h
closest_line_line.hpp
closest_line_point.h
closest_line_point.hpp
closest_plane_point.h
closest_plane_point.hpp
closest_segment_point.h
closest_segment_point.hpp
closest_segment_segment.h
closest_segment_segment.hpp
)

source_group(
"Algorithms for shapes\\Overlap testing"
FILES
overlaps.txt
overlaps_design.txt
)

source_group(
"Algorithms for shapes\\Overlap testing\\Involving AlignedBox"
FILES
overlaps_alignedbox_alignedbox.h
overlaps_alignedbox_alignedbox.hpp
overlaps_alignedbox_box.h
overlaps_alignedbox_box.hpp
overlaps_alignedbox_plane.h
overlaps_alignedbox_plane.hpp
overlaps_alignedbox_point.h
overlaps_alignedbox_point.hpp
overlaps_alignedbox_sphere.h
overlaps_alignedbox_sphere.hpp
overlaps_alignedbox_triangle.h
overlaps_alignedbox_triangle.hpp
)

source_group(
"Algorithms for shapes\\Overlap testing\\Involving AlignedPlane"
FILES
overlaps_alignedplane_alignedbox.h
overlaps_alignedplane_alignedbox.hpp
overlaps_alignedplane_box.h
overlaps_alignedplane_box.hpp
overlaps_alignedplane_sphere.h
overlaps_alignedplane_sphere.hpp
overlaps_alignedplane_triangle.h
overlaps_alignedplane_triangle.hpp
)

source_group(
"Algorithms for shapes\\Overlap testing\\Involving Box"
FILES
overlaps_box_box.h
overlaps_box_box.hpp
overlaps_box_point.h
overlaps_box_point.hpp
overlaps_box_sphere.h
overlaps_box_sphere.hpp
overlaps_box_triangle.h
overlaps_box_triangle.hpp
)

source_group(
"Algorithms for shapes\\Overlap testing\\Involving Plane"
FILES
overlaps_plane_box.h
overlaps_plane_box.hpp
overlaps_plane_sphere.h
overlaps_plane_sphere.hpp
overlaps_plane_triangle.h
overlaps_plane_triangle.hpp
)

source_group(
"Algorithms for shapes\\Overlap testing\\Involving Sphere"
FILES
overlaps_sphere_point.h
overlaps_sphere_point.hpp
overlaps_sphere_sphere.h
overlaps_sphere_sphere.hpp
)

source_group(
"Algorithms for shapes\\Overlap testing\\Involving others"
FILES
overlaps_capsule_capsule.h
overlaps_capsule_capsule.hpp
overlaps_flat_plane.h
overlaps_flat_plane.hpp
overlaps_segment_segment.h
overlaps_segment_segment.hpp
overlaps_segments.h
overlaps_segments.hpp
overlaps_simplex_point.h
overlaps_simplex_point.hpp
overlaps_triangle_triangle.h
overlaps_triangle_triangle.hpp
)

source_group(
"Algorithms for shapes\\Containment"
FILES
contains.txt
contains_alignedbox_alignedbox.h
contains_alignedbox_alignedbox.hpp
contains_alignedbox_sphere.h
contains_alignedbox_sphere.hpp
contains_sphere_alignedbox.h
contains_sphere_alignedbox.hpp
)

source_group(
"Algorithms for shapes\\Intersection"
FILES
intersect.txt
intersect_alignedbox_alignedbox.h
intersect_alignedbox_alignedbox.hpp
intersect_alignedbox_plane.h
intersect_alignedbox_plane.hpp
intersect_alignedbox_segment.h
intersect_alignedbox_segment.hpp
intersect_flat_plane.h
intersect_flat_plane.hpp
intersect_line_alignedbox.h
intersect_line_alignedbox.hpp
intersect_line_alignedplane.h
intersect_line_alignedplane.hpp
intersect_line_box.h
intersect_line_box.hpp
intersect_line_plane.h
intersect_line_plane.hpp
intersect_line_polygon.h
intersect_line_polygon.hpp
intersect_line_sphere.h
intersect_line_sphere.hpp
intersect_line_triangle.h
intersect_line_triangle.hpp
intersect_plane_sphere.h
intersect_plane_sphere.hpp
intersect_ray_alignedbox.h
intersect_ray_alignedbox.hpp
intersect_ray_alignedplane.h
intersect_ray_alignedplane.hpp
intersect_ray_plane.h
intersect_ray_plane.hpp
intersect_ray_sphere.h
intersect_ray_sphere.hpp
intersect_segment_halfspace.h
intersect_segment_halfspace.hpp
intersect_triangle_triangle.h
intersect_triangle_triangle.hpp
mathintersect.h
)

source_group(
"Algorithms for shapes\\Bounding"
FILES
bounding.h
bounding.txt
)

source_group(
"Algorithms for shapes\\Bounding\\Bounding aligned box"
FILES
bounding_alignedbox.h
bounding_alignedbox.txt
bounding_alignedbox_alignedbox.h
bounding_alignedbox_alignedbox.hpp
bounding_alignedbox_box.h
bounding_alignedbox_box.hpp
bounding_alignedbox_pointset.h
bounding_alignedbox_pointset.hpp
bounding_alignedbox_segment.h
bounding_alignedbox_segment.hpp
bounding_alignedbox_sphere.h
bounding_alignedbox_sphere.hpp
)

source_group(
"Algorithms for shapes\\Bounding\\Bounding sphere"
FILES
bounding_sphere.h
bounding_sphere.hpp
bounding_sphere.txt
bounding_sphere_alignedbox.h
bounding_sphere_alignedbox.hpp
bounding_sphere_box.h
bounding_sphere_box.hpp
bounding_sphere_pointset.h
bounding_sphere_pointset.hpp
bounding_sphere_segment.h
bounding_sphere_segment.hpp
)

source_group(
"Algorithms for shapes\\Bounding\\Bounding box"
FILES
bounding_box.h
bounding_box.hpp
bounding_box.txt
)

source_group(
"Algorithms for shapes\\Area"
FILES
area.h
area.txt
box_area.h
box_area.hpp
polygon_area.h
polygon_area.hpp
sphere_area.cpp
sphere_area.h
sphere_area.hpp
)

source_group(
"Algorithms for shapes\\Diameter"
FILES
alignedbox_diameter.h
alignedbox_diameter.hpp
box_diameter.h
box_diameter.hpp
diameter.h
diameter.txt
sphere_diameter.h
sphere_diameter.hpp
)

source_group(
"Algorithms for shapes\\Volume"
FILES
box_volume.h
box_volume.hpp
sphere_volume.cpp
sphere_volume.h
sphere_volume.hpp
volume.txt
)

source_group(
"Algorithms for shapes\\Projection"
FILES
linear_projection.h
linear_projection.hpp
planar_projection.cpp
planar_projection.h
planar_projection.hpp
projection.txt
)

source_group(
"Algorithms for point-sets"
FILES
pointsets.txt
)

source_group(
"Algorithms for point-sets\\Eigenstructure"
FILES
pointset_eigen.h
pointset_eigen.hpp
pointset_eigen.txt
)

source_group(
"Algorithms for point-sets\\Convex hull"
FILES
convexhull.h
convexhull.hpp
convexhull.txt
)

source_group(
"Algorithms for point-sets\\Point pattern matching"
FILES
point_pattern_matching.h
point_pattern_matching.hpp
point_pattern_matching.txt
)

source_group(
"Algorithms for point-sets\\Point patterns"
FILES
point_patterns.h
point_patterns.hpp
point_patterns.txt
)

source_group(
"Algorithms for point-sets\\Point patterns\\Poisson-disk pattern"
FILES
poissondiskpattern.h
poissondiskpattern.hpp
poissondiskpattern.txt
)

source_group(
"Algorithms for point-sets\\Orthogonal range searching"
FILES
count_all_range_pointkdtree.h
count_all_range_pointkdtree.hpp
count_range_pointkdtree.h
count_range_pointkdtree.hpp
search_range.txt
search_range_algorithm_pointkdtree.h
search_range_algorithm_pointkdtree.hpp
search_range_pointkdtree.h
search_range_pointkdtree.hpp
)

source_group(
"Algorithms for point-sets\\Nearest neighbors"
FILES
count_nearest_pointkdtree.h
count_nearest_pointkdtree.hpp
nearest_neighbors.txt
nearest_neighbors_semidynamic.txt
search_nearest_algorithm_pointkdtree.h
search_nearest_algorithm_pointkdtree.hpp
search_nearest_one_pointkdtree.h
search_nearest_one_pointkdtree.hpp
search_nearest_pointkdtree.h
search_nearest_pointkdtree.hpp
)

source_group(
"Algorithms for point-sets\\Nearest neighbors\\All nearest neighbors counting"
FILES
count_all_neighbors.h
count_all_neighbors.txt
count_all_neighbors_1d.h
count_all_neighbors_1d.hpp
count_all_neighbors_bruteforce.h
count_all_neighbors_bruteforce.hpp
count_all_neighbors_pointkdtree.h
count_all_neighbors_pointkdtree.hpp
)

source_group(
"Algorithms for point-sets\\Nearest neighbors\\All nearest neighbors searching"
FILES
search_all_neighbors.h
search_all_neighbors.txt
search_all_neighbors_1d.h
search_all_neighbors_1d.hpp
search_all_neighbors_bruteforce.h
search_all_neighbors_bruteforce.hpp
search_all_neighbors_pointkdtree.h
search_all_neighbors_pointkdtree.hpp
)

source_group(
"Algorithms for point-sets\\Nearest neighbors\\AcceptPoint"
FILES
acceptpoint.h
acceptpoint.txt
acceptpoint_concept.h
always_acceptpoint.h
dont_acceptpoint.h
negate_acceptpoint.h
objectdont_acceptpoint.h
)

source_group(
"Algorithms for point-sets\\Nearest neighbors\\SearchAlgorithm_PointKdTree"
FILES
bestfirst_searchalgorithm_pointkdtree.h
bestfirst_searchalgorithm_pointkdtree.hpp
depthfirst_searchalgorithm_pointkdtree.h
depthfirst_searchalgorithm_pointkdtree.hpp
searchalgorithm_pointkdtree.txt
searchalgorithm_pointkdtree_concept.h
)

source_group(
"Algorithms for point-sets\\Point policies"
FILES
array_pointpolicy.h
pointpolicy.txt
pointpolicy_all.h
pointpolicy_concept.h
vector_pointpolicy.h
)

set(PastelGeometry_Files
acceptpoint.h
acceptpoint.txt
acceptpoint_concept.h
alignedbox.h
alignedbox.hpp
alignedbox.txt
alignedbox_diameter.h
alignedbox_diameter.hpp
alignedbox_tools.h
alignedbox_tools.hpp
alignedboxbase.h
alignedboxbase.hpp
alignedplane.h
alignedplane.hpp
always_acceptpoint.h
area.h
area.txt
array_pointpolicy.h
barycentric.h
barycentric.hpp
bestfirst_searchalgorithm_pointkdtree.h
bestfirst_searchalgorithm_pointkdtree.hpp
bihtree.h
bihtree.hpp
bihtree.txt
bihtree_more.h
bihtree_tools.h
bihtree_tools.hpp
bounding.h
bounding.txt
bounding_alignedbox.h
bounding_alignedbox.txt
bounding_alignedbox_alignedbox.h
bounding_alignedbox_alignedbox.hpp
bounding_alignedbox_box.h
bounding_alignedbox_box.hpp
bounding_alignedbox_pointset.h
bounding_alignedbox_pointset.hpp
bounding_alignedbox_segment.h
bounding_alignedbox_segment.hpp
bounding_alignedbox_sphere.h
bounding_alignedbox_sphere.hpp
bounding_box.h
bounding_box.hpp
bounding_box.txt
bounding_sphere.h
bounding_sphere.hpp
bounding_sphere.txt
bounding_sphere_alignedbox.h
bounding_sphere_alignedbox.hpp
bounding_sphere_box.h
bounding_sphere_box.hpp
bounding_sphere_pointset.h
bounding_sphere_pointset.hpp
bounding_sphere_segment.h
bounding_sphere_segment.hpp
box.h
box.hpp
box.txt
box_area.h
box_area.hpp
box_diameter.h
box_diameter.hpp
box_volume.h
box_volume.hpp
boxtools.h
boxtools.hpp
bsp_tree.txt
capsule.h
capsule.hpp
capsule.txt
closest.txt
closest_alignedbox_point.h
closest_alignedbox_point.hpp
closest_line_line.h
closest_line_line.hpp
closest_line_point.h
closest_line_point.hpp
closest_plane_point.h
closest_plane_point.hpp
closest_segment_point.h
closest_segment_point.hpp
closest_segment_segment.h
closest_segment_segment.hpp
common.txt
concept_shapequery.h
contains.txt
contains_alignedbox_alignedbox.h
contains_alignedbox_alignedbox.hpp
contains_alignedbox_sphere.h
contains_alignedbox_sphere.hpp
contains_sphere_alignedbox.h
contains_sphere_alignedbox.hpp
convexhull.h
convexhull.hpp
convexhull.txt
count_all_neighbors.h
count_all_neighbors.txt
count_all_neighbors_1d.h
count_all_neighbors_1d.hpp
count_all_neighbors_bruteforce.h
count_all_neighbors_bruteforce.hpp
count_all_neighbors_pointkdtree.h
count_all_neighbors_pointkdtree.hpp
count_all_range_pointkdtree.h
count_all_range_pointkdtree.hpp
count_nearest_pointkdtree.h
count_nearest_pointkdtree.hpp
count_range_pointkdtree.h
count_range_pointkdtree.hpp
data_structures.txt
depthfirst_searchalgorithm_pointkdtree.h
depthfirst_searchalgorithm_pointkdtree.hpp
diameter.h
diameter.txt
distance.txt
distance_alignedbox_alignedbox.h
distance_alignedbox_alignedbox.hpp
distance_alignedbox_point.h
distance_alignedbox_point.hpp
distance_alignedbox_sphere.h
distance_alignedbox_sphere.hpp
distance_line_line.h
distance_line_line.hpp
distance_line_point.h
distance_line_point.hpp
distance_plane_point.h
distance_plane_point.hpp
distance_plane_sphere.h
distance_plane_sphere.hpp
distance_point_point.h
distance_point_point.hpp
distance_segment_point.h
distance_segment_point.hpp
distance_segment_segment.h
distance_segment_segment.hpp
distance_segment_sphere.h
distance_segment_sphere.hpp
dont_acceptpoint.h
ellipsoid.h
ellipsoid.hpp
ellipsoid.txt
fair_splitrule_pointkdtree.h
fair_splitrule_pointkdtree.hpp
fair_splitrule_pointkdtree.txt
flat.h
flat.hpp
geometrylibrary.cpp
geometrylibrary.h
halfmesh.h
halfmesh.hpp
halfmesh.txt
halfmesh_tools.h
intersect.txt
intersect_alignedbox_alignedbox.h
intersect_alignedbox_alignedbox.hpp
intersect_alignedbox_plane.h
intersect_alignedbox_plane.hpp
intersect_alignedbox_segment.h
intersect_alignedbox_segment.hpp
intersect_flat_plane.h
intersect_flat_plane.hpp
intersect_line_alignedbox.h
intersect_line_alignedbox.hpp
intersect_line_alignedplane.h
intersect_line_alignedplane.hpp
intersect_line_box.h
intersect_line_box.hpp
intersect_line_plane.h
intersect_line_plane.hpp
intersect_line_polygon.h
intersect_line_polygon.hpp
intersect_line_sphere.h
intersect_line_sphere.hpp
intersect_line_triangle.h
intersect_line_triangle.hpp
intersect_plane_sphere.h
intersect_plane_sphere.hpp
intersect_ray_alignedbox.h
intersect_ray_alignedbox.hpp
intersect_ray_alignedplane.h
intersect_ray_alignedplane.hpp
intersect_ray_plane.h
intersect_ray_plane.hpp
intersect_ray_sphere.h
intersect_ray_sphere.hpp
intersect_segment_halfspace.h
intersect_segment_halfspace.hpp
intersect_triangle_triangle.h
intersect_triangle_triangle.hpp
kdtree.h
kdtree.hpp
kdtree.txt
kdtree_raytraversal.h
kdtree_raytraversal.hpp
kdtree_refine.h
kdtree_refine.hpp
kdtree_tools.h
kdtree_tools.hpp
line.h
line.hpp
line.txt
linear_projection.h
linear_projection.hpp
longestmedian_splitrule_pointkdtree.h
longestmedian_splitrule_pointkdtree.hpp
longestmedian_splitrule_pointkdtree.txt
mathintersect.h
mathobjects.h
meshes.txt
minimumvolume_splitrule_pointkdtree.h
minimumvolume_splitrule_pointkdtree.hpp
minimumvolume_splitrule_pointkdtree.txt
multilevel_kdtree.txt
nearest_main_axis.h
nearest_main_axis.hpp
nearest_neighbors.txt
nearest_neighbors_semidynamic.txt
negate_acceptpoint.h
objectdont_acceptpoint.h
overlaps.txt
overlaps_alignedbox_alignedbox.h
overlaps_alignedbox_alignedbox.hpp
overlaps_alignedbox_box.h
overlaps_alignedbox_box.hpp
overlaps_alignedbox_plane.h
overlaps_alignedbox_plane.hpp
overlaps_alignedbox_point.h
overlaps_alignedbox_point.hpp
overlaps_alignedbox_sphere.h
overlaps_alignedbox_sphere.hpp
overlaps_alignedbox_triangle.h
overlaps_alignedbox_triangle.hpp
overlaps_alignedplane_alignedbox.h
overlaps_alignedplane_alignedbox.hpp
overlaps_alignedplane_box.h
overlaps_alignedplane_box.hpp
overlaps_alignedplane_sphere.h
overlaps_alignedplane_sphere.hpp
overlaps_alignedplane_triangle.h
overlaps_alignedplane_triangle.hpp
overlaps_box_box.h
overlaps_box_box.hpp
overlaps_box_point.h
overlaps_box_point.hpp
overlaps_box_sphere.h
overlaps_box_sphere.hpp
overlaps_box_triangle.h
overlaps_box_triangle.hpp
overlaps_capsule_capsule.h
overlaps_capsule_capsule.hpp
overlaps_design.txt
overlaps_flat_plane.h
overlaps_flat_plane.hpp
overlaps_plane_box.h
overlaps_plane_box.hpp
overlaps_plane_sphere.h
overlaps_plane_sphere.hpp
overlaps_plane_triangle.h
overlaps_plane_triangle.hpp
overlaps_segment_segment.h
overlaps_segment_segment.hpp
overlaps_segments.h
overlaps_segments.hpp
overlaps_simplex_point.h
overlaps_simplex_point.hpp
overlaps_sphere_point.h
overlaps_sphere_point.hpp
overlaps_sphere_sphere.h
overlaps_sphere_sphere.hpp
overlaps_triangle_triangle.h
overlaps_triangle_triangle.hpp
pastelgeometry.txt
planar_projection.cpp
planar_projection.h
planar_projection.hpp
plane.h
plane.hpp
plane.txt
point_pattern_matching.h
point_pattern_matching.hpp
point_pattern_matching.txt
point_patterns.h
point_patterns.hpp
point_patterns.txt
pointkdtree.h
pointkdtree.hpp
pointkdtree.txt
pointkdtree_cursor.h
pointkdtree_design.txt
pointkdtree_node.h
pointkdtree_private.hpp
pointkdtree_splitpredicate.h
pointkdtree_tools.h
pointkdtree_tools.hpp
pointpolicy.txt
pointpolicy_all.h
pointpolicy_concept.h
pointset_eigen.h
pointset_eigen.hpp
pointset_eigen.txt
pointsets.txt
poissondiskpattern.h
poissondiskpattern.hpp
poissondiskpattern.txt
polygon.h
polygon.hpp
polygon_area.h
polygon_area.hpp
predicates.h
predicates.hpp
projection.txt
propertyhalfmesh.cpp
propertyhalfmesh.h
propertyhalfmesh.hpp
ray.h
ray.hpp
search_all_neighbors.h
search_all_neighbors.txt
search_all_neighbors_1d.h
search_all_neighbors_1d.hpp
search_all_neighbors_bruteforce.h
search_all_neighbors_bruteforce.hpp
search_all_neighbors_pointkdtree.h
search_all_neighbors_pointkdtree.hpp
search_nearest_algorithm_pointkdtree.h
search_nearest_algorithm_pointkdtree.hpp
search_nearest_one_pointkdtree.h
search_nearest_one_pointkdtree.hpp
search_nearest_pointkdtree.h
search_nearest_pointkdtree.hpp
search_range.txt
search_range_algorithm_pointkdtree.h
search_range_algorithm_pointkdtree.hpp
search_range_pointkdtree.h
search_range_pointkdtree.hpp
searchalgorithm_pointkdtree.txt
searchalgorithm_pointkdtree_concept.h
segment.h
segment.hpp
segment.txt
shape_algorithms.txt
shapes.txt
simplehalfmesh.h
simplehalfmesh.hpp
simplex.h
slidingmidpoint2_splitrule_pointkdtree.h
slidingmidpoint2_splitrule_pointkdtree.hpp
slidingmidpoint2_splitrule_pointkdtree.txt
slidingmidpoint_splitrule_pointkdtree.h
slidingmidpoint_splitrule_pointkdtree.hpp
slidingmidpoint_splitrule_pointkdtree.txt
sphere.h
sphere.hpp
sphere.txt
sphere_area.cpp
sphere_area.h
sphere_area.hpp
sphere_diameter.h
sphere_diameter.hpp
sphere_volume.cpp
sphere_volume.h
sphere_volume.hpp
splitrule_pointkdtree.h
splitrule_pointkdtree.txt
splitrule_pointkdtree_concept.h
triangle.h
vector_pointpolicy.h
volume.txt
)

include_directories(${Pastel_SOURCE_DIR})
link_directories(${Pastel_Binary_Dir}/lib)
add_library(PastelGeometry ${PastelGeometry_Files})
