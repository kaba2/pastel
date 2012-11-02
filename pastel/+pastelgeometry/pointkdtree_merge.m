% POINTKDTREE_MERGE
% Flattens a kd-tree into a single node.
%
% pointkdtree_merge(kdTree)
%
% where
%
% KDTREE is a kd-tree constructed with pointkdtree_construct().

% Description: Flattens a kd-tree into a single node.
% Documentation: matlab_pointkdtree.txt

function pointkdtree_merge(kdTree)

eval(import_pastel);

pastelgeometrymatlab('pointkdtree_merge', kdTree)

end