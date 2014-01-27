% SEARCH_NEAREST
% Searches for k nearest neighbors in a kd-tree.
%
% [neighborSet, distanceSet] = search_nearest(querySet, 'key', value, ...)
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
% Optional input arguments in 'key'-value pairs:
%
% KNEAREST ('kNearest') is a positive integer which specifies the 
% number of nearest neighbors to search. 
% Default: 1
%
% MAXDISTANCESET ('maxDistanceSet') is a numeric array with n 
% elements whose linearization contains the maximum allowed (open)
% distance for the neighbors of each query point. For the euclidean
% norm the distance is specified as squared-distance. A scalar M 
% is automatically extended to ones(1, n) * M. Use Inf for no restrictions.
% Default: Inf
%
% NORM ('norm') is a string which specifies the norm to use. 
% Must be one of
%     euclidean: the euclidean norm (default)
%     maximum: the maximum norm

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
	self, querySet, varargin)
   
    eval(import_pastel);

	% Optional input arguments
	kNearest = 1;
	maxDistanceSet = Inf;
	norm = 'euclidean';
	eval(process_options({...
	    'kNearest', 
	    'maxDistanceSet', 
	    'norm'}, ...
	    varargin));

    concept_check(...
    	maxDistanceSet, 'real_matrix', ...
    	norm, 'string', ...
    	kNearest, 'integer', ...
    	kNearest, 'positive');

	if isinteger(querySet)
		n = numel(querySet);
	else
		n = size(querySet, 2);
	end

	if isscalar(maxDistanceSet)
		maxDistanceSet = ones(1, n) * maxDistanceSet;
	end

    [neighborSet, distanceSet] = pastelgeometrymatlab(...
        'pointkdtree_search_nearest', ...
        self.kdTree, querySet, ...
        maxDistanceSet, kNearest, ...
        norm);
end
