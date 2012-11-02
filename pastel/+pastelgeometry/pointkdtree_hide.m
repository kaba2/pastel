% POINTKDTREE_HIDE
% Hides a set of points in a kd-tree.
%
% pointkdtree_hide(kdTree, idSet)
%
% where
%
% KDTREE is a kd-tree constructed with pointkdtree_construct().
%
% IDSET is an arbitrary-dimensional numeric array which 
% contains point identifiers as returned by 'pointkdtree_insert'.
% Native type: integer.
%
% Hiding behaves equivalent to erasing a point. The difference
% is that the lifetime of the point does not end, and can be
% restored back to the kd-tree later, called showing. Hiding and 
% showing are the preferred way to implement a semi-dymamic
% point-set.
%
% If 'idSet' is not given, then all points are hided.

% Description: Hides a set of points in a kd-tree
% Documentation: matlab_pointkdtree.txt

function pointkdtree_hide(kdTree, idSet)

eval(import_pastel);

if nargin < 2
   pastelgeometrymatlab('pointkdtree_hide', kdTree);
else
   pastelgeometrymatlab('pointkdtree_hide', kdTree, idSet);
end

end