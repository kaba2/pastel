% Description: A test for PointKdTree Matlab interface
% Documentation: matlab_pointkdtree.txt

clear all;
close all;

eval(import_pastel);

d = 2;
n = 200;
kNearest = 8;

% Generate a point-set.
pointSet = randn(d, n);

% Construct a kd-tree.
kdTree = pointkdtree_construct(d);

% Insert the points into the kd-tree.
idSet = pointkdtree_insert(kdTree, pointSet);

% Compute the correct nearest neighbors by brute-force.
correctSet = int32(zeros(n, kNearest));
for i = 1 : n
    distanceSet = zeros(1, n);
    for j = 1 : n
        distanceSet(j) = sum((pointSet(:, i) - pointSet(:, j)).^2);
    end
    distanceSet(i) = Inf;
    [Y, I] = sort(distanceSet);
    correctSet(i, :) = idSet(I(1 : kNearest));
end

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
neighborSet = pointkdtree_search_nearest(kdTree, querySet, ...
    maxDistanceSet, kNearest);

if correctSet ~= neighborSet
    error('Kd-tree did not find the same points as brute-force.');
end

nearestSet = pointkdtree_as_points(kdTree, neighborSet(:, 1));

% Draw a picture.
figure;
axis equal;
scatter(pointSet(1, :), pointSet(2, :), 'bx');
hold on;
for i = 1 : numel(querySet)
    line([pointSet(1, i), nearestSet(1, i)], ...
        [pointSet(2, i), nearestSet(2, i)]);
end
hold off

% Search using queries in component form.
querySet = randn(d, 100);
maxDistanceSet = Inf(1, size(querySet, 2));
neighborSet = pointkdtree_search_nearest(kdTree, querySet, ...
    maxDistanceSet, kNearest);
nearestSet = pointkdtree_as_points(kdTree, neighborSet(:, 1));

% Draw a picture.
figure;
axis equal;
scatter(pointSet(1, :), pointSet(2, :), 'bx');
hold on;
scatter(querySet(1, :), querySet(2, :), 'r+');
for i = 1 : size(querySet, 2)
    line([querySet(1, i), nearestSet(1, i)], ...
        [querySet(2, i), nearestSet(2, i)]);
end
hold off

% Hide some of the points.
pointkdtree_hide(kdTree, idSet(101 : 200));

disp(['Points after hiding 100 of them = ', ...
    int2str(pointkdtree_points(kdTree))]);

% Bring back the hided points.
pointkdtree_show(kdTree, idSet(101 : 200));

disp(['Points after showing the 100 hided points = ', ...
    int2str(pointkdtree_points(kdTree))]);

% Remove some of the points completely.
pointkdtree_erase(kdTree, idSet(1 : 100));

disp(['Points after erasing 100 of them = ', ...
    int2str(pointkdtree_points(kdTree))]);

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

