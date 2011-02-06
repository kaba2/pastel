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

disp(['Leaf nodes = ', int2str(pointkdtree_leaves(kdTree))]);
disp(['Nodes = ', int2str(pointkdtree_nodes(kdTree))]);
disp(['Points = ', int2str(pointkdtree_points(kdTree))]);

% Refine the subdivision of the kd-tree.
pointkdtree_refine(kdTree);

disp(['Leaf nodes = ', int2str(pointkdtree_leaves(kdTree))]);
disp(['Nodes = ', int2str(pointkdtree_nodes(kdTree))]);
disp(['Points = ', int2str(pointkdtree_points(kdTree))]);

% The query points are given as coordinates.
% Use all of the points as query points.
%querySet = pointkdtree_as_points(kdTree, idSet);
querySet = idSet;

% No maximum distance requirements.
maxDistanceSet = Inf(1, size(querySet, 2));

% Search for k nearest neighbors.
pointkdtree_search_nearest(kdTree, querySet, ...
    maxDistanceSet, kNearest);

% Destruct the kd-tree.
pointkdtree_destruct(kdTree);
