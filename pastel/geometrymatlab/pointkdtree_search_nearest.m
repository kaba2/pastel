% POINTKDTREE_SEARCH_NEAREST
% Searches for k nearest neighbors in a kd-tree.
%
% neighborSet = pointkdtree_search_nearest(...
%		kdTree, querySet, maxDistanceSet, kNearest)
%
% where
%
% KDTREE is a kd-tree constructed with pointkdtree_construct().
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
% maximum allowed (norm bijection) distance for the neighbors of each 
% query point. Use Inf for no restrictions. Native type: double.
%
% KNEAREST is the number of nearest neighbors to search.
%
% NEIGHBORSET is an (k x n) integer array, which contains k nearest
% neighbors for each of the n query points.

% Description: Searches for k nearest neighbors in a kd-tree.
% Documentation: matlab_pointkdtree.txt

function neighborSet = pointkdtree_search_nearest(...
	kdTree, querySet, maxDistanceSet, kNearest)

neighborSet = pastelgeometrymatlab('pointkdtree_search_nearest', ...
	kdTree, querySet, maxDistanceSet, kNearest);

end