% POINTKDTREE_DIMENSION
% Returns the dimension of a kd-tree.
%
% D = pointkdtree_dimension(kdTree)
%
% where
%
% KDTREE is a kd-tree constructed with pointkdtree_construct().
%
% D is the dimension of the tree.

% Description: Returns the dimension of a kd-tree
% Documentation: matlab_pointkdtree.txt

function D = pointkdtree_dimension(kdTree)

D = pastelgeometrymatlab('pointkdtree_dimension', kdTree);

end