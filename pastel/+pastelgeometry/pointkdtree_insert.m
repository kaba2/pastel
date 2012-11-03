% POINTKDTREE_INSERT
% Inserts points into a kd-tree.
%
% idSet = pointkdtree_insert(kdTree, pointSet)
%
% where
%
% KDTREE is a kd-tree constructed with pointkdtree_construct().
%
% POINTSET is an (m x n) numeric array which contains n points
% of dimension m. The m must match with the dimension
% of the kd-tree. Native type: real.
%
% IDSET is a positive integer array containing the identifiers of 
% the inserted points in the kd-tree.

% Description: Inserts points into a kd-tree
% Documentation: matlab_pointkdtree.txt

function idSet = pointkdtree_insert(kdTree, pointSet)

eval(import_pastel);

if pointkdtree_dimension(kdTree) ~= size(pointSet, 1)
    error('Dimensions of the kd-tree and the point-set do not match.');
end

idSet = pastelgeometrymatlab('pointkdtree_insert', kdTree, pointSet);

end