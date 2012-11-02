% POINTKDTREE_REFINE
% Refines the subdivision of a kd-tree.
%
% pointkdtree_refine(kdTree, bucketSize)
%
% where
%
% KDTREE is a kd-tree constructed with pointkdtree_construct().
%
% BUCKETSIZE is the maximum number of objects in a node
% to stop the recursive subdivision. Default: 8.

% Description: Refines the subdivision of a kd-tree
% Documentation: matlab_pointkdtree.txt

function pointkdtree_refine(kdTree, bucketSize)

eval(import_pastel);

if nargin < 2
    bucketSize = 8;
end

if bucketSize <= 0
    error('bucketSize must be a positive integer');
end

pastelgeometrymatlab('pointkdtree_refine', kdTree, bucketSize);

end