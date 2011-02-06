function test_pointkdtree()

d = 10;
n = 10000;
kNearest = 8;

% Generate a point-set.
pointSet = randn(d, n);

% Construct a kd-tree.
kdTree = pointkdtree_construct(d);

% Insert the points into the kd-tree.
idSet = pointkdtree_insert(kdTree, pointSet);

% The query points are given as coordinates.
% Use all of the points as query points.
querySet = pointkdtree_as_points(kdTree, idSet);

% No maximum distance requirements.
maxDistanceSet = Inf(1, size(querySet, 2));

% Search for k nearest neighbors.
pointkdtree_search_nearest(kdTree, querySet, ...
    maxDistanceSet, kNearest);

% Destruct the kd-tree.
pointkdtree_destruct(kdTree);
