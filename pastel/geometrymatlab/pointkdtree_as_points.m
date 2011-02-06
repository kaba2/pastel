% POINTKDTREE_AS_POINTS
% Returns the coordinates of an identifier point-set.
%
% pointSet = pointkdtree_as_points(kdTree, idSet)
%
% where
%
% KDTREE is a kd-tree constructed with pointkdtree_construct().
%
% IDSET is a numeric array whose linearization contains
% n point identifiers, whose coordinates are to be 
% returned. Native type: integer.
%
% POINTSET is an (m x n) real array, which contains n
% points of dimension m.
%
% If a point identifier is not part of the kd-tree, a
% NaN vector is returned for that point.

% Description: Returns the coordinates of an identifier point-set.
% Documentation: matlab_pointkdtree.txt

function pointSet = poindktree_as_points(kdTree, idSet)

pointSet = pastelgeometrymatlab(...
	'pointkdtree_as_points', kdTree, idSet);