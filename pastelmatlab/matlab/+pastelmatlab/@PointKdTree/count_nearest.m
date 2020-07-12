% COUNT_NEAREST
% Counts points in an open ball in a kd-tree.
%
% countSet = count_nearest(querySet, maxDistanceSet, 'key', value, ...)
%
% where
%
% QUERYSET is an integer array whose linearization contains 
% n ids of points in the KDTREE.
%
% MAXDISTANCESET is a numeric array whose i:th element contains the
% radius of the search ball around the i:th query point. For the euclidean
% norm the radius must be specified in squared-distance. A scalar M is
% automatically extended to ones(1, n) * M.
%
% Optional input arguments in 'key'-value pairs:
%
% NORM ('norm') is a string which specifies the norm to use. 
% Must be one of
%     euclidean: the euclidean norm (default)
%     maximum: the maximum norm
%
% Return values
% -------------
%
% COUNTSET is an (1 x n) non-negative integer array, whose i:th element
% contains the number of points in the open ball around the i:th query
% point.

% Description: Counts points in an open ball in a kd-tree.
% DocumentationOf: PointKdTree.m

function countSet = count_nearest(self, querySet, ...
    maxDistanceSet, varargin)

    eval(import_pastel);
    
	% Optional input arguments
	norm = 'euclidean';
	eval(process_options({...
	    'norm'}, ...
	    varargin));

    concept_check(...
        maxDistanceSet, 'real_matrix', ...
        norm, 'string');

    n = numel(querySet);
    if isscalar(maxDistanceSet)
    	maxDistanceSet = ones(1, n) * maxDistanceSet;
    else
        if numel(maxDistanceSet) ~= n
            error(['The number of elements in maxDistanceSet ', ...
                'and querySet differ.']);
        end
    end

    countSet = pastelmatlab(...
        'pointkdtree_count_nearest', ...
        self.kdTree, querySet, ...
        maxDistanceSet, norm);
end
