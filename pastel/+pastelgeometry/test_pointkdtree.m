% Description: A test for PointKdTree Matlab interface
% Documentation: matlab_pointkdtree.txt

function test_pointkdtree()

clear all;
close all;

eval(import_pastel);

d = 2;
n = 200;
kNearest = 8;

% Generate a point-set.
pointSet = randn(d, n);

% Construct a kd-tree.
kdTree = PointKdTree(d);

% Insert the points into the kd-tree.
idSet = kdTree.insert(pointSet);

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
kdTree.refine();

% Print out some statistics.
disp(['Points = ', int2str(kdTree.points())]);
disp(['Leaf nodes = ', int2str(kdTree.leaves())]);
disp(['Nodes = ', int2str(kdTree.nodes())]);

% Make a copy of the tree.
copyTree = kdTree.copy();

% Destruct the copy.
clear copyTree;

% Use all of the points as query points.
querySet = idSet;

% No maximum distance requirements.
maxDistanceSet = Inf(1, size(querySet, 2));

% Search for k nearest neighbors.
neighborSet = kdTree.search_nearest(querySet, ...
    maxDistanceSet, kNearest);

if ~isequal(correctSet, neighborSet)
    error('Kd-tree did not find the same points as brute-force.');
end

nearestSet = kdTree.as_points(neighborSet(:, 1));

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
neighborSet = kdTree.search_nearest(querySet, ...
    maxDistanceSet, kNearest);
nearestSet = kdTree.as_points(neighborSet(:, 1));

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
kdTree.hide(idSet(101 : 200));

disp(['Points after hiding 100 of them = ', ...
    int2str(kdTree.points())]);

% Bring back the hided points.
kdTree.show(idSet(101 : 200));

disp(['Points after showing the 100 hided points = ', ...
    int2str(kdTree.points())]);

% Remove some of the points completely.
kdTree.erase(idSet(1 : 100));

disp(['Points after erasing 100 of them = ', ...
    int2str(kdTree.points())]);

% Hide all points.
kdTree.hide();

% Show all points.
kdTree.hide();

% Flatten the kd-tree.
kdTree.merge();

% Remove all points, but not the subdivision.
kdTree.erase();

% Remove everything.
kdTree.clear();

% Destruct the kd-tree.
clear kdTree;

