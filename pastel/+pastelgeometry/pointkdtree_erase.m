% POINTKDTREE_ERASE
% Removes a set of points from a kd-tree.
%
% pointkdtree_erase(kdTree, idSet)
%
% where
%
% KDTREE is a kd-tree constructed with pointkdtree_construct().
%
% IDSET is an arbitrary-dimensional numeric array which 
% contains point identifiers as returned by 'pointkdtree_insert'.
% Native type: integer.
%
% If 'idSet' is not given, then all points are removed.

% Description: Removes a set of points in a kd-tree
% Documentation: matlab_pointkdtree.txt

function pointkdtree_erase(kdTree, idSet)

eval(import_pastel);

if nargin < 2
    pastelgeometrymatlab('pointkdtree_erase', kdTree);
else
    pastelgeometrymatlab('pointkdtree_erase', kdTree, idSet);
end

end
