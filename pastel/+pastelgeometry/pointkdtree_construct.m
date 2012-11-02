% POINTKDTREE_CONSTRUCT
% Constructs an empty kd-tree.
%
% kdTree = pointkdtree_construct(dimension)
%
% where
%
% DIMENSION is a positive integer defining the
% dimension of the tree.

% Description: Constructs an empty kd-tree
% Documentation: matlab_pointkdtree.txt

function kdTree = pointkdtree_construct(dimension)

eval(import_pastel);

if dimension <= 0
    error('dimension must be a positive integer');
end

kdTree = pastelgeometrymatlab('pointkdtree_construct', dimension);

end