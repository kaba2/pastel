% Description: A test for PointKdTree Matlab interface
% Documentation: matlab_pointkdtree.txt

function test_pointkdtree()

d = 10;
n = 9000;
kNearest = 8;

% Generate a point-set.
pointSet = randn(d, n);

% Construct a kd-tree.
kdTree = pointkdtree_construct(d);

% Insert the points into the kd-tree.
idSet = pointkdtree_insert(kdTree, pointSet);

% Refine the subdivision of the kd-tree.
pointkdtree_refine(kdTree);

% Print out some statistics.
disp(['Points = ', int2str(pointkdtree_points(kdTree))]);
disp(['Leaf nodes = ', int2str(pointkdtree_leaves(kdTree))]);
disp(['Nodes = ', int2str(pointkdtree_nodes(kdTree))]);

% Make a copy of the tree.
copyTree = pointkdtree_copy(kdTree);

% Destruct the copy.
pointkdtree_destruct(copyTree);

% Use all of the points as query points.
querySet = idSet;

% No maximum distance requirements.
maxDistanceSet = Inf(1, size(querySet, 2));

% Search for k nearest neighbors.
pointkdtree_search_nearest(kdTree, querySet, ...
    maxDistanceSet, kNearest);

% Hide some of the points.
pointkdtree_hide(kdTree, idSet(101 : 200));

disp(['Points after hiding = ', int2str(pointkdtree_points(kdTree))]);

% Search again for k nearest neighbors in
% the reduced point set (Note: hidden points can still
% be used as query points).
pointkdtree_search_nearest(kdTree, querySet, ...
    maxDistanceSet, kNearest);

% Bring back the hided points.
pointkdtree_show(kdTree, idSet(101 : 200));

disp(['Points after showing = ', int2str(pointkdtree_points(kdTree))]);

% Remove some of the points completely.
pointkdtree_erase(kdTree, idSet(1 : 100));

disp(['Points after erasing = ', int2str(pointkdtree_points(kdTree))]);

% Hide all points.
pointkdtree_hide(kdTree);

% Show all points.
pointkdtree_hide(kdTree);

% Flatten the kd-tree.
pointkdtree_merge(kdTree);

% Remove all points, but not the subdivision.
pointkdtree_erase(kdTree);

% Remove everything.
pointkdtree_clear(kdTree);

% Destruct the kd-tree.
pointkdtree_destruct(kdTree);
