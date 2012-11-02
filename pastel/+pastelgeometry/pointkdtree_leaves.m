% POINTKDTREE_LEAVES
% Returns the number of leaf nodes in a kd-tree.
%
% L = pointkdtree_leaves(kdTree)
%
% where
%
% KDTREE is a kd-tree constructed with pointkdtree_construct().
%
% L is the number of leaf nodes in the kd-tree.

% Description: Returns the number of leaf nodes in a kd-tree
% Documentation: matlab_pointkdtree.txt

function L = pointkdtree_leaves(kdTree)

eval(import_pastel);

L = pastelgeometrymatlab('pointkdtree_leaves', kdTree);

end