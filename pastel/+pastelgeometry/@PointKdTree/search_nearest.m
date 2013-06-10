% SEARCH_NEAREST
% Searches for k nearest neighbors in a kd-tree.
%
% [neighborSet, distanceSet] = search_nearest(...
%       querySet, maxDistanceSet, kNearest)
%
% where
%
% QUERYSET is a numeric array which contains the query points. 
% If the array is of type single or double, and of extent d x n, 
% then each column is taken to contain the coordinates of a 
% d-dimensional point, where d is the dimension of the KDTREE, with
% QUERYSET containing n query points. If the array is of integer type, 
% and contains n elements, then the linearization of QUERYSET is taken 
% to contain n ids of points in the KDTREE. When the query is by an id, 
% the query point will not match as a nearest neighbor of itself.
%
% MAXDISTANCESET is a numeric array whose linearization contains the
% maximum allowed squared-distance for the neighbors of each 
% query point. Use Inf for no restrictions. Native type: double.
%
% KNEAREST is the number of nearest neighbors to search.
%
% Return values
% -------------
%
% NEIGHBORSET is an (n x k) non-negative integer array, which at (i, j) 
% contains the j:th nearest neighbor of querySet(i). The value 0
% denotes a missing neighbor.
%
% DISTANCESET is an (n x k) real array, which at (i, j) contains the
% squared-distance of the i:th point in QUERYSET to its j:th neighbor.
% The value Inf denotes a missing neighbor.

% Description: Searches for k nearest neighbors in a kd-tree.
% DocumentationOf: PointKdTree.m

function [neighborSet, distanceSet] = search_nearest(...
    self, querySet, maxDistanceSet, kNearest)
   
    eval(import_pastel);

    [neighborSet, distanceSet] = pastelgeometrymatlab(...
        'pointkdtree_search_nearest', ...
        self.kdTree, querySet, ...
        maxDistanceSet, kNearest);
end
