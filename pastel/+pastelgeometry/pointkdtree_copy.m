% POINTKDTREE_COPY
% Makes a copy of a kd-tree.
%
% kdTreeCopy = pointkdtree_copy(kdTree)
%
% where
%
% KDTREE is a kd-tree constructed with pointkdtree_construct().
%
% KDTREECOPY is a copy of 'kdTree'.

% Description: Makes a copy of a kd-tree.
% Documentation: matlab_pointkdtree.txt

function kdTreeCopy = pointkdtree_copy(kdTree)

eval(import_pastel);

kdTreeCopy = pastelgeometrymatlab('pointkdtree_copy', kdTree);

end