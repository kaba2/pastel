% POINTKDTREE_OBJECTS
% Returns the number of points in a kd-tree.
%
% N = pointkdtree_objects(kdTree)
%
% where
%
% KDTREE is a kd-tree constructed with pointkdtree_construct().
%
% N is the number of points in the kd-tree.

% Description: Returns the number of points in a kd-tree
% Documentation: matlab_pointkdtree.txt

function N = pointkdtree_objects(kdTree)

N = pastelgeometrymatlab('pointkdtree_objects', kdTree);

end