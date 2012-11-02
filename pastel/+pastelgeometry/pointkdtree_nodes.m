% POINTKDTREE_NODES
% Returns the number of nodes in a kd-tree.
%
% N = pointkdtree_nodes(kdTree)
%
% where
%
% KDTREE is a kd-tree constructed with pointkdtree_construct().
%
% N is the number of nodes in the kd-tree.

% Description: Returns the number of nodes in a kd-tree
% Documentation: matlab_pointkdtree.txt

function N = pointkdtree_nodes(kdTree)

eval(import_pastel);

N = pastelgeometrymatlab('pointkdtree_nodes', kdTree);

end