% POINTKDTREE_POINTS
% Returns the number of points in a kd-tree.
%
% N = pointkdtree_points(kdTree)
%
% where
%
% KDTREE is a kd-tree constructed with pointkdtree_construct().
%
% N is the number of points in the kd-tree.

% Description: Returns the number of points in a kd-tree
% Documentation: matlab_pointkdtree.txt

function N = pointkdtree_points(kdTree)

eval(import_pastel);

N = pastelgeometrymatlab('pointkdtree_points', kdTree);

end