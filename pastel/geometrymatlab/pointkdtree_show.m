% POINTKDTREE_SHOW
% Unhides a set of points in a kd-tree.
%
% pointkdtree_show(kdTree, idSet)
%
% where
%
% KDTREE is a kd-tree constructed with pointkdtree_construct().
%
% IDSET is an arbitrary-dimensional numeric array which 
% contains point identifiers as returned by 'pointkdtree_insert'.
% Native type: integer.
%
% If 'idSet' is not given, then all points are shown.
%
% Showing behaves equivalent to inserting a point. The difference 
% is that here the point already exists, but was hided previously. 
% Hiding and showing are the preferred way to implement a 
% semi-dymamic point-set.

% Description: Unhides a set of points in a kd-tree
% Documentation: matlab_pointkdtree.txt

function pointkdtree_show(kdTree, idSet)

if nargin < 2
    pastelgeometrymatlab('pointkdtree_show', kdTree);
else
    pastelgeometrymatlab('pointkdtree_show', kdTree, idSet);
end

end