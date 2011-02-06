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
% QUERYSET is an (m x n) numeric array, which contains n query points
% of dimension m. The m must correspond with the dimension of
% the kd-tree. Native type: double.
%
% MAXDISTANCESET is a numeric array whose linearization contains the
% maximum allowed (norm bijection) distance for the neighbors of each 
% query point. Use Inf for no restrictions. Native type: double.
%
% KNEAREST is the number of nearest neighbors to search.
%
% NEIGHBORSET is an (k x n) integer array, which contains k nearest
% neighbors m for each of the n query points.

% Description: Searches for k nearest neighbors in a kd-tree.
% Documentation: matlab_pointkdtree.txt

function neighborSet = pointkdtree_search_nearest(...
	kdTree, querySet, maxDistanceSet, kNearest)

neighborSet = pastelgeometrymatlab('pointkdtree_search_nearest', ...
	kdTree, querySet, maxDistanceSet, kNearest);

end